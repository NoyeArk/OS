#pragma once

#include <string>

#include "disk.h"
#include "process_commucation.h"

enum Command {
	null, Read, WriteFileBlock, WriteIdxBlock
};


struct ICB {
	Command op;
	std::vector<short> rs;
	std::string dataToWrite;  // 要写入的数据
	// 更新索引块
	std::string writeAddr;    // 要写入的地址
	std::string blockIdToWrite;
	// 读出的数据
	std::vector<std::string> datasRead;

	ICB();
};


class MessageMapLayer
{
public:
	Disk disk;
	ICB icb;

	bool ContainsLetter(const std::string& str);
	std::vector<short> GetIdxBlocksId(std::vector<std::string> messages);
	std::string GetDataToWrite(std::vector<std::string> messages);

public:
	void UnpackMessage(const std::vector<std::string>& msgs);
	void ExecuteInst();
	std::string PackMessage();
};