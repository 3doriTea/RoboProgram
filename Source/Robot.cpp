#include "Robot.h"
#include "../Library/Time.h"
#include "Stage.h"
#include "ByteCodeDefine.h"
#include <cassert>


namespace
{
	static const float RUNNING_TIME_SEC{ 0.3f }; // ���鎞�� �b
}

Robot::Robot(
	Rectan& _rect,
	float& _moveSpeed,
	float& _jumpV0,
	Stage*& _pStage,
	bool& _isGrounded,
	Vector2& _velocity,
	bool& _prevPushedSpace,
	const std::vector<Byte>& _byteCode) :
	rect_{ _rect },
	moveSpeed_{ _moveSpeed },
	jumpV0_{ _jumpV0 },
	pStage_{ _pStage },
	isGrounded_{ _isGrounded },
	isLeftDir_{ false },
	runningTimeLeft_{ 0.0f },
	velocity_{ _velocity },
	prevPushedSpace_{ _prevPushedSpace },
	codeRunner_{ bcr_, memory_, stackMachine_, callStack_, register_, REGISTER_SIZE },
	bcr_{ _byteCode },
	toJump_{ false }
{
	codeRunner_
		.OnActionMessage([&, this](const ActionMessage _msg)
			{
				if (_msg == ActionMessage::Run)
				{
					Run();
				}
				else if (_msg == ActionMessage::Jump)
				{
					Jump();
				}
			})
		.OnGetIOMessage([&, this](const GetIOMessage _msg) -> int
			{
				switch (_msg)
				{
				case GetIOMessage::IsGrounded:
					return isGrounded_ ? 1 : 0;
				case GetIOMessage::GetOnTileNumber:
					{
						Vector2Int tilePosition
						{
							pStage_->ToTilePosition(
								rect_.pivot + Vector2{ rect_.size.x / 2, rect_.size.y } + Vector2{ 0, 30.0f })
							.ToInt()
						};

						int tile{ pStage_->GetTile(tilePosition) };
						switch (tile)
						{
						case 6:
							tile = 1;
							break;
						default:
							tile = -1;
							break;
						}
						return tile;
					}
				case GetIOMessage::CheckTile:  // �^�C���擾������
				default:
					return 0;
				}
			});
}

Robot::~Robot()
{
}

void Robot::Update()
{
	if (runningTimeLeft_ >= 0)
	{
		runningTimeLeft_ -= Time::DeltaTime();
	}

	if (runningTimeLeft_ >= 0)
	{
		if (isLeftDir_)
		{
			rect_.pivot.x -= moveSpeed_;

			float push = pStage_->CheckLeft(rect_.pivot + Vector2{ 0, 0 });
			rect_.pivot.x += push;
			push = pStage_->CheckLeft(rect_.pivot + Vector2{ 0, rect_.height });
			rect_.pivot.x += push;
		}
		else
		{
			rect_.pivot.x += moveSpeed_;
			// �E��̓����蔻��Ɩ��ߍ��ݔ����擾
			float push = pStage_->CheckRight(rect_.pivot + Vector2{ rect_.width, 0 });
			rect_.pivot.x -= push;
			// �E���̓����蔻��Ɩ��ߍ��ݔ����擾
			push = pStage_->CheckRight(rect_.pivot + Vector2{ rect_.width, rect_.height });
			rect_.pivot.x -= push;
		}
	}

	if (isGrounded_)
	{
		if (toJump_)
		{
			toJump_ = false;
			if (prevPushedSpace_ == false)
			{
				velocity_.y = jumpV0_;
			}
			prevPushedSpace_ = true;
		}
		else
		{
			prevPushedSpace_ = false;
		}
	}
}

bool Robot::TryReadNext()
{
	return codeRunner_.TryReadNext();

	if (bcr_.IsEndOfCode())
	{
		return false;  // �I���n�_�Ȃ玸�s
	}

	if (bcr_.Consume(BCD_NOP))
	{
		// �������Ȃ�
	}
	else if (bcr_.Consume(BCD_HALT))
	{
		using SeekPoint = ByteCodeReader::SeekPoint;
		bcr_.SeekTo(SeekPoint::Tail);
	}
	else if (bcr_.Consume(BCD_ACT))
	{
		if (bcr_.Consume(BCD_ACT_NONE))
		{
		}
		else if (bcr_.Consume(BCD_ACT_RUN))
		{
			Run();
		}
		else if (bcr_.Consume(BCD_ACT_JUMP))
		{
			Jump();
		}
		else
		{
			// ����`
			assert(false && "����`��ACT���e");
		}
	}
	else
	{
		// ����`
		assert(false && "����`�̖���");
	}
	return true;  // ����������s�ł����Ȃ琬��
}

void Robot::Reset()
{
	codeRunner_.Reset();
}

void Robot::Run()
{
	runningTimeLeft_ = RUNNING_TIME_SEC;
}

void Robot::Jump()
{
	toJump_ = true;
}

void Robot::ChangeDir()
{
	isLeftDir_ = !isLeftDir_;
}

void Robot::SetDir(const bool _isLeft)
{
	isLeftDir_ = _isLeft;
}

inline void Robot::GetMemoryRef(
	const std::function<void(
		const ByteCodeReader& _codeReader,
		const std::vector<Byte>& _memory,
		const Stack<int>& _stackMachine,
		const Stack<Byte>& _callStack,
		const std::vector<Byte>& _register)>& _callback) const
{
	_callback(bcr_, memory_, stackMachine_, callStack_, register_);
}

const int Robot::REGISTER_SIZE{ 4 };
