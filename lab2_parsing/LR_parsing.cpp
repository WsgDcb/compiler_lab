#include"scanner.h"
#include<stack>
using namespace std;

class LR_parsing{
public:
    void parse(vector<pair<string,int>> tokens) {
        stack<int> stk;
        stk.push(0);
        int error_flag = 0;
        int cnt = 0;
        int reduce_flag = 0;
        string now_char = "";
        while(cnt < tokens.size()) {
            if(reduce_flag == 0) {
                pair<string,int> now_word = tokens[cnt];
                int now_token_num = now_word.second;
                if(now_token_num == 0 || now_token_num == 3) now_char = "n";
                else if(now_word.second == 30) now_char = "(";
                else if(now_word.second == 31) now_char = ")";
                else if(now_word.second == 36 || now_word.second == 39) now_char = "p";
                else if(now_word.second == 42 || now_word.second == 44) now_char = "q";
                else if(now_word.second == -1) now_char = "#";
                else{
                    error_flag = 2;
                    break;
                }
            }
            int now_state = stk.top();
            //cout << now_state<< " " << now_char<<endl;
            if(LR_table[now_state].find(now_char) != LR_table[now_state].end()) {
                string action = LR_table[now_state][now_char];
                if(action[0] == 's') {
                    int next_state = 0;
                    for(int i = 1 ; i < action.size() ; i++) {
                        next_state = next_state*10+(action[i]-'0');
                    }
                    stk.push(next_state);
                    if(!reduce_flag) cnt ++;
                    reduce_flag = 0;
                }
                else if(action[0] == 'r') {
                    int rule = 0;
                    for(int i = 1 ; i < action.size() ; i++) {
                        rule = rule*10+(action[i]-'0');
                    }
                    int length = syntax[rule].size();
                    char reduce = syntax[rule][length-1];
                    int num_reduce = length - 1;
                    now_char = "";
                    now_char += reduce;
                    reduce_flag = 1;
                    for(int i = 0 ; i < num_reduce ; i ++) stk.pop();
                }
                else{
                    cout << "right!" << endl;
                    break;
                }
            }
            else{
                error_flag = 1;
                break;
            }
        }
        if(error_flag == 1) cout << "syntax error!" << endl;
        if(error_flag == 2) cout << "error input in string postion " << cnt+1 << ": " << tokens[cnt].first << endl;
    }
private:
    vector<map<string, string>> LR_table = {
        {{"n","s8"}, {"(","s9"}, {"E","s1"}, {"T","s4"}, {"F","s7"}},
        {{"p","s2"}, {"#","OK"}},
        {{"n","s8"}, {"(","s9"}, {"T","s3"}, {"F","s7"}},
        {{"p","r2"}, {"q","s5"}, {")","r2"}, {"#","r2"}},
        {{"p","r1"}, {"q","s5"}, {")","r1"}, {"#","r1"}},
        {{"n","s8"}, {"(","s9"}, {"F","s6"}},
        {{"n","r4"}, {"p","r4"}, {"q","r4"}, {"(","r4"}, {")","r4"}, {"#","r4"}},
        {{"n","r3"}, {"p","r3"}, {"q","r3"}, {"(","r3"}, {")","r3"}, {"#","r3"}},
        {{"n","r5"}, {"p","r5"}, {"q","r5"}, {"(","r5"}, {")","r5"}, {"#","r5"}},
        {{"n","s8"}, {"(","s9"}, {"E","s10"}, {"T","s4"}, {"F","s7"}},
        {{"p","s2"}, {")","s11"}},
        {{"n","r6"}, {"p","r6"}, {"q","r6"}, {"(","r6"}, {")","r6"}, {"#","r6"}}
     };
    map<int, string> syntax = {
        {0,"EZ"},
        {1,"TE"}, {2,"EpTE"},
        {3,"FT"}, {4,"TqFT"},
        {5,"nF"}, {6,"(E)F"}    
    };
};

int main() {
    cout << "LR_parsing" << endl;
    string code = read_file("test.txt");
    cout << "string: "<<code<<endl;
    Lexer Lex1;
    int error_flag = 0;
    vector<pair<string,int>> tokens = Lex1.word_analysis(code,error_flag);
    tokens.push_back(make_pair("#",-1));
    if(error_flag) cout << "Error!" << endl;
    LR_parsing parse1;
    parse1.parse(tokens);
}