#pragma once


#include <string>
#include "node.h"
#include "individual.h"
using namespace std;
class hashtable {
public:
hashtable(individual i);
node* search(string lex);
node* insert(string newlex);
node* bucket[1000]; 
int numCollisions;
int hash(string s);
individual i;
void findCollisions();
};