/*hash.cpp*/

//
// Prof. Joe Hummel
// U. of Illinois, Chicago
// CS 251: Spring 2020
// 

#include <iostream>
#include <string>
#include <cctype>  /*isdigit*/
#include <regex>   /*regular expressions*/

#include "hash.h"

using namespace std;

//
// isNumeric
//
// Returns true if the given string is numeric (all digits), false
// if not.  If the string is empty, false is returned since there 
// are no digits present.
//
bool isNumeric(string s)
{
	//
	// A string is numeric if it contains 1 or more digits, so let's
	// use a regular expression to check that.
	//
	// we are using POSIX basic notation for regular expressions, see
	//   https://en.cppreference.com/w/cpp/regex/ecmascript
	// where [:d:] means digit, [[:d:]] means match a single digit.
	// The + means match the preceding sub-expression 1 or more times.
	//
	regex pattern("[[:d:]]+", regex::ECMAScript);

	smatch matchResults;  // provides more details on the match

	if (regex_match(s, matchResults, pattern))
		return true;
	else
		return false;
}



//Hash station ID function, returns index
int HashStatID(string id, int N)
{
    if (id.length()<0)
        return -1;
    if (isNumeric(id)==false)
        return -1;
    int ID=stoi(id);
	return ((ID)*10)%N; //returns ID times 10
}

//Hash station Abbreviation function, returns index
int HashStatAbbrev(string abbrev, int N)
{   
    	
    unsigned long long val = 7462;
    //goal is to make indices as random and spaced out as possible
    for (size_t i=0;i<abbrev.length();i++){ //loop through entire abbreviation
        val = ((val << 5)+ val) + abbrev[i];
    }
    int index = (int) val;
    if (index < 0){
        index = -(index); //makes index positive if its negative
    }
    index+=10;
    return index % (N/10); //mod by N/10 to fit it into hashtable size
}

//Hash Trip ID function, returns index
int HashTripID(string id, int N)
{	
    if (id.length()<3)
        return -1;
    if (id.substr(0,2)!="Tr")
        return -1;
    if (isNumeric(id.substr(2))==false)
        return -1;
    int ID=stoi(id.substr(2));

	return ID%N; //returns inputted ID, modded by N to fit into hashtable
}

//Hash Bike ID function, returns index
int HashBikeID(string id, int N)
{
	
    if (id.length()<2)
        return -1;
    if (id.substr(0,1)!="B")
        return -1;
    if (isNumeric(id.substr(1))==false)
        return -1;
    int ID=stoi(id.substr(1));

	return ID%N; //returns inputted ID, modded by N to fit into hashtable
}

