#pragma once

#include <Windows.h>
#include <string>

namespace choda {

	class FileSystem 
	{
	public:
		static std::string GetWorkingDir();
		static std::string GetAbsolutePath(const char* relativePath);
	private:
		static const unsigned int BUF_SIZE;
	};
}

