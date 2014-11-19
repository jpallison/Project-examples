#ifndef OS_H
#define OS_H

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
#include <cstdio>
#include "Assembler.h"
#include "VirtualMachine.h"
#include "PCB.h"

using namespace std;

class os
{
	friend class PCB;
	friend class VirtualMachine;
public:
    os();
    void run();
private:
    Assembler assm;
    VirtualMachine vm;
    vector<string> progs;
    list<PCB *> jobs;
    queue<PCB *> readyQ, waitQ;
    PCB * running;
    
    int sysTime, ctxtSwitchTime, jbsCPUtime, numProcsCmpltd;
    double sysCPUutil, usrCPUutil, thrghput;
    
    void ctxtSwitch();

};

#endif