#include <cassert>
#include "PlayScene.h"
#include "IO/Input.h"
#include "Utility/Timer.h"
#include "Utility/StringUtility.h"
#include "IO/SourceObserver.h"

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

namespace
{
	static const char SRC_FILE_NAME[]{ "SourceCode.txt" };
	static const char PLAY_IMAGE_PATH[]{ "Data/Image/Background.png" };
}

PlayScene::PlayScene() :
	pSourceObserver_{ new SourceObserver{ SRC_FILE_NAME } }
{
	Timer::Instance().Clear();  // 最初にタイマーをクリアする

	(new Fader{ PLAY_IMAGE_PATH, 1.0f, true })->OnFinish([&, this]()
		{
			//SceneManager::ChangeScene("PLAY");
		});

	new Background{ PLAY_IMAGE_PATH };
	Stage* pStage{ new Stage{} };
	CodeBox* pCodeBox{ new CodeBox{} };

	ViewerBox* pViewerBox{ new ViewerBox{} };
	//pViewerBox->SetPosition();

	pPlayer_ = pStage->GetPlayer();
	assert(pPlayer_ != nullptr
		&& "ステージからプレイヤーの取得に失敗 @PlayScene::PlayScene");

	using StringUtility::ToString;

	pSourceObserver_
		->OnCreateSource([&, this](
			std::vector<std::string>& _newSource)
			{
				_newSource =
				{
					u8"// 毎ビート呼ばれるよ！",
					u8"void Update()",
					u8"{",
					u8"  Run();  // これを書くと走るよ",
					u8"}",
				};
			})
		.OnUpdateSource([&, this, pCodeBox, pViewerBox](
			const std::vector<std::string>& _newSource)
			{
				//　コンパイル処理

				bool isError{ false };
				std::string errorMessage{};
				SOURCE_POS errorPosition{ -1, -1 };
				//printfDx("ソースファイルに変更があった\n");
 				//pCodeBox->SetSourceLines(_newSource);
				pViewerBox
					->SetTextLines(_newSource)
					.SetFrameWidth(5)
					.SetShowLineCount(7)
					.SetPosition({ 30, 30 }, ViewerBox::Pivot::TopLeft);

				/*pPlayer_->SetByteCode(
					{
						{ -1, BCD_CALL },
						{ -1, 1 },
						{ -1, BCD_HALT },
						{ 3, BCD_ACT },
						{ 3, BCD_ACT_RUN },
						{ 4, BCD_RET }
					});*/

				/*std::vector<std::pair<int, Byte>> byteCodeAndLines{};
				ProtoAnalyzer* analyzer{ new ProtoAnalyzer{ _newSource, byteCodeAndLines } };
				analyzer->OnError([&, this](const char* _msg, const LineCount& _line)
					{
						
					});
				analyzer->Analyze()*/;
				
				//pPlayer_->SetByteCode(byteCodeAndLines);

				SourceLines sourceLines{};

				for (int l = 0; l < _newSource.size(); l++)
				{
					sourceLines.push_back({ l, _newSource[l] });
				}

				Tokens tokens{};

				// 字句解析をする
				LexicalAnalyzer{ sourceLines, tokens }
					.OnError([&, this](const char* _msg, const SOURCE_POS& _srcPos)
						{
							errorMessage = std::string{ "トークンエラー " };
							if (_srcPos.line >= 0)
							{
								errorMessage += std::to_string(_srcPos.line) + "行 ";
							}
							if (_srcPos.column >= 0)
							{
								errorMessage += std::to_string(_srcPos.column) + "文字目 ";
							}
							errorMessage += _msg;
							errorPosition = _srcPos;
							// printfDx("トークンエラー(%d行:%d文字目) %s\n", _srcPos.line, _srcPos.column, _msg);
							isError = true;
						})
					.Analyze();

				if (isError)  // エラーなら止める
				{
					pPlayer_->SetError(errorMessage, errorPosition);
					return;
				}

				Nodes nodes{};

				SyntaxAnalyzer{ tokens, nodes }
					.OnError([&, this](const char* _msg, const SOURCE_POS& _srcPos)
						{
							errorMessage = std::string{ "構文エラー " };
							if (_srcPos.line >= 0)
							{
								errorMessage += std::to_string(_srcPos.line) + "行 ";
							}
							if (_srcPos.column >= 0)
							{
								errorMessage += std::to_string(_srcPos.column) + "文字目 ";
							}
							errorMessage += _msg;
							errorPosition = _srcPos;
							//printfDx("構文エラー(%d行:%d文字目) %s\n", _srcPos.line, _srcPos.column, _msg);
							isError = true;
						})
					.Analyze();

				if (isError)  // エラーなら止める
				{
					pPlayer_->SetError(errorMessage, errorPosition);
					return;
				}

				std::pair<Nodes&, Tokens&> nodeAndTokens{ nodes, tokens };

				ByteCodes byteCodes{};

				SemanticAnalyzer{ nodeAndTokens, byteCodes }
					.OnError([&, this](const char* _msg, const SOURCE_POS& _srcPos)
						{
							errorMessage = std::string{ "文法エラー " };
							if (_srcPos.line >= 0)
							{
								errorMessage += std::to_string(_srcPos.line) + "行 ";
							}
							if (_srcPos.column >= 0)
							{
								errorMessage += std::to_string(_srcPos.column) + "文字目 ";
							}
							errorMessage += _msg;
							errorPosition = _srcPos;
							//printfDx("文法エラー(%d行:%d文字目) %s\n", _srcPos.line, _srcPos.column, _msg);
							isError = true;
						})
					.Analyze();

				if (isError)  // エラーなら止める
				{
					pPlayer_->SetError(errorMessage, errorPosition);
					return;
				}

				for (auto& node : nodeAndTokens.first)
				{
					delete node;
				}


				std::string asTex{};
				Assembler::ToAssemble(byteCodes, asTex);
				OutputDebugString(asTex.c_str());


				//std::string testout = test.str();
				//const char* strtestout = testout.c_str();
				//OutputDebugString(strtestout);

				pPlayer_->SetByteCode(byteCodes);
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
