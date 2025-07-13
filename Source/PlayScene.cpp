#include <Windows.h>
#include <cassert>
#include "PlayScene.h"
#include "IO/Input.h"
#include "Utility/Timer.h"
#include "Utility/StringUtility.h"
#include "IO/SourceObserver.h"
#include "Screen.h"

#include "Background.h"
#include "Stage.h"
#include "CodeBox.h"
#include "Player.h"
#include "ViewerBox.h"

#include "Compiler/ProtoAnalyzer.h"
#include "Compiler/LexicalAnalyzer.h"
#include "Compiler/SyntaxAnalyzer.h"
#include "Compiler/SemanticAnalyzer.h"

#include <sstream>
#include <iomanip>
#include "ByteCodeDefine.h"
#include "Compiler/Assembler.h"
#include "Fader.h"

#include "DocButton.h"
#include "ReloadButton.h"
#include "WriteButton.h"

#include "CursorText.h"

#include "Compiler/Compiler.h"

namespace
{
	static const char SRC_FILE_NAME[]{ "SourceCode.txt" };
	static const char DOC_FILE_NAME[]{ "Document.txt" };
	static const char ERROR_LOG_FILE_NAME[]{ "ErrorLog.txt" };
	static const char PLAY_IMAGE_PATH[]{ "Data/Image/Background.png" };
	static const char ERROR_FILE[]{ "ErrorLog.txt" };
}

PlayScene::PlayScene() :
	pSourceObserver_{ new SourceObserver{ SRC_FILE_NAME } }
{
	Timer::Instance().Clear();  // �ŏ��Ƀ^�C�}�[���N���A����

	// �G���[���O�t�@�C��������Ȃ瑦�R���p�C��
	if (FileSaver::ExistFile(ERROR_LOG_FILE_NAME))
	{
		ByteCodes byteCodes{};

		Compiler{ FileSaver::QuickReadTextLines(SRC_FILE_NAME), byteCodes }
			.OnError([&, this](
				const std::string& _message,
				const SOURCE_POS _position,
				const Compiler::ErrorExitCode _code)
				{
					FileSaver::QuickWriteText(ERROR_FILE, _message);
					//exit(_code);
					ExitProcess(_code);
					//SceneManager::Exit();
				})
			.Start();
		// �R���p�C�����������Ȃ炱����ʉ߂���

		DeleteFile(ERROR_LOG_FILE_NAME);  // �G���[���O�t�@�C������
	}


	// �n�܂����Ƃ��̃t�F�[�h�C��
	new Fader{ PLAY_IMAGE_PATH, 1.0f, true };

	Stage* pStage{ new Stage{} };
	// NOTE: Stage���Q�Ƃ���I�u�W�F�N�g�͂���ȍ~�ɏ����K�v������

	new CursorText{};

	new DocButton{};
	new ReloadButton{};
	new WriteButton{};

	new Background{ PLAY_IMAGE_PATH };
	
#pragma region �r���[�{�b�N�X
	ViewerBox* pCodeViewer{ new ViewerBox{} };
	pCodeViewer
		->SetFrameWidth(5)
		.SetIsScrollable(true)
		.SetShowLineCount(10)
		.SetTextBoxMargin(3)
		.SetIsShowLineCountBar(true);

	ViewerBox* pByteViewer{ new ViewerBox{} };
	pByteViewer
		->SetFrameWidth(5)
		.SetIsScrollable(true)
		.SetShowLineCount(10)
		.SetTextBoxMargin(3)
		.SetIsShowLineCountBar(true);
	
	ViewerBox* pRegisterViewer{ new ViewerBox{} };
	pRegisterViewer
		->SetFrameWidth(5)
		.SetIsScrollable(true)
		.SetUseGhost(true)
		.SetTextBoxMargin(3)
		.SetIsShowLineCountBar(false)
		.SetPosition({ 1070, 430 }, ViewerBox::Pivot::TopLeft);

	ViewerBox* pStackViewer{ new ViewerBox{} };
	pStackViewer
		->SetFrameWidth(5)
		.SetIsScrollable(true)
		.SetUseGhost(true)
		.SetShowLineCount(7)
		.SetTextBoxMargin(3)
		.SetIsShowLineCountBar(false)
		.SetPosition({ 1129, 260 }, ViewerBox::Pivot::TopLeft);

	ViewerBox* pCallStackViewer{ new ViewerBox{} };
	pCallStackViewer
		->SetFrameWidth(5)
		.SetIsScrollable(true)
		.SetUseGhost(true)
		.SetShowLineCount(7)
		.SetTextBoxMargin(3)
		.SetIsShowLineCountBar(false)
		.SetPosition({ 999, 260 }, ViewerBox::Pivot::TopLeft);

	ViewerBox* pMemoryViewer{ new ViewerBox{} };
	pMemoryViewer
		->SetFrameWidth(5)
		.SetIsScrollable(false)
		.SetUseGhost(true)
		.SetTextBoxMargin(3)
		.SetIsShowLineCountBar(false)
		.SetPosition({ 1020, 502 }, ViewerBox::Pivot::TopLeft);
#pragma endregion

	pPlayer_ = pStage->GetPlayer();
	assert(pPlayer_ != nullptr
		&& "�X�e�[�W����v���C���[�̎擾�Ɏ��s @PlayScene::PlayScene");

	pPlayer_->SetViewerBoxes(
		pCodeViewer,
		pByteViewer,
		pMemoryViewer,
		pCallStackViewer,
		pStackViewer,
		pRegisterViewer);

	using StringUtility::ToString;

	pSourceObserver_
		->OnCreateSource([&, this](
			std::vector<std::string>& _newSource)
			{
				_newSource =
				{
					u8"// ���r�[�g�Ă΂���I",
					u8"void Update()",
					u8"{",
					u8"  Run();  // ����������Ƒ����",
					u8"}",
				};
			})
		.OnUpdateSource([&, this, pCodeViewer](
			const std::vector<std::string>& _newSource)
			{
				srcCodeCache_ = "";
				for (const auto& line : _newSource)
				{
					srcCodeCache_ += line + "\n";
				}

				//�@�R���p�C������

				bool isError{ false };
				std::string errorMessage{};
				SOURCE_POS errorPosition{ -1, -1 };
				pCodeViewer->SetTextLines(_newSource);

				ByteCodes byteCodes{};

				Compiler{ _newSource, byteCodes }
					.OnError([&, this](
						const std::string& _message,
						const SOURCE_POS _position,
						const Compiler::ErrorExitCode _code)
						{
							switch (_code)
							{
							case Compiler::ERR_LEXICAL:
								errorMessage = std::string{ "�g�[�N���G���[ " };
								if (_position.line >= 0)
								{
									errorMessage += std::to_string(_position.line) + "�s ";
								}
								if (_position.column >= 0)
								{
									errorMessage += std::to_string(_position.column) + "������ ";
								}
								errorMessage += _message;
								errorPosition = _position;
								isError = true;
								break;
							case Compiler::ERR_SYNTAX:
								errorMessage = std::string{ "�\���G���[ " };
								if (_position.line >= 0)
								{
									errorMessage += std::to_string(_position.line) + "�s ";
								}
								if (_position.column >= 0)
								{
									errorMessage += std::to_string(_position.column) + "������ ";
								}
								errorMessage += _message;
								errorPosition = _position;
								isError = true;
								break;
							case Compiler::ERR_SEMANTIC:
								errorMessage = std::string{ "���@�G���[ " };
								if (_position.line >= 0)
								{
									errorMessage += std::to_string(_position.line) + "�s ";
								}
								if (_position.column >= 0)
								{
									errorMessage += std::to_string(_position.column) + "������ ";
								}
								errorMessage += _message;
								errorPosition = _position;
								isError = true;
								break;
							default:
								break;
							}
						})
					.Start();

				if (isError)  // �G���[�Ȃ�~�߂�
				{
					FileSaver::QuickWriteText(ERROR_FILE, errorMessage);
					pPlayer_->SetError(errorMessage, errorPosition);
					return;
				}

				Assembler::ToString(byteCodes, assembleText_);

				OutputDebugString(assembleText_.c_str());

				pPlayer_->SetByteCode(byteCodes);

				if (FileSaver::ExistFile(ERROR_LOG_FILE_NAME))
				{
					DeleteFile(ERROR_LOG_FILE_NAME);
				}
			});

	Timer::AddInterval(1.0f, [&, this]()
	{
		pSourceObserver_->Update();
	}, true);
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	Input::Instance().Update();
	Timer::Instance().Update();

	if (CheckHitKey(KEY_INPUT_T))
	{
		SceneManager::ChangeScene("TITLE");
	}

	if (Input::IsKeyDown(KEY_INPUT_F))
	{
		OpenSrcFile();
	}
}

void PlayScene::Draw()
{
	/*DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));*/
}

void PlayScene::OpenSrcFile()
{
	//system(SRC_FILE_NAME);
	ShellExecute(NULL, "open", SRC_FILE_NAME, "", "", SW_SHOW);
}

void PlayScene::OpenDocument()
{
	ShellExecute(NULL, "open", DOC_FILE_NAME, "", "", SW_SHOW);
}

const char* PlayScene::GetErrorFileName()
{
	return ERROR_FILE;
}

std::string PlayScene::srcCodeCache_{};
std::string PlayScene::assembleText_{};
bool PlayScene::isFinalMode_{ false };
