#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <tchar.h>
#include <windows.h>

#include "message_map_layer.h"

MessageMapLayer msgAgent;
ProcessCommunication communicateAgent;


void test() {
    Disk disk;
    std::string data = "1";
    disk.WriteSingleBlockToDisk(0, 2, data.c_str());
    data = "2";
    disk.WriteSingleBlockToDisk(4, 2, data.c_str());
}

int main() {

    //test();
    
    while (true) {
        std::cout << "磁盘进程::等待执行进程通信消息..." << std::endl;

        // 接受到的信息
        std::vector<std::string> messages = communicateAgent.ShakeReceive();

        // 处理命令
        msgAgent.UnpackMessage(messages);
        msgAgent.ExecuteInst();
        std::string msgToSend = msgAgent.PackMessage();

        // 发送执行结果
        communicateAgent.Send(msgToSend.c_str());
    }
    return 0;
}


/**
 * Read: Read blocksId
 *  return： Ready data
 * Write: Write blockId data
 *  return:  Ready
 * 
 * main→外存：
 *    映射名：main_to_disk_commucation.txt
 *    事件名：main_to_disk_event
 * 外存→main：
 *    映射名：disk_to_main_commucation.txt
 *    事件名：disk_to_main_event
 * main→内存：
 *    映射名：main_to_disk_commucation.txt
 *    事件名：main_to_momery_event
 * 内存→main：
 *    映射名：disk_to_main_commucation.txt
 *    事件名：disk_to_main_event
 */