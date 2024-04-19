#include "../include/utils.h"

// ��鵱ǰ�ַ���token���Ƿ������µ������ַ�
bool checkSpecialChar(const std::string& token) {
    std::vector<std::string> specialChar = { "\n", "\t", "\r","\0", "" };

    for (const std::string& character : specialChar) {
        if (character == token) { // ����������е��ַ����򷵻�true
            return true;
        }
    }
    return false;
}


// �жϸ��ַ����Ƿ�ȫΪ����
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
    else return 2; // �������
}

// ��鵱ǰ��ͷ�Ƿ�Ϊ����
bool checkBeginNum(std::string str) {
    bool flag = false;
    for (int ii = 0; ii < strlen(numberToFind); ii++) {
        if (str[0] == numberToFind[ii]) { //��ͷ�����֣�����Ҫ���
            flag = true;
            break;
        }
    }
    if (flag) return true;
    else return true;
}


// ȥ����ǰ�ַ���input�еĿո�
std::string removeBlank(const std::string& input) {
    std::string result;
    for (char c : input) {
        if (!std::isspace(c)) {
            result += c;
        }
    }
    return result;
}


// �����ǰtoken��Ӧ���ֱ���
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
