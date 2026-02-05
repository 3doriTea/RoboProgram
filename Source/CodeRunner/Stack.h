#pragma once
#include <list>


/// <summary>
/// <para>オリジナルのスタックコレクションクラス</para>
/// <para>Popと取得が同時にできる</para>
/// </summary>
/// <typeparam name="ValueT">スタックに入れる値の型</typeparam>
template<typename ValueT>
class Stack
{
public:
	Stack();
	~Stack();

	/// <summary>
	/// スタックをクリアする
	/// </summary>
	void Clear();

	/// <summary>
	/// スタックからポップする
	/// </summary>
	/// <returns>ポップされた値</returns>
	ValueT Pop();

	/// <summary>
	/// スタックに値をプッシュする
	/// </summary>
	/// <param name="_byte">プッシュする値</param>
	void Push(const ValueT _byte);

	/// <summary>
	/// 先頭イテレータ
	/// </summary>
	/// <returns>普通のイテレータ</returns>
	inline auto begin()       { return listCollection_.begin(); }
	/// <summary>
	/// 番兵イテレータ
	/// </summary>
	/// <returns>普通のイテレータ</returns>
	inline auto end()         { return listCollection_.end(); }
	/// <summary>
	/// 先頭コンストイテレータ
	/// </summary>
	/// <returns>コンストイテレータ</returns>
	inline auto begin() const { return listCollection_.cbegin(); }
	/// <summary>
	/// 番兵コンストイテレータ
	/// </summary>
	/// <returns>コンストイテレータ</returns>
	inline auto end() const   { return listCollection_.cend(); }

private:
	std::list<ValueT> listCollection_;  // スタックの中身
};

template<typename ValueT>
inline Stack<ValueT>::Stack()
{
}

template<typename ValueT>
inline Stack<ValueT>::~Stack()
{
}

template<typename ValueT>
inline void Stack<ValueT>::Clear()
{
	listCollection_.clear();
}

template<typename ValueT>
inline ValueT Stack<ValueT>::Pop()
{
	ValueT byte = (*listCollection_.begin());
	listCollection_.erase(listCollection_.begin());
	
	return byte;
}

template<typename ValueT>
inline void Stack<ValueT>::Push(const ValueT _value)
{
	listCollection_.push_front(_value);
}
