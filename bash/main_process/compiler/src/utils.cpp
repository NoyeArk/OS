#include "../include/utils.h"

// 检查当前字符串token中是否含有如下的特殊字符
bool checkSpecialChar(const std::string& token) {
    std::vector<std::string> specialChar = { "\n", "\t", "\r","\0", "" };

    for (const std::string& character : specialChar) {
        if (character == token) { // 如果含有其中的字符，则返回true
            return true;
        }
    }
    return false;
}


// 判断该字符串是否全为数字
int isNumber(const std::string& str) {
    bool isLoop = false;
    for (int jj = 0; jj < str.size(); jj++) {
        isLoop = true;
        bool haveNum = false;
        for (int ii = 0; ii < strlen(numberToFind); ii++) {
            if (str[jj] == numberToFind[ii]) {
                haveNum = true;
                break;
            }
        }
        if (!haveNum)
            return 0;
    }
    if (isLoop) return 1;
    else return 2; // 特殊情况
}

// 检查当前开头是否为数字
bool checkBeginNum(std::string str) {
    bool flag = false;
    for (int ii = 0; ii < strlen(numberToFind); ii++) {
        if (str[0] == numberToFind[ii]) { //开头是数字，则不需要拆分
            flag = true;
            break;
        }
    }
    if (flag) return true;
    else return true;
}


// 去除当前字符串input中的空格
std::string removeBlank(const std::string& input) {
    std::string result;
    for (char c : input) {
        if (!std::isspace(c)) {
            result += c;
        }
    }
    return result;
}


// 输出当前token对应的种别码
std::string transName(std::string& token, bool strcon) {
    if (strcon) return "STRCON";
    token = removeBlank(token);
    if (isNumber(token) == 1) return "INTCON";
    else if (isNumber(token) == 2) return "ERROR";
    if (token == "FormatString") return "STRCON";
    if (token == "main") return "MAINTK";
    if (token == "const") return "CONSTTK";
    if (token == "int") return "INTTK";
    if (token == "break") return "BREAKTK";
    if (token == "continue") return "CONTINUETK";
    if (token == "if") return "IFTK";
    if (token == "else") return "ELSETK";
    if (token == "!") return "NOT";
    if (token == "&&") return "AND";
    if (token == "||") return "OR";
    if (token == "while") return "WHILETK";
    if (token == "getint") return "GETINTTK";
    if (token == "printf") return "PRINTFTK";
    if (token == "return") return "RETURNTK";
    if (token == "+") return "PLUS";
    if (token == "-") return "MINU";
    if (token == "void") return "VOIDTK";
    if (token == "*") return "MULT";
    if (token == "/") return "DIV";
    if (token == "%") return "MOD";
    if (token == "<") return "LSS";
    if (token == "<=") return "LEQ";
    if (token == ">") return "GRE";
    if (token == ">=") return "GEQ";
    if (token == "==") return "EQL";
    if (token == "!=") return "NEQ";
    if (token == "=") return "ASSIGN";
    if (token == ";") return "SEMICN";
    if (token == ",") return "COMMA";
    if (token == "(") return "LPARENT";
    if (token == ")") return "RPARENT";
    if (token == "[") return "LBRACK";
    if (token == "]") return "RBRACK";
    if (token == "{") return "LBRACE";
    if (token == "}") return "RBRACE";
    return "IDENFR";
}

void printResult() {
    std::string file_path = "error.txt";
    std::ifstream input_file(file_path);

    if (!input_file.is_open()) {
        std::cerr << "Unable to open file: " << file_path << std::endl;
        exit(1);
    }
    std::string line;
    while (std::getline(input_file, line)) {
        std::cout << line << std::endl;
    }

    input_file.close();
}
