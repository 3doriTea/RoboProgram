#include "Player.h"
#include "Stage.h"
#include <cassert>
#include <cmath>


namespace
{
	static const int IMAGE_SIZE{ 80 };
	static const char PLAYER_IMAGE_FILE[]{ "Data/Image/Player.png" };
	static const float MOVE_SPEED{ 5 };
	static const float JUMP_HEIGHT{ 80 };
	static const float GRAVITY{ 0.05 };
	static const int IMAGE_WIDTH{ 80 };
	static const int IMAGE_HEIGHT{ 160 };
}

Player::Player(const Vector2& _position) :
	velocity_{},
	gravity{ GRAVITY },
	jumpHeight{ JUMP_HEIGHT },
	moveSpeed{ MOVE_SPEED },
	jumpV0{}
{
	jumpV0 = -std::sqrtf(2.0f * gravity * jumpHeight);

	rect_.pivot = _position + Vector2(0, -50);
	rect_.width = IMAGE_WIDTH;
	rect_.height = IMAGE_HEIGHT;

	hImage_ = LoadGraph(PLAYER_IMAGE_FILE);
	assert(hImage_ > 0
		&& "Playerのイメージ読み込みに失敗 @Player::Player");
}

Player::~Player()
{
}

void Player::Update()
{
	if (CheckHitKey(KEY_INPUT_A))
	{
		rect_.pivot.x -= moveSpeed;

		float push = pStage_->CheckLeft(rect_.pivot + Vector2{ 0, 0 });
		rect_.pivot.x += push;
		push = pStage_->CheckLeft(rect_.pivot + Vector2{ 0, rect_.height });
		rect_.pivot.x += push;
	}
	if (CheckHitKey(KEY_INPUT_D))
	{
		rect_.pivot.x += moveSpeed;
		// 右上の当たり判定と埋め込み反発取得
		float push = pStage_->CheckRight(rect_.pivot + Vector2{ rect_.width, 0 });
		rect_.pivot.x -= push;
		// 右下の当たり判定と埋め込み反発取得
		push = pStage_->CheckRight(rect_.pivot + Vector2{ rect_.width, rect_.height });
		rect_.pivot.x -= push;
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
	{
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
	}
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
