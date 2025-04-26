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
    int val[4] = {};
    int tag;
    bool dirtyBit = 0;
    bool validBit = 0;
};

int GetMemVal (int address, int memory[]);
int GetLine (int address);
int GetTag (int address);
int GetWay (CacheSlots cache[][2], int address);
void WriteToCache (CacheSlots cache[][2], int memory[], int address, int val);
void CacheToMem (CacheSlots cache[][2], int  memory[],  int line, int way);
void MemToCache (CacheSlots cache[][2], int  memory[], int address, int way);
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

void WriteToCache (CacheSlots cache[][2], int memory[], int address, int val){
    int way = GetWay(cache, address), line = GetLine(address), tag = GetTag(address), offset = address % 16;
    
    if (way == 3){
        way = rand() % 2;
        MemToCache(cache, memory, address, way);
    }
    cache[line][way].val[offset] = val;
    cache[line][way].tag = tag;
    cache[line][way].dirtyBit = true;
    cache[line][way].validBit = true;
};

void CacheToMem (CacheSlots cache[][2], int  memory[],  int line, int way){
    int address, memIndex;
    address = (cache[line][way].tag << 10) + (line << 4);
    memIndex = address >> 2;
    
    if (cache[line][way].dirtyBit == true){
        memory[memIndex] = cache[line][way].val[0];
        memory[memIndex + 1] = cache[line][way].val[1];
        memory[memIndex + 2] = cache[line][way].val[2];
        memory[memIndex + 3] = cache[line][way].val[3];
    }
    cache[line][way].dirtyBit = false;
}

void MemToCache (CacheSlots cache[][2], int  memory[], int address, int way){
    int line = GetLine(address), memIndex = address >> 2;
    
    
    if (way == 3){
        way = rand() % 2;
    }
    if (cache[line][way].dirtyBit == true){
        CacheToMem(cache, memory, line, way);
    }
    cache[line][way].val[0] = memory[memIndex];
    cache[line][way].val[1] = memory[memIndex + 1];
    cache[line][way].val[2] = memory[memIndex + 2];
    cache[line][way].val[3] = memory[memIndex + 3];
    
    cache[line][way].tag = GetTag(address);
    cache[line][way].dirtyBit = false;
    cache[line][way].validBit = true;
    
}
