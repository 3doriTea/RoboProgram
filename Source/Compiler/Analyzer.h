#pragma once
#include <vector>
#include <string>
#include <functional>


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

// ���[�h�̌^
enum NodeType
{
	NODE_PROC,  // ����
	NODE_EXPR,  // ��
	NODE_OR,  // ||
	NODE_AND,  // &&
	NODE_EQUAL,  // ==
	NODE_NOTEQUAL,  // !=
	NODE_LESSTHAN,  // <
	NODE_GREATERTHEN,  // >
	NODE_LESSEQUAL,  // <=
	NODE_GREATEREQUAL,  // >=
	NODE_ADD,  // +
	NODE_SUB,  // -
	NODE_MUL,  // *
	NODE_DIV,  // /
	NODE_NFOR,  // for��
	NODE_NIF,   // if��
	NODE_VARDEC,  // �ϐ��錾
	NODE_FUNCDEC,  // �֐��錾
	NODE_VALUE,  // �l
	NODE_INTEGER,  // �����l

	NODE_LITER_DIGIT,  // �������e����

	NODE_CALLFUNC,  // �֐��Ăяo��
	NODE_INCREMENT,  // ++
	NODE_DECREMENT,  // --

	NODE_NAME,  // ��
	NODE_TYPE,  // �^

	NODE_RET,  // return

	NODE_ARG,  // ������

	NODE_ASSIGN,  // �����

	NODE_PARAM,  // ������

	//NODE_NUMBER,             // ����
	NODE_REGISTER_FUNC_NAME, // �o�^����Ă���֐�
	NODE_REGISTER_VAR_NAME,  // (�o�^����Ă���ϐ���)
	//NODE_BOOL,               // true | false
	//NODE_REGISTER_TYPE_NAME, // (�o�^����Ă���^��)
};

// �m�[�h
struct NODE
{  // total 32byte
	int tokenIndex_;  // �g�[�N���̃C���f�b�N�X
	NodeType type_;  // �m�[�h�̎��
	
	union  // 32byte
	{
		union  // 32byte
		{
			struct  // �֐��錾
			{
				NODE* type;
				NODE* name;
				NODE* param;
				NODE* proc;  // ����
			} fancDec;

			struct  // ��������
			{
				NODE* init;  // ����������
				NODE* expr;  // �p������
				NODE* updt;  // �X�V����
				NODE* proc;  // ����
			} nfor;
		};

		union  // 24byte
		{
			struct  // �����
			{
				NODE* name;
				NODE* expr;
				NODE* next;
			} assigns;
		};

		union  // 16byte
		{
			struct  // ��
			{
				NODE* ls;  // left side
				NODE* rs;  // right side
			} expr;

			struct  // �ϐ��錾
			{
				NODE* type;
				NODE* assigns;
			} varDec;

			struct  // �֐��ďo��
			{
				NODE* name;
				NODE* args;
			} callFunc;

			struct  // ����
			{
				NODE* proc;
				NODE* next;
			} proc;

			struct  // ������
			{
				NODE* expr;
				NODE* next;
			} arg;

			struct  // ������
			{
				NODE* varDec;
				NODE* next;
			} param;

			struct  // ��������
			{
				NODE* expr;
				NODE* proc;
			} nif;
		};

		union  // 8byte
		{
			struct  // �ԋp��
			{
				NODE* expr;
			} ret;

			struct  // �ϐ�
			{
				NODE* name;
			} var;

			struct  // ���e����
			{
				NODE* liter;
			} liter;
		};
	};
};

// �\���m�[�h
using Nodes = std::vector<NODE>;

// �o�C�g�R�[�h
using ByteCode = std::pair<SOURCE_POS, Byte>;
using ByteCodes = std::vector<ByteCode>;

/// <summary>
/// ��͋@
/// </summary>
/// <typeparam name="SubAnalyzerT">�h����������̓N���X�̌^</typeparam>
/// <typeparam name="InT">��͂���I�u�W�F�N�g�̌^</typeparam>
/// <typeparam name="OutT">���ʕ��Ƃ����I�u�W�F�N�g�̌^</typeparam>
/// <typeparam name="ErrorArgT">�G���[���̈���</typeparam>
template<typename SubAnalyzerT, typename InT, typename OutT, typename ErrorArgT>
class Analyzer
{
public:
	Analyzer(const InT& _inRef, OutT& out_);
	virtual ~Analyzer();

	/// <summary>
	/// ��͂���
	/// </summary>
	/// <param name="_outRef">���ʕ�������Q�ƃ|�C���^</param>
	virtual void Analyze() = 0;

	inline SubAnalyzerT& OnError(const std::function<void(const char* _message, const ErrorArgT& _arg)>& _onError)
	{
		onError_ = _onError;
		return Get();
	}

protected:
	virtual SubAnalyzerT& Get() = 0;

	/// <summary>
	/// �G���[����
	/// </summary>
	/// <param name="_message">�G���[���b�Z�[�W</param>
	/// <param name="_arg">�G���[����</param>
	inline void ErrorFull(const char* _message, const ErrorArgT& _arg)
	{
		onError_(_message, _arg);
	}

	/// <summary>
	/// �G���[����(�ȈՔł�h����ŗp�ӂ�����)
	/// </summary>
	/// <param name="_message">�G���[���b�Z�[�W</param>
	inline virtual void Error(const char* _message) {};

protected:
	const InT& in_;
	OutT& out_;

private:
	std::function<void(const char* _message, const ErrorArgT& _arg)> onError_;
};

template<typename SubAnalyzerT, typename InT, typename OutT, typename ErrorArgT>
inline Analyzer<SubAnalyzerT, InT, OutT, ErrorArgT>::Analyzer(const InT& _inRef, OutT& _outRef) :
	in_{ _inRef },
	out_{ _outRef }
{
}

template<typename SubAnalyzerT, typename InT, typename OutT, typename ErrorArgT>
inline Analyzer<SubAnalyzerT, InT, OutT, ErrorArgT>::~Analyzer()
{
}
