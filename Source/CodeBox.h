#pragma once
#include "Object2D.h"
#include <vector>
#include <string>


class CodeBox : public Object2D
{
public:
	CodeBox();
	~CodeBox();

	void Update() override;
	void Draw() override;

	void SetSourceLines(
		const std::vector<std::string>& _lines);

	int GetBoxWidth() const;
	int GetBoxHeight() const;
private:
	std::vector<std::string> sourceLines_;
	int sourceMaxLength_;  // TODO: �����ő啝�Ŏ��s������A�ő�̍s�ԍ���ۑ�����A�������畝�v�Z
};
