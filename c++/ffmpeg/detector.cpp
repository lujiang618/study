#include <iostream>
#include <stdio.h>
#include <string>
#include <filesystem>

std::string executeCommand(const char* cmd)
{
    char        buffer[128];
    std::string result = "";
    FILE*       pipe   = popen(cmd, "r"); // 执行命令
    if (!pipe)
        throw std::runtime_error("popen() failed!");
    try
    {
        while (fgets(buffer, sizeof buffer, pipe) != NULL)
        {
            result += buffer; // 读取输出
        }
    }
    catch (...)
    {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

bool checkCommandExists(const char* cmd)
{

    std::string file_path = "/usr/bin/nvidia-smi";

    return std::filesystem::exists(file_path);
}

int main()
{
    try
    {
        // 调用 vainfo 并获取基本信息
        std::string vainfoOutput = executeCommand("nvidia-smi --query-gpu=name --format=csv,noheader");
        std::cout << "VA-API Support Info:\n"
                  << vainfoOutput << std::endl;

        // 调用 nvidia-smi 获取显卡型号
        bool isNvidia = checkCommandExists("nvidia-smi --query-gpu=name --format=csv,noheader");
        std::cout << "is nvidia: " << isNvidia << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}