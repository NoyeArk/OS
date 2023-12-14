#include "kernel.h"

PCB* Kernel::Fork() {
	// 使用 C++11 随机数引擎
	std::random_device rd;   // 获取随机设备
	std::mt19937 mt(rd());   // 使用 Mersenne Twister 引擎
	std::uniform_int_distribution<int> dist(1, 10000);  // 定义范围

	PCB p;
	p.pid = dist(mt);


	return nullptr;
}

std::string Kernel::getCurPath() {
	return directory.getCurPath();
}

// 这里使用command来代替中断类型号，应该宏定义几个中断类型
void Kernel::SysCall(COMMAND command, const std::string eax) {
	switch (command)
	{
	// File指令
	case FILE_FORMAT:
		directory.format();
		break;
	case FILE_MKDIR:
		directory.mkdir();
		break;
	case FILE_RMDIR:
		directory.rmdir();
		break;
	case FILE_LS:
		directory.ls();
		break;
	case FILE_CD: {
		std::string targetFile = eax.substr(eax.find(" ") + 1);
		directory.cd(targetFile);
		break;
	}
	case FILE_CREATE:
		directory.create();
		break;
	case FILE_BACK:
		directory.back();
		break;
	case FILE_OPEN:
		directory.open();
		break;
	case FILE_CLOSE:
		directory.close();
		break;
	case FILE_WRITE:
		directory.write();
		break;
	case FILE_READ:
		directory.read();
		break;
	case FILE_RM:
		directory.rm();
		break;
	default:
		break;
	}
}
