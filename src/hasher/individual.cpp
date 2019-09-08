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
   int i =0;
   string siz ="";
   string number = "";
   while(line[i]!=' '){siz +=line[i]; i++;}
    int size = stoi(siz);
    i++;
     for(i; i<line.length(); i++) {
       kOps.push_back((int)line[i]-48);
       i+=2;
       while(line[i] != ' ' && i < line.length()) {
           number+= line[i];
		   i++;
       }
       string::size_type sc; 
       kVals.push_back(stoi(number,&sc));
       number ="";
   }
   
}
void individual::setH(string line){
   int i =0;
   string siz ="";
   string number = "";
   while(line[i]!=' '){ i++;}
    i++; 
     for(i; i<line.length(); i++) {
       hOps.push_back((int)line[i]-48);
       i+=2;
       while(line[i] != ' ' && i < line.length()) {
           number+= line[i];
		   i++;
       }
       string::size_type sc; 
       hVals.push_back(stoi(number,&sc));
       number ="";
   }
}
void individual::setR(string line){
     int i =0;
   string siz ="";
   string number = "";
   while(line[i]!=' '){ i++;}
    i++; 
     for(i; i<line.length(); i++) {
       rOps.push_back((int)line[i]-48);
       i+=2;
        while(line[i] != ' ' && i < line.length()) {
           number+= line[i];
		   i++;
       }
       string::size_type sc; 
       rVals.push_back(stoi(number,&sc));
       number ="";
   }
}
void individual::setF(string line){
      int i =0;
   string siz ="";
   string number = "";
   while(line[i]!=' '){i++;}
    i++; 
     for(i; i<line.length(); i++) {
       fOps.push_back((int)line[i]-48);
       i+=2;
       while(line[i] != ' ' && i < line.length()) {
           number+= line[i];
		   i++;
       }
       string::size_type sc; 
       fVals.push_back(stoi(number,&sc));
       number ="";
   }
}

uint32_t individual::preformOperation(int op,uint32_t value, int index, char list){
    uint32_t val = getVal(list,index);
    switch(op) {
        case 0 : return value & val; break;
        case 1 : return value | val; break;
        case 2 : return ~ value; break;
        case 3 : return value ^ val; break;
        case 4 : return value << val%32; break;
        case 5: return value >> val%32; break;
        case 6 : return value + val; break;
        case 7 : return value - val; break;
    }
}
uint32_t individual::kOp(uint32_t k) {
    for(int i=0; i<kOps.size();i++) {
        k = preformOperation(kOps[i],k,i,'k'); 
    }
	return k;
}
uint32_t individual::hOp(uint32_t h) {
    for(int i=0; i<hOps.size();i++) {
        h = preformOperation(hOps[i],h,i,'h'); 
    }
	return h; 
}
uint32_t individual::rOp(uint32_t r) {
    for(int i=0; i<rOps.size();i++) {
        r = preformOperation(rOps[i],r,i,'r'); 
    }
	return r; 
}
uint32_t individual::fOp(uint32_t f) {
    for(int i=0; i<fOps.size();i++) {
        f = preformOperation(fOps[i],f,i,'f'); 
    }
	return f; 
}

uint32_t individual::hash(const uint8_t* key, size_t len, uint32_t seed) {
uint32_t h = seed;
if (len > b) {
		size_t i = len/b;
		do {
			uint32_t k = 0;
			memcpy(&k, key, (size_t)b);
			key += sizeof(uint32_t);
			k = kOp(k);
            h ^= k;
			h = hOp(h);
		} while (--i);
	}
    if (len & b) {
		size_t i = len & b; 
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
