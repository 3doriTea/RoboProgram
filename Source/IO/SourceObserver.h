#pragma once
#include <functional>
#include <vector>
#include <string>


/// <summary>
/// ソースコードを変化を監視する
/// </summary>
class SourceObserver
{
public:
	SourceObserver(const std::string& _fileName);
	~SourceObserver();

	/// <summary>
	/// 更新、チェックする
	/// </summary>
	void Update();
	/// <summary>
	/// ソースが変更されたときのコールバック処理
	/// </summary>
	/// <param name="_callback">void()</param>
	void OnUpdateSource(const std::function<void()>& _callback) { onUpdateSource_ = _callback; }

private:
	std::string fileName_;
	std::vector<std::string> prevSourceLines_;
	std::function<void()> onUpdateSource_;
};
