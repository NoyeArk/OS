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
	std::string dataToWrite;  // Ҫд�������
	// ����������
	std::string writeAddr;    // Ҫд��ĵ�ַ
	std::string blockIdToWrite;
	// ����������
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