#pragma once
#include <list>

template<typename ValueT>
class Stack
{
public:
	Stack();
	~Stack();

	void Clear();

	ValueT Pop();
	void Push(const ValueT _byte);

	inline auto begin()       { return listCollection_.begin(); }
	inline auto end()         { return listCollection_.end(); }
	inline auto begin() const { return listCollection_.begin(); }
	inline auto end() const   { return listCollection_.end(); }
private:
	std::list<ValueT> listCollection_;
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
	ValueT byte = *(listCollection_.end());
	listCollection_.erase(listCollection_.end());
	
	return byte;
}

template<typename ValueT>
inline void Stack<ValueT>::Push(const ValueT _value)
{
	listCollection_.push_back(_value);
}
