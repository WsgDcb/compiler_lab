#include "class.h"

class code_generator{
public:
    void load_QT(vector<quart> QT) { this->QT = QT;}

    vector<quart> getQT() { return QT; }

    void load_ID_table(map<string,id_attribute> ID_table) { this->ID_table = ID_table; }

    map<string,id_attribute> getID_table() { return ID_table; }

    void load_CT(map<string,string> CT) { this->CT = CT; }

    vector<string> getObj() { return this->obj; }

    vector<string> getRDL() { return this->RDL; }

    void generate_code() {
        //初始化RDL表
        for(int i = 0 ; i < 15 ; i ++) {
            RDL.push_back(" ");
            RDL_STATE.push_back(" ");
        }
        vector<int> partitions = part_basic_part();
        int start = 0;
        int end = 0;
        int address = start_address;
        for(int j = 0 ; j < partitions.size() ; j ++) {
            start = end;
            end = partitions[j]+1;
            load_Active_Information(start,end-1);
            for(int i = start ; i < end ; i++) {
                quart now_quart = QT[i];
                string now_res = now_quart.res;
                string now_operand1 = now_quart.operand1;
                string now_operand2 = now_quart.operand2;
                string now_operator = now_quart.Operator;
                int index_begin_res, index_end_res;
                string res, state_res;
                if(now_operator != "if" && now_operator != "el" && now_operator != "ie") {
                    index_begin_res = now_res.find("(");
                    index_end_res = now_res.find(")");
                    res = now_res.substr(0,index_begin_res);
                    state_res = now_res.substr(index_begin_res,index_end_res);
                }
                if(now_operator == "=") {
                    int rd1 = 0, rd2 = 0, target_rd = 0;
                    //如果是赋值操作数为常数 
                    if(CT.find(now_operand1) != CT.end()) {
                        int pos = LOCR(res);
                        if(pos >= 15) {
                            int index_free_RD = findfreeRD();
                            if(index_free_RD != -1) {
                                RDL[index_free_RD] = res;
                                RDL_STATE[index_free_RD] = state_res;
                                target_rd = index_free_RD;
                            }
                            else{
                                int pos1 = findRD();
                                if(RDL_STATE[pos1] == "n") {
                                    RDL[pos1] = res;
                                    RDL_STATE[pos1] = state_res;
                                    target_rd = pos1;
                                }
                                else{
                                    string id = RDL[pos1];
                                    if(ID_table[id].getType() == "int") {
                                        string code = "sw $" + to_string(pos1+8)+", "+ to_string(start_address+ID_table[id].getADD());
                                        obj.push_back(code);
                                    }
                                    else{
                                        string code = "sb $" + to_string(pos1+8)+", "+ to_string(start_address+ID_table[id].getADD());
                                        obj.push_back(code);
                                    }
                                    RDL[pos1] = res;
                                    RDL_STATE[pos1] = state_res;
                                    target_rd = pos1;
                                }
                            }
                        }
                        else {
                            RDL_STATE[pos] = state_res;
                            target_rd = pos;
                        }
                        string code = "addi $" + to_string(target_rd+8) + ", $0, " + now_operand1;
                        obj.push_back(code);
                    }
                    //赋值操作数为变量
                    else {
                        int index_begin_1 = now_operand1.find("(");
                        int index_end_1 = now_operand1.find(")");
                        string operand1 = now_operand1.substr(0,index_begin_1);
                        string state1 = now_operand1.substr(index_begin_1,index_end_1);
                        int pos = LOCR(operand1);
                        int pre_pos = LOCR(res);
                        if(pre_pos < 15) {
                            RDL[pre_pos] = " ";
                            RDL_STATE[pre_pos] = " ";
                        }
                        if(pos >= 15) {
                            int index_free_RD = findfreeRD();
                            if(index_free_RD != -1) {
                                RDL[index_free_RD] = res;
                                RDL_STATE[index_free_RD] = state_res;
                                target_rd = index_free_RD;
                                if(ID_table[operand1].getType() == "int") {
                                    string code = "lw $" + to_string(target_rd+8)+", "+ to_string(start_address+ID_table[operand1].getADD());
                                    obj.push_back(code);
                                }
                                else{
                                    string code = "lb $" + to_string(target_rd+8)+", "+ to_string(start_address+ID_table[operand1].getADD());
                                    obj.push_back(code);
                                }
                            }
                            else{
                                int pos1 = findRD();
                                if(RDL_STATE[pos1] == "n") {
                                    RDL[pos1] = res;
                                    RDL_STATE[pos1] = state_res;
                                    
                                }
                                else{
                                    string id = RDL[pos1];
                                    if(ID_table[id].getType() == "int") {
                                        string code = "sw $" + to_string(pos1+8)+", "+ to_string(start_address+ID_table[id].getADD());
                                        obj.push_back(code);
                                    }
                                    else{
                                        string code = "sb $" + to_string(pos1+8)+", "+ to_string(start_address+ID_table[id].getADD());
                                        obj.push_back(code);
                                    }
                                    RDL[pos1] = res;
                                    RDL_STATE[pos1] = state_res;
                                }
                                if(ID_table[operand1].getType() == "int") {
                                    string code = "lw $" + to_string(pos1+8)+", "+ to_string(start_address+ID_table[operand1].getADD());
                                    obj.push_back(code);
                                }
                                else{
                                    string code = "lb $" + to_string(pos1+8)+", "+ to_string(start_address+ID_table[operand1].getADD());
                                    obj.push_back(code);
                                }
                            }
                        }
                        else{
                            if(RDL_STATE[pos] == "n") {
                                RDL[pos] = res;
                                RDL_STATE[pos] = state_res;
                            }
                            else {
                                string id = RDL[pos];
                                if(ID_table[id].getType() == "int") {
                                    string code = "sw $" + to_string(pos+8)+", "+ to_string(start_address+ID_table[id].getADD());
                                    obj.push_back(code);
                                }
                                else{
                                    string code = "sb $" + to_string(pos+8)+", "+ to_string(start_address+ID_table[id].getADD());
                                    obj.push_back(code);
                                }
                                RDL[pos] = res;
                                RDL_STATE[pos] = state_res;
                            }
                        }
                    }
                }
                else if(now_operator == "if") {
                    int index_begin_1 = now_operand1.find("(");
                    int index_end_1 = now_operand1.find(")");
                    string operand1 = now_operand1.substr(0,index_begin_1);
                    string state1 = now_operand1.substr(index_begin_1,index_end_1);
                    string label = "FJ"+to_string(num_if);
                    stk_if.push(label);
                    num_if ++;
                    int pos = LOCR(operand1);
                    if(pos < 15) {
                        string code = "beq $" + to_string(pos+8) + ",$0," + label;
                        obj.push_back(code);
                    }
                    else {
                        int target_rd = load_id(operand1, state1, operand1, state1);
                        string code = "beq $" + to_string(target_rd+8) + ",$0," + label;
                        obj.push_back(code);
                    }
                }
                else if(now_operator == "el") {
                    string label = stk_if.top();
                    stk_if.pop();
                    string label1 = "ie"+to_string(num_ie);
                    num_ie ++;
                    stk_if.push(label1);
                    string code = "j " + label1;
                    obj.push_back(code);
                    code = label + ":";
                    obj.push_back(code);
                }
                else if(now_operator == "ie") {
                    string label = stk_if.top();
                    stk_if.pop();
                    string code = label + ":";
                    obj.push_back(code);
                }
                else if(now_operator == "-" && now_operand2 == "_") {
                    int target_rd = 0;
                    //对常数取负
                    if(CT.find(now_operand1) != CT.end()) {
                        int index_free_RD = findfreeRD();
                        if(index_free_RD != -1) {
                            RDL[index_free_RD] = res;
                            RDL_STATE[index_free_RD] = state_res;
                            target_rd = index_free_RD;
                        }
                        else{
                            int pos1 = findRD();
                            if(RDL_STATE[pos1] == "n") {
                                RDL[pos1] = res;
                                RDL_STATE[pos1] = state_res;
                                target_rd = pos1;
                            }
                            else{
                                string id = RDL[pos1];
                                if(ID_table[id].getType() == "int") {
                                    string code = "sw $" + to_string(pos1+8)+", "+ to_string(start_address+ID_table[id].getADD());
                                    obj.push_back(code);
                                }
                                else{
                                    string code = "sb $" + to_string(pos1+8)+", "+ to_string(start_address+ID_table[id].getADD());
                                    obj.push_back(code);
                                }
                                RDL[pos1] = res;
                                RDL_STATE[pos1] = state_res;
                                target_rd = pos1;
                            }
                        }
                        string code = "addi $" + to_string(target_rd+8)+", $0, "+  now_operand1;
                        obj.push_back(code);
                        code = "not $" + to_string(target_rd+8)+", $"+to_string(target_rd+8);
                        obj.push_back(code);
                        code = "addi $" + to_string(target_rd+8)+", $"+to_string(target_rd+8) + ", 1";
                        obj.push_back(code);
                    }
                    //对标识符取负
                    else {
                        int index_begin_1 = now_operand1.find("(");
                        int index_end_1 = now_operand1.find(")");
                        string operand1 = now_operand1.substr(0,index_begin_1);
                        string state1 = now_operand1.substr(index_begin_1,index_end_1);
                        int pos = LOCR(operand1);
                        if(pos < 15) {
                            set_res_rd(pos, res, state_res);
                            string code = "not $" + to_string(pos+8)+", $"+to_string(pos+8);
                            obj.push_back(code);
                            code = "addi $" + to_string(pos+8)+", $"+to_string(pos+8) + ", 1";
                            obj.push_back(code);
                        }
                        else {
                            target_rd = load_id(operand1, state1, res, state_res);
                            string code = "not $" + to_string(target_rd+8)+", $"+to_string(target_rd+8);
                            obj.push_back(code);
                            code = "addi $" + to_string(target_rd+8)+", $"+to_string(target_rd+8) + ", 1";
                            obj.push_back(code);
                        }
                    }
                }
                else {
                    int target_rd = 0;
                    //如果第一个操作数为常数
                    if(CT.find(now_operand1) != CT.end()) {
                        //如果两个操作数均为常数
                        if(CT.find(now_operand2) != CT.end()) {
                            int index_free_RD = findfreeRD();
                            if(index_free_RD != -1) {
                                RDL[index_free_RD] = res;
                                RDL_STATE[index_free_RD] = state_res;
                                target_rd = index_free_RD;
                            }
                            else{
                                int pos1 = findRD();
                                if(RDL_STATE[pos1] == "n") {
                                    RDL[pos1] = res;
                                    RDL_STATE[pos1] = state_res;
                                    target_rd = pos1;
                                }
                                else{
                                    string id = RDL[pos1];
                                    if(ID_table[id].getType() == "int") {
                                        string code = "sw $" + to_string(pos1+8)+", "+ to_string(start_address+ID_table[id].getADD());
                                        obj.push_back(code);
                                    }
                                    else{
                                        string code = "sb $" + to_string(pos1+8)+", "+ to_string(start_address+ID_table[id].getADD());
                                        obj.push_back(code);
                                    }
                                    RDL[pos1] = res;
                                    RDL_STATE[pos1] = state_res;
                                    target_rd = pos1;
                                }
                            }
                            if(now_operator == "+") {
                                string code = "addi $" + to_string(target_rd+8) + ", $0, " + now_operand1;
                                obj.push_back(code);
                                code = "addi $" + to_string(target_rd+8) + ", $" + to_string(target_rd+8) + ", " + now_operand2;
                                obj.push_back(code);
                            }
                            else if(now_operator == "&&") {
                                string code = "addi $" + to_string(target_rd+8) + ", $0, " + now_operand1;
                                obj.push_back(code);
                                code = "andi $" + to_string(target_rd+8) + ", $" + to_string(target_rd+8) + ", " + now_operand2;
                                obj.push_back(code);
                            }
                            else if(now_operator == "||") {
                                string code = "addi $" + to_string(target_rd+8) + ", $0, " + now_operand1;
                                obj.push_back(code);
                                code = "ori $" + to_string(target_rd+8) + ", $" + to_string(target_rd+8) + ", " + now_operand2;
                                obj.push_back(code);
                            }
                            else if (now_operator == "-") {
                                string code = "addi $" + to_string(target_rd+8) + ", $0, " + now_operand1;
                                obj.push_back(code);
                                code = "subi $" + to_string(target_rd+8) + ", $" + to_string(target_rd+8) + ", " + now_operand2;
                                obj.push_back(code);
                            }
                            else if (now_operator == "*") {
                                string code = "addi $" + to_string(target_rd+8) + ", $0, " + now_operand1;
                                obj.push_back(code);
                                code = "mul $" + to_string(target_rd+8) + ", $" + to_string(target_rd+8) + ", " + now_operand2;
                                obj.push_back(code);
                            } 
                            else if (now_operator == "/") {
                                string code = "addi $" + to_string(target_rd+8) + ", $0, " + now_operand1;
                                obj.push_back(code);
                                code = "div $" + to_string(target_rd+8) + ", $" + to_string(target_rd+8) + ", " + now_operand2;
                                obj.push_back(code);
                            }
                            else if(now_operator == ">" || now_operator == "<" || now_operator == "<=" || now_operator == ">=" || now_operator == "==") {
                                string code = "addi $" + to_string(target_rd+8) + ", $0, " + now_operand1;
                                obj.push_back(code);
                                code = "subi $" + to_string(target_rd+8) + ", $" + to_string(target_rd+8) + ", " + now_operand2;
                                obj.push_back(code);
                                string label = "false" + to_string(num_false);
                                num_false ++;
                                string label2 = "true" + to_string(num_true);
                                num_true ++;
                                if(now_operator == ">"){
                                    code = "blez $" + to_string(target_rd+8) + "," + label;
                                    obj.push_back(code);
                                }
                                else if(now_operator == "<") {
                                    code = "bgez $" + to_string(target_rd+8) + "," + label;
                                    obj.push_back(code);
                                }
                                else if(now_operator == "<=") {
                                    code = "bgtz $" + to_string(target_rd+8) + "," + label;
                                    obj.push_back(code);
                                }
                                else if(now_operator == ">=") {
                                    code = "bltz $" + to_string(target_rd+8) + "," + label;
                                    obj.push_back(code);
                                }
                                else if(now_operator == "==") {
                                    code = "bne $" + to_string(target_rd+8) + ", $0, " + label;
                                    obj.push_back(code);
                                }
                                code = "addi $" + to_string(target_rd+8) +", $0, 1";
                                obj.push_back(code);
                                code = "j "+ label2;
                                obj.push_back(code);
                                string label1 = label+":";
                                obj.push_back(label1);
                                code = "addi $" + to_string(target_rd+8) +", $0, 0";
                                obj.push_back(code);
                                code = label2 + ":";
                                obj.push_back(code);    
                            } 
                        }
                        //如果第二个操作数为变量
                        else{
                            int index_begin_2 = now_operand2.find("(");
                            int index_end_2 = now_operand2.find(")");
                            string operand2 = now_operand2.substr(0,index_begin_2);
                            string state2 = now_operand2.substr(index_begin_2,index_end_2);
                            int pos_operand2 = LOCR(operand2);
                            if(pos_operand2 < 15) {
                                set_res_rd(pos_operand2, res, state_res);
                                if(now_operator == "+") {
                                    string code = "addi $" + to_string(pos_operand2+8) + ", $" + to_string(pos_operand2+8) + ", " +now_operand1;
                                    obj.push_back(code);
                                }
                                else if(now_operator == "&&") {
                                    string code = "andi $" + to_string(pos_operand2+8) + ", $" + to_string(pos_operand2+8) + ", " +now_operand1;
                                    obj.push_back(code);
                                }
                                else if(now_operator == "||") {
                                    string code = "ori $" + to_string(pos_operand2+8) + ", $" + to_string(pos_operand2+8) + ", " +now_operand1;
                                    obj.push_back(code);
                                }
                                else if(now_operator == "-") {
                                    string code = "not $" + to_string(pos_operand2+8) + ", $" + to_string(pos_operand2+8);
                                    obj.push_back(code);
                                    code = "addi $" + to_string(pos_operand2+8) + ", $" + to_string(pos_operand2+8) + ", 1";
                                    obj.push_back(code);
                                    code = "addi $" + to_string(pos_operand2+8) + ", $" + to_string(pos_operand2+8) + ", " + now_operand1;
                                    obj.push_back(code);
                                }
                                else if(now_operator == "*") {
                                    string code = "mul $" + to_string(pos_operand2+8) + ", $" + to_string(pos_operand2+8) + ", " +now_operand1;
                                    obj.push_back(code);
                                }
                                else if(now_operator == "/") {
                                    string code = "addi $24, $0 " + now_operand1;
                                    obj.push_back(code); 
                                    code = "div $" + to_string(pos_operand2+8) + " $24, $" + to_string(pos_operand2+8);
                                    obj.push_back(code);
                                }
                                else if(now_operator == ">" || now_operator == "<" || now_operator == "<=" || now_operator == ">=" || now_operator == "==") {
                                    string code = "not $" + to_string(pos_operand2+8) + ", $" + to_string(pos_operand2+8);
                                    obj.push_back(code);
                                    code = "addi $" + to_string(pos_operand2+8) + ", $" + to_string(pos_operand2+8) + ", 1";
                                    obj.push_back(code);
                                    code = "addi $" + to_string(pos_operand2+8) + ", $" + to_string(pos_operand2+8) + ", " + now_operand1;
                                    obj.push_back(code);
                                    string label = "false" + to_string(num_false);
                                    num_false ++;
                                    string label2 = "true" + to_string(num_true);
                                    num_true ++;
                                    if(now_operator == ">"){
                                        code = "blez $" + to_string(pos_operand2+8) + "," + label;
                                        obj.push_back(code);
                                    }
                                    else if(now_operator == "<") {
                                        code = "bgez $" + to_string(pos_operand2+8) + "," + label;
                                        obj.push_back(code);
                                    }
                                    else if(now_operator == "<=") {
                                        code = "bgtz $" + to_string(pos_operand2+8) + "," + label;
                                        obj.push_back(code);
                                    }
                                    else if(now_operator == ">=") {
                                        code = "bltz $" + to_string(pos_operand2+8) + "," + label;
                                        obj.push_back(code);
                                    }
                                    else if(now_operator == "==") {
                                        code = "bne $" + to_string(target_rd+8) + ", $0, " + label;
                                        obj.push_back(code);
                                    }
                                    code = "addi $" + to_string(pos_operand2+8) +", $0, 1";
                                    obj.push_back(code);
                                    code = "j "+ label2;
                                    obj.push_back(code);
                                    string label1 = label+":";
                                    obj.push_back(label1);
                                    code = "addi $" + to_string(pos_operand2+8) +", $0, 0";
                                    obj.push_back(code);
                                    code = label2 + ":";
                                    obj.push_back(code); 
                                } 
                            }
                            else {
                                target_rd = load_id(operand2, state2, res, state_res);
                                if(now_operator == "+") {
                                    string code = "addi $" + to_string(target_rd+8) + ", $" + to_string(target_rd+8) + ", " +now_operand1;
                                    obj.push_back(code);
                                }
                                else if(now_operator == "&&") {
                                    string code = "andi $" + to_string(target_rd+8) + ", $" + to_string(target_rd+8) + ", " +now_operand1;
                                    obj.push_back(code);
                                }
                                else if(now_operator == "||") {
                                    string code = "ori $" + to_string(target_rd+8) + ", $" + to_string(target_rd+8) + ", " +now_operand1;
                                    obj.push_back(code);
                                }
                                else if(now_operator == "-") {
                                    string code = "not $" + to_string(target_rd+8) + ", $" + to_string(target_rd+8);
                                    obj.push_back(code);
                                    code = "addi $" + to_string(target_rd+8) + ", $" + to_string(target_rd+8) + ", 1";
                                    obj.push_back(code);
                                    code = "addi $" + to_string(target_rd+8) + ", $" + to_string(target_rd+8) + ", " + now_operand1;
                                    obj.push_back(code);
                                }
                                else if(now_operator == "*") {
                                    string code = "mul $" + to_string(target_rd+8) + ", $" + to_string(target_rd+8) + ", " +now_operand1;
                                    obj.push_back(code);
                                }
                                else if(now_operator == "/") {
                                    string code = "addi $24, $0, " + now_operand1;
                                    obj.push_back(code); 
                                    code = "div $" + to_string(pos_operand2+8) + " $24, $" + to_string(pos_operand2+8);
                                    obj.push_back(code);
                                }
                                else if(now_operator == ">" || now_operator == "<" || now_operator == "<=" || now_operator == ">=" || now_operator == "==") {
                                    string code = "not $" + to_string(target_rd+8) + ", $" + to_string(target_rd+8);
                                    obj.push_back(code);
                                    code = "addi $" + to_string(target_rd+8) + ", $" + to_string(target_rd+8) + ", 1";
                                    obj.push_back(code);
                                    code = "addi $" + to_string(target_rd+8) + ", $" + to_string(target_rd+8) + ", " + now_operand1;
                                    obj.push_back(code);
                                    string label = "false" + to_string(num_false);
                                    num_false ++;
                                    string label2 = "true" + to_string(num_true);
                                    num_true ++;
                                    if(now_operator == ">"){
                                        code = "blez $" + to_string(target_rd+8) + "," + label;
                                        obj.push_back(code);
                                    }
                                    else if(now_operator == "<") {
                                        code = "bgez $" + to_string(target_rd+8) + "," + label;
                                        obj.push_back(code);
                                    }
                                    else if(now_operator == "<=") {
                                        code = "bgtz $" + to_string(target_rd+8) + "," + label;
                                        obj.push_back(code);
                                    }
                                    else if(now_operator == ">=") {
                                        code = "bltz $" + to_string(target_rd+8) + "," + label;
                                        obj.push_back(code);
                                    }
                                    else if(now_operator == "==") {
                                        code = "bne $" + to_string(target_rd+8) + ", $0, " + label;
                                        obj.push_back(code);
                                    }
                                    code = "addi $" + to_string(target_rd+8) +", $0, 1";
                                    obj.push_back(code);
                                    code = "j "+ label2;
                                    obj.push_back(code);
                                    string label1 = label+":";
                                    obj.push_back(label1);
                                    code = "addi $" + to_string(target_rd+8) +", $0, 0";
                                    obj.push_back(code);
                                    code = label2 + ":";
                                    obj.push_back(code); 
                                }
                            }
                        }
                    }
                    else {
                        //如果第一个操作数为变量，第二个为常数时
                        if(CT.find(now_operand2) != CT.end()) {
                            int index_begin_1 = now_operand1.find("(");
                            int index_end_1 = now_operand1.find(")");
                            string operand1 = now_operand1.substr(0,index_begin_1);
                            string state1 = now_operand1.substr(index_begin_1,index_end_1);
                            int pos_operand1 = LOCR(operand1);
                            if(pos_operand1 < 15) {
                                set_res_rd(pos_operand1, res, state_res);
                                if(now_operator == "+") {
                                    string code = "addi $" + to_string(pos_operand1+8) + ", $" + to_string(pos_operand1+8) + ", " +now_operand2;
                                    obj.push_back(code);
                                }
                                else if(now_operator == "-"){
                                    string code = "subi $" + to_string(pos_operand1+8) + ", $" + to_string(pos_operand1+8) + ", " +now_operand2;
                                    obj.push_back(code);
                                }
                                else if(now_operator == "&&") {
                                    string code = "andi $" + to_string(pos_operand1+8) + ", $" + to_string(pos_operand1+8) + ", " +now_operand2;
                                    obj.push_back(code);
                                }
                                else if(now_operator == "||") {
                                    string code = "ori $" + to_string(pos_operand1+8) + ", $" + to_string(pos_operand1+8) + ", " +now_operand2;
                                    obj.push_back(code);
                                }
                                else if(now_operator == "*") {
                                    string code = "mul $" + to_string(pos_operand1+8) + ", $" + to_string(pos_operand1+8) + ", " +now_operand2;
                                    obj.push_back(code);
                                }
                                else if(now_operator == "/") {
                                    string code = "div $" + to_string(pos_operand1+8) + ", $" + to_string(pos_operand1+8) + ", " +now_operand2;
                                    obj.push_back(code);
                                }
                                else if(now_operator == ">" || now_operator == "<" || now_operator == "<=" || now_operator == ">=" || now_operator == "==") {
                                    string code = "subi $" + to_string(pos_operand1+8) + ", $" + to_string(pos_operand1+8) + ", " +now_operand2;
                                    obj.push_back(code);
                                    string label = "false" + to_string(num_false);
                                    num_false ++;
                                    string label2 = "true" + to_string(num_true);
                                    num_true ++;
                                    if(now_operator == ">"){
                                        code = "blez $" + to_string(pos_operand1+8) + "," + label;
                                        obj.push_back(code);
                                    }
                                    else if(now_operator == "<") {
                                        code = "bgez $" + to_string(pos_operand1+8) + "," + label;
                                        obj.push_back(code);
                                    }
                                    else if(now_operator == "<=") {
                                        code = "bgtz $" + to_string(pos_operand1+8) + "," + label;
                                        obj.push_back(code);
                                    }
                                    else if(now_operator == ">=") {
                                        code = "bltz $" + to_string(pos_operand1+8) + "," + label;
                                        obj.push_back(code);
                                    }
                                    else if(now_operator == "==") {
                                        code = "bne $" + to_string(pos_operand1+8) + ", $0, " + label;
                                        obj.push_back(code);
                                    }
                                    code = "addi $" + to_string(pos_operand1+8) +", $0, 1";
                                    obj.push_back(code);
                                    code = "j "+ label2;
                                    obj.push_back(code);
                                    string label1 = label+":";
                                    obj.push_back(label1);
                                    code = "addi $" + to_string(pos_operand1+8) +", $0, 0";
                                    obj.push_back(code);
                                    code = label2 + ":";
                                    obj.push_back(code);  
                                }
                            }
                            else{
                                target_rd = load_id(operand1, state1, res, state_res);
                                if(now_operator == "+") {
                                    string code = "addi $" + to_string(target_rd+8) + ", $" + to_string(target_rd+8) + ", " +now_operand2;
                                    obj.push_back(code);
                                }
                                else if(now_operator == "-"){
                                    string code = "subi $" + to_string(target_rd+8) + ", $" + to_string(target_rd+8) + ", " +now_operand2;
                                    obj.push_back(code);
                                }
                                else if(now_operator == "&&") {
                                    string code = "andi $" + to_string(target_rd+8) + ", $" + to_string(target_rd+8) + ", " +now_operand2;
                                    obj.push_back(code);
                                }
                                else if(now_operator == "||") {
                                    string code = "ori $" + to_string(target_rd+8) + ", $" + to_string(target_rd+8) + ", " +now_operand2;
                                    obj.push_back(code);
                                }
                                else if(now_operator == "*") {
                                    string code = "mul $" + to_string(target_rd+8) + ", $" + to_string(target_rd+8) + ", " +now_operand2;
                                    obj.push_back(code);
                                }
                                else if(now_operator == "/") {
                                    string code = "div $" + to_string(target_rd+8) + ", $" + to_string(target_rd+8) + ", " +now_operand2;
                                    obj.push_back(code);
                                }
                                else if(now_operator == ">" || now_operator == "<" || now_operator == "<=" || now_operator == ">=" || now_operator == "==") {
                                    string code = "subi $" + to_string(target_rd+8) + ", $" + to_string(target_rd+8) + ", " +now_operand2;
                                    obj.push_back(code);
                                    string label = "false" + to_string(num_false);
                                    num_false ++;
                                    string label2 = "true" + to_string(num_true);
                                    num_true ++;
                                    if(now_operator == ">"){
                                        code = "blez $" + to_string(target_rd+8) + "," + label;
                                        obj.push_back(code);
                                    }
                                    else if(now_operator == "<") {
                                        code = "bgez $" + to_string(target_rd+8) + "," + label;
                                        obj.push_back(code);
                                    }
                                    else if(now_operator == "<=") {
                                        code = "bgtz $" + to_string(target_rd+8) + "," + label;
                                        obj.push_back(code);
                                    }
                                    else if(now_operator == ">=") {
                                        code = "bltz $" + to_string(target_rd+8) + "," + label;
                                        obj.push_back(code);
                                    }
                                    else if(now_operator == "==") {
                                        code = "bne $" + to_string(target_rd+8) + ", $0, " + label;
                                        obj.push_back(code);
                                    }
                                    code = "addi $" + to_string(target_rd+8) +", $0, 1";
                                    obj.push_back(code);
                                    code = "j "+ label2;
                                    obj.push_back(code);
                                    string label1 = label+":";
                                    obj.push_back(label1);
                                    code = "addi $" + to_string(target_rd+8) +", $0, 0";
                                    obj.push_back(code);
                                    code = label2 + ":";
                                    obj.push_back(code);    
                                }
                            }
                            
                        }
                        //如果两个都为变量时
                        else{
                            int index_begin_1 = now_operand1.find("(");
                            int index_end_1 = now_operand1.find(")");
                            string operand1 = now_operand1.substr(0,index_begin_1);
                            string state1 = now_operand1.substr(index_begin_1,index_end_1);
                            int index_begin_2 = now_operand2.find("(");
                            int index_end_2 = now_operand2.find(")");
                            string operand2 = now_operand2.substr(0,index_begin_2);
                            string state2 = now_operand2.substr(index_begin_2,index_end_2);

                            int pos_operand1 = LOCR(operand1);
                            int pos_operand2 = LOCR(operand2);

                            if(pos_operand1 < 15) {
                                set_res_rd(pos_operand1, res, state_res);
                                //如果两个操作数均在寄存器中
                                if(pos_operand2 < 15) {
                                    if(now_operator == "+") {
                                        string code = "add $" + to_string(pos_operand1+8) +", $"+to_string(pos_operand1+8)+", $"+to_string(pos_operand2+8);
                                        obj.push_back(code); 
                                    }
                                    else if(now_operator == "&&") {
                                        string code = "and $" + to_string(pos_operand1+8) +", $"+to_string(pos_operand1+8)+", $"+to_string(pos_operand2+8);
                                        obj.push_back(code); 
                                    }
                                    else if(now_operator == "||") {
                                        string code = "or $" + to_string(pos_operand1+8) +", $"+to_string(pos_operand1+8)+", $"+to_string(pos_operand2+8);
                                        obj.push_back(code); 
                                    }
                                    else if(now_operator == "-") {
                                        string code = "sub $" + to_string(pos_operand1+8) +", $"+to_string(pos_operand1+8)+", $"+to_string(pos_operand2+8);
                                        obj.push_back(code); 
                                    }
                                    else if(now_operator == "*") {
                                        string code = "mul $" + to_string(pos_operand1+8) +", $"+to_string(pos_operand1+8)+", $"+to_string(pos_operand2+8);
                                        obj.push_back(code); 
                                    }
                                    else if(now_operator == "/") {
                                        string code = "div $" + to_string(pos_operand1+8) +", $"+to_string(pos_operand1+8)+", $"+to_string(pos_operand2+8);
                                        obj.push_back(code); 
                                    }
                                    else if(now_operator == ">" || now_operator == "<" || now_operator == "<=" || now_operator == ">=" || now_operator == "==") {
                                        string code = "sub $" + to_string(pos_operand1+8) +", $"+to_string(pos_operand1+8)+", $"+to_string(pos_operand2+8);
                                        obj.push_back(code);
                                        string label = "false" + to_string(num_false);
                                        num_false ++;
                                        string label2 = "true" + to_string(num_true);
                                        num_true ++;
                                        if(now_operator == ">"){
                                            code = "blez $" + to_string(pos_operand1+8) + "," + label;
                                            obj.push_back(code);
                                        }
                                        else if(now_operator == "<") {
                                            code = "bgez $" + to_string(pos_operand1+8) + "," + label;
                                            obj.push_back(code);
                                        }
                                        else if(now_operator == "<=") {
                                            code = "bgtz $" + to_string(pos_operand1+8) + "," + label;
                                            obj.push_back(code);
                                        }
                                        else if(now_operator == ">=") {
                                            code = "bltz $" + to_string(pos_operand1+8) + "," + label;
                                            obj.push_back(code);
                                        }
                                        else if(now_operator == "==") {
                                            code = "bne $" + to_string(pos_operand1+8) + ", $0, " + label;
                                            obj.push_back(code);
                                        }
                                        code = "addi $" + to_string(pos_operand1+8) +", $0, 1";
                                        obj.push_back(code);
                                        code = "j "+ label2;
                                        obj.push_back(code);
                                        string label1 = label+":";
                                        obj.push_back(label1);
                                        code = "addi $" + to_string(pos_operand1+8) +", $0, 0";
                                        obj.push_back(code);
                                        code = label2 + ":";
                                        obj.push_back(code);
                                    }
                                }
                                //操作数1在而操作数2不在
                                else{
                                    target_rd = load_id(operand2, state2, operand2, state2);
                                    if(now_operator == "+") {
                                        string code = "add $" + to_string(pos_operand1+8) +", $"+to_string(pos_operand1+8)+", $"+to_string(target_rd+8);
                                        obj.push_back(code);
                                    } 
                                    else if(now_operator == "-"){
                                        string code = "sub $" + to_string(pos_operand1+8) +", $"+to_string(pos_operand1+8)+", $"+to_string(target_rd+8);
                                        obj.push_back(code); 
                                    }
                                    else if(now_operator == "&&") {
                                        string code = "and $" + to_string(pos_operand1+8) +", $"+to_string(pos_operand1+8)+", $"+to_string(target_rd+8);
                                        obj.push_back(code);
                                    } 
                                    else if(now_operator == "||") {
                                        string code = "or $" + to_string(pos_operand1+8) +", $"+to_string(pos_operand1+8)+", $"+to_string(target_rd+8);
                                        obj.push_back(code);
                                    } 
                                    else if(now_operator == "*") {
                                        string code = "mul $" + to_string(pos_operand1+8) +", $"+to_string(pos_operand1+8)+", $"+to_string(target_rd+8);
                                        obj.push_back(code); 
                                    }
                                    else if(now_operator == "/") {
                                        string code = "div $" + to_string(pos_operand1+8) +", $"+to_string(pos_operand1+8)+", $"+to_string(target_rd+8);
                                        obj.push_back(code); 
                                    }
                                    else if(now_operator == ">" || now_operator == "<" || now_operator == "<=" || now_operator == ">=" || now_operator == "==") {
                                        string code = "sub $" + to_string(pos_operand1+8) +", $"+to_string(pos_operand1+8)+", $"+to_string(target_rd+8);
                                        obj.push_back(code);
                                        string label = "false" + to_string(num_false);
                                        num_false ++;
                                        string label2 = "true" + to_string(num_true);
                                        num_true ++;
                                        if(now_operator == ">"){
                                            code = "blez $" + to_string(pos_operand1+8) + "," + label;
                                            obj.push_back(code);
                                        }
                                        else if(now_operator == "<") {
                                            code = "bgez $" + to_string(pos_operand1+8) + "," + label;
                                            obj.push_back(code);
                                        }
                                        else if(now_operator == "<=") {
                                            code = "bgtz $" + to_string(pos_operand1+8) + "," + label;
                                            obj.push_back(code);
                                        }
                                        else if(now_operator == ">=") {
                                            code = "bltz $" + to_string(pos_operand1+8) + "," + label;
                                            obj.push_back(code);
                                        }
                                        else if(now_operator == "==") {
                                            code = "bne $" + to_string(pos_operand1+8) + ", $0, " + label;
                                            obj.push_back(code);
                                        }
                                        code = "addi $" + to_string(pos_operand1+8) +", $0, 1";
                                        obj.push_back(code);
                                        code = "j "+ label2;
                                        obj.push_back(code);
                                        string label1 = label+":";
                                        obj.push_back(label1);
                                        code = "addi $" + to_string(pos_operand1+8) +", $0, 0";
                                        obj.push_back(code);
                                        code = label2 + ":";
                                        obj.push_back(code);
                                    }
                                }
                            }
                            else if(pos_operand2 < 15) {
                                set_res_rd(pos_operand2, res, state_res);
                                target_rd = load_id(operand1, state1, operand1, state1);
                                if(now_operator == "+") {
                                    string code = "add $" + to_string(pos_operand2+8) +", $"+to_string(pos_operand2+8)+", $"+to_string(target_rd+8);
                                    obj.push_back(code);
                                }
                                else if(now_operator == "-"){
                                    string code = "sub $" + to_string(pos_operand2+8) +", $"+to_string(target_rd+8)+", $"+to_string(pos_operand2+8);
                                    obj.push_back(code); 
                                }
                                else if(now_operator == "&&") {
                                    string code = "and $" + to_string(pos_operand2+8) +", $"+to_string(pos_operand2+8)+", $"+to_string(target_rd+8);
                                    obj.push_back(code);
                                }
                                else if(now_operator == "||") {
                                    string code = "or $" + to_string(pos_operand2+8) +", $"+to_string(pos_operand2+8)+", $"+to_string(target_rd+8);
                                    obj.push_back(code);
                                }
                                else if(now_operator == "*") {
                                    string code = "mul $" + to_string(pos_operand2+8) +", $"+to_string(pos_operand2+8)+", $"+to_string(target_rd+8);
                                    obj.push_back(code);
                                }
                                else if(now_operator == "/") {
                                    string code = "div $" + to_string(pos_operand2+8) +", $"+to_string(target_rd+8)+", $"+to_string(pos_operand2+8);
                                    obj.push_back(code);
                                }
                                else if(now_operator == ">" || now_operator == "<" || now_operator == "<=" || now_operator == ">=" || now_operator == "==") {
                                    string code = "sub $" + to_string(pos_operand2+8) +", $"+to_string(target_rd+8)+", $"+to_string(pos_operand2+8);
                                    obj.push_back(code);
                                    string label = "false" + to_string(num_false);
                                    num_false ++;
                                    string label2 = "true" + to_string(num_true);
                                    num_true ++;
                                    if(now_operator == ">"){
                                        code = "blez $" + to_string(pos_operand2+8) + "," + label;
                                        obj.push_back(code);
                                    }
                                    else if(now_operator == "<") {
                                        code = "bgez $" + to_string(pos_operand2+8) + "," + label;
                                        obj.push_back(code);
                                    }
                                    else if(now_operator == "<=") {
                                        code = "bgtz $" + to_string(pos_operand2+8) + "," + label;
                                        obj.push_back(code);
                                    }
                                    else if(now_operator == ">=") {
                                        code = "bltz $" + to_string(pos_operand2+8) + "," + label;
                                        obj.push_back(code);
                                    }
                                    else if(now_operator == "==") {
                                        code = "bne $" + to_string(pos_operand2+8) + ", $0, " + label;
                                        obj.push_back(code);
                                    }
                                    code = "addi $" + to_string(pos_operand2+8) +", $0, 1";
                                    obj.push_back(code);
                                    code = "j "+ label2;
                                    obj.push_back(code);
                                    string label1 = label+":";
                                    obj.push_back(label1);
                                    code = "addi $" + to_string(pos_operand2+8) +", $0, 0";
                                    obj.push_back(code);
                                    code = label2 + ":";
                                    obj.push_back(code);   
                                }
                            }
                            else {
                                int target_rd1 = load_id(operand1, state1,operand1, state1);
                                int target_rd2 = load_id(operand2, state2,operand2, state2);
                                set_res_rd(target_rd1, res, state_res);
                                if(now_operator == "+") {
                                    string code = "add $" + to_string(target_rd1+8) +", $"+to_string(target_rd1+8)+", $"+to_string(target_rd2+8);
                                    obj.push_back(code);
                                }
                                else if(now_operator == "&&") {
                                    string code = "and $" + to_string(target_rd1+8) +", $"+to_string(target_rd1+8)+", $"+to_string(target_rd2+8);
                                    obj.push_back(code);
                                }
                                else if(now_operator == "||") {
                                    string code = "or $" + to_string(target_rd1+8) +", $"+to_string(target_rd1+8)+", $"+to_string(target_rd2+8);
                                    obj.push_back(code);
                                }
                                else if(now_operator == "-"){
                                    string code = "sub $" + to_string(target_rd1+8) +", $"+to_string(target_rd1+8)+", $"+to_string(target_rd2+8);
                                    obj.push_back(code);
                                }
                                else if(now_operator == "*") {
                                    string code = "mul $" + to_string(target_rd1+8) +", $"+to_string(target_rd1+8)+", $"+to_string(target_rd2+8);
                                    obj.push_back(code);
                                }
                                else if(now_operator == "/") {
                                    string code = "div $" + to_string(target_rd1+8) +", $"+to_string(target_rd1+8)+", $"+to_string(target_rd2+8);
                                    obj.push_back(code);
                                }
                                else if(now_operator == ">" || now_operator == "<" || now_operator == "<=" || now_operator == ">=" || now_operator == "==") {
                                    string code = "sub $" + to_string(target_rd1+8) +", $"+to_string(target_rd1+8)+", $"+to_string(target_rd2+8);
                                    obj.push_back(code);
                                    string label = "false" + to_string(num_false);
                                    num_false ++;
                                    string label2 = "true" + to_string(num_true);
                                    num_true ++;
                                    if(now_operator == ">"){
                                        code = "blez $" + to_string(target_rd1+8) + "," + label;
                                        obj.push_back(code);
                                    }
                                    else if(now_operator == "<") {
                                        code = "bgez $" + to_string(target_rd1+8) + "," + label;
                                        obj.push_back(code);
                                    }
                                    else if(now_operator == "<=") {
                                        code = "bgtz $" + to_string(target_rd1+8) + "," + label;
                                        obj.push_back(code);
                                    }
                                    else if(now_operator == ">=") {
                                        code = "bltz $" + to_string(target_rd1+8) + "," + label;
                                        obj.push_back(code);
                                    }
                                    else if(now_operator == "==") {
                                        code = "bne $" + to_string(target_rd1+8) + ", $0, " + label;
                                        obj.push_back(code);
                                    }
                                    code = "addi $" + to_string(target_rd1+8) +", $0, 1";
                                    obj.push_back(code);
                                    code = "j "+ label2;
                                    obj.push_back(code);
                                    string label1 = label+":";
                                    obj.push_back(label1);
                                    code = "addi $" + to_string(target_rd1+8) +", $0, 0";
                                    obj.push_back(code);
                                    code = label2 + ":";
                                    obj.push_back(code);    
                                }
                            }
                        }
                    }
                }
            }
            release_RD();
        }
        release_all();
    }

private:
    int LOCR(string id) {
        for(int i = 0 ; i < RDL.size() ; i ++) {
            if(RDL[i] == id) return i;
        }
        return start_address + ID_table[id].getADD();
    }
    int findRD() {
        int index = -1;
        int max = -1;
        for(int i = 0 ; i < 15 ; i ++) {
            if(RDL_STATE[i] == "n") {
                return i;
            }
            else if(RDL_STATE[i] == "y") continue;
            else{
                int pos = atoi(RDL_STATE[i].c_str());
                if(pos > max) {
                    pos = max;
                    index = i;
                }
            }
        }
        return index;
    }
    int findfreeRD() {
        int index = -1;
        for (int i = 0 ; i < 15; i ++) {
            if(RDL_STATE[i] == " ") {
                return i;
            }
        }
        return index;
    }

    int load_id(string id_m, string state, string id1, string state1) {
        int target_rd = -1;
        int index_free_RD = findfreeRD();
        if(index_free_RD != -1) {
            RDL[index_free_RD] = id1;
            RDL_STATE[index_free_RD] = state1;
            target_rd = index_free_RD;
            if(ID_table[id_m].getType() == "int") {
                string code = "lw $" + to_string(target_rd+8)+", "+ to_string(start_address+ID_table[id_m].getADD());
                obj.push_back(code);
            }
            else{
                string code = "lb $" + to_string(target_rd+8)+", "+ to_string(start_address+ID_table[id_m].getADD());
                obj.push_back(code);
            }
        }
        else {
            int pos1 = findRD();
            if(RDL_STATE[pos1] == "n") {
                RDL[pos1] = id1;
                RDL_STATE[pos1] = state1;
            }
            else{
                string id = RDL[pos1];
                if(ID_table[id].getType() == "int") {
                    string code = "sw $" + to_string(pos1+8)+", "+ to_string(start_address+ID_table[id].getADD());
                    obj.push_back(code);
                }
                else{
                    string code = "sb $" + to_string(pos1+8)+", "+ to_string(start_address+ID_table[id].getADD());
                    obj.push_back(code);
                }
                RDL[pos1] = id1;
                RDL_STATE[pos1] = state1;
            }
            if(ID_table[id_m].getType() == "int") {
                string code = "lw $" + to_string(pos1+8)+", "+ to_string(start_address+ID_table[id_m].getADD());
                obj.push_back(code);
            }
            else{
                string code = "lb $" + to_string(pos1+8)+", "+ to_string(start_address+ID_table[id_m].getADD());
                obj.push_back(code);
            }
            target_rd  = pos1;
        }
        return target_rd;
    }

    void set_res_rd(int pos, string res, string state_res) {
        if(RDL_STATE[pos] == "n") {
            RDL[pos] = res;
            RDL_STATE[pos] = state_res;
        }
        else{
            string id = RDL[pos];
            if(ID_table[id].getType() == "int") {
                string code = "sw $" + to_string(pos+8)+", "+ to_string(start_address+ID_table[id].getADD());
                obj.push_back(code);
            }
            else{
                string code = "sb $" + to_string(pos+8)+", "+ to_string(start_address+ID_table[id].getADD());
                obj.push_back(code);
            }
            RDL[pos] = res;
            RDL_STATE[pos] = state_res;
        }
    }

    vector<int> part_basic_part() {
        vector<int> partitions;
        for(int i = 0 ; i < QT.size() ; i ++) {
            quart now_quart = QT[i];
            string now_operator = now_quart.Operator;
            if(i!=QT.size()-1&&(now_operator == "if" || now_operator == "el" || now_operator == "ie")){
                partitions.push_back(i);
            }
        }
        partitions.push_back(QT.size()-1);
        return partitions;
    }

    void load_Active_Information(int start, int end) {
        int QT_size = QT.size();
        for(int i = end ; i >= start ; i --) {
            quart now_quart = QT[i];
            string now_res = now_quart.res;
            string now_operand1 = now_quart.operand1;
            string now_operand2 = now_quart.operand2;
            if(now_res != "_" || now_quart.Operator == "if") {
                QT[i].res += ID_table[now_res].STATE;
                ID_table[now_res].STATE = "(n)";
                if(now_operand2 != "_" && (CT.find(now_operand2) == CT.end())) {
                    QT[i].operand2 += ID_table[now_operand2].STATE;
                    ID_table[now_operand2].STATE = "("+ to_string(i+1) +")";
                }
                if(now_operand1 != "_" && (CT.find(now_operand1) == CT.end())) {
                    QT[i].operand1 += ID_table[now_operand1].STATE;
                    ID_table[now_operand1].STATE = "("+ to_string(i+1) +")";
                }
            }
        }
    }

    void release_RD() {
        for(int i = 0; i < 15 ; i ++) {
            if(RDL_STATE[i]=="n") {
                if(ID_table[RDL[i]].getType() == "int") {
                    string code = "sw $" + to_string(i+8)+", "+ to_string(start_address+ID_table[RDL[i]].getADD());
                    obj.push_back(code);
                }
                else{
                    string code = "sb $" + to_string(i+8)+", "+ to_string(start_address+ID_table[RDL[i]].getADD());
                    obj.push_back(code);
                }
                RDL[i] = " ";
                RDL_STATE[i] = " ";
            }
        }
    }

    void release_all() {
        for(int i = 0; i < 15 ; i ++) {
            if(RDL_STATE[i] != " ") {
                if(ID_table[RDL[i]].getType() == "int") {
                    string code = "sw $" + to_string(i+8)+", "+ to_string(start_address+ID_table[RDL[i]].getADD());
                    obj.push_back(code);
                }
                else{
                    string code = "sb $" + to_string(i+8)+", "+ to_string(start_address+ID_table[RDL[i]].getADD());
                    obj.push_back(code);
                }
                RDL[i] = " ";
                RDL_STATE[i] = " ";
            }
        }
    }

    map<string,id_attribute> ID_table;
    
    map<string,string> CT;

    vector<quart> QT;

    vector<string> RDL;
    vector<string> RDL_STATE;

    vector<string> obj;
    stack<string> stk_if;

    int start_address = 0x00002000;
    int num_if = 0;
    int num_ie = 0;
    int num_false = 0;
    int num_true = 0;
    int index_free_RD = 0;
};