#include "scanner.h"
#include <stack>
using namespace std;

class LL1_parsing{
//根据token判断文法是否正确
public:
    void parse(vector<pair<string,int>> tokens) {
        stack<string> stk;
        stk.push("#");
        stk.push("E");
        int cnt = 0;
        int error_flag = 0;
        while(cnt < tokens.size()) {
            pair<string,int> now_word = tokens[cnt];
            string temp = stk.top();
            stk.pop();
            vector<string>::iterator it;
            it = find(VX.begin(), VX.end(), temp);
            if(it != VX.end()) {
                string table_key = temp;
                if(now_word.second == 0 || now_word.second == 3) table_key += "!n";
                else if(now_word.second == 30) table_key += "!(";
                else if(now_word.second == 31) table_key += "!)";
                else if(now_word.second == 36 || now_word.second == 39) table_key += "!p";
                else if(now_word.second == 42 || now_word.second == 44) table_key += "!q";
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
                else if(temp == "p") {
                    if(now_word.second != 36 && now_word.second != 39) error_flag = 3;
                }
                else if(temp == "q") {
                    if(now_word.second != 42 && now_word.second != 44) error_flag = 3;
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
        if(stk.empty()) cout << "right!!" << endl;
    }

private:
    map<string,string> LL1_table = {
        {"E!n","TA"}, {"E!(","TA"},
        {"A!p","pTA"}, {"A!)","#"}, {"A!#","#"},
        {"T!n","FB"}, {"T!(","FB"},
        {"B!p","#"}, {"B!q","qFB"}, {"B!)","#"}, {"B!#","#"},
        {"F!n","n"}, {"F!(","(E)"}};
    vector<string> VX = {"E","A","T","F","B"};
    vector<string> VT = {"n","q","p","(",")","#"};
};

int main() {
    cout << "LL1_parsing" << endl;
    string code = read_file("test.txt");
    cout << "string: "<<code<<endl;
    Lexer Lex1;
    int error_flag = 0;
    vector<pair<string,int>> tokens = Lex1.word_analysis(code,error_flag);
    tokens.push_back(make_pair("#",-1));
    if(error_flag) cout << "Error!" << endl;
    LL1_parsing parse1;
    parse1.parse(tokens);
}