#include "VirtualMachine.h"

using namespace std;

VirtualMachine::VirtualMachine()
{
	pc = 0, clk = 0, sp = 256, ir = 0, carry = 0, lim = 0; // initialize everything.
	statusreg.i = 0;
	statusreg.sr.carry = 0; 
	statusreg.sr.less = 0;
	statusreg.sr.equal = 0;
	statusreg.sr.greater = 0;
	reg = vector <int> (REG_FILE_SIZE); // initialize registers, (4)
	mem = vector <int> (MEM_SIZE);		// initialize  memorory (256)

	instrIN[0] = &VirtualMachine::load;
	instrIN[1] = &VirtualMachine::loadi;
	instrIN[2] = &VirtualMachine::store;
	instrIN[4] = &VirtualMachine::add;
	instrIN[5] = &VirtualMachine::addi;
	instrIN[6] = &VirtualMachine::addc;
	instrIN[7] = &VirtualMachine::addci;
	instrIN[8] = &VirtualMachine::sub;
	instrIN[9] = &VirtualMachine::subi;
	instrIN[10] = &VirtualMachine::subc;
	instrIN[11] = &VirtualMachine::subci;
	instrIN[12] = &VirtualMachine::And;
	instrIN[13] = &VirtualMachine::Andi;
	instrIN[14] = &VirtualMachine::Xor;
	instrIN[15] = &VirtualMachine::Xori;
	instrIN[16] = &VirtualMachine::Compl;
	instrIN[18] = &VirtualMachine::shl;
	instrIN[20] = &VirtualMachine::shla;
	instrIN[22] = &VirtualMachine::shr;
	instrIN[24] = &VirtualMachine::shra;
	instrIN[26] = &VirtualMachine::compr;
	instrIN[27] = &VirtualMachine::compri;
	instrIN[28] = &VirtualMachine::getstat;
	instrIN[30] = &VirtualMachine::putstat;
	instrIN[32] = &VirtualMachine::jump;
	instrIN[34] = &VirtualMachine::jumpl;
	instrIN[36] = &VirtualMachine::jumpe;
	instrIN[38] = &VirtualMachine::jumpg;
	instrIN[40] = &VirtualMachine::call;
	instrIN[42] = &VirtualMachine::Return;
	instrIN[44] = &VirtualMachine::read;
	instrIN[46] = &VirtualMachine::write;
	instrIN[48] = &VirtualMachine::halt;
	instrIN[50] = &VirtualMachine::noop; 
};

void VirtualMachine::ldprog(string infile)
{
	this->infile=infile;
    int i = 0; 
	string ofile = infile;
    fstream loadProg;
	loadProg.open(ofile.c_str(), ios::in);
	if (!loadProg.is_open()) 
	{
		exit(10);
		system("PAUSE");
	}

	
	while (getline(loadProg, line).good()) 
	{
		istringstream str(line.c_str());
		str >> objcode;

		mem[i] = objcode;
               
        instruction ins;
        ins.i = objcode;
        OP=ins.f1.OP;
        I = ins.f1.I;
        opcode = (OP << 1) + I;
		opcodes.push_back(opcode);
        //cout << "stack position: mem["<< i <<"] is opcode = " << opcode << endl <<endl;
        i++;
		
                
        lim++;
	}
}

void VirtualMachine::execute()
{
	//for(unsigned int k = 0; k < opcodes.size(); k++)
		//cout<<opcodes[k]<<" op "<<endl;
	while(pc != lim) 
	{	
		ir = mem[pc];
		++pc;
		unsigned int cnt=0;
		instruction ins;
		ins.i = ir;
		OP=ins.f1.OP;
		rd = ins.f1.RD;
		I = ins.f1.I;
		rs = ins.f1.RS;
		constt = ins.f3.CONST;
		address = ins.f2.ADDR;
		int op = ((OP << 1) + I);
		try
		{
			if (!instrIN[op])
			{
				throw NullPointerException();
			}
		}
		catch (NullPointerException e) 
		{
			cerr << e.what() << endl;
			exit(1);
		}
		runinstr(op);
		//system("pause");
		if(mem[pc]==0)
		{
			for(int g = 0; g < 15; g++)
				cout<<mem[g]<< endl;
			system("pause");
            exit(11);
			system("PAUSE");
        }
	}
}

void VirtualMachine::runinstr(int inst)
{
	(this->*instrIN[inst])();
}

int VirtualMachine::checkNeg(int value)
{
	if(value<0)
		value &= 0x0000FFFF;
	return value;
}

void VirtualMachine::shiftCarryRight(int value)
{
	int check = value & 1; 
	if(check == 1)
		statusreg.sr.carry = 1;
	else 
		statusreg.sr.carry = 0;
}

void VirtualMachine::shiftCarryLeft(int value)
{
	int hexv = 0x8000;
	int check = hexv & value;
	if(check == hexv)
		statusreg.sr.carry = 1;
	else 
		statusreg.sr.carry = 0;

}

void VirtualMachine::shiftRight(int value)
{
	int hexv = 0x8000; 
	int check = hexv & value;
	if(check == hexv)
		reg[rd] |= 0xFFFF0000;
	else 
		reg[rd] &= 0x0000FFFF;
}

void VirtualMachine::checkSign()
{
	int temp = reg[inst.f3.RD];
	if(temp &= 0x8000) // if true, then the MSB in the number is 1, so we must sign extend 1
		reg[inst.f3.RD] |= 0xffff8000;
	else
        reg[inst.f3.RD] &= 0x00007fff;
}

void VirtualMachine::checkLimit()
{
	if(pc>lim)
		exit(14);
}

void VirtualMachine::load()
{
	cout<<"load: ";
	//system("pause");
	reg[rd] = mem[address];
	reg[rd] &= 0x0000FFFF;
	clk += lscrtick;
	timeSlice += lscrtick;
}

void VirtualMachine::loadi()
{
	cout<<"loadi: ";
	//system("pause");
	constt = checkNeg(constt);
	reg[rd] = constt;
	reg[rd] &= 0x0000FFFF;
	clk += tick;
	timeSlice += tick;
}

void VirtualMachine::store()
{
	cout<<"store: ";
	//system("pause");
	if(reg[rd]>32767)
	{
		reg[rd] |= 0xFFFF0000;
	}
	mem[address] = reg[rd];
	clk += lscrtick;
	timeSlice += lscrtick;
}

void VirtualMachine::add()
{
	cout<<"add: ";
	//system("pause");
	if(reg[rs]>32767)
		reg[rs] |= 0xFFFF0000;
	if(reg[rd]>32767)
		reg[rd] |= 0xFFFF0000;
	reg[rd] += reg[rs];
	if(reg[rd]>32767)
	{
		reg[rd] = 32767;
		statusreg.sr.carry = 1;
	}
	else if(reg[rd]<-32768)
	{
		reg[rd]=-32768;
		statusreg.sr.carry = 1;
	}
	reg[rd] &= 0x0000FFFF;
	clk += tick;
	timeSlice += tick;
}

void VirtualMachine::addi()
{
	cout<<"addi: ";
	//system("pause");
	if(reg[rd]>32767)
		reg[rd] |= 0xFFFF0000;
	reg[rd] += constt;
	if(reg[rd]>32767)
	{
		reg[rd] = 32767;
		statusreg.sr.carry = 1;
	}
	else if(reg[rd] < -32768)
	{
		reg[rd] = -32768;
		statusreg.sr.carry = 1;
	}
	reg[rd] &= 0x0000FFFF;
	clk += tick;
	timeSlice += tick;
}

void VirtualMachine::addc()
{
	cout<<"addc: ";
	//system("pause");
	if(reg[rs]>32767)
		reg[rs] |= 0xFFFF0000;
	if(reg[rd]>32767)
		reg[rd] |= 0xFFFF0000;
	reg[rd] += reg[rs] + carry;
	if(reg[rd]>32767)
	{
		reg[rd] = 32767;
		statusreg.sr.carry = 1;
	}
	else if(reg[rd]<-32768)
	{
		reg[rd]=-32768;
		statusreg.sr.carry = 1;
	}
	reg[rd] &= 0x0000FFFF;
	clk += tick;
	timeSlice += tick;
}

void VirtualMachine::addci()
{
	cout<<"addci: ";
	//system("pause");
	if(reg[rd]>32767)
		reg[rd] |= 0xFFFF0000;
	reg[rd] += carry+ constt;
	if(reg[rd]>32767)
	{
		reg[rd] = 32767;
		statusreg.sr.carry = 1;
	}
	else if(reg[rd]<-32768)
	{
		reg[rd]=-32768;
		statusreg.sr.carry = 1;
	}
	reg[rd] &= 0x0000FFFF;
	clk += tick;
	timeSlice += tick;
}

void VirtualMachine::sub()
{
	cout<<"sub: ";
	//system("pause");
	if(reg[rs]>32767)
		reg[rs] |= 0xFFFF0000;
	if(reg[rd]>32767)
		reg[rd] |= 0xFFFF0000;
	reg[rd] -= reg[rs];
	if(reg[rd]>32767)
	{
		reg[rd] = 32767;
		statusreg.sr.carry = 1;
	}
	else if(reg[rd]<-32768)
	{
		reg[rd]=-32768;
		statusreg.sr.carry = 1;
	}
	reg[rd] &= 0x0000FFFF;
	clk += tick;
	timeSlice += tick;
}

void VirtualMachine::subi()
{
	cout<<"subi: ";
	//system("pause");
	if(reg[rs]>32767)
		reg[rs] |= 0xFFFF0000;
	reg[rd] -= constt;
	if(reg[rd]>32767)
	{
		reg[rd] = 32767;
		statusreg.sr.carry = 1;
	}
	else if(reg[rd]<-32768)
	{
		reg[rd]=-32768;
		statusreg.sr.carry = 1;
	}
	reg[rd] &= 0x0000FFFF;
	clk += tick;
	timeSlice += tick;
}

void VirtualMachine::subc()
{
	cout<<"subc: ";
	//system("pause");
	if(reg[rs]>32767)
		reg[rs] |= 0xFFFF0000;
	if(reg[rd]>32767)
		reg[rd] |= 0xFFFF0000;
	reg[rd] -= (reg[rs] + carry);
	if(reg[rd]>32767)
	{
		reg[rd] = 32767;
		statusreg.sr.carry = 1;
	}
	else if(reg[rd]<-32768)
	{
		reg[rd]=-32768;
		statusreg.sr.carry = 1;
	}
	reg[rd] &= 0x0000FFFF;
	clk += tick;
	timeSlice += tick;
}

void VirtualMachine::subci()
{
	cout<<"subci: ";
	//system("pause");
	if(reg[rs]>32767)
		reg[rs] |= 0xFFFF0000;
	reg[rd] -= (constt+ carry);
	if(reg[rd]>32767)
	{
		reg[rd] = 32767;
		statusreg.sr.carry = 1;
	}
	else if(reg[rd]<-32768)
	{
		reg[rd]=-32768;
		statusreg.sr.carry = 1;
	}
	reg[rd] &= 0x0000FFFF;
	clk += tick;
	timeSlice += tick;
}

void VirtualMachine::And()
{
	cout<<"and: ";
	//system("pause");
	reg[rd] &= reg[rs];
	clk += tick;
}

void VirtualMachine::Andi()
{
	cout<<"andi: ";
	//system("pause");
	reg[rd] &= constt;
	clk += tick;
	timeSlice += tick;
}

void VirtualMachine::Xor()
{
	cout<<"xor: ";
	//system("pause");
	reg[rd] ^=  reg[rs];
	clk += tick;
	timeSlice += tick;
}

void VirtualMachine::Xori()
{
	cout<<"xori: ";
	//system("pause");
	reg[rd] ^=  constt;
	clk += tick;
	timeSlice += tick;
}

void VirtualMachine::Compl()
{
	cout<<"compl: ";
	//system("pause");
	reg[rd] =  ~reg[rd];
	clk += tick;
	timeSlice += tick;
}

void VirtualMachine::shl()
{
	cout<<"shl: ";
	//system("pause");
	shiftCarryLeft(reg[rd]);
	reg[rd] = reg[rd] << 1;
	clk += tick;
	timeSlice += tick;
}

void VirtualMachine::shla()
{
	cout<<"shla: ";
	//system("pause");
	shiftCarryLeft(reg[rd]);
    reg[rd] = reg[rd] << 1;

    clk += tick;
	timeSlice += tick;
}

void VirtualMachine::shr()
{
	cout<<"shr: ";
	//system("pause");
	shiftCarryRight(reg[rd]);
	reg[rd] = reg[rd] >> 1;
   
	clk += tick;
	timeSlice += tick;
}

void VirtualMachine::shra()
{
	cout<<"shra: ";
	//system("pause");
	shiftCarryRight(reg[rd]);
    shiftRight(reg[rd]);
    reg[rd] = reg[rd] >> 1;

    clk += tick;
	timeSlice += tick;
}

void VirtualMachine::compr()
{
	cout<<"compr: ";
	//system("pause");
	if(reg[rd] < reg[rs])
	{
		statusreg.sr.less = 1;
		statusreg.sr.equal = 0;
		statusreg.sr.greater = 0;
	}
	else if(reg[rd] = reg[rs])
	{
		statusreg.sr.less = 0;
		statusreg.sr.equal = 1;
		statusreg.sr.greater = 0;
	}
	else if(reg[rd] > reg[rs])
	{
		statusreg.sr.less = 0;
		statusreg.sr.equal = 0;
		statusreg.sr.greater = 1;
	}
	clk += tick;
	timeSlice += tick;
}

void VirtualMachine::compri()
{
	cout<<"compri: ";
	//system("pause");
	if(reg[rd]>32767)
		reg[rd] |= 0xFFFF0000;
	if(reg[rd] < constt)
	{
		statusreg.sr.less = 1;
		statusreg.sr.equal = 0;
		statusreg.sr.greater = 0;
	}
	else if(reg[rd] == constt)
	{
		statusreg.sr.less = 0;
		statusreg.sr.equal = 1;
		statusreg.sr.greater = 0;
	}
	else if(reg[rd] > constt)
	{
		statusreg.sr.less = 0;
		statusreg.sr.equal = 0;
		statusreg.sr.greater = 1;
	}
	reg[rd] &= 0x0000FFFF;
	clk += tick;
	timeSlice += tick;
}

void VirtualMachine::getstat()
{
	cout<<"getstat: ";
	//system("pause");
	reg[rd] = statusreg.i;
	clk += tick;
	timeSlice += tick;
}

void VirtualMachine::putstat()
{
	cout<<"putstat: ";
	//system("pause");
	statusreg.i = reg[rd];
	clk += tick;
	timeSlice += tick;
}

void VirtualMachine::jump()
{
	cout<<"jump: ";
	//system("pause");
	pc = address;
	checkLimit();
	clk += tick;
	timeSlice += tick;
}

void VirtualMachine::jumpl()
{
	cout<<"jumpl: ";
	//system("pause");
	if(statusreg.sr.less==1)
	{ 
		pc = address;
		checkLimit();
	}
	clk += tick;
	timeSlice += tick;
}

void VirtualMachine::jumpe()
{
	cout<<"jumpe: ";
	//system("pause");
	if(statusreg.sr.equal==1)
	{ 
		pc = address;
		checkLimit();
	}
	clk += tick;
	timeSlice += tick;
}

void VirtualMachine::jumpg()
{
	cout<<"jumpg: ";
	//system("pause");
	if(statusreg.sr.greater==1)
	{ 
		pc = address;
		checkLimit();
	}
	clk += tick;
	timeSlice += tick;
}

void VirtualMachine::call()
{
	cout<<"call: ";
	//system("pause");
	sp = sp - 6;
	mem[sp+5] = pc;
	mem[sp+4] = reg[0];
	mem[sp+3] = reg[1];
	mem[sp+2] = reg[2];
	mem[sp+1] = reg[3];
	mem[sp] = statusreg.i;

	pc = address;
	clk += lscrtick;
	timeSlice += lscrtick;
}

void VirtualMachine::Return()
{
	cout<<"return: ";
	//system("pause");
	sp = sp + 6;
	pc =    mem[sp-1];
	reg[0] =  mem[sp-2];
	reg[1] =  mem[sp-3];
	reg[2] =  mem[sp-4];
	reg[3] =  mem[sp-5];
	statusreg.i  =  mem[sp-6];
  
   clk += lscrtick;
   timeSlice += lscrtick;
}

void VirtualMachine::read()
{
	cout<<"read: ";
	//system("pause");
	string ifile = infile.substr(0,infile.size()-1);
	ifile = ifile + "in"; 
	ifstream inputfile;
	inputfile.open(ifile.c_str());
	int content;
	if(!inputfile.is_open())
	{
		cout<<"Error opening file"<<endl;
		system("pause");
	}
	else
	{
		inputfile >> content;
		inputfile.close();
	}
	reg[rd] = content;
	reg[rd] &= 0x0000FFFF;
	clk += rwtick;
	timeSlice += rwtick;
}

void VirtualMachine::write()
{
	cout <<"write: ";
	//system("pause");
	filename = infile.substr(0,infile.size()-1);
	filename += "out"; 
	ofstream outputfile;
	outputfile.open(filename.c_str());
	if(reg[rd]>32767)
		reg[rd]|= 0xFFFF0000;
	if(!outputfile.is_open())
	{
		cout <<"Error when opening"<<endl;
		system("pause");
	}
	else
	{
		outputfile << reg[rd] << endl;
		outputfile.close();
	}

	clk += rwtick;  
	timeSlice += rwtick;
}

void VirtualMachine::halt()
{
	clk += tick;
	timeSlice += tick;
	cout<<"halt: ";//clock= "<<clk<<endl<<endl;
	ofstream out; 
	out.open(filename.c_str(), ios_base::app);
	out<<"clock: "<<clk;
	out.close();
	system("pause");
	exit(1);
}

void VirtualMachine::noop()
{
	//noop->do nothing
}


/*
void main()
{
cout<< " error(10) - (LOAD()) - File failed to open." << endl << "error(11) - (FETCH()) - Program out of bounds" << endl << "error(12) - (checkOverflow) - An overflow has occured:" << endl << "error(13) - (checkOverflow) - An overflow has occured:" << endl << "error(14) - (checklimit) - Program counter exceeds limit";
	Assembler Test;
	Test.assemble("Prog.s");
	VirtualMachine Tester;
	Tester.ldprog("Prog.o");
	Tester.execute();
	system("pause");
}*/