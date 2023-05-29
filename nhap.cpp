#include <iostream>
#include <string>
#include <map>
#include <sstream>

// Hàm tính toán địa chỉ cho các label và lưu vào bảng label
void calculateLabelAddresses(std::string& program, std::map<std::string, int>& labelTable) {
    std::istringstream iss(program);
    std::string line;
    int address = 0;

    while (std::getline(iss, line)) {
        // Kiểm tra xem dòng hiện tại có phải là label hay không
        if (!line.empty() && line.back() == ':') {
            // Lấy tên label bằng cách loại bỏ dấu hai chấm
            std::string label = line.substr(0, line.length() - 1);

            // Lưu địa chỉ của label vào bảng label
            labelTable[label] = address;
        } else {
            // Đếm số byte của lệnh hoặc dữ liệu
            address += 4;
        }
    }
}

// Hàm loại bỏ các label khỏi chương trình
std::string removeLabels(const std::string& program) {
    std::istringstream iss(program);
    std::string line;
    std::string output;

    while (std::getline(iss, line)) {
        // Kiểm tra xem dòng hiện tại có phải là label hay không
        if (!line.empty() && line.back() == ':') {
            // Bỏ qua dòng label
            continue;
        }

        // Thêm dòng vào chương trình đầu ra
        output += line + "\n";
    }

    return output;
}

int main() {
    // Đoạn chương trình MIPS đầu vào
    std::string program = R"(
        loop:
            add $t0, $t0, $t1
            sub $t1, $t1, 1
            bne $t1, $zero, loop
        )";

    // Tạo bảng label
    std::map<std::string, int> labelTable;

    // Tính toán địa chỉ cho các label và lưu vào bảng label
    calculateLabelAddresses(program, labelTable);

    // In bảng label
    std::cout << "Label Table:\n";
    for (const auto& entry : labelTable) {
        std::cout << entry.first << ": " << entry.second << "\n";
    }
    std::cout << std::endl;

    // Loại bỏ các label khỏi chương trình
    std::string programWithoutLabels = removeLabels(program);

    // In chương trình đã loại bỏ label
    std::cout << "Program without labels:\n";
    std::cout << programWithoutLabels << std::endl;

    return 0;
}
