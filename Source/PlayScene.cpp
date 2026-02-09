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
	static const std::vector<std::string> DEFAULT_SOURCE_LINE
	{
		u8"// 毎ビート呼ばれるよ！",
		u8"void Update()",
		u8"{",
		u8"  Run();  // これを書くと走るよ",
		u8"}",
	};
	static const float SOURCE_CHECK_INTERVAL_SEC{ 1.0f };

	static constexpr auto createCodeViewer
	{
		[](ViewerBox* _p) -> ViewerBox*
		{
			return &_p
				->SetFrameWidth(5)
				.SetIsScrollable(true)
				.SetShowLineCount(10)
				.SetTextBoxMargin(3)
				.SetIsShowLineCountBar(true);
		}
	};

	static constexpr auto createByteViewer
	{
		[](ViewerBox* _p) -> ViewerBox*
		{
			return &_p
				->SetFrameWidth(5)
				.SetIsScrollable(true)
				.SetShowLineCount(10)
				.SetTextBoxMargin(3)
				.SetIsShowLineCountBar(true);
		}
	};

	static constexpr auto createRegisterViewer
	{
		[](ViewerBox* _p) -> ViewerBox*
		{
			return &_p
				->SetFrameWidth(5)
				.SetIsScrollable(true)
				.SetUseGhost(true)
				.SetTextBoxMargin(3)
				.SetIsShowLineCountBar(false)
				.SetPosition({ 1070, 430 }, ViewerBox::Pivot::TopLeft);
		}
	};

	static constexpr auto createStackViewer
	{
		[](ViewerBox* _p) -> ViewerBox*
		{
			return &_p
				->SetFrameWidth(5)
				.SetIsScrollable(true)
				.SetUseGhost(true)
				.SetShowLineCount(7)
				.SetTextBoxMargin(3)
				.SetIsShowLineCountBar(false)
				.SetPosition({ 1129, 260 }, ViewerBox::Pivot::TopLeft);
		}
	};

	static constexpr auto createCallStackViewer
	{
		[](ViewerBox* _p) -> ViewerBox*
		{
			return &_p
				->SetFrameWidth(5)
				.SetIsScrollable(true)
				.SetUseGhost(true)
				.SetShowLineCount(7)
				.SetTextBoxMargin(3)
				.SetIsShowLineCountBar(false)
				.SetPosition({ 999, 260 }, ViewerBox::Pivot::TopLeft);
		}
	};

	static constexpr auto createMemoryViewer
	{
		[](ViewerBox* _p) -> ViewerBox*
		{
			return &_p
				->SetFrameWidth(5)
				.SetIsScrollable(false)
				.SetUseGhost(true)
				.SetTextBoxMargin(3)
				.SetIsShowLineCountBar(false)
				.SetPosition({ 1020, 502 }, ViewerBox::Pivot::TopLeft);
		}
	};
}

PlayScene::PlayScene() :
	pSourceObserver_{ new SourceObserver{ SRC_FILE_NAME } }
{
	Timer::Instance().Clear();  // 最初にタイマーをクリアする

	// エラーログファイルがあるなら即コンパイル
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
		// コンパイル成功したならここを通過する

		DeleteFile(ERROR_LOG_FILE_NAME);  // エラーログファイル消す
	}


	// 始まったときのフェードイン
	new Fader{ PLAY_IMAGE_PATH, 1.0f, true };

	Stage* pStage{ new Stage{} };
	// NOTE: Stageを参照するオブジェクトはこれ以降に書く必要がある

	new CursorText{};

	new DocButton{};
	new ReloadButton{};
	new WriteButton{};

	new Background{ PLAY_IMAGE_PATH };
	
#pragma region ビューボックス
	ViewerBox* pCodeViewer{ createCodeViewer(new ViewerBox{}) };
	pCodeViewer->SetOnClick([&, this]()
		{
			OpenSrcFile();
		});

	ViewerBox* pByteViewer{ createByteViewer(new ViewerBox{}) };
	
	ViewerBox* pRegisterViewer{ createRegisterViewer(new ViewerBox{}) };

	ViewerBox* pStackViewer{ createStackViewer(new ViewerBox{}) };

	ViewerBox* pCallStackViewer{ createCallStackViewer(new ViewerBox{}) };

	ViewerBox* pMemoryViewer{ createMemoryViewer(new ViewerBox{}) };
#pragma endregion

	pPlayer_ = pStage->GetPlayer();
	assert(pPlayer_ != nullptr
		&& "ステージからプレイヤーの取得に失敗 @PlayScene::PlayScene");

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
				// ソースファイルを新しく作るときの処理
				_newSource = DEFAULT_SOURCE_LINE;
			})
		.OnUpdateSource([&, this, pCodeViewer](
			const std::vector<std::string>& _newSource)
			{
				// ソースファイルが更新されたときの処理

				srcCodeCache_ = "";
				for (const auto& line : _newSource)
				{
					srcCodeCache_ += line + "\n";
				}

				//　コンパイル処理

				bool isError{ false };
				std::string errorMessage{};
				SOURCE_POS errorPosition{ -1, -1 };
				pCodeViewer->SetTextLines(_newSource);

				ByteCodes byteCodes{};

				// コンパイラ作成
				Compiler{ _newSource, byteCodes }
					.OnError([&, this](
						const std::string& _message,
						const SOURCE_POS _position,
						const Compiler::ErrorExitCode _code)
						{
							// コンパイルエラー発生時の処理
							switch (_code)
							{
							case Compiler::ERR_LEXICAL:
								errorMessage = std::string{ "トークンエラー " };
								if (_position.line >= 0)
								{
									errorMessage += std::to_string(_position.line) + "行 ";
								}
								if (_position.column >= 0)
								{
									errorMessage += std::to_string(_position.column) + "文字目 ";
								}
								errorMessage += _message;
								errorPosition = _position;
								isError = true;
								break;
							case Compiler::ERR_SYNTAX:
								errorMessage = std::string{ "構文エラー " };
								if (_position.line >= 0)
								{
									errorMessage += std::to_string(_position.line) + "行 ";
								}
								if (_position.column >= 0)
								{
									errorMessage += std::to_string(_position.column) + "文字目 ";
								}
								errorMessage += _message;
								errorPosition = _position;
								isError = true;
								break;
							case Compiler::ERR_SEMANTIC:
								errorMessage = std::string{ "文法エラー " };
								if (_position.line >= 0)
								{
									errorMessage += std::to_string(_position.line) + "行 ";
								}
								if (_position.column >= 0)
								{
									errorMessage += std::to_string(_position.column) + "文字目 ";
								}
								errorMessage += _message;
								errorPosition = _position;
								isError = true;
								break;
							default:
								break;
							}
						})
					.Start();  // いざコンパイル！

				if (isError)  // エラーなら止める
				{
					FileSaver::QuickWriteText(ERROR_FILE, errorMessage);
					pPlayer_->SetError(errorMessage, errorPosition);
					return;
				}

				// 表示するバイトコードも更新する
				Assembler::ToString(byteCodes, assembleText_);
				OutputDebugString(assembleText_.c_str());
				pPlayer_->SetByteCode(byteCodes);

				if (FileSaver::ExistFile(ERROR_LOG_FILE_NAME))
				{
					// 前回のエラーログが残っているなら、コンパイル成功したから消す
					DeleteFile(ERROR_LOG_FILE_NAME);
				}
			});


	Timer::AddInterval(SOURCE_CHECK_INTERVAL_SEC, [&, this]()
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
}

void PlayScene::OpenSrcFile()
{
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
