#include "Stage.h"
#include "IO/Input.h"
#include "../Library/CsvReader/CsvReader.h"
#include <cassert>
#include <sstream>
#include "Player.h"
#include "Coin.h"
#include "Flag.h"
#include "Doc.h"
#include "Info.h"
#include "Goal.h"
#include "Lightning.h"
#include "PlayScene.h"
#include "DocButton.h"


namespace
{
	static const int TILE_WIDTH{ 80 };
	static const int TILE_HEIGHT{ 80 };
	static const float INVALED_POSITION{ -100 };
	static const char DOC_FILE_NAME[]{ "Document.txt" };
	static const char* TILE_FILES[]
	{
		"",
		"Data/Image/tileGround1.png",
		"",  // coin
		"",  // player
		"",  // flag
		"",  // doc
		"Data/Image/Tile1.png",  // tile1
	};

	static const char SAVE_FILE_NAME[]{ "SaveFile.bin" };
	static const size_t SAVE_FILE_BUFFER_SIZE{ 1024 };
	static const char MAP_FILE_NAME_DEFAULT[]{ "Data/map.csv" };
	static const char MAP_FILE_NAME_FINAL[]{ "Data/mapFinal.csv" };
}

Stage::Stage() :
	saveFile_{ SAVE_FILE_NAME, SAVE_FILE_BUFFER_SIZE },
	checkPoint_{ INVALED_POSITION, INVALED_POSITION },
	infoLevel_{ 0 },
	documentLevel_{ 0 },
	readedDocLevel_{ 0 },
	isGoaled_{ false }
{
	saveFile_.OnLoad([&, this](mtbin::MemoryStream& _ms)
		{
			checkPoint_.x = _ms.Read<float>();
			checkPoint_.y = _ms.Read<float>();
			documentLevel_ = _ms.Read<int>();
			infoLevel_ = _ms.Read<int>();
			//PlayScene::SetIsFinalMode(_ms.Read<bool>());
			isGoaled_ = _ms.Read<bool>();
			readedDocLevel_ = _ms.Read<int>();
		});
	saveFile_.OnSave([&, this](mtbin::MemoryStream& _ms)
		{
			_ms.Write(checkPoint_.x);
			_ms.Write(checkPoint_.y);
			_ms.Write(documentLevel_);
			_ms.Write(infoLevel_);
			_ms.Write(isGoaled_);
			_ms.Write(readedDocLevel_);
		});

	saveFile_.TryLoad();

	int hImage = LoadGraph(TILE_FILES[TILE_GROUND]);
	assert(hImage > 0
		&& "画像読み込みに失敗 @Stage::Stage");
	hImages_.insert({ TILE_GROUND, hImage });

	hImage = LoadGraph(TILE_FILES[TILE_1]);
	assert(hImage > 0
		&& "画像読み込みに失敗 @Stage::Stage");
	hImages_.insert({ TILE_1, hImage });

	// ロードするマップのcsvファイル
	const char* LOAD_CSV_FILE_NAME
	{
		PlayScene::GetIsFinalMode()  // ファイナルモードなら専用のcsv
		? MAP_FILE_NAME_FINAL
		: MAP_FILE_NAME_DEFAULT
	};

	CsvReader* csv{ new CsvReader { LOAD_CSV_FILE_NAME } };
	
	// CSVからステージデータを読み込む
	for (int line = 0; line < csv->GetLines(); line++)
	{
		std::vector<Tile> mapLine{};
		for (int column = 0; column < csv->GetColumns(line); column++)
		{
			mapLine.push_back((Tile)csv->GetInt(line, column));
		}
		map_.push_back(mapLine);
	}

	delete csv;

	// チェックポイントの情報がない
	if (PlayScene::GetIsFinalMode() == true ||
		(checkPoint_.x <= INVALED_POSITION + 1
		&& checkPoint_.y <= INVALED_POSITION + 1))
	{
		Vector2 playerPosition{};
		assert(TryFindPlayerPositionFromMap(&playerPosition)
			&& "マップデータにプレイヤーがいない");

		pPlayer_ = new Player{ playerPosition };
	}
	else
	{
		pPlayer_ = new Player{ checkPoint_ };
	}

	int docLevelCount{ 0 };
	for (int y = 0; y < map_.size(); y++)
	{
		for (int x = 0; x < map_[y].size(); x++)
		{
			switch (map_[y][x])
			{
			case TILE_COIN:
				new Coin
				{
					{
						static_cast<float>(x) * TILE_WIDTH,
						static_cast<float>(y) * TILE_HEIGHT
					}
				};
				break;
			case TILE_FLAG:
				if (PlayScene::GetIsFinalMode())
				{
					break;  // ファイナルモードならフラッグなし
				}
				new Flag
				{
					{
						static_cast<float>(x)* TILE_WIDTH,
						static_cast<float>(y)* TILE_HEIGHT
					}
				};
				break;
			case TILE_DOC:
				new Doc
				{
					{
						static_cast<float>(x) * TILE_WIDTH,
						static_cast<float>(y) * TILE_HEIGHT
					},
					++docLevelCount
				};
				break;
			case TILE_LIGHTNING:
				new Lightning
				{
					{
						static_cast<float>(x) * TILE_WIDTH,
						static_cast<float>(y) * TILE_HEIGHT
					}
				};
				break;
			case TILE_INFO:
				new Info
				{
					{
						static_cast<float>(x) * TILE_WIDTH,
						static_cast<float>(y) * TILE_HEIGHT
					},
					1
				};
				break;
			case TILE_GOAL:
				new Goal
				{
					{
						static_cast<float>(x) * TILE_WIDTH,
						static_cast<float>(y) * TILE_HEIGHT
					}
				};
				break;
			default:
				break;
			}
		}
	}
}

Stage::~Stage()
{
	for (auto& hImage : hImages_)
	{
		DeleteGraph(hImage.second);
	}
}

//void Stage::Update()
//{
//}

void Stage::Draw()
{
	for (int y = 0; y < map_.size(); y++)
	{
		for (int x = 0; x < map_[y].size(); x++)
		{
			position_.x = x * TILE_WIDTH;
			position_.y = y * TILE_HEIGHT;

			int tileType = map_[y][x];

			switch (tileType)
			{
			case TILE_GROUND:
				DrawTile({ x, y }, TILE_GROUND);
				break;
			case TILE_1:
				DrawTile({ x, y }, TILE_1);
				break;
			default:
				break;
			}

		}
	}
}

#pragma region 当たり判定
float Stage::CheckRight(Vector2 _position)
{
	if (IsWall(_position) == false)
	{
		return 0.0f;
	}

	Vector2 tileWorldPosition{ ToWorldPosition(ToTilePosition(_position)) };
	return _position.x - tileWorldPosition.x + 1;  // 端数が0の場合も1足す
}

float Stage::CheckLeft(Vector2 _position)
{
	if (IsWall(_position) == false)
	{
		return 0.0f;
	}

	Vector2 tileWorldPosition{ ToWorldPosition(ToTilePosition(_position)) };

	return TILE_WIDTH - (_position.x - tileWorldPosition.x);
}

float Stage::CheckBottom(Vector2 _position)
{
	if (IsWall(_position) == false)
	{
		return 0.0f;
	}

	Vector2 tileWorldPosition{ ToWorldPosition(ToTilePosition(_position)) };
	return _position.y - tileWorldPosition.y + 1;  // こっちも端数を1足している
}

float Stage::CheckTop(Vector2 _position)
{
	if (IsWall(_position) == false)
	{
		return 0.0f;
	}

	Vector2 tileWorldPosition{ ToWorldPosition(ToTilePosition(_position)) };

	return TILE_HEIGHT - (_position.y - tileWorldPosition.y);
}

bool Stage::IsWall(const Vector2& _position)
{
	Vector2 tilePosition{ ToTilePosition(_position) };

	// 配列の外に行くかもしれない 落ちた先は何もないから当たってない判定に
	if (tilePosition.y < 0.0f || tilePosition.y >= map_.size())
	{
		return false;
	}
	// 同じく配列の外の処理 TODO: 壁の外にいけないようにしたいなら、ここはtrue返す
	if (tilePosition.x < 0.0f || tilePosition.x >= map_[tilePosition.y].size())
	{
		return false;
	}

	// タイルの番号を見て、壁かどうか確定する
	switch (map_[tilePosition.y][tilePosition.x])
	{
	case TILE_NONE:  // 空白
	case TILE_PLAYER:  // プレイヤー
	case TILE_COIN:  // コイン
	case TILE_FLAG:  // チェックポイント
	case TILE_DOC:  // チェックポイント
	case TILE_LIGHTNING:  // 感電
	case TILE_INFO:  // 情報
	case TILE_GOAL:  // 博士(ゴール)
	case TILE_MAX:  // ?
		return false;  // 壁でない
	default:
		break;
	}
	// 値を返す関数がちゃんと値を返しているかを保障するために、見やすさのため
	return true;  // 壁である
}
#pragma endregion

#pragma region 変換系
Vector2 Stage::ToWorldPosition(const Vector2& tilePosition)
{
	return
	{
		tilePosition.x * TILE_WIDTH,
		tilePosition.y * TILE_HEIGHT,
	};
}

Vector2 Stage::ToWorldPosition(const Vector2Int& tilePosition)
{
	return
	{
		static_cast<float>(tilePosition.x * TILE_WIDTH),
		static_cast<float>(tilePosition.y * TILE_HEIGHT),
	};
}

Vector2 Stage::ToTilePosition(const Vector2& worldPosition)
{
	return
	{  // 端数を削る
		static_cast<float>(static_cast<int>(worldPosition.x / TILE_WIDTH)),
		static_cast<float>(static_cast<int>(worldPosition.y / TILE_HEIGHT)),
	};
}
const int Stage::GetTile(const Vector2Int& tilePosition) const
{
	int tileX{ tilePosition.x };
	int tileY{ tilePosition.y };

	// 範囲外除外
	if (tileY < 0 || map_.size() <= tileY)
	{
		return -1;
	}
	if (tileX < 0 || map_[tileY].size() <= tileX)
	{
		return -1;
	}

	return map_[tileY][tileX];
}
#pragma endregion

const Vector2Int Stage::GetPush(const Vector2Int& _point, const PushDir _dir) const
{
	int tileX{ _point.x / TILE_WIDTH };
	int tileY{ _point.y / TILE_HEIGHT };

	// 範囲外除外
	if (tileY < 0 || map_.size() <= tileY)
	{
		return Vector2Int::Zero();
	}
	if (tileX < 0 || map_[tileY].size() <= tileX)
	{
		return Vector2Int::Zero();
	}

	// タイルが壁であるか
	Tile tile = map_[tileY][tileX];
	switch (tile)
	{
	case TILE_GROUND:
		break;
	default:
		return Vector2Int::Zero();  // 壁ではないなら回帰
	}

	Vector2Int push{};

	switch (_dir)
	{
	case PushDir::Up:
		push.y = TILE_HEIGHT - (_point.y % TILE_HEIGHT) + 1;
		break;
	case PushDir::Down:
		push.y = -(_point.y % TILE_HEIGHT) - 1;
		break;
	case PushDir::Left:
		push.x = TILE_WIDTH - (_point.x % TILE_WIDTH) + 1;
		break;
	case PushDir::Right:
		push.x = -(_point.x % TILE_WIDTH) - 1;
		break;
	default:
		break;
	}
	return push;
}

const Vector2Int Stage::GetPush(const RectInt& _rect) const
{
	Vector2Int push{};
	GetPush({}, PushDir::Up);

	return Vector2Int();
}

void Stage::DrawTile(const Vector2Int& _tilePosition, const Tile& _tileId)
{
	int w{ TILE_WIDTH };
	int h{ TILE_HEIGHT };

	DrawGraph(
		_tilePosition.x * w - scroll_.x,
		_tilePosition.y * h - scroll_.y,
		hImages_[_tileId], TRUE);
	/*DrawRectGraph(
		_tilePosition.x * w - scroll_.x, _tilePosition.y * h - scroll_.y,
		TILE_WIDTH * w, TILE_HEIGHT * h,
		w, h,
		hImages_[_tileId], TRUE);*/
}

bool Stage::TryFindPlayerPositionFromMap(Vector2* _pPosition) const
{
	for (int y = 0; y < map_.size(); y++)
	{
		for (int x = 0; x < map_[y].size(); x++)
		{
			if (map_[y][x] == TILE_PLAYER)
			{
				*_pPosition = Vector2(
					x * TILE_WIDTH + TILE_WIDTH / 2.0f,
					y * TILE_HEIGHT + TILE_HEIGHT / 2.0f);
				return true;
			}
		}
	}

	return false;
}

Player* Stage::GetPlayer()
{
	assert(pPlayer_ != nullptr);
	return pPlayer_;
}

void Stage::SetCheckPoint(const Vector2& _point)
{
	checkPoint_ = _point;
	Save();
}

Vector2 Stage::GetCheckPoint() const
{
	return checkPoint_;
}

void Stage::OpenDocument()
{
	FileSaver::QuickWriteText(DOC_FILE_NAME, WriteDocument());
	GetScene<PlayScene>()->OpenDocument();

	readedDocLevel_ = documentLevel_;
}

void Stage::GetDocument(const int _docLevel)
{
	if (documentLevel_ < _docLevel)
	{
		documentLevel_ = _docLevel;
		DocButton* pDocButton{ FindGameObject<DocButton>() };
		if (pDocButton != nullptr)
		{
			pDocButton->News();
		}
	}
}

void Stage::GetInfo(const int _infoLevel)
{
	infoLevel_ = _infoLevel;
	pPlayer_->SetInfoLevel(true);
}

void Stage::Save()
{
	saveFile_.TrySave();
}

std::string Stage::WriteDocument() const
{
	std::stringstream ss{};
	using std::endl;

	ss << "【ドキュメント】" << endl
		<< "コードを書くためのヒントが載っているよ！" << endl
		<< "サンプルだから、コピペしてもいいけど自分でいい感じに編集してね！" << endl;

	if (documentLevel_ >= 2)
	{
		ss << endl << endl
		<< "【変数と条件分岐、チェック関数】" << endl << endl
		<< "void Sample2()" << endl
		<< "{" << endl
		<< "  // 乗っているタイルに書かれている番号を、変数「tileNumber」に代入するよ" << endl
		<< "  int tileNumber = GetOnTileNumber();" << endl
		<< "" << endl
		<< "  // もし、tileの番号が1なら、ジャンプするよ" << endl
		<< "  if (tileNumber == 1)" << endl
		<< "  {" << endl
		<< "    Jump();  // 条件が満たされたとき、「Jump関数」が呼び出される" << endl
		<< "  }" << endl
		<< "}" << endl
			<< "" << endl;
	}
	if (documentLevel_ >= 1)
	{
		ss << endl << endl
			<< "【関数定義とジャンプ関数】" << endl << endl
			<< "// 「Update関数」以外に、オリジナルの関数を定義できるよ！" << endl
			<< "// ただし、「Update関数」より上に書かないと見つからないよ" << endl
			<< "void Sample1()" << endl
			<< "{" << endl
			<< "  Jump();  // ロボットをジャンプさせるときは、「ジャンプ関数」を呼び出すよ" << endl
			<< "}" << endl
			<< "// 「Sample1関数」は「Jump関数」と同じように呼び出せるよ" << endl
			<< "// Sample1();" << endl
			<< "" << endl;
	}
	if (documentLevel_ >= 0)
	{
		ss << endl << endl
			<< "【更新処理】" << endl << endl
			<< "// 「Update関数」は一定間隔で呼ばれるよ！" << endl
			<< "void Update()" << endl
			<< "{" << endl
			<< "  // ここに書いた処理は上から順に実行されていくよ" << endl
			<< "  Run();  // ロボットを走らせるときは、「Run関数」を呼び出すよ" << endl
			<< "}" << endl
			<< "" << endl;
	}

	return ss.str();
}
