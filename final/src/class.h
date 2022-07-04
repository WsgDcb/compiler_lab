#ifndef __CLASS_H__
#define __CLASS_H__
#include<iostream>
#include<vector>
#include<map>
#include<algorithm>
#include<string>
#include<stack>
using namespace std;

class quart {
public:
    quart(string _operator,string _operand1=NULL, string _operand2=NULL, string _res = NULL) {
        res = _res;
        operand1 = _operand1;
        operand2 = _operand2;
        Operator = _operator;
    }

    void show() {
        cout << "(" << Operator<< "," << operand1<< "," << operand2 << "," <<res<< ")" << endl;
    }

    string res;
    string operand1;
    string operand2;
    string Operator;
};

class id_attribute{
public:
    id_attribute() {
       ADD = 0; 
    }
    id_attribute(char _CAT, string _TYPE, int _ADD, string _STATE=NULL){
        CAT = _CAT;
        TYPE = _TYPE;
        ADD = _ADD;
        STATE = _STATE;
    }
    bool operator<(const id_attribute& a) const { return a.ADD >= ADD; }

    void show() { cout << CAT << " " << TYPE << " " << ADD << " " << STATE << endl; }

    string getType() { return TYPE; }
    int getADD() { return ADD; }

    string STATE;
private:
    char CAT;
    string TYPE;
    int ADD;
};

class id{
public:
    id() {
        entry = "";
    }
    id(id_attribute _attribute, string _entry) {
        entry = _entry;
        attribute = _attribute;
    }
private:
    string entry;
    id_attribute attribute;
};

#endif