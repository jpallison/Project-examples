#include "os.h"

using namespace std;

os::os()
{
    system("ls *.s > progs");
    
    sysTime = 0;
    sysCPUutil = 0;
    usrCPUutil = 0;
    thrghput = 0;
    ctxtSwitchTime = 0;
    jbsCPUtime = 0;
    numProcsCmpltd = 0;

    
    fstream ifile;
    ifile.open("progs", ios::in);
    if(!ifile.is_open())
    {
        cout << "Error: progs failed to open." << endl;
        exit(4);
    }
    
    while(!ifile.eof())
    {
        //Populate the PCB list and assemble each .s file into .o files
        string filename;
        getline(ifile, filename);
        
        if(filename.empty())//Accounts for the blank line added at the end of progs
            break;
        
        PCB * p = new PCB(filename);
        jobs.push_back(p);
        assm.assemble(filename.c_str(), p);
        
        //Read in each .o file and add opcodes to vm memory
        p->ofile.open(p->ofname.c_str(), ios::in);
        if(!p->ofile.is_open())
        {
            cout << "Error: " << p->ofname << " failed to open." << endl;
            exit(4);
        }
        
        cout << p->ofname << endl;
        
        int limit = p->pbase;
        string line;
        getline(p->ofile, line);
        istringstream str(line.c_str());
        str >> vm.mem[limit];
        
        cout << vm.mem[limit] << endl;

        limit++;
        vm.memLim++;
        while(!p->ofile.eof())
        {
            string line;
            getline(p->ofile, line);
            istringstream str(line.c_str());
            str >> vm.mem[limit];
            
            cout << vm.mem[limit] << endl;
            
            limit++;
            vm.memLim++;
        }
        
        cout << "base = " << p->pbase << endl;
        cout << "limit = " << p->limit << endl;
        
        p->ofile.close();
        p->outfile.open(p->outfname.c_str(), ios::out);
        p->ifile.open(p->ifname.c_str(), ios::in);
    }
    
    cout << "Memory limit = " << vm.memLim << endl;
    
    //Add each process to the ready queue and create out files
    for(list<PCB *>::iterator it = jobs.begin(); it != jobs.end(); it++)
    {
        readyQ.push(*it);
    }
}

void os::run()
{
    running = readyQ.front();
    readyQ.pop();
    while(true)
    {
        cout << endl << running->procsName << endl;

        if((readyQ.empty() & waitQ.empty()) & (*running).sreg.sr.VM_RETURN_STATUS == 1)
        {
            sysTime = ctxtSwitchTime + vm.idleTime;
            sysCPUutil = (double)(vm.clk - vm.idleTime)/vm.clk;
            usrCPUutil = (double)jbsCPUtime/vm.clk;
            thrghput = numProcsCmpltd/(vm.clk/1000.0);
            
            fstream sysfile;
            sysfile.open("system.out", ios::out);
            sysfile << "System Time = " << sysTime << " cycles" << endl;
            sysfile << "System CPU Utilization = " << sysCPUutil*100 << "%" << endl;
            sysfile << "User CPU Utilization = " << usrCPUutil*100 << "%" << endl;
            sysfile << "Throughput = " << thrghput << " processes per second" << endl;
            sysfile.close();
            
            cout << "All programs executed." << endl;
            exit(1);
        }
        //Set CPU registers
        vm.ir = 0;
        vm.statusreg.i = running->sreg.i;
        vm.sp = running->sp;
        vm.base = running->pbase;
        vm.lim = running->limit;
        vm.pc = running->pc;
        vm.outfname = running->outfname;
        vm.ifname = running->ifname;
        vm.reg[0] = running->r[0];
        vm.reg[1] = running->r[1];
        vm.reg[2] = running->r[2];
        vm.reg[3] = running->r[3];
        
        //Read from .st file if needed
        cout << "running->sp < vm.MEM_SIZE = " << running->sp << " " << vm.MEM_SIZE << endl;
        if(running->sp < vm.MEM_SIZE)
        {
            running->stfile.open(running->stfname.c_str(), ios::in);
            if(!running->stfile.is_open())
            {
                cout << "Error: stfile failed to open." << endl;
            }
            //Read from top of the stack, down
            running->stfile >> vm.mem[vm.MEM_SIZE - 1];
            cout << "vm.mem[vm.sp] = " << vm.mem[vm.sp] << endl;
            for(int i = vm.MEM_SIZE - 2; i >= vm.sp; i--)
            {
                running->stfile >> vm.mem[i];
                cout << "i = " << i << " and vm.mem[i] = " << vm.mem[i] << endl;
            }
            running->stfile.close();
        }
        
        //Run process running
        running->waitTime += (vm.clk - running->crtClk);
        vm.execute();
        running->VMCPUtime += vm.timeSlice;
        cout << "running->VMCPUtime = " << running->VMCPUtime << endl;
        ctxtSwitch();
    }
}

void os::ctxtSwitch()
{
    cout << "contextSwitch " << vm.statusreg.sr.VM_RETURN_STATUS << endl;
    
    //Save to .st file if needed
    cout << "vm.sp = " << vm.sp << " and vm.MEM_SIZE = " << vm.MEM_SIZE << endl;
    if(vm.sp < vm.MEM_SIZE)
    {
        running->stfile.open(running->stfname.c_str(), ios::out);
        //Write from bottom of the stack, up
        if(!running->stfile.is_open())
        {
            cout << "Error: stfile did not open." << endl;
        }
        running->stfile << vm.mem[vm.MEM_SIZE - 1];
        int stackSize = 1;
        
        cout << "vm.mem[vm.MEM_SIZE -1] = " << vm.mem[vm.MEM_SIZE - 1] << endl;
        cout.flush();
        for(int i = vm.MEM_SIZE - 2; i >= vm.sp; i--)
        {
            running->stfile << endl << vm.mem[i];
            stackSize++;
            
            cout << ".st file - vm.mem[i] = " << vm.mem[i] << endl;
            cout.flush();
        }
        running->stfile.close();
        
        if(running->largestStackSize < stackSize)
        {
            running->largestStackSize = stackSize;
        }
    }
    
    //Move all completed I/O operations to the readyQ
    while((readyQ.empty() & !waitQ.empty()) & waitQ.front()->ioClk > vm.clk)//Idle time for CPU
    {
        vm.clk += 1;
        vm.idleTime += 1;
    }
    while(!waitQ.empty() & waitQ.front()->ioClk <= vm.clk)
    {
        readyQ.push(waitQ.front());
        waitQ.front()->IOtime += (vm.clk - waitQ.front()->crtClk);
        running->crtClk = vm.clk;
        waitQ.pop();
    }
    
    //Examine vm sr to determine which action to take
    if(vm.statusreg.sr.VM_RETURN_STATUS == 0)//Time Slice
    {
        readyQ.push(running);
        running->crtClk = vm.clk;
    }
    else if(vm.statusreg.sr.VM_RETURN_STATUS == 1)//HALT Instruction
    {
        running->outfile << "Process CPU Time = " << running->VMCPUtime << " cycles" << endl;
        running->outfile << "Process Waiting Time = " << running->waitTime << " cycles" << endl;
        running->outfile << "Process I/O Time = " << running->IOtime << " cycles" << endl;
        running->outfile << "Process Turnaround Time(1 sec = 1000 cycles) = " << vm.clk/1000.0 << " seconds" << endl;
        running->outfile << "Process Largest Stack Size = " << running->largestStackSize << endl;
        
        jbsCPUtime += running->VMCPUtime;
        numProcsCmpltd++;

        running->ifile.close();
        running->outfile.close();
        remove(running->stfname.c_str());
    }
    else if(vm.statusreg.sr.VM_RETURN_STATUS == 2)//Out-of-bound Reference
    {
        running->outfile.seekp(0, ios_base::end);
        running->outfile << "Error: Out-of-bound reference in memory. " << "Process terminated.";
        running->ifile.close();
        running->outfile.close();
    }
    else if(vm.statusreg.sr.VM_RETURN_STATUS == 3)//Stack Overflow
    {
        running->outfile.seekp(0, ios_base::end);
        running->outfile << "Error: stack overflow in memory. " << "Process terminated.";
        running->ifile.close();
        running->outfile.close();
    }
    else if(vm.statusreg.sr.VM_RETURN_STATUS == 4)//Stack Underflow
    {
        running->outfile.seekp(0, ios_base::end);
        running->outfile << "Error: stack underflow in memory. " << "Process terminated.";
        running->ifile.close();
        running->outfile.close();
    }
    else if(vm.statusreg.sr.VM_RETURN_STATUS == 5)//Invalid Opcode
    {
        running->outfile.seekp(0, ios_base::end);
        running->outfile << "Error: invalid opcode. " << "Process terminated.";
        running->ifile.close();
        running->outfile.close();
    }
    else if(vm.statusreg.sr.VM_RETURN_STATUS == 6)//Read Operation
    {
        running->ifile >> vm.reg[vm.inst.f3.RD];
        
        cout << ".in file = " << vm.reg[vm.inst.f3.RD] << endl;
        
        running->ioClk = vm.clk + 27;
        running->crtClk = vm.clk;
        waitQ.push(running);
    }
    else if(vm.statusreg.sr.VM_RETURN_STATUS == 7)//Write Operation
    {
        cout << "WRITE(before sign extend) = " << vm.reg[vm.inst.f3.RD] << endl;
        vm.checkSign();
        running->outfile.seekp(0, ios_base::end);
        running->outfile << vm.reg[vm.inst.f3.RD] << endl;
        
        cout << "WRITE(after sign extend) = " << vm.reg[vm.inst.f3.RD] << endl;
        
        running->ioClk = vm.clk + 27;
        running->crtClk = vm.clk;
        waitQ.push(running);
    }
    
    //update running process PCB
	int k = vm.statusreg.i;
	running->sreg.i = k;
    running->sp = vm.sp;
    running->pc = vm.pc;
    running->r[0] = vm.reg[0];
    running->r[1] = vm.reg[1];
    running->r[2] = vm.reg[2];
    running->r[3] = vm.reg[3];

    if(!readyQ.empty())
    {
        running = readyQ.front();
        readyQ.pop();
    }
    
    vm.clk += 5;
    ctxtSwitchTime += 5;
}
