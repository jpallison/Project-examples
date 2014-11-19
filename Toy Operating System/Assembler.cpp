#include "Assembler.h"


using namespace std;

int op; //opcode
int rd; // register destination
int rs; //register source
int im; // immediate bit
int instr; // instruction 

Assembler::Assembler() // constructor
{
	glbBase = 0;
	instr["load"] = &Assembler::load;
	instr["loadi"] = &Assembler::loadi;
	instr["store"] = &Assembler::store;
	instr["add"] = &Assembler::add;
	instr["addi"] = &Assembler::addi;
	instr["addc"] = &Assembler::addc;
	instr["addci"] = &Assembler::addci;
	instr["sub"] = &Assembler::sub;
	instr["subi"] = &Assembler::subi;
	instr["subc"] = &Assembler::subc;
	instr["subci"] = &Assembler::subci;
	instr["and"] = &Assembler::And;
	instr["andi"] = &Assembler::Andi;
	instr["xor"] = &Assembler::Xor;
	instr["xori"] = &Assembler::Xori;
	instr["compl"] = &Assembler::Compl;
	instr["shl"] = &Assembler::shl;
	instr["shla"] = &Assembler::shla;
	instr["shr"] = &Assembler::shr;
	instr["shra"] = &Assembler::shra;
	instr["compr"] = &Assembler::compr;
	instr["compri"] = &Assembler::compri;
	instr["getstat"] = &Assembler::getstat;
	instr["putstat"] = &Assembler::putstat;
	instr["jump"] = &Assembler::jump;
	instr["jumpl"] = &Assembler::jumpl;
	instr["jumpe"] = &Assembler::jumpe;
	instr["jumpg"] = &Assembler::jumpg;
	instr["call"] = &Assembler::call;
	instr["return"] = &Assembler::Return;
	instr["read"] = &Assembler::read;
	instr["write"] = &Assembler::write;
	instr["halt"] = &Assembler::halt;
	instr["noop"] = &Assembler::noop; 

}

void Assembler::assemble(string infile, PCB * p)
{
	p->pbase = glbBase;
    p->pc = p->pbase;
	ifstream inProg;
	filename = infile;
	inProg.open(infile.c_str());
	if (!inProg.is_open()) 
	{
		cout << infile << " failed to open.\n";
		exit(1);
		system("PAUSE");
	}
	else
	{
		p->ofile.open(p->ofname.c_str(), ios::out);
		while (getline (inProg, line).good())
		{
			if (line[0] != '!')
			{
				int pos = line.find("!");
				if(pos!=string::npos)
					line = line.substr(0,pos-1) + " 0 0";
				istringstream parts( line.c_str());
				parts >> opcode >> rd >> temp;
				//cout<<temp;
				//system("pause");
				try
				{
					if (!instr[opcode])
					{
						throw NullPointerException();
					}
                       
   
				}
				catch (NullPointerException e) 
				{
					cerr << e.what() << endl;
					exit(1);
				}
				p->ofile << (this->*instr[opcode])();
				p->limit += 1;
				glbBase += 1;
			} 
                           
		}
		inProg.close();
		p->ofile.close();
	}
	//for(unsigned int i = 0; i < vecout.size(); i++)
		//cout<<vecout[i]<<endl;
	//assembleOut();    
}

void Assembler::assembleOut()
{
	ofstream ProgOut;
	filename = filename.substr(0,filename.size()-1);
	filename += "o";
	ProgOut.open(filename.c_str());
	for (unsigned int i = 0; i < vecout.size(); i++)
	{
		ProgOut << vecout[i];
		ProgOut<<endl;
	}
	ProgOut.close();

}

void Assembler::checkSize(int checkSize)
{
	if((checkSize>127) | (checkSize < -128))
		exit(0);
}

void Assembler::checkNegative(int &checkNegIn)
{
	if(checkNegIn < 0)
		checkNegIn &= 0x000000FF; 
}

void Assembler::checkRegister()
{
	if (rd > 3 || rd < 0)
	{
		cout << "Register does not exist " << endl;
		exit(3);
		system("PAUSE");
	}
	if (rs > 3 || rs < 0)
	{
		cout << "Register does not exist " << endl;
		exit(4);
		system("PAUSE");
	}

}

int Assembler::load()
{

	rs = 0;
	checkRegister();
	int out = 0;
	int loadcmd = 0;
	int i = 0;

	out = (loadcmd << 11)+(rd << 9)+(i << 8) + temp; 
	return out;
}

int Assembler::loadi()
{
	rs = 0;
	checkRegister();
	checkSize(temp);
	checkNegative(temp);
	int out = 0;
	int loadicmd = 0;
	int i = 1;

	out = (loadicmd << 11)+(rd << 9)+(i << 8) + temp; 
	return out;
}

int Assembler::store()
{
	rs = 0;
	checkRegister();
	int out = 0;
	int storecmd = 1;
	int i = 0;

	out = (storecmd << 11)+(rd << 9)+(i << 8) + temp; 
	return out;
}

int Assembler::add()
{
	checkSize(temp);
	checkNegative(temp);
	rs = temp;
	checkRegister();
	int out = 0;
	int addcmd = 2;
	int i = 0;

	out = (addcmd << 11)+(rd << 9)+(i << 8) + (rs << 6);
	return out;
}

int Assembler::addi()
{
	rs = 0;
	checkSize(temp);
	checkNegative(temp);
	checkRegister();
	int out = 0;
	int addicmd = 2;
	int i = 1;

	out = (addicmd << 11)+(rd << 9)+(i << 8) + temp;
	return out;
}

int Assembler::addc()
{
	checkSize(temp);
	checkNegative(temp);
	rs = temp;
	checkRegister();
	int out = 0;
	int addccmd = 3;
	int i = 0;

	out = (addccmd << 11)+(rd << 9)+(i << 8) + (rs << 6);
	return out;
}

int Assembler::addci()
{
	checkSize(temp);
	checkNegative(temp);
	rs = 0;
	checkRegister();
	int out = 0;
	int addcicmd = 3;
	int i = 1;

	out = (addcicmd << 11)+(rd << 9)+(i << 8) + (temp);
	return out;
}

int Assembler::sub()
{
	checkSize(temp);
	checkNegative(temp);
	rs = temp;
	checkRegister();
	int out = 0;
	int subcmd = 4;
	int i = 0;

	out = (subcmd << 11)+(rd << 9)+(i << 8) + (rs << 6);
	return out;
}

int Assembler::subi()
{
	checkSize(temp);
	checkNegative(temp);
	rs = 0;
	checkRegister();
	int out = 0;
	int subicmd = 4;
	int i = 1;

	out = (subicmd << 11)+(rd << 9)+(i << 8) + (temp);
	return out;
}

int Assembler::subc()
{
	checkSize(temp);
	checkNegative(temp);
	rs = temp;
	checkRegister();
	int out = 0;
	int subccmd = 5;
	int i = 0;

	out = (subccmd << 11)+(rd << 9)+(i << 8) + (rs << 6);
	return out;
}

int Assembler::subci()
{
	checkSize(temp);
	checkNegative(temp);
	rs = 0;
	checkRegister();
	int out = 0;
	int subcicmd = 5;
	int i = 1;

	out = (subcicmd << 11)+(rd << 9)+(i << 8) + (temp);
	return out;
}

int Assembler::And()
{
	rs = temp;
	checkRegister();
	int out = 0;
	int andcmd = 6;
	int i = 0;

	out = (andcmd << 11)+(rd << 9)+(i << 8) + (rs << 6);
	return out;
}

int Assembler::Andi()
{
	checkSize(temp);
	checkNegative(temp);
	rs = 0;
	checkRegister();
	int out = 0;
	int andicmd = 6;
	int i = 1;

	out = (andicmd << 11)+(rd << 9)+(i << 8) + (temp);
	return out;
}

int Assembler::Xor()
{
	rs = temp;
	checkRegister();
	int out = 0;
	int xorcmd = 7;
	int i = 0;

	out = (xorcmd << 11)+(rd << 9)+(i << 8) + (rs << 6);
	return out;
}

int Assembler::Xori()
{
	checkSize(temp);
	checkNegative(temp);
	rs = 0;
	checkRegister();
	int out = 0;
	int xoricmd = 7;
	int i = 1;

	out = (xoricmd << 11)+(rd << 9)+(i << 8) + (temp);
	return out;
}

int Assembler::Compl()
{
	rs = 0;
	checkRegister();
	int out = 0;
	int complcmd = 8;
	int i = 0;

	out = (complcmd << 11)+(rd << 9)+(i << 8) + (0);
	return out;
}

int Assembler::shl()
{
	rs = 0;
	checkRegister();
	int out = 0;
	int shlcmd = 9;
	int i = 0;

	out = (shlcmd << 11)+(rd << 9)+(i << 8) + (0);
	return out;
}

int Assembler::shla()
{
	rs = 0;
	checkRegister();
	int out = 0;
	int shlacmd = 10;
	int i = 0;

	out = (shlacmd << 11)+(rd << 9)+(i << 8) + (0);
	return out;
}

int Assembler::shr()
{
	rs = 0;
	checkRegister();
	int out = 0;
	int shrcmd = 11;
	int i = 0;

	out = (shrcmd << 11)+(rd << 9)+(i << 8) + (0);
	return out;
}

int Assembler::shra()
{
	rs = 0;
	checkRegister();
	int out = 0;
	int addccmd = 12;
	int i = 0;

	out = (addccmd << 11)+(rd << 9)+(i << 8) + (0);
	return out;
}

int Assembler::compr()
{
	rs = temp;
	checkRegister();
	int out = 0;
	int comprcmd = 13;
	int i = 0;

	out = (comprcmd << 11)+(rd << 9)+(i << 8) + (rs << 6);
	return out;
}

int Assembler::compri()
{
	checkSize(temp);
	checkNegative(temp);
	rs = 0;
	checkRegister();
	int out = 0;
	int addccmd = 13;
	int i = 1;

	out = (addccmd << 11)+(rd << 9)+(i << 8) + (temp);
	return out;
}

int Assembler::getstat()
{
	rs = 0;
	checkRegister();
	int out = 0;
	int getstatcmd = 14;
	int i = 0;

	out = (getstatcmd << 11)+(rd << 9)+(i << 8) + (0);
	return out;
}

int Assembler::putstat()
{
	rs = 0;
	checkRegister();
	int out = 0;
	int putstatcmd = 15;
	int i = 0;

	out = (putstatcmd << 11)+(rd << 9)+(i << 8) + (0);
	return out;
}

int Assembler::jump()
{
	rs = 0;
	int addr = rd;
	rd = 0;
	checkRegister();
	int out = 0;
	int jumpcmd = 16;
	int i = 0;

	out = (jumpcmd << 11)+(rd << 9)+(i << 8) + (addr);
	return out;
}

int Assembler::jumpl()
{
	rs = 0;
	int addr = rd;
	rd = 0;
	checkRegister();
	int out = 0;
	int jumplcmd = 17;
	int i = 0;

	out = (jumplcmd << 11)+(rd << 9)+(i << 8) + (addr);
	return out;
}

int Assembler::jumpe()
{
	rs = 0;
	int addr = rd;
	rd = 0;
	checkRegister();
	int out = 0;
	int jumpecmd = 18;
	int i = 0;

	out = (jumpecmd << 11)+(rd << 9)+(i << 8) + (addr);
	return out;
}

int Assembler::jumpg()
{
	rs = 0;
	int addr = rd; 
	rd = 0;
	checkRegister();
	int out = 0;
	int jumpgcmd = 19;
	int i = 0;

	out = (jumpgcmd << 11)+(rd << 9)+(i << 8) + (addr);
	return out;
}

int Assembler::call()
{
	rs = 0;
	int addr = rd;
	rd = 0;
	checkRegister();
	int out = 0;
	int callcmd = 20;
	int i = 0;

	out = (callcmd << 11)+(0)+(0) + (addr);
	return out;
}

int Assembler::Return()
{
	rs = 0;
	rd = 0;
	checkRegister();
	int out = 0;
	int returncmd = 21;
	int i = 0;

	out = (returncmd << 11)+(0)+(0) + (0);
	return out;
}

int Assembler::read()
{
	rs = 0;
	checkRegister();
	int out = 0;
	int readcmd = 22;
	int i = 0;

	out = (readcmd << 11)+(rd << 9)+(i << 8) + (0);
	return out;
}

int Assembler::write()
{
	rs = 0;
	checkRegister();
	int out = 0;
	int writecmd = 23;
	int i = 0;

	out = (writecmd << 11)+(rd << 9)+(i << 8) + (0);
	return out;
}

int Assembler::halt()
{
	rs = 0;
	rd = 0;
	checkRegister();
	int out = 0;
	int haltcmd = 24;
	int i = 0;

	out = (haltcmd << 11)+(0)+(0) + (0);
	return out;
}

int Assembler::noop()
{
	rs = 0;
	rd = 0;
	checkRegister();
	int out = 0;
	int addccmd = 25;
	int i = 0;

	out = (addccmd << 11)+(0)+(0) + (0);
	return out;
}

/*void main()
{
	Assembler Test;
	Test.assemble("Prog.s");
	system("pause");
}*/