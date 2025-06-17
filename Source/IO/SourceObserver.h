#pragma once
#include <functional>


/// <summary>
/// ソースコードを変化を監視する
/// </summary>
class SourceObserver
{
public:
	SourceObserver();
	~SourceObserver();

	void Update();
};
