#include "PCB.h"

using namespace std;

PCB::PCB(string filename)
{
    sreg.i = 0;
    sp = 256;
    pbase = 0;
    pc = pbase;
    limit = 0;	
    r = vector<int>(4);
    r[0] = 0;
    r[1] = 0;
    r[2] = 0;
    r[3] = 0;
    procsName = filename.substr(0, filename.size() - 1);
    ofname = procsName + 'o';
    ifname = procsName + "in";
    outfname = procsName + "out";
    stfname = procsName + "st";
    
    VMCPUtime = 0;
    waitTime = 0;
    TurnaroundTime = 0;
    IOtime = 0;
    largestStackSize = 0;
    crtClk = 0;
}