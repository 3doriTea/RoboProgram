#pragma once


/// <summary>
/// ��͋@
/// </summary>
/// <typeparam name="InT">��͂���I�u�W�F�N�g�̌^</typeparam>
/// <typeparam name="OutT">���ʕ��Ƃ����I�u�W�F�N�g�̌^</typeparam>
template<typename InT, typename OutT>
class Analyzer
{
public:
	Analyzer(InT& _inRef);
	virtual ~Analyzer();

	/// <summary>
	/// ��͂���
	/// </summary>
	/// <param name="_outRef">���ʕ�������Q�ƃ|�C���^</param>
	virtual void Analyze(OutT& _outRef) = 0;

protected:
	InT& in_;
};

template<typename InT, typename OutT>
inline Analyzer<InT, OutT>::Analyzer(InT& _inRef) :
	in_{ _inRef }
{
}

template<typename InT, typename OutT>
inline Analyzer<InT, OutT>::~Analyzer()
{
}
