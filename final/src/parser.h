#include<iostream>
#include<vector>
#include<map>
using namespace std;

class parser{
public:
    bool parse() {
        token_pointer = 0;
        lookahead = tokens[token_pointer++];
        program();
        cout << "completely right" << endl;
    }

    void load_tokens(vector<pair<string,int>> tokens) {
        this->tokens=tokens;
    }

private:
    int token_pointer = 0;
    pair<string,int> lookahead;
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
        if(lookahead.second == kt["int"]) match(kt["int"]);
        else if (lookahead.second == kt["bool"]) match(kt["bool"]);
        else if (lookahead.second == kt["double"]) match(kt["double"]);
    }

    void id_list() {
        match(0);
        option_assignment();
        id_list1();
    }

    void option_assignment() {
        if(lookahead.second == pt["="]) {
            match(pt["="]);
            expression();
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
        match(pt["{"]);
        state_seq();
        match(pt["}"]);
        option_else();
    }

    void option_else() {
        if(lookahead.second == kt["else"]){
            match(kt["else"]);
            match(pt["{"]);
            state_seq();
            match(pt["}"]);
        }
        else {}
    }

    void assignment() {
        match(0);
        match(pt["="]);
        expression();
    }

    void expression() {
        simple_expression();
        expression1();
    }

    void expression1() {
        if(lookahead.second == pt[">"]) {
            match(pt[">"]);
            simple_expression();
            expression1();
        }
        else if(lookahead.second == pt["<"]) {
            match(pt["<"]);
            simple_expression();
            expression1();
        }
        else if(lookahead.second == pt[">="]) {
            match(pt[">="]);
            simple_expression();
            expression1();
        }
        else if(lookahead.second == pt["<="]) {
            match(pt["<="]);
            simple_expression();
            expression1();
        }
        else if(lookahead.second == pt["=="]) {
            match(pt["=="]);
            simple_expression();
            expression1();
        }
        else { }
    }

    void simple_expression() {
        if(lookahead.second == pt["-d"]) {
            match(pt["-d"]);
            term();
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
            simple_expression1();
        }
        else if(lookahead.second == pt["-"]) {
            match(pt["-"]);
            term();
            simple_expression1();
        }
        else if(lookahead.second == pt["||"]) {
            match(pt["||"]);
            term();
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
            term1();
        }
        else if(lookahead.second == pt["/"]) {
            match(pt["/"]);
            factor();
            term1();
        }
        else if(lookahead.second == pt["&&"]) {
            match(pt["&&"]);
            factor();
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
            factor();
        }
        else if(lookahead.second == 0) {
            match(0);
        }
        else if(lookahead.second == 3) {
            match(3);
        }
    }

    void match(int token) {
        if(lookahead.second == token) {
            lookahead = tokens[token_pointer++];
        }
        else {
            cout << "unmatch" << lookahead.first << "with" << token;
            exit(0);
        }
    }

};