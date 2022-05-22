#include "scanner.h"
#include <stack>
using namespace std;

class semantic_analyzer{
//根据token和翻译文法将表达式转换为四元式
public:
    void analyze(vector<pair<string,int>> tokens) {
        stack<string> stk;
        stack<string> sem;
        stack<string> word_stack;
        int cnt_temp_variable = 0;
        stk.push("#");
        stk.push("S");
        int cnt = 0;
        int error_flag = 0;
        while(cnt < tokens.size()) {
            pair<string,int> now_word = tokens[cnt];
            string temp = stk.top();
            stk.pop();
            if(temp == "{") {
                string operate = stk.top();
                stk.pop();
                stk.pop();
                if(operate == "+" || operate == "-" || operate == "*" || operate == "/" || operate == ">" || operate == "<") {
                    string operater1 = sem.top();
                    sem.pop();
                    string operater2 = sem.top();
                    sem.pop();
                    string temp_variable = "t" + to_string(cnt_temp_variable+1);
                    cnt_temp_variable += 1;
                    sem.push(temp_variable);
                    string result = "(";
                    result += operate + ", " + operater2 + ", " + operater1 + ", " + temp_variable + ")";
                    QT.push_back(result);
                }
                else {
                    string word = word_stack.top();
                    word_stack.pop();
                    sem.push(word);
                }
                continue;
            }
            vector<string>::iterator it;
            it = find(VX.begin(), VX.end(), temp);
            if(it != VX.end()) {
                string table_key = temp;
                if(now_word.second == 0 || now_word.second == 3) {
                    word_stack.push(now_word.first);
                    table_key += "!n";
                }
                else if(now_word.second == 30) table_key += "!(";
                else if(now_word.second == 31) table_key += "!)";
                else if(now_word.second == 36) table_key += "!+";
                else if(now_word.second == 39) table_key += "!-";
                else if(now_word.second == 42) table_key += "!*";
                else if(now_word.second == 44) table_key += "!/";
                else if(now_word.second == 47) table_key += "!<";
                else if(now_word.second == 48) table_key += "!>";
                else if(now_word.second == -1) table_key += "!#";
                else {
                    error_flag = 1;
                    break;
                }
                if(LL1_table.find(table_key) != LL1_table.end()) {
                    string value = LL1_table[table_key];
                    for(int i = 0; i <value.size() ; i ++) {
                        if(value[value.size()-1-i]!='#') {
                            string temp_string = "";
                            temp_string.push_back(value[value.size()-1-i]);
                            stk.push(temp_string);
                        }
                    }
                }
                else{
                    error_flag = 2;
                    break;
                }

            }
            else{
                it = find(VT.begin(),VT.end(),temp);
                
                if(it == VT.end()){
                    error_flag = 1;
                    break;
                }
                if(temp == "n") {
                    if(now_word.second != 0 && now_word.second != 3) error_flag = 3;
                }
                else if(temp == "+") {
                    if(now_word.second != 36) error_flag = 3;
                }
                else if(temp == "-") {
                    if(now_word.second != 39) error_flag = 3;
                }
                else if(temp == "*") {
                    if(now_word.second != 42) error_flag = 3;
                }
                else if(temp == "/") {
                    if(now_word.second != 44) error_flag = 3;
                }
                else if(temp == "<") {
                    if(now_word.second != 47) error_flag = 3;
                }
                else if(temp == ">") {
                    if(now_word.second != 48) error_flag = 3;
                }
                else if(temp != now_word.first) error_flag = 3;
                
                if(!error_flag) {
                    cnt ++;
                }
                else break;
            }
        }
        if(error_flag == 1) cout << "error input in string postion " << cnt+1 << ": " << tokens[cnt].first << endl;
        if(error_flag == 2) cout<<"syntax error: No such syntax rule!" << endl;
        if(error_flag == 3) cout<<"syntax error: no match!"<<endl;
        if(error_flag == 0 && stk.empty()) cout << "syntax right!" << endl;
    }
    void show_QT(){
        cout << "--------result---------" << endl;
        for(int i = 0 ; i < QT.size() ; i ++) {
            cout << QT[i] << endl;
        }
        cout << "-----------------------" << endl;
    }

private:
    map<string,string> LL1_table = {
        {"S!n","EC"},{"S!(","EC"},
        {"C!>",">E{>}C"},{"C!<","<E{<}C"},{"C!)","#"},{"C!#","#"},
        {"E!n","TA"}, {"E!(","TA"},
        {"A!+","+T{+}A"}, {"A!-","-T{-}A"}, {"A!)","#"}, {"A!#","#"}, {"A!>","#"}, {"A!<","#"},
        {"T!n","FB"}, {"T!(","FB"},
        {"B!+","#"}, {"B!-","#"}, {"B!*","*F{*}B"}, {"B!/","/F{/}B"}, {"B!)","#"}, {"B!#","#"}, {"B!>","#"}, {"B!<","#"},
        {"F!n","n{n}"}, {"F!(","(E)"}};
    vector<string> VX = {"E","A","T","F","B","S","C"};
    vector<string> VT = {"n","+","-","*","/","(",")",">","<","#"};
    vector<string> QT;
};

int main() {
    string code = read_file("test.txt");
    cout << "string: "<<code<<endl;
    Lexer Lex1;
    int error_flag = 0;
    vector<pair<string,int>> tokens = Lex1.word_analysis(code,error_flag);
    tokens.push_back(make_pair("#",-1));
    if(error_flag) cout << "Error!" << endl;
    semantic_analyzer analyzer1;
    analyzer1.analyze(tokens);
    analyzer1.show_QT();
}