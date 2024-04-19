#include "../include/compiler.h"

// 用于删除当前输入行中的行注释/**/
std::string Compiler::removeDivXing(const std::string& input) {
    std::string result;
    bool insideComment = false; // 标记是否在注释块内

    for (size_t i = 0; i < input.length(); ++i) {
        if (!insideComment) {
            // 如果当前字符是 /，并且下一个字符是 *，则进入注释块
            if (input[i] == '/' && i + 1 < input.length() && input[i + 1] == '*') {
                insideComment = true;
                ++i; // 跳过下一个字符 (*)
            }
            else {
                result += input[i]; // 将字符添加到结果中
            }
        }
        else {
            // 如果当前字符是 *，并且下一个字符是 /，则退出注释块
            if (input[i] == '*' && i + 1 < input.length() && input[i + 1] == '/') {
                insideComment = false;
                ++i; // 跳过下一个字符 (/)
            }
        }
    }
    return result;
}


// 用于去除当前输入中的注释部分
int Compiler::removeComments(std::string& input) {
    bool commentTypes[3] = { false, false, false };
    size_t begin = input.find("/*");
    size_t end = input.find("*/");

    if (begin != std::string::npos && end != std::string::npos) {
        input = removeDivXing(input); // 去除行注释中间的部分
        commentTypes[1] = true;
        commentTypes[2] = true;
    }
    else if (begin != std::string::npos) {
        input = input.substr(0, begin);
        commentTypes[1] = true;
    }
    else if (end != std::string::npos) {
        input = input.substr(end + 2);
        commentTypes[2] = true;
    }

    size_t start = input.find("//");
    if (start != std::string::npos) {
        input = input.substr(0, start);
        commentTypes[0] = true;
    }

    if (commentTypes[0] and commentTypes[1] and commentTypes[2]) return LDIVDIV;
    if (commentTypes[0] and commentTypes[1]) return LDIVDIV;
    if (commentTypes[0] and commentTypes[2]) return RDIVDIV;
    if (commentTypes[1] and commentTypes[2]) return DIVXING;
    if (commentTypes[0]) return DIVDIV;
    if (commentTypes[1]) return LEDIVXING;
    if (commentTypes[2]) return RIDIVXING;
    return NONEDIV;
}


void Compiler::lexing(std::vector<std::string>& tokenVector, std::vector<int>& lineNumberVector) {
    std::ifstream inputFile("D:\\qtonline\\run\\compiler\\testfile.txt");

    std::string lineInput;       // 读取当前文件的一行
    std::vector<std::string> batchTokenVector;
    bool haveLeDivXing = false;  // 是否读取到左行注释符
    int lineNumber = 0;
    std::string preFormatLineInput, FormatLineInput, afterFormatLineInput;

    while (std::getline(inputFile, lineInput)) {
        ++lineNumber;

        if (lineNumber == 18)
            printf("");

        size_t commentType = removeComments(lineInput); // 去掉注释

        // 需要注意当前是/*的情况
        switch (commentType) {
        case DIVDIV:
            if (lineInput == "") continue; // 去掉注释为空，跳过
            break;
        case LEDIVXING: // 当前行含有左注释
            haveLeDivXing = true;
            if (lineInput == "") continue; // 去掉左注释为空，跳过
            break;
        case RIDIVXING:
            haveLeDivXing = false;
            if (lineInput == "") continue; // 去掉右注释为空，跳过
            break;
        case LDIVDIV:
            if (lineInput == "") continue;
            break;
        case RDIVDIV:
            haveLeDivXing = false;
            if (lineInput == "") continue;
            break;
        case DIVXING:
            if (lineInput == "") continue;
            break;
        case DIVDIVXING:
            if (lineInput == "") continue;
            break;
        case NONEDIV: // 当前行没有注释
            if (haveLeDivXing == true || lineInput == "") continue;
            break;
        }
        bool isFormat = splitQuotedText(lineInput, preFormatLineInput, FormatLineInput, afterFormatLineInput);

        if (isFormat) { // format
            batchTokenVector = splitEveryToken(preFormatLineInput); // 输出format之前的内容
            tokenVector.insert(tokenVector.end(), batchTokenVector.begin(), batchTokenVector.end());

            tokenVector.push_back(FormatLineInput);

            batchTokenVector = splitEveryToken(afterFormatLineInput); // 输出format之后的内容
            tokenVector.insert(tokenVector.end(), batchTokenVector.begin(), batchTokenVector.end());
        }
        else {
            batchTokenVector = splitEveryToken(lineInput);
            tokenVector.insert(tokenVector.end(), batchTokenVector.begin(), batchTokenVector.end());
        }
        size_t vectorLength = lineNumberVector.size();
        for (size_t i = 0; i < tokenVector.size() - vectorLength; i++) {
            lineNumberVector.push_back(lineNumber);
        }
    }
    inputFile.close();
}


void Compiler::GCC(const std::string& code) {
    // 打开文件
    std::ofstream outFile("D:\\qtonline\\run\\compiler\\testfile.txt", std::ios::trunc);
    std::cout << "编译的代码：" << code << std::endl;
    if (!outFile.is_open()) {
        std::cerr << "进程通信失败" << std::endl;
    }
    outFile << code << std::endl;
    outFile.close();

    lexing(tokenVector, lineNumberVector);     // lex program
    parsering(tokenVector, lineNumberVector);  // parser program
    printResult();
} 
