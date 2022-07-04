#include "scanner.h"
#include "parser.h"
#include "semantic_analyzer.h"
#include "code_generator.h"
#include <fstream>
using namespace std;


string read_file(const char* filename) {
    ifstream infile(filename);
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
    code += " ";
    infile.close();
    return code;
}

int main() {
    string code = read_file("demo.txt");
    scanner Lex1;
    int error_flag = 0;
    vector<pair<string,int>> tokens = Lex1.word_analysis(code,error_flag);
    tokens.push_back(make_pair("#",-1));
    if(error_flag) cout << "Error!" << endl;
    ofstream res_file("tokens.txt");
    for(int i = 0 ; i < tokens.size() ; i ++) {
        res_file << "<" << tokens[i].first << " ," << tokens[i].second << ">" << endl;
    }
    parser parser1;
    parser1.load_tokens(tokens);
    parser1.parse();
    semantic_analyzer semantic_analyzer1;
    semantic_analyzer1.load_tokens(tokens);
    semantic_analyzer1.analyze();
    map<string, id_attribute> ID_table = semantic_analyzer1.getID_table();
    vector<quart> QT = semantic_analyzer1.getQT();
    map<string,string> CT = semantic_analyzer1.getCT();
    code_generator generator;
    generator.load_ID_table(ID_table);
    generator.load_QT(QT);
    generator.load_CT(CT);
    vector<quart> QT1 = generator.getQT();
    map<string, id_attribute> ID_table1 = generator.getID_table();
    for (auto iter : ID_table1) {
		cout << " " << iter.first << " ";
        iter.second.show();
	}
    cout << "QT: " << QT1.size() << endl;
    for(int i = 0 ; i < QT1.size() ; i ++) {
        QT1[i].show();
    }
    generator.generate_code();
    vector<string> obj = generator.getObj();
    for(int i = 0 ; i < obj.size() ; i ++) {
        cout << obj[i] << endl;
    }
    vector<string> RDL = generator.getRDL();
    for(int i = 0 ; i < RDL.size() ; i ++) {
        cout << RDL[i] << "#";
    }
    ofstream mips;
    mips.open("mips.asm",ios::out);
    mips << ".mips" <<endl;
    for(int i = 0 ; i < obj.size() ; i ++) {
        mips << obj[i] << endl;
    }
    mips.close();
}