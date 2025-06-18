#include "SourceObserver.h"
#include <fstream>
#include <cassert>
#include "../Utility/VectorUtil.h"
#include <DxLib.h>


SourceObserver::SourceObserver(const std::string& _fileName) :
	fileName_{ _fileName }
{
}

SourceObserver::~SourceObserver()
{
}

void SourceObserver::Update()
{
	printfDx("Checking\n");

	using VectorUtil::CopyVector;

	std::ifstream ifs(fileName_);
	
	assert(ifs.is_open()
		&& "ファイルを開くのに失敗した @SourceObserver::Update");

	std::vector<std::string> lines{};
	std::string line;
	while (std::getline(ifs, line))
	{
		lines.push_back(line);
	}
	ifs.close();

	if (lines.size() != prevSourceLines_.size())
	{
		CopyVector(prevSourceLines_, lines);
		onUpdateSource_();
		return;
	}

	for (int i = 0; i < lines.size(); i++)
	{
		if (lines[i] != prevSourceLines_[i])
		{
			CopyVector(prevSourceLines_, lines);
			onUpdateSource_();
			return;
		}
	}
}
