#include "message_map_layer.h"

ICB::ICB() {
    this->op = null;
    this->rs = {};
    this->dataToWrite = "";
    this->writeAddr = "";
    this->blockIdToWrite = "";
}


std::vector<short> MessageMapLayer::GetIdxBlocksId(std::vector<std::string> messages) {
    std::vector<short> idxBlocksId;
    for (size_t ii = 1; ii < messages.size(); ii++) {
        try {
            short blockId = static_cast<short>(std::stoi(messages[ii]));
            idxBlocksId.push_back(blockId);
        }
        catch (const std::exception&) {
            break;
        }
    }
    return idxBlocksId;
}


bool MessageMapLayer::ContainsLetter(const std::string& str) {
    for (char ch : str) {
        if (std::isalpha(ch)) {
            return true;
        }
    }
    return false;
}


std::string MessageMapLayer::GetDataToWrite(std::vector<std::string> messages) {
    for (size_t ii = 1; ii < messages.size(); ii++) {
        if (ContainsLetter(messages[ii])) {
            return messages[ii];
        }
    }
    return "";
}


void MessageMapLayer::UnpackMessage(const std::vector<std::string>& msgs) {
    std::string command = msgs[0];

    if (command == "Read") {
        icb.op = Read;
        icb.rs = GetIdxBlocksId(msgs);
    }
    else if (command == "WriteFileBlock") {
        icb.op = WriteFileBlock;
        icb.rs = GetIdxBlocksId(msgs);
        icb.dataToWrite = GetDataToWrite(msgs);
    }
    else if (command == "WriteBlockId") {
        icb.op = WriteIdxBlock;
        icb.writeAddr = msgs[1];
        icb.dataToWrite = msgs[2];
    }
    else {
        std::cout << command << std::endl;
        std::cout << "ÐÅÏ¢½âÎö´íÎó£¡" << std::endl;
    }
}


void MessageMapLayer::ExecuteInst() {
    switch (icb.op)
    {
    case Read: {
        icb.datasRead = disk.ReadFile(icb.rs);
        break;
    }
    case WriteFileBlock: {
        disk.WriteFile(icb.rs, icb.dataToWrite);
        break;
    }
    case WriteIdxBlock: {
        disk.WriteSingleBlockToDisk(std::stoi(icb.writeAddr), IDX_SIZE, icb.dataToWrite.c_str());
        disk.FlushBuffer();
        break;
    }
    default:
        break;
    }
}


std::string MessageMapLayer::PackMessage() {
    std::string msgToSend = "Ready ";
    switch (icb.op)
    {
    case Read: {
        for (const std::string& str : icb.datasRead) {
            msgToSend += str;
        }
        break;
    }
    case WriteFileBlock: {
        break;
    }
    case WriteIdxBlock: {
        break;
    }
    default:
        break;
    }
    return msgToSend;
}