#include "Player.h"
#include "Stage.h"


namespace
{
	static const int IMAGE_SIZE{ 80 };
}

Player::Player(const Vector2& _position) :
	velocity_{}
{
}

Player::~Player()
{
}

void Player::Update()
{
	if (CheckHitKey(KEY_INPUT_A))
	{
		position_.x -= moveSpeed;

		float
			push = pStage_->CheckLeft(position_ + Vector2{ -24, -31 });
		position_.x += push;
		push = pStage_->CheckLeft(position_ + Vector2{ -24, 31 });
		position_.x += push;
	}
	if (CheckHitKey(KEY_INPUT_D))
	{
		position_.x += moveSpeed;
		// �E��̓����蔻��Ɩ��ߍ��ݔ����擾
		float push = pStage_->CheckRight(position_ + Vector2{ 24, -31 });
		position_.x -= push;
		// �E���̓����蔻��Ɩ��ߍ��ݔ����擾
		push = pStage_->CheckRight(position_ + Vector2{ 24, 31 });
		position_.x -= push;
	}
	{
		isGrounded = false;

		position_ = position_ + velocity_;
		velocity_.y += gravity;

		//position_.y += 1.0f;
		// �����̓����蔻��Ɩ��ߍ��ݔ����擾
		float push = pStage_->CheckBottom(position_ + Vector2
			{
				-24,
				31 + 1  // 1���ߍ���ł݂镪�̓�����ʒu��������
			});
		if (push > 0)
		{
			position_.y -= push - 1;  // �����ĂP�߂荞��ł����Ԃɂ��Ă���
			velocity_.y = 0;
			isGrounded = true;
		}
		// �E���̓����蔻��Ɩ��ߍ��ݔ����擾
		push = pStage_->CheckBottom(position_ + Vector2{ 24, 31 + 1 });
		if (push > 0)
		{
			position_.y -= push - 1;  // �����ĂP�߂荞��ł����Ԃɂ��Ă���
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
		// ��̕ǂɓ�����Ȃ��悤�ɂ��鏈��
		if (velocity_.y < 0.0f)
		{
			float push = pStage_->CheckTop(position_ + Vector2{ 24, -31 });
			if (push > 0)
			{
				velocity_.y = 0.0f;
				position_.y += push;
			}
			push = pStage_->CheckTop(position_ + Vector2{ -24, -31 });
			if (push > 0)
			{
				velocity_.y = 0.0f;
				position_.y += push;
			}
		}
		else
		{

		}
	}

	// �X�N���[���̌v�Z
	Vector2 scroll{ pStage_->GetScroll() };
	float drawX{ position_.x - scroll.x };

	static const int RIGHT_LIMIT{ 400 };
	if (position_.x > RIGHT_LIMIT)  // �X�N���[�����鋫�E
	{
		//pStage_->SetScroll({ scroll.x + (drawX - 400), 0.0f });
		pStage_->SetScroll({ position_.x - RIGHT_LIMIT, 0.0f });
	}

	static const int LEFT_LIMIT{ static_cast<int>(IMAGE_SIZE / 2.0f) };
	if (position_.x < LEFT_LIMIT)
	{
		position_.x = LEFT_LIMIT;
	}
}

void Player::Draw()
{
}
