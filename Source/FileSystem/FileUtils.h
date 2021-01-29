#pragma once
#include "StdCommon.h"
#include <fstream>

class FileUtils
{
public:
	static bool SyncLoadFile(FString path, char** ppBuffer, size_t& size);
	static bool SyncSaveFile(FString path, char* buffer, size_t size);
};

