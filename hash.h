/*hash.h*/

//
// Prof. Joe Hummel
// U. of Illinois, Chicago
// CS 251: Spring 2020
// 

#include <string>

using namespace std;

bool isNumeric(string s);
int Hash(string theDate, int N);
int HashStatID(string id, int N);
int HashStatAbbrev(string abbrev, int N);
int HashTripID(string id, int N);
int HashBikeID(string id, int N);

