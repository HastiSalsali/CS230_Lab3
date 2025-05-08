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
const int BitsInMem = 32768 , BitsInCache = 2048 , CacheLineSize = 16, LinesInCache = 2048 / (2 * 16), SizeOfInt = 4 /*bytes*/ ;

class Memory {
private:
    //int *memory = new int[BitsInMem]; //Switch before submit
    int memory[BitsInMem];
public:
    Memory (){};
    
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
    void SetVal (int address, int newVal);
    
    int GetVal (int index) const {return val[index];}
    int GetAddress (int valIndex) const;
    bool TagHit (int address) const;
};

class Cache {
private:
    CacheLine cache [LinesInCache][2];
    Memory mainMem;
public:
    Cache();
    
    void Print(int row) const;
    int GetWay(int address) const;
    
    void CacheToMem (int line, int way);
    void MemToCache (int address, int way);
    void Write (int address, int value);
    void Read (int address);
};

int GetMemIndex (int address);
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
    myCache.Write(4, 1);
    myCache.Write(4, 2);
    myCache.Write(12, 3);
    myCache.Print(GetLine(12));
    myCache.Write(1024, 11);
    myCache.Write(1032, 22);
    myCache.Print(GetLine(1024));
    myCache.Write(3072, 111);
    myCache.Print(GetLine(3072));
    
    





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
    return memory[GetMemIndex(address)];
};

void Memory::SetVal (int address, int value){
    memory[GetMemIndex(address)] = value;
};
void Memory::Print (int address) const{
    cout << "Address: " << address << "\n"
    << "Memory: " << memory[GetMemIndex(address)] << "\n";
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
    if (!validBit){
        throw runtime_error("Calculating address of an invalid cache line");
    }
    int address = 0;
    address += valIndex;
    address += (line << 4);
    address += (tag << 10);
    return (address);
};

bool CacheLine::TagHit(int address) const{
    return (tag == GetTag(address));
};

void CacheLine::SetVal (int address, int newVal){
    tag = GetTag(address);
    val[GetMemIndex(GetOffset(address))] = newVal;
};


//-----------------------------------------------------------------
Cache::Cache(){
    for (int i=0; i < LinesInCache; i++){
        cache[i][0].SetLine(i);
        cache[i][1].SetLine(i);
    }
};
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
void Cache::CacheToMem (int line, int way){
    int address;
    
    if (cache[line][way].dirtyBit && cache[line][way].validBit){
        address = cache[line][way].GetAddress(0);
        mainMem.SetVal(address, cache[line][way].GetVal(0));
        address += SizeOfInt;
        mainMem.SetVal(address, cache[line][way].GetVal(1));
        address += SizeOfInt;
        mainMem.SetVal(address, cache[line][way].GetVal(2));
        address += SizeOfInt;
        mainMem.SetVal(address, cache[line][way].GetVal(3));
        address += SizeOfInt;
    }
    cache[line][way].dirtyBit = false;
};

void Cache::MemToCache (int address, int way){
    int line = GetLine(address);
    if (cache[line][way].dirtyBit && cache[line][way].validBit){
        CacheToMem(line, way);
    }
    cache[line][way].SetVal(address, mainMem.GetVal(address));
    address += SizeOfInt;
    cache[line][way].SetVal(address, mainMem.GetVal(address));
    address += SizeOfInt;
    cache[line][way].SetVal(address, mainMem.GetVal(address));
    address += SizeOfInt;
    cache[line][way].SetVal(address, mainMem.GetVal(address));
    address += SizeOfInt;
    
    cache[line][way].dirtyBit = false;
    cache[line][way].validBit = true;
};

void Cache::Write (int address, int value){
    int way = GetWay(address), line = GetLine(address);
    if (way == -1){
        way = rand() % 2;
        CacheToMem(line, way);
        MemToCache(address, way);
    }
    cache[line][way].SetVal(address, value);
    cache[line][way].dirtyBit = true;
};


void Cache::Read (int address){
    int way = GetWay(address), line = GetLine(address);
    if (way == -1){
        way = rand() % 2;
        CacheToMem(line, way);
        MemToCache(address, way);
    }
    cout << "->" << cache[line][way].GetVal(GetMemIndex(GetOffset(address))) << "\n";
};

//-----------------------------------------------------------------


int GetOffset (int address) {return address % 16;}; //  4 rightmost bits for offset
int GetLine (int address){return ((address / 16) % 64);};
int GetTag (int address){return address >> 10;};
int GetMemIndex (int address){return (address >> 2);}; //get the index of the position of the memory location within the array. ex : address 8 -> index 2 in the array

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

    }
}


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
//
