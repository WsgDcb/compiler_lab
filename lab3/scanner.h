#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <vector>
#include <utility>
using namespace std;

string read_file(const char* filename) {
    ifstream infile(filename, ios::in|ios::binary);
    char c;
    string code;
    if(!infile) {
        cout << "open failed!" << endl;
        return code;
    }
    while(!infile.eof()){
        c = infile.get();
        if(c == -1) {
            break;
        }
        code += c;
    }
    code += " ";
    infile.close();
    return code;
}

class Lexer{
public:
    //进行词法分析的函数,返回tokens
    vector<pair<string,int>> word_analysis(string code, int& error_flag) {
        vector<pair<string,int>> tokens;
        int pointer_code = 0;
        int state = 1;
        int state_before = 0;
        string temp;
        while(pointer_code < code.size()) {
            state_before = state;
            state_change(state, code[pointer_code], error_flag);
            if(state!=0 && state!=1 && state != 16 && state != 17 && state != 18) {
                //非终止态非注释情况下对字符进行记录
                if(state == 8 || state == 9) {
                    if(code[pointer_code] == '\\') pointer_code ++;
                }
                temp += code[pointer_code];
            }
            if(state == 0) {
                //终止态时进行tokens的储存
                if(state_before == 2) {
                    int category_code = 0;
                    if(kt.count(temp)) {
                        category_code = kt[temp];
                    }
                    else{
                        vector<string>::iterator iter;
                        iter = find(iT.begin(),iT.end(),temp);
                        if(iter == iT.end()) iT.push_back(temp);
                    }
                    tokens.push_back(make_pair(temp,category_code));
                }
                else if(state_before == 3 || state_before == 5 || state_before == 7) {
                    vector<string>::iterator iter;
                    iter = find(CT.begin(),CT.end(),temp);
                    if(iter == CT.end()) CT.push_back(temp);
                    tokens.push_back(make_pair(temp,3));
                }
                else if(state_before == 16) {
                    tokens.push_back(make_pair("//",pt["//"]));
                }
                else if(state_before == 20) {
                    //判断为负号或者减号
                    if(tokens.size() != 0) {
                        pair<string,int> last_token = tokens.back();
                        if(last_token.second == 31 || last_token.second == 3 || last_token.second == 0) {
                            tokens.push_back(make_pair(temp,pt[temp]));
                        }
                        else{
                            tokens.push_back(make_pair("-",66));
                        }
                    } 
                    else{
                        tokens.push_back(make_pair("-",66));
                    }
                }
                else if(state_before == 25) {
                    tokens.push_back(make_pair("/*",pt["/*"]));
                    tokens.push_back(make_pair("*/",pt["*/"]));
                }
                else if(state_before == 24){
                    cT.push_back(temp);
                    tokens.push_back(make_pair(temp,1));
                }
                else if(state_before == 23) {
                    sT.push_back(temp);
                    tokens.push_back(make_pair(temp,2));
                }
                else{
                    tokens.push_back(make_pair(temp,pt[temp]));
                }
                state = 1;
                temp.clear();
                pointer_code --;
            }
            if(error_flag) {
                tokens.clear();
                break;
            }
            pointer_code ++;
        }
        if(state != 1) {
            error_flag = 1;
            tokens.clear();
        }
        return tokens;
    }

    void show_iT() {
        cout << "------" <<"iT" << "------" << endl;
        for(int i = 0; i < iT.size(); i++) {
            cout << iT[i] << endl;
        }
    }

    void show_cT() {
        cout << "------" <<"cT" << "------" << endl;
        for(int i = 0; i < cT.size(); i++) {
            cout << cT[i] << endl;
        }
    }

    void show_CT() {
        cout << "------" <<"CT" << "------" << endl;
        for(int i = 0; i < CT.size(); i++) {
            cout << CT[i] << endl;
        }
    }

    void show_sT() {
        cout << "------" <<"sT" << "------" << endl;
        for(int i = 0; i < sT.size(); i++) {
            cout << sT[i] << endl;
        }
    }
    
private:
    bool isDigit(char c) { return c >= '0' && c <= '9'; }
    
    bool isLetter(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c=='_'; }
    //自动机状态控制转换函数
    void state_change(int& state, char ch, int& error_flag) {
        switch(state) {
            case 1:
                if(isLetter(ch)) state = 2;
                else if(isDigit(ch)) state = 3;
                else if(ch == '"') state = 8;
                else if(ch == '\'') state = 9;
                else if(ch == '>') state = 11;
                else if(ch == '<') state = 12;
                else if(ch == '=') state = 13;
                else if(ch == '*') state = 14;
                else if(ch == '/') state = 15;
                else if(ch == '+') state = 19;
                else if(ch == '-') state = 20;
                else if(ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == '[' || ch == ']') state = 10;
                else if(ch == '%' || ch == '^' || ch == ',' || ch == ';') state = 10;
                else if(ch == '&') state = 21;
                else if(ch == '|') state = 22;
                break;
            case 2:
                if(isLetter(ch) || isDigit(ch)) state = 2;
                else state = 0;
                break;
            case 3:
                if(isDigit(ch)) state = 3;
                else if(ch == '.') state = 4;
                else if(ch == 'e') state = 6;
                else state = 0;
                break;
            case 4:
                if(isDigit(ch)) state = 5;
                else state = 99;
                break;
            case 5:
                if(isDigit(ch)) state = 5;
                else if(ch == 'e') state = 6;
                else state = 0;
                break;
            case 6:
                if(isDigit(ch)) state = 6;
                else if(ch == '-') state = 7;
                else state = 99;
                break;
            case 7:
                if(isDigit(ch)) state = 7;
                else state = 0;
                break;
            case 8:
                if(ch == '"') state = 23; break;
            case 9:
                if(ch == '\'') state = 24; break;
            case 10:
                state = 0; break;
            case 11:
                if(ch == '=') state = 10;
                else if(ch == '>') state = 10;
                else state = 0;
                break;
            case 12:
                if(ch == '=') state = 10;
                else if(ch == '<') state = 10;
                else state = 0;
                break;
            case 13:
                if(ch == '=') state = 10;
                else state = 0;
                break;
            case 14:
                if(ch == '=') state = 10;
                else state = 0;
                break;
            case 15:
                if(ch == '=') state = 10;
                else if (ch == '/') state = 16;
                else if (ch == '*') state = 17;
                else state = 0;
                break;
            case 16:
                if(ch == '\n') state = 0; break;
            case 17:
                if(ch == '*') state = 18; break;
            case 18:
                if(ch == '/') state = 25;
                else state = 17;
                break;
            case 19:
                if(ch == '=') state = 10;
                else if(ch == '+') state = 10;
                else state = 0;
                break;
            case 20:
                if(ch == '=') state = 10;
                else if(ch == '-') state = 10;
                else if(ch == '>') state = 10;
                else state = 0;
                break;
            case 21:
                if(ch == '&') state = 10;
                else state = 0;
                break;
            case 22:
                if(ch == '|') state = 10;
                else state = 0;
                break;
            case 23:
                state = 0; break;
            case 24:
                state = 0; break;
            case 25:
                state = 0; break;
            case 99:
                error_flag = 1;
                break;
            default:
                break;
        }
    }

    map<string, int> kt = {
            {"auto",4},{"void",5},{"char",6},{"const",7},{"double",8},{"float",9},
            {"int",10},{"long",11},{"if",12},{"else",13},{"for",14},{"while",15},{"continue",16},
            {"break",17},{"switch",18},{"case",19},{"main",20}};

    map<string, int> pt = {
            {"(",30},{")",31},{"[",32},{"]",33},{"{",34},{"}",35},{"+",36},{"++",37},{"+=",38},{"-",39},
            {"--",40},{"-=",41},{"*",42},{"*=",43},{"/",44},{"/=",45},{"%",46},{"<",47},{">",48},{"=",49},
            {"<=",50},{"==",51},{">=",52},{"&",53},{"|",54},{"^",55},{"&&",56},{"||",57},{"//",58},{"/*",59},
            {"*/",60},{",",61},{";",62},{"<<",63},{">>",64},{"->",65},{"-d",66}};

    vector<string> sT;
    vector<string> cT;
    vector<string> iT;
    vector<string> CT;
};
