#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "individual.h"
#include "hashtable.h"
#include <ctime>
using namespace std;


string gen_random(const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    string s ="";
    for (int i = 0; i < len; i++) {
        s += alphanum[rand() % (sizeof(alphanum) - 1)];
    } 
    return s;
}
int main(){
ifstream f("outfile.txt");
	if (f.fail()) {
		return 1;
	}
vector<individual> pol; 
string line;
int count  = 1; 
while(!f.eof()) {
    getline(f,line);
    switch(count) {
        case 1: pol.push_back(* new individual((int)line[0]-48)); break;
        case 2: pol.back().setK(line); break;
        case 3: pol.back().setH(line); break;
        case 4: pol.back().setR(line); break;
        case 5: pol.back().setF(line); count =0; break;
    }
    count++;
}
string list[1000];
for(int i = 0; i<1000; i++) {
    int len = rand()%30+1; 
    list[i] = gen_random(len);
}
ofstream a;
a.open("infile.txt");
for(int i =0; i < pol.size(); i++) {
    hashtable h(pol[i]);
     for(int j =0; j < 1000; j++) {
         h.insert(list[j]);
     }
     h.findCollisions();
     a << pol[i].performaceValues[0] <<pol[i].performaceValues[1] <<pol[i].performaceValues[2];
     a<<endl;
}
	
	a.close();
return 0;
}