#include <bits/stdc++.h>
using namespace std;

// de luu label va dia chi tuong ung cua label do
unordered_map<string, int> labelTable;
int address = 0x100000; // Địa chỉ ban đầu của chương trình máy

// ham xoa comment va xoa khoang trang du
string deleteComment(string line) {
    int pos = line.find('#');
    if (pos != string::npos) {
        line = line.substr(0, pos);
    }
    stringstream ss(line);
    string word;
    string output;
    while(ss >> word){
        output += word + " ";
    }
    output.erase(output.find_last_not_of(' ') + 1);
    if(!output.empty())
        return output;
    else return "";
}

// ham tinh toan dia chi cho cac label
int calculateImmediate(string line){
    int colonpos = line.find(':');
    if(colonpos != string::npos){
        string lable = line.substr(0, colonpos);
        labelTable[lable] = address;
    }
    return colonpos;
}

//ham xay dung bang label va xoa label khoi chuong trinh
void buildLabelTable(ifstream& input){
    string line;
    while(getline(input, line)){
        int pos = calculateImmediate(line);
        address += 4;
        line.erase(0, pos);
        line = deleteComment(line);
    }
    input.clear();
    input.seekg(0, ios::beg);
}
// chuyen doi opcode thanh ma nhi phan
string getOpcodeBinary(string opcode) {
    if (opcode == "add") {
        return "000000";
    } else if (opcode == "addu") {
        return "000000";
    } else if (opcode == "and") {
        return "000000";
    } else if (opcode == "jr") {
        return "000000";
    } else if (opcode == "addi") {
        return "001000";
    } else if (opcode == "addiu") {
        return "001001";
    } else if (opcode == "andi") {
        return "001100";
    } else if (opcode == "beq") {
        return "000100";
    } else if (opcode == "bne") {
        return "000101";
    } else if (opcode == "lbu") {
        return "100100";
    } else if (opcode == "lhu") {
        return "100101";
    } else if (opcode == "lui") {
        return "001111";
    } else if (opcode == "lw") {
        return "100011";
    } else if (opcode == "sw") {
        return "101011";
    } else if (opcode == "j") {
        return "000010";
    } else if (opcode == "jal") {
        return "000011";
    }
    return "";
}
// Hàm chuyển đổi thanh ghi thành mã nhị phân
string getRegisterBinary(string reg) {
    if (reg == "$zero") {
        return "00000";
    } else if (reg == "$at") {
        return "00001";
    } else if (reg == "$v0") {
        return "00010";
    } else if (reg == "$v1") {
        return "00011";
    } else if (reg == "$a0") {
        return "00100";
    } else if (reg == "$a1") {
        return "00101";
    } else if (reg == "$a2") {
        return "00110";
    } else if (reg == "$a3") {
        return "00111";
    } else if (reg == "$t0") {
        return "01000";
    } else if (reg == "$t1") {
        return "01001";
    } else if (reg == "$t2") {
        return "01010";
    } else if (reg == "$t3") {
        return "01011";
    } else if (reg == "$t4") {
        return "01100";
    } else if (reg == "$t5") {
        return "01101";
    } else if (reg == "$t6") {
        return "01110";
    } else if (reg == "$t7") {
        return "01111";
    } else if (reg == "$s0") {
        return "10000";
    } else if (reg == "$s1") {
        return "10001";
    } else if (reg == "$s2") {
        return "10010";
    } else if (reg == "$s3") {
        return "10011";
    } else if (reg == "$s4") {
        return "10100";
    } else if (reg == "$s5") {
        return "10101";
    } else if (reg == "$s6") {
        return "10110";
    } else if (reg == "$s7") {
        return "10111";
    } else if (reg == "$t8") {
        return "11000";
    } else if (reg == "$t9") {
        return "11001";
    } else if (reg == "$k0") {
        return "11010";
    } else if (reg == "$k1") {
        return "11011";
    } else if (reg == "$gp") {
        return "11100";
    } else if (reg == "$sp") {
        return "11101";
    } else if (reg == "$fp") {
        return "11110";
    } else if (reg == "$ra") {
        return "11111";
    }
    return "";
}
// Hàm tạo mã nhị phân cho lệnh R-type
string generateRTypeBinary(string opcode, string rs, string rt, string rd, string shamt, string funct) {
    string opcodeBinary = getOpcodeBinary(opcode);
    string rsBinary = getRegisterBinary(rs);
    string rtBinary = getRegisterBinary(rt);
    string rdBinary = getRegisterBinary(rd);
    string shamtBinary = bitset<5>(stoi(shamt)).to_string();
    string functBinary = bitset<6>(stoi(funct)).to_string();

    return opcodeBinary + rsBinary + rtBinary + rdBinary + shamtBinary + functBinary;
}

// Hàm tạo mã nhị phân cho lệnh I-type
string generateITypeBinary(string opcode, string rs, string rt, string immediate) {
    string opcodeBinary = getOpcodeBinary(opcode);
    string rsBinary = getRegisterBinary(rs);
    string rtBinary = getRegisterBinary(rt);
    string immediateBinary = bitset<16>(stoi(immediate)).to_string();

    return opcodeBinary + rsBinary + rtBinary + immediateBinary;
}

// Hàm tạo mã nhị phân cho lệnh J-type
string generateJTypeBinary(string opcode, string target) {
    string opcodeBinary = getOpcodeBinary(opcode);
    string targetBinary = bitset<26>(stoi(target)).to_string();

    return opcodeBinary + targetBinary;
}


// Hàm tạo mã nhị phân cho lệnh dựa trên định dạng
string generateBinary(string line) {
    istringstream iss(line);
    string opcode;
    iss >> opcode;

    if (opcode == "add" || opcode == "addu" || opcode == "and" || opcode == "jr") {
        string rd, rs, rt, shamt, funct;
        iss >> rd >> rs >> rt >> shamt >> funct;
        return generateRTypeBinary(opcode, rs, rt, rd, shamt, funct);
    } else if (opcode == "addi" || opcode == "addiu" || opcode == "andi" || opcode == "beq" || opcode == "bne" || opcode == "lbu" || opcode == "lhu" || opcode == "lui" || opcode == "lw" || opcode == "sw") {
        string rt, rs, immediate;
        iss >> rt >> rs >> immediate;
        return generateITypeBinary(opcode, rs, rt, immediate);
    } else if (opcode == "j" || opcode == "jal") {
        string target;
        iss >> target;
        return generateJTypeBinary(opcode, target);
    }

    return "";
}

// Hàm thực hiện chuyến 1
void firstPass(ifstream& input) {
    ofstream temp("temp.txt");
    string line;
    while (getline(input, line)) {
        line = deleteComment(line);
        if (!line.empty()) {
            temp << line << endl;
            calculateImmediate(line);
        }
    }
    // buildLabelTable(input);
    temp.close();
    input.clear();
    input.seekg(0, ios::beg); // Đặt con trỏ đọc tập tin về đầu
}

// Hàm thực hiện chuyến 2
void secondPass(ifstream& temp) {
    ofstream output("output.txt");
    string line;
    while (getline(temp, line)) {
        // line = deleteComment(line);
        line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end()); // Xóa các khoảng trắng
        if (!line.empty()) {
            string binary = generateBinary(line);
            if (!binary.empty()) {
                output << binary << endl;
            }
        }
    }
    output.close();
}

int main() {
    ifstream input("input.txt");
    if (!input) {
        cout << "Không thể mở tập tin đầu vào!" << endl;
        return 1;
    }

    firstPass(input);
    input.close();

    ifstream temp("temp.txt");
    if (!temp) {
        cout << "Không thể mở tập tin tạm!" << endl;
        return 1;
    }

    secondPass(temp);
    temp.close();

    cout << "thanh cong" << endl;

    return 0;
}