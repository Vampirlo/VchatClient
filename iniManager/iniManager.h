#pragma once

#include <Windows.h>
#include <iostream>
#include <filesystem>
#include <string>
#include <SFML/Network.hpp>

std::string iniRead(std::filesystem::path directory, const char* section, const char* IniKey);
int iniWrite(std::filesystem::path directory, const char* section, const char* IniKey, std::string str);
int checkingTheExistenceOfValues(std::filesystem::path directory, const char* section, const char* IniKey);