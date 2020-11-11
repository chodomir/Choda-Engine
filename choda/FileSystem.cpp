#include "FileSystem.h"

const unsigned int choda::FileSystem::BUF_SIZE = 1000;

std::string choda::FileSystem::GetWorkingDir()
{
    // dirty little thing

    TCHAR dir[BUF_SIZE];
    DWORD ret = GetCurrentDirectory(BUF_SIZE, dir);
    return std::string(&dir[0], &dir[ret]);
}

std::string choda::FileSystem::GetAbsolutePath(const char* relativeWindowsPath)
{
    return FileSystem::GetWorkingDir() + "\\" + std::string(relativeWindowsPath);
}
