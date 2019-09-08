#include "hashtable.h"
#include <ctime>
float hammingDistance(int n1, int n2) 
{ 
    int x = n1 ^ n2; 
    float setBits = 0; 
  
    while (x > 0) { 
        setBits += x & 1; 
        x >>= 1; 
    } 
  
    return setBits; 
} 
hashtable::hashtable(individual i){
    for (int i = 0; i <1000; i++)
		bucket[i] = NULL;
    this->i = i; 
}
node* hashtable::insert(string newlex){
    node* s = search(newlex);
	if (!s) {
		int hv = hash(newlex);
		s = new node;
		s->value=newlex;
		s->next = bucket[hv];
		bucket[hv] = s;
	}
	return s;
} 
node* hashtable::search(string lex){
    int hv = hash(lex);
	node* p = bucket[hv];
	while (p) {
		if (p->value == lex)
			return p;
		p = p->next;
	}
	return NULL;
}
int hashtable::hash(string s) {
    uint32_t seed = 0xe6546b64; 
    int r;
    clock_t start;
    float duration;

    start = std::clock();

    r = (int)i.hash(reinterpret_cast<const uint8_t*>(&s[0]),s.length(),seed);
    duration = ( clock() - start ) / (float) CLOCKS_PER_SEC;
    s[0] ^= 1;
    int b = (int)i.hash(reinterpret_cast<const uint8_t*>(&s[0]),s.length(),seed); 
    i.performaceValues[2] += hammingDistance(r,b)/(float)1000;
    i.performaceValues[0] += duration/(float)1000;
    return r; 
}
void hashtable::findCollisions(){
    numCollisions =0;
    for (int i = 0; i < 1000; i++) {
		if (bucket[i] != NULL) {
			node* p = bucket[i];
			p = bucket[i]->next;
			while (p) {
                numCollisions++;
				p = p->next;
			} 
		} 
	} 
    i.performaceValues[1] = numCollisions;
}