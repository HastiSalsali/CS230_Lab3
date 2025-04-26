// Salsali, Hasti        CS230 Section 15562 May 5
// Third Laboratory Assignment – Cache Simulation
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

const int BitsInMem = 32768 , BitsInCache = 2048 , CacheLineSize = 16, LinesInCache = 2048 / (2 * 16);
struct CacheSlots {
    int val = 0;
    int tag;
    bool dirtyBit = 0;
    bool validBit = 0;
};

int GetMemVal (int address, int memory[]);
int GetLine (int address);
int GetTag (int address);
int GetWay (CacheSlots cache[][2], int address);
void WriteToCache (CacheSlots cache[][2], int address, int val);
//-----------------------------------------------------------------
int main () {
    srand(time(0));
    int memory[BitsInMem] = {};
    CacheSlots cache [LinesInCache][2];
    
}
//-----------------------------------------------------------------

int GetMemVal (int address, int memory[]){
    return memory[address / 4];
}

int GetLine (int address){
    return ((address / 16) % 64);
};
int GetTag (int address){
    return address / 1024;
};

int GetWay (CacheSlots cache[][2], int address){
    int way = 3, line = GetLine(address), tag = GetTag(address);
    
    if (cache[line][0].tag == tag){
        way = 0;
    }
    else if (cache[line][1].tag == tag){
        way = 1;
    }
    return way;
};

void WriteToCache (CacheSlots cache[][2], int address, int val){
    int way = GetWay(cache, address), line = GetLine(address), tag = GetTag(address);
    
    if (way == 3){
        way = rand() % 2;
        //write cache to mem
    }
    cache[line][way].val = val;
    cache[line][way].tag = tag;
    cache[line][way].dirtyBit = true;
    cache[line][way].validBit = true;
};

void CacheToMem (CacheSlots cache[][2], int address){
    int way = GetWay(cache, address), line = GetLine(address), tag = GetTag(address);
}
