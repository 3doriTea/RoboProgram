#pragma once


/// <summary>
/// 解析機
/// </summary>
/// <typeparam name="InT">解析するオブジェクトの型</typeparam>
/// <typeparam name="OutT">成果物とされるオブジェクトの型</typeparam>
template<typename InT, typename OutT>
class Analyzer
{
public:
	Analyzer(const InT& _inRef);
	virtual ~Analyzer();

	/// <summary>
	/// 解析する
	/// </summary>
	/// <param name="_outRef">成果物が入る参照ポインタ</param>
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
