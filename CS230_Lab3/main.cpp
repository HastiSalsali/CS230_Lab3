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
    
    
    cout << "Salsali, Hasti        CS230 Section 11091 May 5 \nThird Laboratory Assignment – Cache Simulation\n";
    
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
                OptionA(myCache);
                break;
            case 'B':
            case 'b':
                OptionB(myCache, continueMenu);
                break;
            default:
                cout << "\n- Command must be A, a, B, or b\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        
    };
    
    
}
//-----------------------------------------------------------------
int Memory::GetVal (int address) const{
    return memory[GetMemIndex(address)];
};

void Memory::SetVal (int address, int value){
    memory[GetMemIndex(address)] = value;
};
void Memory::Print (int address) const{
    cout << "Address: " << address << " "
    << "Memory: " << memory[GetMemIndex(address)] << " ";
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
        << " " << cache[line][1].GetVal(3) << " " << ((cache[line][0].validBit) ? 1 : -1 )<< " " << ((cache[line][1].validBit) ? 1 : -1 ) << " " << cache[line][0].dirtyBit << " " << cache[line][1].dirtyBit << "\n";
//    cout << "Cache: " << right << "\n"
//    << "\t" << setw(4) << cache[line][0].GetVal(0)
//        << setw(4) << cache[line][0].GetVal(1)
//        << setw(4) << cache[line][0].GetVal(2)
//        << setw(4) << cache[line][0].GetVal(3) << "\n"
//        << "\t" << setw(4) << cache[line][1].GetVal(0)
//        << setw(4) << cache[line][1].GetVal(1)
//        << setw(4) << cache[line][1].GetVal(2)
//        << setw(4) << cache[line][1].GetVal(3) << "\n"
//        << "Valid bits : " << ((cache[line][0].validBit) ? 1 : -1 )<< " " << ((cache[line][1].validBit) ? 1 : -1 ) << "\n"
//        << "Dirty bits: " << cache[line][0].dirtyBit << " " << cache[line][1].dirtyBit << "\n";
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
    cout << "\nEnter the address: \n";
    cin >> address;
    while (address < 0 || address > (BytesInMem - 1)) {
        cout << "Input address \"" << address << "\" invalid\n"
        "Enter the address: \n";
        cin >> address;
    }
    if (address % 4 != 0){
        cout << "Setting address to next lower multiple of 4\n";
        address -= address % 4;
    }

    cout << "Enter R to read memory or W to write memory: \n";
    cin >> typeOfA;
    if (typeOfA != 'R' && typeOfA != 'r' && typeOfA != 'W' && typeOfA != 'w'){
        cout << "- Request type must be R or W\n";
    }
    else if (typeOfA == 'W' || typeOfA == 'w'){
        cout << "Enter integer data to be written: \n";
        cin >> value;
        myCache.Write(address, value);
//        could be removed because not in instrucitons FIXME:
//        myCache.PrintMem(address);
//        myCache.Print(GetLine(address));
    }
    else if (typeOfA == 'R' || typeOfA == 'r'){
        myCache.Read(address);
        cout << "Value at address " << address << "-> " << myCache.GetValMain(address) << endl;
//        FIXME -> remove it:
//        myCache.PrintMem(address);
//        myCache.Print(GetLine(address));
    }
    else {
        cout << "Request type must be R or W\n";
    }
}

void OptionB (Cache &myCache, bool &continueMenu){
    int address;
    cout << "\nEnter the address: \n";
    cin >> address;
    if (address != -1) {
        while (address < 0 || address > (BytesInMem - 1)) {
            cout << "Input address \"" << address << "\" invalid\n"
            "Enter the address: \n";
            cin >> address;
        }
        
        if (address % 4 != 0){
            cout << "Setting address to next lower multiple of 4\n";
            address -= address % 4;
        }
        myCache.PrintMem(address);
        myCache.Print(GetLine(address));
    } else {
        continueMenu = false;
    }
};
