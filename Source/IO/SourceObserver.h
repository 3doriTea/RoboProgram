#pragma once
#include <functional>
#include <vector>
#include <string>


/// <summary>
/// �\�[�X�R�[�h��ω����Ď�����
/// </summary>
class SourceObserver
{
public:
	SourceObserver(const std::string& _fileName);
	~SourceObserver();

	/// <summary>
	/// �X�V�A�`�F�b�N����
	/// </summary>
	void Update();
	/// <summary>
	/// �\�[�X���ύX���ꂽ�Ƃ��̃R�[���o�b�N����
	/// </summary>
	/// <param name="_callback">void()</param>
	void OnUpdateSource(const std::function<void()>& _callback) { onUpdateSource_ = _callback; }

private:
	std::string fileName_;
	std::vector<std::string> prevSourceLines_;
	std::function<void()> onUpdateSource_;
};
