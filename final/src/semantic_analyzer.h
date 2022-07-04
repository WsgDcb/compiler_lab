#include "class.h"

class semantic_analyzer{
public:
    void analyze() {
        cnt_temp_variable = 0;
        id_offset = 0;
        token_pointer = 0;
        lookahead = tokens[token_pointer++];
        program();
        cout << "over" << endl;
    }

    void load_tokens(vector<pair<string,int>> tokens) { this->tokens=tokens; }
    
    vector<quart> getQT() { return this->QT; }

    map<string,id_attribute> getID_table() { return this->ID_table; }

    map<string,string> getCT() { return this->CT; }
private:
    void program() {
        state_seq();
        //保证tokens串判断完
        if(lookahead.second != -1) {
            cout << "wrong" << endl;
            exit(0);
        }
    }

    void state_seq(){
        if(lookahead.second == kt["int"] || lookahead.second == kt["bool"] || lookahead.second == kt["double"]){
            declaration();
            match(pt[";"]);
            state_seq();
        }
        else if(lookahead.second == 0){
            assignment();
            match(pt[";"]);
            state_seq();
        }
        else if(lookahead.second == kt["if"]){
            state_if();
            state_seq();
        }
        else {}
    }

    void declaration() {
        if(lookahead.second == kt["int"] || lookahead.second == kt["bool"] || lookahead.second == kt["double"]) {
            type();
            id_list();
        }
    }

    void type() {
        if(lookahead.second == kt["int"]) {
            match(kt["int"]);
            change_type("int",4);
        }
        else if (lookahead.second == kt["bool"]) { 
            match(kt["bool"]);
            change_type("bool",1);
        }
        else if (lookahead.second == kt["double"]) { 
            match(kt["double"]);
            change_type("double",8);
        }
    }

    void id_list() {
        match(0);
        write_IDtable();
        option_assignment();
        id_list1();
    }

    void option_assignment() {
        if(lookahead.second == pt["="]) {
            push();
            match(pt["="]);
            expression();
            ASSIGN();
        }
        else{}
    }

    void id_list1() {
        if(lookahead.second == pt[","]) {
            match(pt[","]);
            id_list();
        }
        else {}
    }
    
    void state_if() {
        match(kt["if"]);
        match(pt["("]);
        state_if1();
    }
    
    void state_if1(){
        expression();
        match(pt[")"]);
        IF();
        match(pt["{"]);
        state_seq();
        match(pt["}"]);
        option_else();
        IE();
    }

    void option_else() {
        if(lookahead.second == kt["else"]){
            match(kt["else"]);
            EL();
            match(pt["{"]);
            state_seq();
            match(pt["}"]);
        }
        else {}
    }

    void assignment() {
        match(0);
        push();
        match(pt["="]);
        expression();
        ASSIGN();
    }

    void expression() {
        simple_expression();
        expression1();
    }

    void expression1() {
        if(lookahead.second == pt[">"]) {
            match(pt[">"]);
            simple_expression();
            GEQ(">");
            expression1();
        }
        else if(lookahead.second == pt["<"]) {
            match(pt["<"]);
            simple_expression();
            GEQ("<");
            expression1();
        }
        else if(lookahead.second == pt[">="]) {
            match(pt[">="]);
            simple_expression();
            GEQ(">=");
            expression1();
        }
        else if(lookahead.second == pt["<="]) {
            match(pt["<="]);
            simple_expression();
            GEQ("<=");
            expression1();
        }
        else if(lookahead.second == pt["=="]) {
            match(pt["=="]);
            simple_expression();
            GEQ("==");
            expression1();
        }
        else { }
    }

    void simple_expression() {
        if(lookahead.second == pt["-d"]) {
            match(pt["-d"]);
            term();
            GEQ1("-");
            simple_expression1();
        }
        else if(lookahead.second == pt["("] || lookahead.second == pt["~"] || lookahead.second == 0 || lookahead.second ==3) {
            term();
            simple_expression1();
        }
    }

    void simple_expression1() {
        if(lookahead.second == pt["+"]) {
            match(pt["+"]);
            term();
            GEQ("+");
            simple_expression1();
        }
        else if(lookahead.second == pt["-"]) {
            match(pt["-"]);
            term();
            GEQ("-");
            simple_expression1();
        }
        else if(lookahead.second == pt["||"]) {
            match(pt["||"]);
            term();
            GEQ("||");
            simple_expression1();
        }
        else {}
    }

    void term() {
        factor();
        term1();
    }

    void term1() {
        if(lookahead.second == pt["*"]) {
            match(pt["*"]);
            factor();
            GEQ("*");
            term1();
        }
        else if(lookahead.second == pt["/"]) {
            match(pt["/"]);
            factor();
            GEQ("/");
            term1();
        }
        else if(lookahead.second == pt["&&"]) {
            match(pt["&&"]);
            factor();
            GEQ("&&");
            term1();
        }
        else {}
    }

    void factor() {
        if(lookahead.second == pt["("]){
            match(pt["("]);
            expression();
            match(pt[")"]);
        }
        else if(lookahead.second == pt["~"]) {
            match(pt["~"]);
            //GEQ1("~");
            factor();
            GEQ1("~");
        }
        else if(lookahead.second == 0) {
            match(0);
            push();
        }
        else if(lookahead.second == 3) {
            match(3);
            push();
        }
    }

    void match(int token) {
        if(lookahead.second == token) {
            pre_lookahead = lookahead;
            lookahead = tokens[token_pointer++];
        }
        else {
            cout << "unmatch" << lookahead.first << "with" << token;
            exit(0);
        }
    }
    
    //语义动作函数
    //1.转变声明种类
    void change_type(string type, int len) {
        Type.first = type;
        Type.second = len;
    }
    //2.写入符号表
    void write_IDtable() {
        string entry = pre_lookahead.first;
        id_attribute attribute = id_attribute('v',Type.first,id_offset,"(y)");
        ID_table[entry] = attribute;
        id_offset += Type.second;
    }

    void GEQ(string _operator) {
        string oprand1 = sem.top();
        sem.pop();
        string oprand2 = sem.top();
        sem.pop();
        string temp_variable = "t" + to_string(cnt_temp_variable+1);
        cnt_temp_variable += 1;
        sem.push(temp_variable);
        QT.push_back(quart(_operator,oprand2,oprand1,temp_variable));
        string type_operand1, type_operand2;
        if(CT.find(oprand1) != CT.end()) {
            type_operand1 = CT[oprand1];
        }
        else type_operand1 = ID_table[oprand1].getType();
        if(CT.find(oprand2) != CT.end()) {
            type_operand2 = CT[oprand2];
        }
        else type_operand2 = ID_table[oprand2].getType();
        if(_operator == "<=" || _operator == ">=" || _operator == ">" || _operator == "<" || _operator == "==") {
            if(type_operand2 != type_operand1) {
                cout << "The types of operand are different in comparision expression!" << endl;
                exit(0);
            }
            else {
                ID_table[temp_variable] = id_attribute('v',"bool",id_offset,"(n)");
                id_offset += 4;
            }
        }
        else if(_operator == "&&" || _operator == "||") {
            if(type_operand2 != "bool" || type_operand1 != "bool") {
                cout << "The types of operands on && || operation can only be bool"<<endl;
                exit(0);
            }
            else {
                ID_table[temp_variable] = id_attribute('v',"bool",id_offset,"(n)");
                id_offset += 4;
            }
        }
        else {
            if(type_operand2 == "bool" || type_operand1 == "bool") {
                cout << "bool variable can't + - * / !"<<endl;
                exit(0);
            }
            else {
                if(type_operand2 == "double" || type_operand1 == "double") {
                    ID_table[temp_variable] = id_attribute('v',"double",id_offset,"(n)");
                    id_offset += 8;
                }
                else {
                    ID_table[temp_variable] = id_attribute('v',"int",id_offset,"(n)");
                    id_offset += 4;
                }
            }
        }
    }

    void GEQ1(string _operator) {
        string oprand1 = sem.top();
        sem.pop();
        string temp_variable = "t" + to_string(cnt_temp_variable+1);
        cnt_temp_variable += 1;
        sem.push(temp_variable);
        QT.push_back(quart(_operator,oprand1,"_",temp_variable));
        string type_operand1;
        if(CT.find(oprand1) != CT.end()) {
            type_operand1 = CT[oprand1];
        }
        else type_operand1 = ID_table[oprand1].getType();
        if(_operator == "-") {
            if(type_operand1 == "bool") {
                cout << "error: - oprator can't used for bool" << endl;
                exit(0);
            }
            else {
                if(type_operand1 == "int") {
                    ID_table[temp_variable] = id_attribute('v',"int",id_offset,"(n)");
                    id_offset += 4;
                }
                else {
                    ID_table[temp_variable] = id_attribute('v',"double",id_offset,"(n)");
                    id_offset += 8;
                }
            }
        }
        else if(_operator == "~") {
            if(type_operand1 != "bool") {
                cout << "error: ~ oprator can be only used for bool" << endl;
                exit(0);
            }
            else {
                ID_table[temp_variable] = id_attribute('v',"bool",id_offset,"(n)");
                id_offset += 4;
            }
        }
    }

    void push() {
        sem.push(pre_lookahead.first);
        if(pre_lookahead.second == 3) {
            if(pre_lookahead.first.find('.') != string::npos) {
                CT[pre_lookahead.first] = "double";
            }
            else if(pre_lookahead.first.find('e') != string::npos){
                CT[pre_lookahead.first] = "double";
            }
            else {
                CT[pre_lookahead.first] = "int";
            }
        }
    }

    void ASSIGN() {
        string oprand1 = sem.top();
        sem.pop();
        string oprand2 = sem.top();
        sem.pop();
        QT.push_back(quart("=",oprand1,"_",oprand2));
        string type_operand1, type_operand2;
        if(CT.find(oprand1) != CT.end()) {
            type_operand1 = CT[oprand1];
        }
        else type_operand1 = ID_table[oprand1].getType();
        if(CT.find(oprand2) != CT.end()) {
            type_operand2 = CT[oprand2];
        }
        else type_operand2 = ID_table[oprand2].getType();
        if(type_operand1 != type_operand2) {
            cout<< "Types are different in assignment operation!" << endl;
            exit(0);
        }
    }

    void IF() {
        string oprand1 = sem.top();
        sem.pop();
        QT.push_back(quart("if",oprand1,"_","_"));  
    }
    void EL() {
        QT.push_back(quart("el","_","_","_"));  
    }
    void IE() {
        QT.push_back(quart("ie","_","_","_"));  
    }

    int token_pointer = 0;
    pair<string,int> lookahead;
    pair<string,int> pre_lookahead;
    vector<pair<string,int>> tokens;

    map<string, int> kt = {
            {"auto",4},{"void",5},{"char",6},{"const",7},{"double",8},{"float",9},
            {"int",10},{"long",11},{"if",12},{"else",13},{"for",14},{"while",15},{"continue",16},
            {"break",17},{"switch",18},{"case",19},{"main",20},{"bool",21}};

    map<string, int> pt = {
            {"(",30},{")",31},{"[",32},{"]",33},{"{",34},{"}",35},{"+",36},{"++",37},{"+=",38},{"-",39},
            {"--",40},{"-=",41},{"*",42},{"*=",43},{"/",44},{"/=",45},{"%",46},{"<",47},{">",48},{"=",49},
            {"<=",50},{"==",51},{">=",52},{"&",53},{"|",54},{"^",55},{"&&",56},{"||",57},{"//",58},{"/*",59},
            {"*/",60},{",",61},{";",62},{"<<",63},{">>",64},{"->",65},{"-d",66},{"~",67}};

    stack<string> sem;
    int id_offset = 0;
    int cnt_temp_variable = 0;
    pair<string,int> Type;
    vector<quart> QT;
    map<string,string> CT;
    map<string, id_attribute> ID_table;
};