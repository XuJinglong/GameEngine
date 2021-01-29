#include "FileUtils.h"

using namespace std;

bool FileUtils::SyncLoadFile(FString path, char** ppBuffer, size_t& size)
{
	fstream fs(path, ios::binary | ios::in);
	if (!fs) 
	{
		size = 0;
		return false;
	}

	fs.seekg(0, ios::end);
	size = fs.tellg();
	fs.seekg(0, ios::beg);

	auto buffer = new char[size];
	fs.read(buffer, size);

	if (!fs) 
	{
		return false;
	}
	ppBuffer = &buffer;

	return true;
}

bool FileUtils::SyncSaveFile(FString path, char* buffer, size_t size)
{
	fstream fs(path, ios::binary | ios::out);
	if (!fs)
	{
		size = 0;
		return false;
	}

	fs.write(buffer, size);
	if (!fs) 
	{
		return false;
	}

	return true;
}
