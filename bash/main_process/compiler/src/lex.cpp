#include "../include/lex.h"


// �õ��ӵ�ǰ�ַ���������������±�
inline int maxNumIdx(const std::string& str) {
    int idx = -1;
    size_t numberToFindLen = strlen(numberToFind);
    for (char c : str) {
        for (int jj = 0; jj < numberToFindLen; jj++) {
            if (numberToFind[jj] == c) {
                idx++;
            }
        }
    }
    return idx;
}


// �Ե�ǰstr�е�ÿһ���ַ�������һ����Ƭ����
std::vector<std::string> stepSlice(const std::vector<std::string>& str) {
    std::vector<std::string> _str; // ��������ÿ���ַ�����Ƭ��Ľ��

    for (const std::string& token : str) {
        bool is_process = false; // ÿ��ѭ��ֻ����һ����Ƭ

        if (token.size() == 1 || token == "!=" || token == "==" || token == ">=" || token == "<=") {
            _str.push_back(token);
            continue;
        }

        // �жϿո�
        size_t foundIndex = token.find(blankTofind);
        if (foundIndex != std::string::npos) { // �����ո�
            if (foundIndex == 0) {
                _str.push_back(token.substr(1));
            }
            else {
                _str.push_back(token.substr(0, foundIndex));
                _str.push_back(token.substr(foundIndex + 1));
            }

            is_process = true;
        }

        // �ж���Ҫ���зָ���ַ�
        for (int ii = 0; ii < characterToFind.size(); ii++) {
            if (is_process) {
                break;
            }

            size_t foundIndex = token.find(characterToFind[ii]);
            if (foundIndex != std::string::npos) { // �������ַ�

                if (foundIndex == 0) {
                    _str.push_back(characterToFind[ii]);
                    _str.push_back(token.substr(1));
                }
                else {
                    std::string substring1 = token.substr(0, foundIndex);
                    std::string substring2 = token.substr(foundIndex);
                    _str.push_back(substring1);
                    _str.push_back(substring2);
                }

                is_process = true;
            }
        }

        // �ж�����
        for (int jj = 0; jj < token.size(); jj++) {
            for (int ii = 0; ii < strlen(numberToFind); ii++) {
                if (is_process) {
                    break;
                }

                if (token[jj] == numberToFind[ii]) { // �������ַ�
                    if (checkBeginNum(token))
                        continue;

                    std::string substring1;
                    std::string substring2;

                    int idx = maxNumIdx(token);
                    substring1 = token.substr(jj, idx + 1);
                    substring2 = token.substr(idx + 1);

                    _str.push_back(substring1);
                    _str.push_back(substring2);

                    is_process = true;
                    break;
                }
            }
        }
        if (!is_process) {
            _str.push_back(token);
        }
    }
    return _str;
}


// ��������input����Ϊ�����֣������������format����
bool splitQuotedText(const std::string& lineInput, std::string& preFormatLineInput, std::string& FormatLineInput, std::string& afterFormatLineInput) {
    size_t start = lineInput.find('"');
    size_t end = lineInput.find('"', start + 1);

    if (start != std::string::npos && end != std::string::npos) {
        preFormatLineInput = lineInput.substr(0, start);
        FormatLineInput = lineInput.substr(start, end - start + 1);
        afterFormatLineInput = lineInput.substr(end + 1);
        return true;
    }

    return false;
}


// �ú����ǶԵõ����ַ���������Ƭ������ֱ���з����
std::vector<std::string> splitEveryToken(const std::string& str) {
    std::vector<std::string> pre_str;
    pre_str.push_back(str);

    std::vector<std::string> new_str = stepSlice(pre_str); // �ٶ�_str�����з�
    while (new_str != pre_str) { // ֱ��ǰ�󻮷ֺ��token������ͬ
        pre_str = new_str;
        new_str = stepSlice(pre_str);
    }
    return new_str;
}