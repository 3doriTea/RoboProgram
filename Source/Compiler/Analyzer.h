#pragma once
#include <vector>
#include <string>

using Byte = unsigned char;

using LineCount = int;
using LineString = std::string;

using ColumnCount = int;

/// <summary>
/// �\�[�X�R�[�h�̍��W���
/// </summary>
struct SOURCE_POS
{
	LineCount line;
	ColumnCount column;
};

// �\�[�X�R�[�h��1�s��
using SourceLine = std::pair<LineCount, LineString>;
using SourceLines = std::vector<SourceLine>;

using TokenString = std::string;

// �g�[�N��
using Token = std::pair<SOURCE_POS, TokenString>;
using Tokens = std::vector<Token>;

// �o�C�g�R�[�h
using ByteCode = std::pair<SOURCE_POS, Byte>;
using ByteCodes = std::vector<ByteCode>;

/// <summary>
/// ��͋@
/// </summary>
/// <typeparam name="InT">��͂���I�u�W�F�N�g�̌^</typeparam>
/// <typeparam name="OutT">���ʕ��Ƃ����I�u�W�F�N�g�̌^</typeparam>
template<typename InT, typename OutT>
class Analyzer
{
public:
	Analyzer(const InT& _inRef);
	virtual ~Analyzer();

	/// <summary>
	/// ��͂���
	/// </summary>
	/// <param name="_outRef">���ʕ�������Q�ƃ|�C���^</param>
	virtual void Analyze(OutT& _outRef) = 0;

protected:
	const InT& in_;
};

template<typename InT, typename OutT>
inline Analyzer<InT, OutT>::Analyzer(const InT& _inRef) :
	in_{ _inRef }
{
}

template<typename InT, typename OutT>
inline Analyzer<InT, OutT>::~Analyzer()
{
}
