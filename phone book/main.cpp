#include "Record.h"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char** argv) {
    // create vectors
    vector<Record> vphones;
    vector<Record> vsocials;
    vector<Record> vsalaries;
   
    // populate vector of phones
    ifstream input;
    input.open("phones.txt");
    
    if (!input){
    cout << "File couldn't be opened." << endl;
    return 1;
    }
    
    while (!input.eof())
    {
        string s;
        string n;
        input >> s;
        input >> n;
        Record newRecord(s,n);
        vphones.push_back(newRecord);
    }
    input.close();
    // populate vector of socials
    input.open("socials.txt");
    
    if (!input){
    cout << "File couldn't be opened." << endl;
    return 1;
    }
    
    while (!input.eof())
    {
        string s;
        string n;
        input >> s;
        input >> n;
        Record newRecord(s,n);
        vsocials.push_back(newRecord);
    }
    input.close();

    //populate vector of salaries
    input.open("salaries.txt");
    
    if (!input){
    cout << "File couldn't be opened." << endl;
    return 1;
    }
    
    while (!input.eof())
    {
        string s;
        string n;
        input >> s;
        input >> n;
        Record newRecord(s,n);
        vsalaries.push_back(newRecord);
    }
    input.close();

    string phone = "";
    string name = "";
    string social = "";
    string salary = "";
    string going = "y";
    
    while (going == "y")
    
    {
    cout << "please enter a phone number XXX-XXX-XXXX: " << endl;
    cin >> phone;
    
    for (int i =0; i < vphones.size(); i++)
    {
        if (vphones[i].get_data()== phone)
        {
            name = vphones[i].get_key();
            cout << name << " , ";
        }
    }
    if (name == "")
    {
        cout << "no name found in database";
    
    }
     for (int i =0; i < vsocials.size(); i++)
    {
        if (vsocials[i].get_key()== name)
        {
            social = vsocials[i].get_data();
            cout << social << " , ";
        }
    
    }
    if (social == "")
    {
        cout << "no social in database";
    
    }
    for (int i = 0; i < vsalaries.size(); i++)
    {
        if (vsalaries[i].get_key() == social)
        {
            salary = vsalaries[i].get_data();
            cout << salary;
        }
    
    }
    if (salary == "")
    {
        cout << "no salary found in database";
    
    }
    
    cout << endl;
    cout << "would you like to look up another phone number? (y/n)" << endl;
    cin >> going;
    
    }
    

}       

