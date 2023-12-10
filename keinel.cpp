#include "kernel.h"

std::string Kernel::getCurPath() {
	return filesystem.getCurPath();
}

// ����ʹ��command�������ж����ͺţ�Ӧ�ú궨�弸���ж�����
void Kernel::SysCall(COMMAND command, const std::string eax) {
	switch (command)
	{
	// Fileָ��
	case FILE_FORMAT:
		filesystem.format();
		break;
	case FILE_MKDIR:
		filesystem.mkdir();
		break;
	case FILE_RMDIR:
		filesystem.rmdir();
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
	case FILE_BACK:
		filesystem.back();
		break;
	case FILE_OPEN:
		filesystem.open();
		break;
	case FILE_CLOSE:
		filesystem.close();
		break;
	case FILE_WRITE:
		filesystem.write();
		break;
	case FILE_READ:
		filesystem.read();
		break;
	case FILE_RM:
		filesystem.rm();
		break;
	default:
		break;
	}
}
