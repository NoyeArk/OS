#include "syscall.h"

std::string Syscall::getCurPath() {
	return filesystem.getCurPath();
}

// 这里使用command来代替中断类型号，应该宏定义几个中断类型
void Syscall::SysCall(COMMAND command, const std::string eax) {
	switch (command)
	{
	// File指令
	case FILE_FORMAT:
		break;
	case FILE_MAKDIR:
		break;
	case FILE_RMDIR:
		break;
	case FILE_LS:
		filesystem.ls();
		break;
	case FILE_CD: {
		std::string targetFile = eax.substr(eax.find(" ") + 1);
		filesystem.cd(targetFile);
		break;
	}
	case FILE_CREATE:
		filesystem.create();
		break;
	case FILE_OPEN:
		break;
	case FILE_CLOSE:
		break;
	case FILE_WRITE:
		break;
	case FILE_READ:
		break;
	case FILE_RM:
		break;
	default:
		break;
	}
}
