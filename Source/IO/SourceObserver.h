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
	using UpdateSourceCallback = std::function<void(const std::vector<std::string>&)>;
	using CreateSourceCallback = std::function<void(std::vector<std::string>&)>;

	SourceObserver(const std::string& _fileName);
	~SourceObserver();

	/// <summary>
	/// 更新、チェックする
	/// </summary>
	void Update();
	/// <summary>
	/// ソースが変更されたときのコールバック処理
	/// </summary>
	/// <param name="_callback">void(const std::vector<std::string>&)</param>
	SourceObserver& OnUpdateSource(const UpdateSourceCallback& _callback) { onUpdateSource_ = _callback; return *this; }
	/// <summary>
	/// ソースを新規で作るときのコールバック関数
	/// </summary>
	/// <param name="_callback">void(std::vector<std::string>&)</param>
	SourceObserver& OnCreateSource(const CreateSourceCallback& _callback) { onCreateSource_ = _callback; return *this; }

private:
	std::string fileName_;
	std::vector<std::string> prevSourceLines_;
	UpdateSourceCallback onUpdateSource_;
	CreateSourceCallback onCreateSource_;
};
