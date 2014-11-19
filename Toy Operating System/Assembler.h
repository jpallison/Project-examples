#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include <iostream>
#include <string>	
#include <map>
#include <vector>
#include <stdexcept>
#include <cstdlib> 
#include <fstream> 
#include <sstream>  
#include "PCB.h"
//#include "os.h"
#include "VirtualMachine.h"

using namespace std;


class NullPointerException: public runtime_error {
	public:
		NullPointerException(): runtime_error("Null Pointer!") { }
};

class Assembler
{
	friend class PCB;
	public:
		typedef int (Assembler::*FP)();
		int glbBase;
		int load();
		int loadi();
		int store();
		int add();
		int addi();
		int addc();
		int addci();
		int sub();
		int subi();
		int subc();
		int subci();
		int And();
		int Andi();
		int Xor();
		int Xori();
		int Compl();
		int shl();
		int shla();
		int shr();
		int shra();
		int compr();
		int compri();
		int getstat();
		int putstat();
		int jump();
		int jumpl();
		int jumpe();
		int jumpg();
		int call();
		int Return();
		int read();
		int write();
		int halt();
		int noop();


		Assembler(); 
		void assemble(string infile, PCB * p);
		void assembleOut();
		void checkNegative(int &checkNegIn);
		void checkSize(int checkSize);
		void checkRegister();
	private:
		// the map
		map<string, FP> instr;
		int rd, rs, constant, temp;
		unsigned int address;
		fstream Program, assembleOutStream;
		string infile, line, opcode, filename;
		vector<int> vecout;

};

#endif
