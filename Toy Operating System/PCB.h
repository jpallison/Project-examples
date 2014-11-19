#ifndef PCB_H
#define PCB_H

#include <map>
#include <list>
#include <queue>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <stdexcept>
#include "VirtualMachine.h"
#include "Assembler.h"
//#include "os.h"

using namespace std;
union statusRegister;
class os;
class Assembler;
class PCB
{
    friend class Assembler;
    friend class os;
public:
    PCB(string filename);
private:
    int pc, sp, pbase, limit, ioClk;
    statusRegister sreg;
    vector<int> r;
    string procsName, ofname, ifname, outfname, stfname;
    fstream ofile, ifile, outfile, stfile;
    
    int VMCPUtime, waitTime, TurnaroundTime, IOtime, largestStackSize, crtClk;
};

#endif