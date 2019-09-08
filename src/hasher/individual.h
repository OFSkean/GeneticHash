#pragma once



#include <stdint.h>
#include <vector>
#include <string>
using namespace std; 

class individual {
public:
enum Operation{AND,OR,NOT,XOR,LEFT,RIGHT,ADD,SUBTRACT};
individual();
individual (int b);
float performaceValues[3]; 
int b; 
vector<int> kOps;
vector<int> hOps;
vector<int> rOps;
vector<int> fOps;
vector<uint32_t> kVals;
vector<uint32_t> hVals;
vector<uint32_t> rVals;
vector<uint32_t> fVals;
uint32_t getVal(char indicator, int index);
uint32_t preformOperation(Operation op,uint32_t value, int index, char list);
uint32_t kOp(uint32_t k);
uint32_t hOp(uint32_t h);
uint32_t rOp(uint32_t r);
uint32_t fOp(uint32_t f);
uint32_t hash(const uint8_t* key, size_t len, uint32_t seed);
void setK(string line);
void setH(string line);
void setR(string line);
void setF(string line);
};
