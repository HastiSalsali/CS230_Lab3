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
const int BytesInMem = 32768 , BitsInCache = 2048 , CacheLineSize = 16, LinesInCache = 2048 / (2 * 16), SizeOfInt = 4 /*bytes*/ ;

class Memory {
private:
    int memory[BytesInMem];
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
    void PrintMem(int address) const;
    
    void CacheToMem (int line, int way);
    void MemToCache (int address, int way);
    void Write (int address, int value);
    void Read (int address);
    int GetValMain (int address) { return mainMem.GetVal(address);}
};

int GetMemIndex (int address);
int GetLine (int address);
int GetTag (int address);
int GetOffset (int address);
void OptionA (Cache &myCache);
void OptionB (Cache &myCache, bool &continueMenu);

//-----------------------------------------------------------------
int main () {
    srand(time(0));
    char menuOpt = 'A';
    bool continueMenu = true;
    Cache myCache;
    
    
    cout << "Salsali, Hasti        CS230 Section 11091 May 5 \nThird Laboratory Assignment – Cache Simulation\n"
    "--- OptionB output format:\n"
    "--- Address: xxxxxxxx memory:nnnnnnnn cache:mmmm mmmm v v d d\n"
    "--- v: valid bit, d: dirty bit"<< endl;
    
    while (continueMenu){
        cout << "\n\nEnter a command, A or B: " << endl;
        cin >> menuOpt;
        if (!cin){
            cin.clear();
            menuOpt = 'c';
        }
        switch (menuOpt){
            case 'A':
            case 'a':
                OptionA(myCache);
                break;
            case 'B':
            case 'b':
                OptionB(myCache, continueMenu);
                break;
            default:
                cout << "\n- Command must be A, a, B, or b" << endl;
                cin.clear(10);
                break;
        }
        
    };
    
    cout << "\nEnd of cache program!";
    
}
//-----------------------------------------------------------------
int Memory::GetVal (int address) const{
    return memory[GetMemIndex(address)];
};

void Memory::SetVal (int address, int value){
    memory[GetMemIndex(address)] = value;
};
void Memory::Print (int address) const{
    int lowerBlockAdd = (address / 16) * 16;
    cout << "Address: " << address << " "
    << "Memory: " << memory[GetMemIndex(lowerBlockAdd)] << " "
    << memory[GetMemIndex(lowerBlockAdd+ 4)] << " "
    << memory[GetMemIndex(lowerBlockAdd+ 8)] << " "
    << memory[GetMemIndex(lowerBlockAdd+ 12)] << " ";
//    cout << "Address: " << address << "\n"
//    << "Memory: " << memory[GetMemIndex(address)] << "\n";
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
    cout << "Cache: " << cache[line][0].GetVal(0)
        << " " << cache[line][0].GetVal(1)
        << " " << cache[line][0].GetVal(2)
        << " " << cache[line][0].GetVal(3) << " " << cache[line][1].GetVal(0)
        << " " << cache[line][1].GetVal(1)
        << " " << cache[line][1].GetVal(2)
        << " " << cache[line][1].GetVal(3) << " " << ((cache[line][0].validBit) ? 1 : -1 )<< " " << ((cache[line][1].validBit) ? 1 : -1 ) << " " << cache[line][0].dirtyBit << " " << cache[line][1].dirtyBit << endl;
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
    cout << "Value at address " << address << "-> " << cache[line][way].GetVal(GetMemIndex(GetOffset(address))) << endl;
};

void Cache::PrintMem(int address) const{
    mainMem.Print(address);
};

//-----------------------------------------------------------------


int GetOffset (int address) {return address % 16;}; //  4 rightmost bits for offset
int GetLine (int address){return ((address / 16) % 64);};
int GetTag (int address){return address >> 10;};
int GetMemIndex (int address){return (address >> 2);}; //get the index of the position of the memory location within the array. ex : address 8 -> index 2 in the array

void OptionA (Cache &myCache){
    int address, value;
    char typeOfA;
    cout << "Enter the address: " << endl;
    cin >> address;
    while (address < 0 || address > (BytesInMem - 1)) {
        cout << "Input address \"" << address << "\" invalid\n"
        "Enter the address: " << endl;
        cin >> address;
    }
    if (address % 4 != 0){
        cout << "Setting address to next lower multiple of 4" << endl;
        address -= address % 4;
    }

    cout << "Enter R to read memory or W to write memory: " << endl;
    cin >> typeOfA;
    if (typeOfA != 'R' && typeOfA != 'r' && typeOfA != 'W' && typeOfA != 'w'){
        cout << "- Request type must be R or W" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    else if (typeOfA == 'W' || typeOfA == 'w'){
        cout << "Enter integer data to be written: " << endl;
        cin >> value;
        myCache.Write(address, value);
//        could be removed because not in instrucitons FIXME:
//        myCache.PrintMem(address);
//        myCache.Print(GetLine(address));
    }
    else if (typeOfA == 'R' || typeOfA == 'r'){
        myCache.Read(address);
        
//        FIXME -> remove it:
//        myCache.PrintMem(address);
//        myCache.Print(GetLine(address));
    }
    else {
        cout << "Request type must be R or W" << endl;
    }
}

void OptionB (Cache &myCache, bool &continueMenu){
    int address;
    cout << "Enter the address: " << endl;
    cin >> address;
    if (address != -1) {
        while (address < 0 || address > (BytesInMem - 1)) {
            cout << "Input address \"" << address << "\" invalid\n"
            "Enter the address: " << endl;
            cin >> address;
        }
        
        if (address % 4 != 0){
            cout << "Setting address to next lower multiple of 4" << endl;
            address -= address % 4;
        }
        myCache.PrintMem(address);
        myCache.Print(GetLine(address));
    } else {
        continueMenu = false;
    }
};
// Salsali, Hasti        CS230 Section 11091 May 5
//Third Laboratory Assignment – Cache Simulation
//--- OptionB output format:
//--- Address: xxxxxxxx memory:nnnnnnnn cache:mmmm mmmm v v d d
//--- v: valid bit, d: dirty bit
//
//
//Enter a command, A or B:
//A 0 W 22222222
//Enter the address:
//Enter R to read memory or W to write memory:
//Enter integer data to be written:
//
//
//Enter a command, A or B:
//A 8 W 1234
//Enter the address:
//Enter R to read memory or W to write memory:
//Enter integer data to be written:
//
//
//Enter a command, A or B:
//A 32 W 1233
//Enter the address:
//Enter R to read memory or W to write memory:
//Enter integer data to be written:
//
//
//Enter a command, A or B:
//A 16 W 888
//Enter the address:
//Enter R to read memory or W to write memory:
//Enter integer data to be written:
//
//
//Enter a command, A or B:
//B 0
//Enter the address:
//Address: 0 Memory: 0 0 0 0 Cache: 0 0 0 0 22222222 0 1234 0 -1 1 0 1
//
//
//Enter a command, A or B:
//A 4 W 33333333
//Enter the address:
//Enter R to read memory or W to write memory:
//Enter integer data to be written:
//
//
//Enter a command, A or B:
//B 8
//Enter the address:
//Address: 8 Memory: 0 0 0 0 Cache: 0 0 0 0 22222222 33333333 1234 0 -1 1 0 1
//
//
//Enter a command, A or B:
//A 1028 W 3231
//Enter the address:
//Enter R to read memory or W to write memory:
//Enter integer data to be written:
//
//
//Enter a command, A or B:
//A 16388 W 5657
//Enter the address:
//Enter R to read memory or W to write memory:
//Enter integer data to be written:
//
//
//Enter a command, A or B:
//b 0
//Enter the address:
//Address: 0 Memory: 22222222 33333333 1234 0 Cache: 0 5657 0 0 0 3231 0 0 1 1 1 1
//
//
//Enter a command, A or B:
//B 4
//Enter the address:
//Address: 4 Memory: 22222222 33333333 1234 0 Cache: 0 5657 0 0 0 3231 0 0 1 1 1 1
//
//
//Enter a command, A or B:
//B 8
//Enter the address:
//Address: 8 Memory: 22222222 33333333 1234 0 Cache: 0 5657 0 0 0 3231 0 0 1 1 1 1
//
//
//Enter a command, A or B:
//B 12
//Enter the address:
//Address: 12 Memory: 22222222 33333333 1234 0 Cache: 0 5657 0 0 0 3231 0 0 1 1 1 1
//
//
//Enter a command, A or B:
//B 16
//Enter the address:
//Address: 16 Memory: 0 0 0 0 Cache: 0 0 0 0 888 0 0 0 -1 1 0 1
//
//
//Enter a command, A or B:
//B 1024
//Enter the address:
//Address: 1024 Memory: 0 0 0 0 Cache: 0 5657 0 0 0 3231 0 0 1 1 1 1
//
//
//Enter a command, A or B:
//B 1028
//Enter the address:
//Address: 1028 Memory: 0 0 0 0 Cache: 0 5657 0 0 0 3231 0 0 1 1 1 1
//
//
//Enter a command, A or B:
//B 16384
//Enter the address:
//Address: 16384 Memory: 0 0 0 0 Cache: 0 5657 0 0 0 3231 0 0 1 1 1 1
//
//
//Enter a command, A or B:
//B 16388
//Enter the address:
//Address: 16388 Memory: 0 0 0 0 Cache: 0 5657 0 0 0 3231 0 0 1 1 1 1
//
//
//Enter a command, A or B:
//b
//Enter the address: -
//1
//
//End of cache program!Program ended with exit code: 0



/*
 a 0 w 111
 a 8 w 888
 a 1024 w 2222
 a 16388 w 444
 
 */
