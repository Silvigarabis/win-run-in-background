#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// 函数：转义字符串中的引号
void escape_quotes(const char *src, char *dest) {
    while (*src) {
        if (*src == '"') {
            // 转义双引号
            *dest++ = '\\';
            *dest++ = '"';
        } else {
            *dest++ = *src;
        }
        src++;
    }
    *dest = '\0';  // 确保字符串结束
}

// 函数：构建命令行字符串
void build_command_line(char *commandLine, char *argv[], int argc) {
    char escapedExePath[MAX_PATH];
    escape_quotes(argv[1], escapedExePath);
    
    // 拼接可执行文件路径
    strcpy(commandLine, "\"");
    strcat(commandLine, escapedExePath);
    strcat(commandLine, "\"");

    if (argc > 2)  // 拼接每个参数
    for (int i = 2; i < argc; i++) {
        strcat(commandLine, " ");
        char escapedArg[1024];
        escape_quotes(argv[i], escapedArg);
        strcat(commandLine, "\"");
        strcat(commandLine, escapedArg);
        strcat(commandLine, "\"");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <command> <args>\n", argv[0]);
        return 1;
    }

    FreeConsole();  // 释放当前进程的控制台窗口
    //MessageBox(NULL, "窗口已隐藏", "提示", MB_OK);

    // 注意这里，命令长度只能到1024
    char commandLine[1024] = {0};

    // 构建命令行
    build_command_line(commandLine, argv, argc);

    // 输出拼接后的命令行
    printf("Command: %s\n", commandLine);

    // 执行命令
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOWNORMAL;  // 隐藏窗口

    ZeroMemory(&pi, sizeof(pi));

    // 创建进程
    if (!CreateProcess(
            NULL,             // 可执行文件路径
            commandLine,      // 命令行字符串
            NULL,             // 进程安全属性
            NULL,             // 线程安全属性
            FALSE,            // 是否继承句柄
            CREATE_NEW_CONSOLE, // 创建新控制台
            NULL,             // 环境变量
            NULL,             // 当前目录
            &si,              // 启动信息
            &pi               // 进程信息
        )) {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return 1;
    }

    printf("Process started successfully.\n");

    // 等待进程结束
    //WaitForSingleObject(pi.hProcess, INFINITE);

    // 关闭进程和线程句柄
    //CloseHandle(pi.hProcess);
    //CloseHandle(pi.hThread);

    return 0;
}

