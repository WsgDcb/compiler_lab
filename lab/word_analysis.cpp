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
            cout << "read finish!" << endl;
            break;
        }
        code += c;
    }
    infile.close();
    return code;
}

class Lexer{
public:
    vector<pair<string,int>> word_analysis(string code, int& error_flag) {
        int pointer_code = 0;
        vector<pair<string,int>> tokens;
        int category_code = 0;
        while(pointer_code < code.size()) {
            if(isDigit(code[pointer_code])) {
                string temp;
                int flag_float = 0;
                int flag_e = 0;
                while(isDigit(code[pointer_code])) {
                    temp += code[pointer_code];
                    pointer_code++;
                    if(pointer_code >= code.size()) {
                        break;
                    }
                    if(code[pointer_code] == '.') {
                        if(flag_float) {
                            error_flag = 1;
                            break;
                        }
                        flag_float = 1;
                        temp += code[pointer_code];
                        pointer_code++;
                    }
                    else if(code[pointer_code] == 'e') {
                        if(flag_e) {
                            error_flag = 1;
                            break;
                        }
                        flag_e = 1;
                        temp += code[pointer_code];
                        pointer_code++;
                        if(code[pointer_code] == '-') {
                            temp += code[pointer_code];
                            pointer_code++;
                        }
                    }
                }
                vector<string>::iterator iter;
                iter = find(CT.begin(),CT.end(),temp);
                if(iter == CT.end()) CT.push_back(temp);
                tokens.push_back(make_pair(temp,3));
            }
            else if(isLetter(code[pointer_code])) {
                string temp;
                while(isLetter(code[pointer_code]) || isDigit(code[pointer_code])) {
                    temp += code[pointer_code];
                    pointer_code++;
                    if(pointer_code >= code.size()) {
                        break;
                    }
                }
                category_code = 0;
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
            else {
                string temp;
                char ch = code[pointer_code];
                switch(ch){
                    case '(':
                        tokens.push_back(make_pair("(",pt["("])); break;
                    case ')':
                        tokens.push_back(make_pair(")",pt[")"])); break;
                    case '{':
                        tokens.push_back(make_pair("{",pt["{"])); break;
                    case '}':
                        tokens.push_back(make_pair("}",pt["}"])); break;
                    case '[':
                        tokens.push_back(make_pair("[",pt["["])); break;
                    case ']':
                        tokens.push_back(make_pair("]",pt["]"])); break;
                    case ',':
                        tokens.push_back(make_pair(",",pt[","])); break;
                    case ';':
                        tokens.push_back(make_pair(";",pt[";"])); break;
                    case '+':
                        if(code[pointer_code + 1] == '=') {
                            tokens.push_back(make_pair("+=", pt["+="]));
                            pointer_code ++;
                        }
                        else if(code[pointer_code + 1] == '+') {
                            tokens.push_back(make_pair("++", pt["++"]));
                            pointer_code ++;
                        }
                        else {
                            tokens.push_back(make_pair("+",pt["+"]));
                        }
                        break;
                    case '-':
                        if(code[pointer_code + 1] == '=') {
                            tokens.push_back(make_pair("-=", pt["-="]));
                            pointer_code ++;
                        }
                        else if(code[pointer_code + 1] == '-') {
                            tokens.push_back(make_pair("--", pt["--"]));
                            pointer_code ++;
                        }
                        else {
                            tokens.push_back(make_pair("-",pt["-"]));
                        }
                        break;
                    case '*':
                        if(code[pointer_code + 1] == '=') {
                            tokens.push_back(make_pair("*=", pt["*="]));
                            pointer_code ++;
                        }
                        else if(code[pointer_code + 1] == '/') {
                            tokens.push_back(make_pair("*/", pt["*/"]));
                            pointer_code ++;
                        }
                        else tokens.push_back(make_pair("*",pt["*"]));
                        break;
                    case '/':
                        if(code[pointer_code + 1] == '=') {
                            tokens.push_back(make_pair("/=", pt["/="]));
                            pointer_code ++;
                        }
                        else if(code[pointer_code+1] == '/') {
                            tokens.push_back(make_pair("//", pt["//"]));
                            pointer_code ++;
                        }
                        else if(code[pointer_code + 1] == '*'){
                            tokens.push_back(make_pair("//", pt["//"]));
                            pointer_code ++;
                        }
                        else tokens.push_back(make_pair("/",pt["/"]));
                        break;
                    case '%':
                        tokens.push_back(make_pair("%", pt["%"]));
                        break;
                    case '&':
                        if(code[pointer_code + 1] == '&') {
                            tokens.push_back(make_pair("&&", pt["&&"]));
                            pointer_code ++;
                        }
                        else{
                            tokens.push_back(make_pair("&", pt["&"]));
                        }
                        break;
                    case '|':
                        if(code[pointer_code + 1] == '|') {
                            tokens.push_back(make_pair("||", pt["||"]));
                            pointer_code ++;
                        }
                        else{
                            tokens.push_back(make_pair("|", pt["|"]));
                        }
                        break;
                    case '>':
                        if(code[pointer_code + 1] == '=') {
                            tokens.push_back(make_pair(">=", pt[">="]));
                            pointer_code ++;
                        }
                        else tokens.push_back(make_pair(">", pt[">"]));
                        break;
                    case '<':
                        if(code[pointer_code + 1] == '=') {
                            tokens.push_back(make_pair("<=", pt["<="]));
                            pointer_code ++;
                        }
                        else tokens.push_back(make_pair("<", pt["<"]));
                        break;
                    case '=':
                        if(code[pointer_code + 1] == '=') {
                            tokens.push_back(make_pair("==", pt["=="]));
                            pointer_code ++;
                        }
                        else tokens.push_back(make_pair("=", pt["="]));
                        break;
                    case '"':
                        temp.clear();
                        temp += "\"";
                        pointer_code ++;
                        while(code[pointer_code] != '"') {
                            if(code[pointer_code] == '\\') {
                                temp += code[pointer_code+1];
                                pointer_code += 2;
                            }
                            else{
                                temp += code[pointer_code];
                                pointer_code++;
                            }
                            if(pointer_code >= code.size()) {
                                error_flag = 1;
                                break;
                            }
                        }
                        temp += "\"";
                        if(!error_flag) {
                            sT.push_back(temp);
                            tokens.push_back(make_pair(temp,2));
                        }
                        break;
                    case '\'':
                        temp.clear();
                        temp += "'";
                        pointer_code++;
                        if(code[pointer_code] == '\\') {
                            if(pointer_code + 2 >= code.size()) {
                                error_flag = 1;
                                break;
                            }
                            pointer_code++;
                            temp += code[pointer_code];
                            temp += "'";
                            pointer_code++;
                        }
                        else{
                            if(pointer_code + 1 >= code.size()) {
                                error_flag = 1;
                                break;
                            }
                            temp += code[pointer_code];
                            temp += "'";
                            pointer_code++;
                        }
                        if(!error_flag) {
                            cT.push_back(temp);
                            tokens.push_back(make_pair(temp,1));
                        }
                        break;
                    default:
                        break;
                }
                pointer_code ++;
            }
            if(error_flag) {
                tokens.clear();
                break;
            }
            //pointer_code++;
        }
        return tokens;
    }
    void show_iT() {
        for(int i = 0; i < iT.size(); i++) {
            cout << iT[i] << endl;
        }
    }
private:
    bool isDigit(char c) { return c >= '0' && c <= '9'; }
    
    bool isLetter(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c=='_'; }

    map<string, int> kt = {
            {"auto",4},{"void",5},{"char",6},{"const",7},{"double",8},{"float",9},
            {"int",10},{"long",11},{"if",12},{"else",13},{"for",14},{"while",15},{"continue",16},
            {"break",17},{"switch",18},{"case",19},{"main",20}};

    map<string, int> pt = {
            {"(",30},{")",31},{"[",32},{"]",33},{"{",34},{"}",35},{"+",36},{"++",37},{"+=",38},{"-",39},
            {"--",40},{"-=",41},{"*",42},{"*=",43},{"/",44},{"/=",45},{"%",46},{"<",47},{">",48},{"=",49},
            {"<=",50},{"==",51},{">=",52},{"&",53},{"|",54},{"^",55},{"&&",56},{"||",57},{"//",58},{"/*",59},
            {"*/",60},{",",61},{";",62}};

    vector<string> sT;
    vector<string> cT;
    vector<string> iT;
    vector<string> CT;
};


int main() {
    string code = read_file("demo.c");
    Lexer Lex1;
    int error_flag = 0;
    vector<pair<string,int>> tokens = Lex1.word_analysis(code,error_flag);
    if(error_flag) cout << "Error!" << endl;
    // for(int i = 0 ; i < tokens.size() ; i ++) {
    //     cout << tokens[i].first << " "<< tokens[i].second << endl;
    // }
    //Lex1.show_iT();
    ofstream res_file("tokens.txt");
    for(int i = 0 ; i < tokens.size() ; i ++) {
        res_file << "<" << tokens[i].first << " ," << tokens[i].second << ">" << endl;
    }
    res_file.close();
}