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
	using UpdateSourceCallback = std::function<void(const std::vector<std::string>&)>;
	using CreateSourceCallback = std::function<void(std::vector<std::string>&)>;

	SourceObserver(const std::string& _fileName);
	~SourceObserver();

	/// <summary>
	/// �X�V�A�`�F�b�N����
	/// </summary>
	void Update();
	/// <summary>
	/// �\�[�X���ύX���ꂽ�Ƃ��̃R�[���o�b�N����
	/// </summary>
	/// <param name="_callback">void(const std::vector<std::string>&)</param>
	SourceObserver& OnUpdateSource(const UpdateSourceCallback& _callback) { onUpdateSource_ = _callback; return *this; }
	/// <summary>
	/// �\�[�X��V�K�ō��Ƃ��̃R�[���o�b�N�֐�
	/// </summary>
	/// <param name="_callback">void(std::vector<std::string>&)</param>
	SourceObserver& OnCreateSource(const CreateSourceCallback& _callback) { onCreateSource_ = _callback; return *this; }

private:
	std::string fileName_;
	std::vector<std::string> prevSourceLines_;
	UpdateSourceCallback onUpdateSource_;
	CreateSourceCallback onCreateSource_;
};
