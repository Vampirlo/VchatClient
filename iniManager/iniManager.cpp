#include "iniManager.h"



std::string iniRead(std::filesystem::path directory, const char* section, const char* IniKey)
{
    std::filesystem::path iniFilePath = directory / "config.ini"; // метод concat
    std::string iniPathString = iniFilePath.string(); // сохраняем строку в std::string
    const char* iniPathCString = iniPathString.c_str(); // используем c_str() только после сохранения строки в std::string
    //`c_str()` используется для получения указателя на константный массив символов (`const char*`),
    //который содержит ту же строку в формате C-style (завершающийся нулевым символом).
    char buffer[256];
    GetPrivateProfileStringA(section, IniKey, "", buffer, sizeof(buffer), iniPathCString);

    return std::string(buffer);
}

int iniWrite(std::filesystem::path directory, const char* section, const char* IniKey, std::string str)
{
    std::filesystem::path iniFilePath = directory / "config.ini";
    std::string iniPathString = iniFilePath.string();
    const char* iniPathCString = iniPathString.c_str();

    WritePrivateProfileStringA(section, IniKey, str.c_str(), iniPathCString);

    return 0;
}
// принимаю str сравниваю, пустой ли он, если да, то в зависимости от IniKey вписываю в Ini файл
// если значения не заданы - считывать их - не правильно. Проверяю, если значение не задано вообще - задаю по дефолту
int checkingTheExistenceOfValues(std::filesystem::path directory, const char* section, const char* IniKey)
{
    std::filesystem::path iniFilePath = directory / "config.ini";

    std::string var = iniRead(directory, section, IniKey);

    if (var.empty())
    {
        if (IniKey == "debug")// проверка введено ли значение debug, если нет - автоматически задать false.
        {
            iniWrite(directory, section, IniKey, "false");
            std::cout << "value 'debug' is set automatically" << std::endl;
        }
        else if (IniKey == "Server_ip") // если не задано - написать об этом и автоматичеки задать local ip
        {
            sf::IpAddress ip = sf::IpAddress::getLocalAddress();
            std::string srtIp = ip.toString();
            iniWrite(directory, section, IniKey, srtIp);
            std::cout << "Server Ip - no value specified. Will be set local address" << std::endl;
        }
        else if (IniKey == "port") // если не написано - написать об этом, автоматически задать 53000
        {
            iniWrite(directory, section, IniKey, "53000");
            std::cout << "Port - no value specified. Will be set default" << std::endl;
        }
        else
        {
            //error manager 
            std::cout << "else checkingTheExistenceOfValues" << std::endl;
        }
    }

    return 0;
}