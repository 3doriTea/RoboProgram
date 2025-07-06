#include "Player.h"
#include "Stage.h"
#include <cassert>
#include <sstream>
#include <cmath>
#include <iomanip>
#include "Utility/RectanUtility.h"
#include "IO/Input.h"
#include "PlayScene.h"
#include "Screen.h"
#include "ViewerBox.h"


namespace
{
	static const int IMAGE_SIZE{ 80 };
	static const char PLAYER_IMAGE_FILE[]{ "Data/Image/Player.png" };
	static const char PLAYER_READY_IMAGE_FILE[]{ "Data/Image/Player-ready.png" };
	static const char PLAYER_RUNNING_IMAGE_FILE[]{ "Data/Image/Player-running.png" };
	static const char PLAYER_ERROR_IMAGE_FILE[]{ "Data/Image/Player-error.png" };
	static const char PLAYER_DOWN_IMAGE_FILE[]{ "Data/Image/Player-down.png" };
	static const float MOVE_SPEED{ 2 };
	static const float JUMP_HEIGHT{ 80 };
	static const float GRAVITY{ 0.05 };
	static const int IMAGE_WIDTH{ 80 };
	static const int IMAGE_HEIGHT{ 160 };
	//static const float ROBOT_STEP_TIME_SEC{ 0.07 / 3.0f };
	static const float ROBOT_STEP_TIME_SEC{ 0.07 };
	static const float ROBOT_BEAT_TIME_SEC{ 1 };
}

Player::Player(const Vector2& _position) :
	velocity_{},
	gravity{ GRAVITY },
	jumpHeight{ JUMP_HEIGHT },
	moveSpeed{ MOVE_SPEED },
	jumpV0{},
	robot_{ rect_, moveSpeed, jumpV0, pStage_, isGrounded, velocity_, prevPushedSpace, byteCode_ },
	hBeatTimer_{ 0 },
	isShockDown_{ false },
	pSrcCodeViewer_{ nullptr },
	pByteCodeViewer_{ nullptr },
	pMemoryViewer_{ nullptr },
	pCallStackViewer_{ nullptr },
	pStackMachineViewer_{ nullptr },
	pRegisterViewer_{ nullptr }
{
	jumpV0 = -std::sqrtf(2.0f * gravity * jumpHeight);

	rect_.pivot = _position + Vector2(0, -50);
	rect_.width = 70;
	rect_.height = 150;

	hStateImages[S_READY] = LoadGraph(PLAYER_READY_IMAGE_FILE);
	assert(hStateImages[S_READY] > 0
		&& "Player��Ready�C���[�W�ǂݍ��݂Ɏ��s @Player::Player");
	hStateImages[S_RUN] = LoadGraph(PLAYER_RUNNING_IMAGE_FILE);
	assert(hStateImages[S_RUN] > 0
		&& "Player��Run�C���[�W�ǂݍ��݂Ɏ��s @Player::Player");
	hStateImages[S_ERROR] = LoadGraph(PLAYER_ERROR_IMAGE_FILE);
	assert(hStateImages[S_ERROR] > 0
		&& "Player��Error�C���[�W�ǂݍ��݂Ɏ��s @Player::Player");
	hStateImages[S_DOWN] = LoadGraph(PLAYER_DOWN_IMAGE_FILE);
	assert(hStateImages[S_DOWN] > 0
		&& "Player��Down�C���[�W�ǂݍ��݂Ɏ��s @Player::Player");

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
				printfDx("�R�[�h�ɃG���[�����邽�߁A���s�ł��܂���B\n");
				GetScene<PlayScene>()->OpenSrcFile();
				break;
			case Player::S_DOWN:
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
		// �����̓����蔻��Ɩ��ߍ��ݔ����擾
		float push = pStage_->CheckBottom(rect_.pivot + Vector2
			{
				0,
				rect_.height + 1  // 1���ߍ���ł݂镪�̓�����ʒu��������
			});
		if (push > 0)
		{
			rect_.pivot.y -= push - 1;  // �����ĂP�߂荞��ł����Ԃɂ��Ă���
			velocity_.y = 0;
			isGrounded = true;
		}
		// �E���̓����蔻��Ɩ��ߍ��ݔ����擾
		push = pStage_->CheckBottom(rect_.pivot + Vector2{ rect_.width, rect_.height + 1 });
		if (push > 0)
		{
			rect_.pivot.y -= push - 1;  // �����ĂP�߂荞��ł����Ԃɂ��Ă���
			velocity_.y = 0;
			isGrounded = true;
		}
	}

	// ���{����
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
	//	// �E��̓����蔻��Ɩ��ߍ��ݔ����擾
	//	float push = pStage_->CheckRight(rect_.pivot + Vector2{ rect_.width, 0 });
	//	rect_.pivot.x -= push;
	//	// �E���̓����蔻��Ɩ��ߍ��ݔ����擾
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
		// ��̕ǂɓ�����Ȃ��悤�ɂ��鏈��
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

	// �X�N���[���̌v�Z
	Vector2 scroll{ pStage_->GetScroll() };
	float drawX{ rect_.pivot.x - scroll.x };

	static const int RIGHT_LIMIT{ 400 };
	if (rect_.pivot.x > RIGHT_LIMIT)  // �X�N���[�����鋫�E
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

	//Vector2 tilePosition
	//{
	//	pStage_->ToTilePosition(
	//		rect_.pivot + Vector2{ rect_.size.x / 2, rect_.size.y } + Vector2{ 0, 30.0f })
	//};

	//Rectan rectan{};
	////rectan.pivot = rect_.pivot + Vector2{ 0, rect_.size.y };
	//rectan.pivot = pStage_->ToWorldPosition(tilePosition);// rect_.pivot + Vector2{ 0, rect_.size.y };
	//rectan.size = { IMAGE_SIZE, IMAGE_SIZE };

	//Vector2 scroll{};
	//if (pStage_ != nullptr)
	//{
	//	scroll = pStage_->GetScroll();
	//}
	//Rectan drawRect{};
	//drawRect.x = rectan.x - scroll.x;
	//drawRect.y = rectan.y - scroll.y;
	//drawRect.size = rectan.size;

	//DrawBox(
	//	drawRect.pivot.x, drawRect.pivot.y,
	//	drawRect.pivot.x + drawRect.size.x, drawRect.pivot.y + drawRect.size.y,
	//	0xff0000, FALSE);
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
	printfDx("%s\n", _message.c_str());
}

void Player::SetByteCode(const ByteCodes& _byteCode)
{
	clsDx();
	robot_.Reset();
	if (currentState_ == S_ERROR)
	{
		SetState(S_READY);
	}

	byteCodeAndLines_ = _byteCode;
	byteCode_.clear();
	std::stringstream ss{};
	std::vector<std::string> textLines{};
	for (auto&& byteCodeAndLine : byteCodeAndLines_)
	{
		byteCode_.push_back(byteCodeAndLine.second);
		ss << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << static_cast<int>(byteCodeAndLine.second);
		textLines.push_back(ss.str());
		ss.str("");
	}

	// �o�C�g�R�[�h�e�L�X�g�Z�b�g
	pByteCodeViewer_->SetTextLines(textLines);
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

void Player::ShockDown()
{
	SetState(S_DOWN);
}

void Player::SetViewerBoxes(
	ViewerBox* _pSrcCodeViewer,
	ViewerBox* _pByteCodeViewer,
	ViewerBox* _pMemoryViewer,
	ViewerBox* _pCallStackViewer,
	ViewerBox* _pStackMachineViewer,
	ViewerBox* _pRegisterViewer)
{
	pSrcCodeViewer_ = _pSrcCodeViewer;
	pByteCodeViewer_ = _pByteCodeViewer;
	pMemoryViewer_ = _pMemoryViewer;
	pCallStackViewer_ = _pCallStackViewer;
	pStackMachineViewer_ = _pStackMachineViewer;
	pRegisterViewer_ = _pRegisterViewer;
}

void Player::SetState(const State _state)
{
	assert(0 <= _state && _state < S_MAX
		&& "�͈͊O�̃X�e�[�^�X�̓Z�b�g�ł��܂���B @Player::SetState");

	hImage_ = hStateImages[static_cast<int>(_state)];
	currentState_ = _state;
	if (pSrcCodeViewer_ != nullptr)
	{
		pSrcCodeViewer_->ClearMarks();
	}

	// �X�e�[�g�ʏ������点��
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
				robot_.GetMemoryRef([&, this](
					const ByteCodeReader& _codeReader,
					const std::vector<Byte>& _memory,
					const Stack<int>& _stackMachine,
					const Stack<int>& _callStack,
					const std::vector<Byte>& _register)
					{
						// ���Ă��� �\�[�X�R�[�h && �o�C�g�R�[�h �s������
						int index{ static_cast<int>(_codeReader.GetCurrentIndex()) };
						if (index < byteCodeAndLines_.size())
						{
							pByteCodeViewer_->ClearMarks();
							pByteCodeViewer_->AddMarkLine(index, 0xff0000, 0xffffff);
							pByteCodeViewer_->ReadLine(index);
							

							int readLine{ byteCodeAndLines_[index].first.line };  // ���Ă���Line
							if (readLine > -1)
							{
								pSrcCodeViewer_->ClearMarks();
								pSrcCodeViewer_->AddMarkLine(readLine, 0xff0000, 0xffffff);
								pSrcCodeViewer_->ReadLine(readLine);
							}
						}


						std::vector<std::string> textLines{};
						std::stringstream ss{};

						if (hasByteCodeViewer_)
						{
							pByteCodeViewer_->SetIsShow(true);
						}
						else
						{
							pByteCodeViewer_->SetIsShow(false);
						}
						if (hasMemoryViewer_)
						{
							ss.str("");
							textLines.clear();

							for (int i = 0; i < _memory.size(); i++)
							{
								ss << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << static_cast<int>(_memory[i]);
								if (((1 + i) % 8 == 0) || i == _memory.size() - 1)
								{
									textLines.push_back(ss.str());
									ss.str("");
								}
								else
								{
									ss << " ";
								}
							}
							pMemoryViewer_->SetTextLines(textLines);
							pMemoryViewer_->SetIsShow(true);
						}
						else
						{
							pMemoryViewer_->SetIsShow(false);
						}
						if (hasCallStackViewer_)
						{
							pCallStackViewer_->SetIsShow(true);
						}
						else
						{
							pCallStackViewer_->SetIsShow(false);
						}
						if (hasRegisterViewer_)
						{
							pRegisterViewer_->SetIsShow(true);
						}
						else
						{
							pRegisterViewer_->SetIsShow(false);
						}
						if (hasStackMachineViewer_)
						{
							pStackMachineViewer_->SetIsShow(true);
						}
						else
						{
							pStackMachineViewer_->SetIsShow(false);
						}
						
					});
			});
		break;
	case S_ERROR:
		prevIsSucceedTryRead = false;
		sleepCount_ = 0;
		robot_.Reset();

		Timer::Remove(hBeatTimer_);
		hBeatTimer_ = nullptr;
		break;
	case S_DOWN:
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
