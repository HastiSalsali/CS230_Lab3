// Salsali, Hasti        CS230 Section 15562 May 5
// Third Laboratory Assignment – Cache Simulation
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
using namespace std;

const int BitsInMem = 32768 , BitsInCache = 2048 , CacheLineSize = 16, LinesInCache = 2048 / (2 * 16);
struct CacheSlots {
    int val = 0;
    bool dirtyBit = 0;
    bool validBit = 0;
};

int GetMemVal (int address, int memory[]);
void WriteCache (int address, int cache[][2]);
int FindLine (int address);

int main () {
    int memory[BitsInMem] = {};
    CacheSlots cache [LinesInCache][2];
    
}

int GetMemVal (int address, int memory[]){
    return memory[address / 4];
}
void WriteCache (int address, int cache[][2]){
    
};
int FindLine (int address){
    return (add)
};
