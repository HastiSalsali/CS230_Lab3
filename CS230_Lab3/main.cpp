// Salsali, Hasti        CS230 Section 11091 May 5
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
    int tag = -1;
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
void OptionA (CacheSlots cache[][2], int memory[]);
void print (CacheSlots cache[][2], int memory[], int address);
//-----------------------------------------------------------------
int main () {
    srand(time(0));
    int memory[BitsInMem] = {};
    CacheSlots cache [LinesInCache][2];
    char menuOpt = 'A';
    bool continueMenu = true;
    
    cout << "Salsali, Hasti        CS230 Section 11091 May 5 \nThird Laboratory Assignment – Cache Simulation\n";
    
    print(cache, memory, 0);
    WriteToCache(cache, memory, 0, 1);
    print(cache, memory, 0);
    WriteToCache(cache, memory, 4, 2);
    print(cache, memory, 4);
    WriteToCache(cache, memory, 4, 3);
    print(cache, memory, 0);
    print(cache, memory, 4);
    WriteToCache(cache, memory, 15, 4);
    print(cache, memory, 0);
    print(cache, memory, 4);
    print(cache, memory, 15);
    





    /*
    while (continueMenu){
        cout << "Enter a command, A or B: ";
        cin >> menuOpt;
        if (!cin){
            cin.clear();
            menuOpt = 'c';
        }
        switch (menuOpt){
            case 'A':
            case 'a':
                OptionA(cache, memory);
                break;
            case 'B':
            case 'b':
                break;
            default:
                cout << "- Command must be A, a, B, or b\n";
        }
        
    };
    */
    
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
    int way = GetWay(cache, address), line = GetLine(address), tag = GetTag(address), offset = (address % 16) / 4;
    
    if (way == 3){
        way = rand() % 2;
        MemToCache(cache, memory, address, way);
    }
    cache[line][way].val[offset] = val;
    cache[line][way].tag = tag;
    cache[line][way].dirtyBit = true;
};

void CacheToMem (CacheSlots cache[][2], int  memory[],  int line, int way){
    int address, memIndex;
    address = (cache[line][way].tag << 10) + (line << 4);
    memIndex = address >> 2;
    
    if (cache[line][way].dirtyBit){
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
    } //why tf would you?
    if (cache[line][way].dirtyBit){
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

void print (CacheSlots cache[][2], int memory[], int address){
    int line = GetLine(address);
    int memIndex = address >> 2;
    
    cout << "Address: " << address << "\n"
    << "Memory: " << memory[memIndex] << "\n"
    << "Cache: " << right << "\n"
    << "\t" << setw(4) << cache[line][0].val[0]
    << setw(4) << cache[line][0].val[1]
    << setw(4) << cache[line][0].val[2]
    << setw(4) << cache[line][0].val[3] << "\n"
    << "\t" << setw(4) << cache[line][1].val[0]
    << setw(4) << cache[line][1].val[1]
    << setw(4) << cache[line][1].val[2]
    << setw(4) << cache[line][1].val[3] << "\n"
    << "Valid bits : " << ((cache[line][0].validBit == 0) ? -1 : 0 )<< " " << ((cache[line][1].validBit == 0) ? -1 : 0 ) << "\n"
    << "Dirty bits: " << cache[line][0].dirtyBit<< " " << cache[line][1].dirtyBit << "\n";
    //Address: xxxxxxxx
    //memory:nnnnnnnn
    //cache:
    //  mmmm
    //  mmmm
    //Valid bits: k k
    //Dirty bits: k k
}

void OptionA (CacheSlots cache[][2], int memory[]){
    int address, data;
    char typeOfA;
    cout << "Enter the address: ";
    cin >> address;
    if (address % 4 != 0){
        cout << "Setting address to next lower multiple of 4\n";
        address -= address % 4;
    }
        
    cout << "Enter R to read memory or W to write memory: ";
    cin >> typeOfA;
    if (typeOfA != 'R' && typeOfA != 'r' && typeOfA != 'W' && typeOfA != 'w'){
        cout << "- Request type must be R or W\n";
    }
    else if (typeOfA == 'W' || typeOfA == 'w'){
        cout << "Enter integer data to be written: ";
        cin >> data;
        WriteToCache(cache, memory, address, data);
        print(cache, memory, address);
    }
    else {
        
        ///////
    }
}
