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
//Switch before submit
const int BitsInMem = 32768 , BitsInCache = 2048 , CacheLineSize = 16, LinesInCache = 2048 / (2 * 16);

class Memory {
private:
    //int *memory = new int[BitsInMem]; //Switch before submit
    int memory[BitsInMem];
public:
    Memory (){};
    
    int GetInex (int address) const;
    int GetVal (int address) const;
    void Print (int address) const;
    
    void SetVal (int address, int value);
    
    ~Memory();
};

class CacheLine{
private:
    int val[4] = {};
    int tag = -1;
    int line = -1;
    
    
public:
    bool dirtyBit = 0;
    bool validBit = 0;
    
    CacheLine();
    
    void SetLine (int lineNum);
    void SetTag (int address);
    void WriteVal (int address, int newVal);
    
    int GetVal (int index) const {return val[index];}
    int GetAddress (int valIndex) const;
    bool TagHit (int address) const;
};

class Cache {
private:
    CacheLine cache [LinesInCache][2];
    Memory mainMem;
public:
    Cache(){};
    
    void Print(int row) const;
    int GetWay(int address) const;
};

int GetLine (int address);
int GetTag (int address);
int GetOffset (int address);
//int GetWay (CacheSlots cache[][2], int address);
//void WriteToCache (CacheSlots cache[][2], int memory[], int address, int val);
//void CacheToMem (CacheSlots cache[][2], int  memory[],  int line, int way);
//void MemToCache (CacheSlots cache[][2], int  memory[], int address, int way);
//void OptionA (CacheSlots cache[][2], int memory[]);
//void print (CacheSlots cache[][2], int memory[], int address);
//-----------------------------------------------------------------
int main () {
    srand(time(0));
    char menuOpt = 'A';
    bool continueMenu = true;
    Cache myCache;
    
    cout << "Salsali, Hasti        CS230 Section 11091 May 5 \nThird Laboratory Assignment – Cache Simulation\n";
    
    myCache.Print(0);
    





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
int Memory::GetVal (int address) const{
    return memory[GetInex(address)];
};
int Memory::GetInex (int address) const{
    return (address << 1);
};

void Memory::SetVal (int address, int value){
    memory[GetInex(address)] = value;
};
void Memory::Print (int address) const{
    cout << "Address: " << address << "\n"
    << "Memory: " << memory[GetInex(address)] << "\n";
};

Memory::~Memory(){
    //delete []memory; //Switch before submit
};
//-----------------------------------------------------------------
CacheLine::CacheLine(){
};
void CacheLine::SetLine (int lineNum){
    if (lineNum >= LinesInCache) {
        throw runtime_error("Line was out of range");
    }
    line = lineNum;
};

void CacheLine::SetTag(int address){
    tag = address >> 10;
}

int CacheLine::GetAddress (int valIndex) const{
    return (valIndex + (line << 4) + (tag << 10));
};

bool CacheLine::TagHit(int address) const{
    return (tag == GetTag(address));
};

void CacheLine::WriteVal (int address, int newVal){
    if (tag != GetTag(address)){
        throw runtime_error("Writing val in the wrong line");
    }
    tag = GetTag(address);
    val[GetOffset(address) << 1] = newVal;
};

//-----------------------------------------------------------------
void Cache::Print(int line) const{
    cout << "Cache: " << right << "\n"
    << "\t" << setw(4) << cache[line][0].GetVal(0)
        << setw(4) << cache[line][0].GetVal(1)
        << setw(4) << cache[line][0].GetVal(2)
        << setw(4) << cache[line][0].GetVal(3) << "\n"
        << "\t" << setw(4) << cache[line][1].GetVal(0)
        << setw(4) << cache[line][1].GetVal(1)
        << setw(4) << cache[line][1].GetVal(2)
        << setw(4) << cache[line][1].GetVal(3) << "\n"
        << "Valid bits : " << ((cache[line][0].validBit) ? 1 : -1 )<< " " << ((cache[line][1].validBit) ? 1 : -1 ) << "\n"
        << "Dirty bits: " << cache[line][0].dirtyBit << " " << cache[line][1].dirtyBit << "\n";
};

int Cache::GetWay(int address) const{
    int way = -1;
    if (cache[GetLine(address)][0].TagHit(address)){
            way = 0;
        }
        else if (cache[GetLine(address)][1].TagHit(address)){
            way = 1;
        }
        return way;
};
//-----------------------------------------------------------------


int GetOffset (int address) {return address % 16;};
int GetLine (int address){return ((address / 16) % 64);};
int GetTag (int address){return address / 1024;};



//int GetMemVal (int address, int memory[]){
//    return memory[address / 4];
//}
//

//int GetWay (CacheSlots cache[][2], int address){
//    int way = 3, line = GetLine(address), tag = GetTag(address);
//    
//    if (cache[line][0].tag == tag){
//        way = 0;
//    }
//    else if (cache[line][1].tag == tag){
//        way = 1;
//    }
//    return way;
//};
//
//void WriteToCache (CacheSlots cache[][2], int memory[], int address, int val){
//    int way = GetWay(cache, address), line = GetLine(address), tag = GetTag(address), offset = (address % 16) / 4;
//    
//    if (way == 3){
//        way = rand() % 2;
//        MemToCache(cache, memory, address, way);
//    }
//    cache[line][way].val[offset] = val;
//    cache[line][way].tag = tag;
//    cache[line][way].dirtyBit = true;
//};
//
//void CacheToMem (CacheSlots cache[][2], int  memory[],  int line, int way){
//    int address, memIndex;
//    address = (cache[line][way].tag << 10) + (line << 4);
//    memIndex = address >> 1;
//    
//    if (cache[line][way].dirtyBit){
//        memory[memIndex] = cache[line][way].val[0];
//        memory[memIndex + 1] = cache[line][way].val[1];
//        memory[memIndex + 2] = cache[line][way].val[2];
//        memory[memIndex + 3] = cache[line][way].val[3];
//    }
//    cache[line][way].dirtyBit = false;
//}
//
//void MemToCache (CacheSlots cache[][2], int  memory[], int address, int way){
//    int line = GetLine(address), memIndex = address >> 1;
//    
//    
//    if (way == 3){
//        way = rand() % 2;
//    } //why tf would you?
//    if (cache[line][way].dirtyBit){
//        CacheToMem(cache, memory, line, way);
//    }
//    cache[line][way].val[0] = memory[memIndex];
//    cache[line][way].val[1] = memory[memIndex + 1];
//    cache[line][way].val[2] = memory[memIndex + 2];
//    cache[line][way].val[3] = memory[memIndex + 3];
//    
//    cache[line][way].tag = GetTag(address);
//    cache[line][way].dirtyBit = false;
//    cache[line][way].validBit = true;
//    
//}
//
//void print (CacheSlots cache[][2], int memory[], int address){
//    int line = GetLine(address);
//    int memIndex = address >> 1;
//    
//    cout << "Address: " << address << "\n"
//    << "Memory: " << memory[memIndex] << "\n"
//    << "Cache: " << right << "\n"
//    << "\t" << setw(4) << cache[line][0].val[0]
//    << setw(4) << cache[line][0].val[1]
//    << setw(4) << cache[line][0].val[2]
//    << setw(4) << cache[line][0].val[3] << "\n"
//    << "\t" << setw(4) << cache[line][1].val[0]
//    << setw(4) << cache[line][1].val[1]
//    << setw(4) << cache[line][1].val[2]
//    << setw(4) << cache[line][1].val[3] << "\n"
//    << "Valid bits : " << ((cache[line][0].validBit == 0) ? -1 : 0 )<< " " << ((cache[line][1].validBit == 0) ? -1 : 0 ) << "\n"
//    << "Dirty bits: " << cache[line][0].dirtyBit<< " " << cache[line][1].dirtyBit << "\n";
//    //Address: xxxxxxxx
//    //memory:nnnnnnnn
//    //cache:
//    //  mmmm
//    //  mmmm
//    //Valid bits: k k
//    //Dirty bits: k k
//}
//
//void OptionA (CacheSlots cache[][2], int memory[]){
//    int address, data;
//    char typeOfA;
//    cout << "Enter the address: ";
//    cin >> address;
//    if (address % 4 != 0){
//        cout << "Setting address to next lower multiple of 4\n";
//        address -= address % 4;
//    }
//        
//    cout << "Enter R to read memory or W to write memory: ";
//    cin >> typeOfA;
//    if (typeOfA != 'R' && typeOfA != 'r' && typeOfA != 'W' && typeOfA != 'w'){
//        cout << "- Request type must be R or W\n";
//    }
//    else if (typeOfA == 'W' || typeOfA == 'w'){
//        cout << "Enter integer data to be written: ";
//        cin >> data;
//        WriteToCache(cache, memory, address, data);
//        print(cache, memory, address);
//    }
//    else {
//        
//        ///////
//    }
//}
