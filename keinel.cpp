#include "kernel.h"

std::string Kernel::getCurPath() {
	return directory.getCurPath();
}

// ����ʹ��command�������ж����ͺţ�Ӧ�ú궨�弸���ж�����
void Kernel::SysCall(COMMAND command, const std::string eax) {
	switch (command)
	{
	// Fileָ��
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
