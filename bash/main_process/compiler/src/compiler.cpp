#include "../include/compiler.h"

// ����ɾ����ǰ�������е���ע��/**/
std::string Compiler::removeDivXing(const std::string& input) {
    std::string result;
    bool insideComment = false; // ����Ƿ���ע�Ϳ���

    for (size_t i = 0; i < input.length(); ++i) {
        if (!insideComment) {
            // �����ǰ�ַ��� /��������һ���ַ��� *�������ע�Ϳ�
            if (input[i] == '/' && i + 1 < input.length() && input[i + 1] == '*') {
                insideComment = true;
                ++i; // ������һ���ַ� (*)
            }
            else {
                result += input[i]; // ���ַ���ӵ������
            }
        }
        else {
            // �����ǰ�ַ��� *��������һ���ַ��� /�����˳�ע�Ϳ�
            if (input[i] == '*' && i + 1 < input.length() && input[i + 1] == '/') {
                insideComment = false;
                ++i; // ������һ���ַ� (/)
            }
        }
    }
    return result;
}


// ����ȥ����ǰ�����е�ע�Ͳ���
int Compiler::removeComments(std::string& input) {
    bool commentTypes[3] = { false, false, false };
    size_t begin = input.find("/*");
    size_t end = input.find("*/");

    if (begin != std::string::npos && end != std::string::npos) {
        input = removeDivXing(input); // ȥ����ע���м�Ĳ���
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
    std::ifstream inputFile("F:/code/os/compiler/testfile.txt");

    std::string lineInput;       // ��ȡ��ǰ�ļ���һ��
    std::vector<std::string> batchTokenVector;
    bool haveLeDivXing = false;  // �Ƿ��ȡ������ע�ͷ�
    int lineNumber = 0;
    std::string preFormatLineInput, FormatLineInput, afterFormatLineInput;

    while (std::getline(inputFile, lineInput)) {
        ++lineNumber;

        if (lineNumber == 18)
            printf("");

        size_t commentType = removeComments(lineInput); // ȥ��ע��

        // ��Ҫע�⵱ǰ��/*�����
        switch (commentType) {
        case DIVDIV:
            if (lineInput == "") continue; // ȥ��ע��Ϊ�գ�����
            break;
        case LEDIVXING: // ��ǰ�к�����ע��
            haveLeDivXing = true;
            if (lineInput == "") continue; // ȥ����ע��Ϊ�գ�����
            break;
        case RIDIVXING:
            haveLeDivXing = false;
            if (lineInput == "") continue; // ȥ����ע��Ϊ�գ�����
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
        case NONEDIV: // ��ǰ��û��ע��
            if (haveLeDivXing == true || lineInput == "") continue;
            break;
        }
        bool isFormat = splitQuotedText(lineInput, preFormatLineInput, FormatLineInput, afterFormatLineInput);

        if (isFormat) { // format
            batchTokenVector = splitEveryToken(preFormatLineInput); // ���format֮ǰ������
            tokenVector.insert(tokenVector.end(), batchTokenVector.begin(), batchTokenVector.end());

            tokenVector.push_back(FormatLineInput);

            batchTokenVector = splitEveryToken(afterFormatLineInput); // ���format֮�������
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
    lexing(tokenVector, lineNumberVector);     // lex program
    parsering(tokenVector, lineNumberVector);  // parser program
    printResult();
} 