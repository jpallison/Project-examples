
#ifndef RECORD_H
#define	RECORD_H
#include <string>

using namespace std; 

class Record
{
string key;
string data;
public:
Record(): key(""), data("") {}
Record(string k, string d): key(k), data(d) {}
string get_key() { return key; }
string get_data() { return data; }
};

#endif	/* RECORD_H */

