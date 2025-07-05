#pragma once
#include <string>

class FileSaver
{
public:
	FileSaver(const std::string& fileName);
	~FileSaver();

	bool TryLoad();

	virtual void OnLoad();
};
