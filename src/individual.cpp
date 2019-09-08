#include <iostream>
#include <stdint.h>
#include <string.h>
#include "individual.h"
using namespace std;
individual::individual(){}
individual::individual(int b){
    this->b = b; 
    for (int i = 0; i < 3; i++){
		performaceValues[i] = 0.0;
    }
}
uint32_t individual::getVal(char indicator, int index) {
    if(indicator =='k') {
        return kVals[index];
    }
    else if(indicator =='h') {
        return hVals[index];
    }
    else if(indicator == 'r') {
        return rVals[index];
    }
    else {
        return fVals[index];
    }
}
void individual::setK(string line){
   int size = (int)line[0];
   for(int i =1; i<size+1; i++) {
       kOps.push_back((int)line[i]);
   }
   string number = "";
   for(int i = size;i<line.length() + 1;i++) { 
       while(line[i] != ' ' && i < line.length()) {
           number += line[i];
		   i++;
       }
    kVals.push_back(stoi(number));
   }
}
void individual::setH(string line){
int size = (int)line[0];
   for(int i =1; i<size+1; i++) {
       hOps.push_back((int)line[i]);
   }
   string number = "";
   for(int i = size;i<line.length() + 1;i++) { 
       while(line[i] != ' '&& i < line.length()) {
           number += line[i];
		   i++;
       }
    hVals.push_back(stoi(number));
   }
}
void individual::setR(string line){
int size = (int)line[0];
   for(int i =1; i<size+1; i++) {
       rOps.push_back((int)line[i]);
	   i++;
   }
   string number = "";
   for(int i = size;i<line.length() + 1;i++) { 
       while(line[i] != ' '&& i<line.length()) {
           number += line[i];
		   i++;
       }
    rVals.push_back(stoi(number));
   }
}
void individual::setF(string line){
int size = (int)line[0];
   for(int i =1; i<size+1; i++) {
       fOps.push_back((int)line[i]);
   }
   string number = "";
   for(int i = size;i<line.length() + 1;i++) { 
       while(line[i] != ' '&& i< line.length()) {
           number += line[i];
		   i++;
       }
    fVals.push_back(stoi(number));
   }
}

uint32_t individual::preformOperation(Operation op,uint32_t value, int index, char list){
    uint32_t val = getVal(list,index);
    switch(op) {
        case AND : return value & val; break;
        case OR : return value | val; break;
        case NOT : return ~ value; break;
        case XOR : return value ^ val; break;
        case LEFT : return value << val%32; break;
        case RIGHT: return value >> val%32; break;
        case ADD : return value + val; break;
        case SUBTRACT : return value - val; break;
    }
}
uint32_t individual::kOp(uint32_t k) {
    for(int i=0; i<kOps.size();i++) {
        k = preformOperation((Operation)kOps[i],k,i,'k'); 
    }
	return k;
}
uint32_t individual::hOp(uint32_t h) {
    for(int i=0; i<hOps.size();i++) {
        h = preformOperation((Operation)hOps[i],h,i,'h'); 
    }
	return h; 
}
uint32_t individual::rOp(uint32_t r) {
    for(int i=0; i<rOps.size();i++) {
        r = preformOperation((Operation)rOps[i],r,i,'r'); 
    }
	return r; 
}
uint32_t individual::fOp(uint32_t f) {
    for(int i=0; i<fOps.size();i++) {
        f = preformOperation((Operation)fOps[i],f,i,'f'); 
    }
	return f; 
}

uint32_t individual::hash(const uint8_t* key, size_t len, uint32_t seed) {
uint32_t h = seed;
if (len > b) {
		size_t i = len >> 2;
		do {
			uint32_t k;
			memcpy(&k, key, sizeof(uint32_t));
			key += sizeof(uint32_t);
			k = kOp(k);
            h ^= k;
			h = hOp(h);
		} while (--i);
	}
    if (len & b) {
		size_t i = len & 3;
		uint32_t k = 0;
		do {
			k <<= 8;
			k |= key[i - 1];
		} while (--i);
		rOp(k);
         h ^= k;
	}
    return fOp(h);
}
