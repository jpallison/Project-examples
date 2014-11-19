#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H
#include <iostream>
#include <string>	
#include <map>
#include <vector>
#include <stdexcept>
#include <cstdlib> 
#include <fstream> 
#include <sstream>  
#include "Assembler.h"
//#include "PCB.h"

using namespace std;

class form0
{
	public:
		unsigned carry:1;
		unsigned greater:1;
		unsigned equal:1;
		unsigned less:1;
		unsigned overflow:1;
		unsigned VM_RETURN_STATUS:3;
		unsigned IO_REGISTER:2;
		unsigned dontcare:6;
	};

class form1 
{
	public:
		unsigned UNUSED:6;
		unsigned RS:2;
		unsigned I:1;
		unsigned RD:2;
		unsigned OP:5;
};

class form2 
{
	public:
		unsigned ADDR:8;
		unsigned I:1;
		unsigned RD:2;
		unsigned OP:5;
};

class form3 
{
	public:
		int CONST:8;
		unsigned I:1;
		unsigned RD:2;
		unsigned OP:5;
};

union instruction 
{
	int i;
	form1 f1;
	form2 f2;
	form3 f3;
};

 union statusRegister
 {
	int i;
    form0 sr;
};

class VirtualMachine
{
	friend class os;
	public:
		typedef void (VirtualMachine::*FP)();
		VirtualMachine();
		void ldprog(string infile);	
		void execute();
		void runinstr(int inst);
		int checkNeg(int value);
		void checkCarry(int value);
		void shiftCarryRight(int value);
		void shiftCarryLeft(int value);
		void shiftRight(int value);
		void checkSign();
		void checkLimit();
		void load();
		void loadi();
		void store();
		void add();
		void addi();
		void addc();
		void addci();
		void sub();
		void subi();
		void subc();
		void subci();
		void And();
		void Andi();
		void Xor();
		void Xori();
		void Compl();
		void shl();
		void shla();
		void shr();
		void shra();
		void compr();
		void compri();
		void getstat();
		void putstat();
		void jump();
		void jumpl();
		void jumpe();
		void jumpg();
		void call();
		void Return();
		void read();
		void write();
		void halt();
		void noop();
	private:
		statusRegister statusreg;
		instruction inst;
		string infile, line, filename, ifname, outfname;
		static const int REG_FILE_SIZE = 4;
		static const int MEM_SIZE = 256;
		vector <int> reg, mem, opcodes;
		map <int,FP> instrIN;
		unsigned int address;
		int sp, pc, ir, carry, lim, lT, eq, gT;
		int rd, rs, constt, clk, objcode, OP, I, opcode, base, memLim, idleTime, timeSlice;
		const static int tick = 1;    
		const static int rwtick = 28; 
		const static int lscrtick = 4;
};
#endif
