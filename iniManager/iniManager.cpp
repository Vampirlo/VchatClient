#include "iniManager.h"



std::string iniRead(std::filesystem::path directory, const char* section, const char* IniKey)
{
    std::filesystem::path iniFilePath = directory / "config.ini"; // ����� concat
    std::string iniPathString = iniFilePath.string(); // ��������� ������ � std::string
    const char* iniPathCString = iniPathString.c_str(); // ���������� c_str() ������ ����� ���������� ������ � std::string
    //`c_str()` ������������ ��� ��������� ��������� �� ����������� ������ �������� (`const char*`),
    //������� �������� �� �� ������ � ������� C-style (������������� ������� ��������).
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
// �������� str ���������, ������ �� ��, ���� ��, �� � ����������� �� IniKey �������� � Ini ����
// ���� �������� �� ������ - ��������� �� - �� ���������. ��������, ���� �������� �� ������ ������ - ����� �� �������
int checkingTheExistenceOfValues(std::filesystem::path directory, const char* section, const char* IniKey)
{
    std::filesystem::path iniFilePath = directory / "config.ini";

    std::string var = iniRead(directory, section, IniKey);

    if (var.empty())
    {
        if (IniKey == "debug")// �������� ������� �� �������� debug, ���� ��� - ������������� ������ false.
        {
            iniWrite(directory, section, IniKey, "false");
            std::cout << "value 'debug' is set automatically" << std::endl;
        }
        else if (IniKey == "Server_ip") // ���� �� ������ - �������� �� ���� � ������������ ������ local ip
        {
            sf::IpAddress ip = sf::IpAddress::getLocalAddress();
            std::string srtIp = ip.toString();
            iniWrite(directory, section, IniKey, srtIp);
            std::cout << "Server Ip - no value specified. Will be set local address" << std::endl;
        }
        else if (IniKey == "port") // ���� �� �������� - �������� �� ����, ������������� ������ 53000
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