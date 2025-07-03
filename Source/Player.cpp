#include "Player.h"
#include "Stage.h"
#include <cassert>
#include <cmath>
#include "Utility/RectanUtility.h"
#include "IO/Input.h"
#include "PlayScene.h"
#include "Screen.h"


namespace
{
	static const int IMAGE_SIZE{ 80 };
	static const char PLAYER_IMAGE_FILE[]{ "Data/Image/Player.png" };
	static const char PLAYER_READY_IMAGE_FILE[]{ "Data/Image/Player-ready.png" };
	static const char PLAYER_RUNNING_IMAGE_FILE[]{ "Data/Image/Player-running.png" };
	static const char PLAYER_ERROR_IMAGE_FILE[]{ "Data/Image/Player-error.png" };
	static const float MOVE_SPEED{ 5 };
	static const float JUMP_HEIGHT{ 80 };
	static const float GRAVITY{ 0.05 };
	static const int IMAGE_WIDTH{ 80 };
	static const int IMAGE_HEIGHT{ 160 };
	static const float ROBOT_STEP_TIME_SEC{ 0.07 / 3.0f };
	static const float ROBOT_BEAT_TIME_SEC{ 1 };
}

Player::Player(const Vector2& _position) :
	velocity_{},
	gravity{ GRAVITY },
	jumpHeight{ JUMP_HEIGHT },
	moveSpeed{ MOVE_SPEED },
	jumpV0{},
	robot_{ rect_, moveSpeed, jumpV0, pStage_, isGrounded, velocity_, prevPushedSpace, byteCode_ },
	hBeatTimer_{ 0 }
{
	jumpV0 = -std::sqrtf(2.0f * gravity * jumpHeight);

	rect_.pivot = _position + Vector2(0, -50);
	rect_.width = IMAGE_WIDTH;
	rect_.height = IMAGE_HEIGHT;

	hStateImages[S_READY] = LoadGraph(PLAYER_READY_IMAGE_FILE);
	assert(hStateImages[S_READY] > 0
		&& "PlayerのReadyイメージ読み込みに失敗 @Player::Player");
	hStateImages[S_RUN] = LoadGraph(PLAYER_RUNNING_IMAGE_FILE);
	assert(hStateImages[S_RUN] > 0
		&& "PlayerのRunイメージ読み込みに失敗 @Player::Player");
	hStateImages[S_ERROR] = LoadGraph(PLAYER_ERROR_IMAGE_FILE);
	assert(hStateImages[S_ERROR] > 0
		&& "PlayerのErrorイメージ読み込みに失敗 @Player::Player");

	SetState(S_READY);
}

Player::~Player()
{
}

void Player::Update()
{
	int mouseX{}, mouseY{};
	GetMousePoint(&mouseX, &mouseY);

	if (RectanUtility::IsHit(GetRect(), {static_cast<float>(mouseX), static_cast<float>(mouseY)}))
	{
		if (Input::IsMouseDown(MOUSE_INPUT_LEFT))
		{
			State current = GetState();
			switch (current)
			{
			case Player::S_READY:
				SetState(S_RUN);
				break;
			case Player::S_RUN:
				SetState(S_READY);
				GetScene<PlayScene>()->OpenSrcFile();
				break;
			case Player::S_ERROR:
				printfDx("コードにエラーがあるため、実行できません。\n");
				break;
			case Player::S_MAX:
			default:
				break;
			}
		}
	}

	{
		isGrounded = false;

		rect_.pivot = rect_.pivot + velocity_;
		velocity_.y += gravity;

		//rect_.pivot.y += 1.0f;
		// 左下の当たり判定と埋め込み反発取得
		float push = pStage_->CheckBottom(rect_.pivot + Vector2
			{
				0,
				rect_.height + 1  // 1つ埋め込んでみる分の当たる位置を下げる
			});
		if (push > 0)
		{
			rect_.pivot.y -= push - 1;  // あえて１つめり込んでいる状態にしておく
			velocity_.y = 0;
			isGrounded = true;
		}
		// 右下の当たり判定と埋め込み反発取得
		push = pStage_->CheckBottom(rect_.pivot + Vector2{ rect_.width, rect_.height + 1 });
		if (push > 0)
		{
			rect_.pivot.y -= push - 1;  // あえて１つめり込んでいる状態にしておく
			velocity_.y = 0;
			isGrounded = true;
		}
	}

	// ロボ処理
	robot_.Update();

	//if (CheckHitKey(KEY_INPUT_A))
	//{
	//	rect_.pivot.x -= moveSpeed;
	//	float push = pStage_->CheckLeft(rect_.pivot + Vector2{ 0, 0 });
	//	rect_.pivot.x += push;
	//	push = pStage_->CheckLeft(rect_.pivot + Vector2{ 0, rect_.height });
	//	rect_.pivot.x += push;
	//}
	//if (CheckHitKey(KEY_INPUT_D))
	//{
	//	rect_.pivot.x += moveSpeed;
	//	// 右上の当たり判定と埋め込み反発取得
	//	float push = pStage_->CheckRight(rect_.pivot + Vector2{ rect_.width, 0 });
	//	rect_.pivot.x -= push;
	//	// 右下の当たり判定と埋め込み反発取得
	//	push = pStage_->CheckRight(rect_.pivot + Vector2{ rect_.width, rect_.height });
	//	rect_.pivot.x -= push;
	//}
	/*{
		if (isGrounded)
		{
			if (CheckHitKey(KEY_INPUT_SPACE))
			{
				if (prevPushedSpace == false)
				{
					velocity_.y = jumpV0;
				}
				prevPushedSpace = true;
			}
			else
			{
				prevPushedSpace = false;
			}
		}
	}*/
	{
		// 上の壁に当たらないようにする処理
		if (velocity_.y < 0.0f)
		{
			float push = pStage_->CheckTop(rect_.pivot + Vector2{ 0, 0 });
			if (push > 0)
			{
				velocity_.y = 0.0f;
				rect_.pivot.y += push;
			}
			push = pStage_->CheckTop(rect_.pivot + Vector2{ rect_.width, 0 });
			if (push > 0)
			{
				velocity_.y = 0.0f;
				rect_.pivot.y += push;
			}
		}
		else
		{

		}
	}

	if (rect_.pivot.y > Screen::HEIGHT)
	{
		rect_.pivot = pStage_->GetCheckPoint();
		SceneManager::ReloadScene();
	}

	// スクロールの計算
	Vector2 scroll{ pStage_->GetScroll() };
	float drawX{ rect_.pivot.x - scroll.x };

	static const int RIGHT_LIMIT{ 400 };
	if (rect_.pivot.x > RIGHT_LIMIT)  // スクロールする境界
	{
		//pStage_->SetScroll({ scroll.x + (drawX - 400), 0.0f });
		pStage_->SetScroll({ rect_.pivot.x - RIGHT_LIMIT, 0.0f });
	}

	static const int LEFT_LIMIT{ static_cast<int>(IMAGE_SIZE / 2.0f) };
	if (rect_.pivot.x < LEFT_LIMIT)
	{
		rect_.pivot.x = LEFT_LIMIT;
	}
}

void Player::Draw()
{
	Object2D::Draw();
}

int Player::GetHeight() const
{
	return IMAGE_HEIGHT;
}

int Player::GetWidth() const
{
	return IMAGE_WIDTH;
}

void Player::SetError(const std::string& _message, const SOURCE_POS& _srcPos)
{
	SetState(S_ERROR);
}

void Player::SetByteCode(const ByteCodes& _byteCode)
{
	robot_.Reset();
	if (currentState_ == S_ERROR)
	{
		SetState(S_READY);
	}

	byteCodeAndLines_ = _byteCode;
	byteCode_.clear();
	for (auto&& byteCodeAndLine : byteCodeAndLines_)
	{
		byteCode_.push_back(byteCodeAndLine.second);
	}
}

int Player::GetReadLine() const
{
	int index{ robot_.GetReadByteCodeIndex() };
	if (index < 0 || byteCodeAndLines_.size() <= index)
	{
		return -1;
	}
	return byteCodeAndLines_[index].first.line;
}

void Player::SetState(const State _state)
{
	assert(0 <= _state && _state < S_MAX
		&& "範囲外のステータスはセットできません。 @Player::SetState");

	hImage_ = hStateImages[static_cast<int>(_state)];
	currentState_ = _state;

	// ステート別処理走らせる
	switch (_state)
	{
	case S_READY:
		Timer::Remove(hBeatTimer_);
		hBeatTimer_ = nullptr;
		break;
	case S_RUN:
		prevIsSucceedTryRead = false;
		sleepCount_ = 0;
		robot_.Reset();

		hBeatTimer_ = Timer::AddInterval(ROBOT_STEP_TIME_SEC, [&, this]()
			{
				bool succeed = robot_.TryReadNext();
				if (succeed != prevIsSucceedTryRead)
				{
					prevIsSucceedTryRead = succeed;
					sleepCount_ = 0;
				}
				if (succeed == false)
				{
					sleepCount_++;
					if (sleepCount_ > static_cast<int>(ROBOT_BEAT_TIME_SEC / ROBOT_STEP_TIME_SEC))
					{
						robot_.Reset();
					}
				}
			});
		break;
	case S_ERROR:
		prevIsSucceedTryRead = false;
		sleepCount_ = 0;
		robot_.Reset();

		Timer::Remove(hBeatTimer_);
		hBeatTimer_ = nullptr;
		break;
	default:
		break;
	}
}
