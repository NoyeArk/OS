#pragma once
#include "define.h"

// 定义三地址码输出类型
#define EXP 0
#define GOTO 1
#define IF 2
#define ASSIGN 3
#define BUFFER 4  // 输出缓冲队列中的中间代码

static std::ofstream CodeOutFile("../code.txt", std::ios::out);
static std::ofstream SemanticOutFile("../output.txt", std::ios::out);
static int LineNum = 0;
static std::vector<std::string> CodeQueue;  // 等待输出的队列

struct CompUnitNode;
struct DeclNode;
struct FuncDefNode;
struct MainFuncDefNode;
struct ConstDeclNode;
struct VarDeclNode;
struct FuncTypeNode;
struct IdentNode;
struct FuncFParamsNode;
struct BlockNode;
struct BTypeNode;
struct VarDefNode;
struct FuncFParamNode;
struct BlockItemNode;
struct ArrayNode;
struct ConstExpNode;
struct InitValNode;
struct StmtNode;
struct AddExpNode;
struct AddExp_Node;
struct ExpNode;
struct LValNode;
struct CondNode;
struct MulExpNode;
struct MulExp_Node;
struct LOrExpNode;
struct LOrExp_Node;
struct LAndExpNode;
struct LAndExp_Node;
struct EqExpNode;
struct EqExp_Node;
struct UnaryExpNode;
struct RelExpNode;
struct RelExp_Node;
struct CharNode;
struct FormatCharNode;
struct NormalCharNode;
struct PrimaryExpNode;
struct FuncRParamsNode;
struct NumberNode;
struct IntConstNode;
struct DecimalConstNode;
struct NonzeroDigitNode;
struct UnaryOpNode;
struct ConstInitValNode;
struct ConstDefNode;

// 用来表示变量的类型
typedef enum {
	emTypeNull,
	Int,
	Float
}EmType;

static int TempNum = 0;
static int IfNum = 0;
static EmType t = emTypeNull;  // 全局变量变量类型
static int w = 0;  // 全局变量变量长度
static int offset = 0;

void gen(const bool& isOut,  // 判断是否进行输出
		 const int& codeType, 
		 const int& op1, 
		 const std::string& opType = "NULL", 
		 const int& op2 = -1, 
		 const std::string& tar="NULL");

// 此处op2的默认为了匹配x = 0的情况
void gen(const bool& isOut,
		 const int& codeType, 
		 const std::string& op1, 
		 const std::string& opType, 
		 const std::string& op2="NULL",
	 	 const std::string& tar = "NULL");
void flushCodeBuffer(int _true, int _false, int ifNum);
int label();
void setLineNum(const int& num);
void rrGoto(const int& line1, const int& line2=-1);

struct BaseNode {
public:
	std::string name;  // 节点名称
	std::shared_ptr<BaseNode> next;

  	BaseNode(std::string name) : name(name)
	{}

};

// 1 CompUnit
struct CompUnitNode : public BaseNode {
	using BaseNode::BaseNode;

	std::vector<std::shared_ptr<DeclNode>> decl;
	std::vector<std::shared_ptr<FuncDefNode>> func_def;
	std::shared_ptr<MainFuncDefNode> main_func_def;
};
// 2 Decl
struct DeclNode : public BaseNode {
	using BaseNode::BaseNode;
	std::shared_ptr<ConstDeclNode> const_decl;
	std::shared_ptr<VarDeclNode> var_decl;
};
// 3 FuncDef
struct FuncDefNode : public BaseNode {
	using BaseNode::BaseNode;
	std::shared_ptr<FuncTypeNode> func_type;
	std::shared_ptr<IdentNode> ident;
	std::shared_ptr<FuncFParamsNode> func_f_params;
	std::shared_ptr<BlockNode> block;
};
// 4 MainFuncDef
struct MainFuncDefNode : public BaseNode {
	using BaseNode::BaseNode;
	std::shared_ptr<BlockNode> block;
};
// 5 ConstDecl
struct ConstDeclNode : public BaseNode {
	using BaseNode::BaseNode;
	std::shared_ptr<VarDeclNode> var_decl;
};
// 6 VarDecl
struct VarDeclNode : public BaseNode {
	using BaseNode::BaseNode;
	std::shared_ptr<BTypeNode> btype;
	std::vector<std::shared_ptr<VarDefNode>> vardef;
};

// 7 FuncType
struct FuncTypeNode : public BaseNode {
	using BaseNode::BaseNode;
	// void | int | float
	std::string funcType;
};
// 8 Ident
struct IdentNode : public BaseNode {
	using BaseNode::BaseNode;
	std::string lexeme = "error";  // 词法分析结果
};
// 9 FuncFParams
struct FuncFParamsNode : public BaseNode {
	using BaseNode::BaseNode;
	std::vector<std::shared_ptr<FuncFParamNode>> func_f_param;
};
// 10 Block
struct BlockNode : public BaseNode {
	using BaseNode::BaseNode;
	std::vector<std::shared_ptr<BlockItemNode>> block_item;

	int retVal;  // 记录函数返回值
};
// 11 BType
struct BTypeNode : public BaseNode {
	using BaseNode::BaseNode;
	// int | float
	EmType emType = emTypeNull;
	int width = 0;
};
// 12 VarDef
struct VarDefNode : public BaseNode {
	using BaseNode::BaseNode;
	std::shared_ptr<IdentNode> ident;  // 变量名
	std::shared_ptr<ArrayNode> array;  // 变量类型
	std::shared_ptr<InitValNode> initval;  // 变量类型
};
// 12.5-多维数组类型
struct ArrayNode : public BaseNode {
	using BaseNode::BaseNode;
	std::vector<int> dimension;  // int-维数大小  EmType-维数类型
	int width = 0;

	std::shared_ptr<ConstExpNode> const_exp;
	std::shared_ptr<ArrayNode> array;

	std::shared_ptr<InitValNode> initval;
};
// 13 FuncFParam
struct FuncFParamNode : public BaseNode {
	using BaseNode::BaseNode;
	std::shared_ptr<BTypeNode> btype;
	std::shared_ptr<IdentNode> ident;
	std::vector<std::shared_ptr<ConstExpNode>> const_exp;
};
// 14 BlockItem
struct BlockItemNode : public BaseNode {
	using BaseNode::BaseNode;
	std::shared_ptr<DeclNode> decl;
	std::shared_ptr<StmtNode> stmt;
	std::vector<std::shared_ptr<ConstExpNode>> const_exp;

	int retVal;  // 目前用来记录函数返回值
};
// 15 ConstExp
struct ConstExpNode : public BaseNode {
	using BaseNode::BaseNode;
	int val = 0;

	std::shared_ptr<AddExpNode> add_exp;
};
// 16 InitVal
struct InitValNode : public BaseNode {
	using BaseNode::BaseNode;
	std::shared_ptr<ExpNode> exp;
	std::vector<std::shared_ptr<InitValNode>> init_val;
};
// 17 Stmt
struct StmtNode : public BaseNode {
	using BaseNode::BaseNode;
	std::shared_ptr<LValNode> lval;
	std::shared_ptr<BlockNode> block;
	std::shared_ptr<StmtNode> stmt;
	std::shared_ptr<StmtNode> stmt1;
	std::shared_ptr<StmtNode> stmt2;
	std::shared_ptr<ExpNode> exp;
	std::shared_ptr<CondNode> cond;
	std::shared_ptr<FormatCharNode> format_char;

	std::vector<std::shared_ptr<ExpNode>> printf_exp;

	int next = -1;  // if使用
	int begin;  // while使用
	int retVal;  // 函数return返回值
	std::string lexeme;
};
// 18 AddExp
struct AddExpNode : public BaseNode {
	using BaseNode::BaseNode;
	std::shared_ptr<MulExpNode> mul_exp;
	std::shared_ptr<AddExp_Node> _add_exp;

	int val;  // 对应的值
	std::string temp_name;
};
struct AddExp_Node : public BaseNode {
	using BaseNode::BaseNode;
	std::shared_ptr<MulExpNode> mul_exp;
	std::shared_ptr<AddExp_Node> _add_exp;
	
	int val = 0;  // 对应的值
	std::string op_type;  // 操作符类型
	std::string temp_name;
};
// 19 Exp
struct ExpNode : public BaseNode {
	using BaseNode::BaseNode;
	std::shared_ptr<AddExpNode> add_exp;

	int val;
	std::string lexeme;
};
// 20 LVal
struct LValNode : public BaseNode {
	using BaseNode::BaseNode;
	std::shared_ptr<IdentNode> ident;
	std::shared_ptr<ExpNode> exp;

	std::string lexeme;  // 对应标识符的名字
	int val;  // 标识符对应的值
};
// 21 Cond
struct CondNode : public BaseNode {
	using BaseNode::BaseNode;
	std::shared_ptr<LOrExpNode> lor_exp;

	int _true;  // 对应的是行数
	int _false;
	bool cond;
};
// 22 FormatString
struct FormatStringNode : public BaseNode {
	using BaseNode::BaseNode;

};
// 23 MulExp
struct MulExpNode : public BaseNode {
	using BaseNode::BaseNode;
	std::shared_ptr<UnaryExpNode> unary_exp;
	std::shared_ptr<MulExp_Node> _mul_exp;

	int val = 0;
	std::string temp_name;
};
struct MulExp_Node : public BaseNode {
	using BaseNode::BaseNode;
	std::shared_ptr<UnaryExpNode> unary_exp;
	std::shared_ptr<MulExp_Node> _mul_exp;

	int val;
	std::string op_type;
	std::string temp_name;
};
// 24 LOrExp
struct LOrExpNode : public BaseNode {
	using BaseNode::BaseNode;
	std::shared_ptr<LAndExpNode> land_exp;
	std::shared_ptr<LOrExp_Node> _lor_exp;

	bool cond;
};
struct LOrExp_Node : public BaseNode {
	using BaseNode::BaseNode;
	std::shared_ptr<LAndExpNode> land_exp;
	std::shared_ptr<LOrExp_Node> _lor_exp;

	std::string op_type;
	bool cond;
};
// 25 LAndExp
struct LAndExpNode : public BaseNode {
	using BaseNode::BaseNode;
	std::shared_ptr<EqExpNode> eq_exp;
	std::shared_ptr<LAndExp_Node> _land_exp;

	bool cond;
};
struct LAndExp_Node : public BaseNode {
	using BaseNode::BaseNode;
	std::shared_ptr<EqExpNode> eq_exp;
	std::shared_ptr<LAndExp_Node> _land_exp;

	std::string op_type;
	bool cond;
};
// 26 EqExp
struct EqExpNode : public BaseNode {
	using BaseNode::BaseNode;
	std::shared_ptr<RelExpNode> rel_exp;
	std::shared_ptr<EqExp_Node> _eq_exp;

	bool cond;
};
struct EqExp_Node : public BaseNode {
	using BaseNode::BaseNode;
	std::shared_ptr<RelExpNode> rel_exp;
	std::shared_ptr<EqExp_Node> _eq_exp;

	std::string op_type;  // == | !=
};
// 27 UnaryExp
struct UnaryExpNode : public BaseNode {
	using BaseNode::BaseNode;
	std::shared_ptr<PrimaryExpNode> primary_exp;
	std::shared_ptr<IdentNode> ident;
	std::shared_ptr<FuncRParamsNode> func_r_params;
	std::shared_ptr<UnaryOpNode> unary_op;
	std::shared_ptr<UnaryExpNode> unary_exp;

	int val;
	std::string lexeme;
};
// 28 RelExp
struct RelExpNode : public BaseNode {
	using BaseNode::BaseNode;
	std::shared_ptr<AddExpNode> add_exp;
	std::shared_ptr<RelExp_Node> _rel_exp;
	
	bool cond;  // 用来表示该条件表达式的值 true | false
	int val;
};
struct RelExp_Node : public BaseNode {
	using BaseNode::BaseNode;
	std::shared_ptr<AddExpNode> add_exp;
	std::shared_ptr<RelExp_Node> _rel_exp;

	std::string op_type;  // < > <= >=
	bool cond;
};
// 29 Char
struct CharNode : public BaseNode {
	using BaseNode::BaseNode;
	std::shared_ptr<FormatCharNode> format_char;
	std::shared_ptr<NormalCharNode> normal_char;
};
// 30 FormatChar
struct FormatCharNode : public BaseNode {
	using BaseNode::BaseNode;

	int d_num = 0;
	std::vector<int> print_exp;  // 匹配%d
	std::string out_string;  // printf中的内容
};
// 31 NormalChar
struct NormalCharNode : public BaseNode {
	using BaseNode::BaseNode;

};
// 32 PrimaryExp
struct PrimaryExpNode : public BaseNode {
	using BaseNode::BaseNode;
	std::shared_ptr<ExpNode> exp;
	std::shared_ptr<LValNode> lval;
	std::shared_ptr<NumberNode> number;

	int val = 0;
	std::string lexeme;  // 变量名
};
// 33 FuncRParams
struct FuncRParamsNode : public BaseNode {
	using BaseNode::BaseNode;

	std::vector<std::shared_ptr<ExpNode>> exp;
};
// 34 Number
struct NumberNode : public BaseNode {
	using BaseNode::BaseNode;
	std::shared_ptr<IntConstNode> int_const;

	int val = 0;
};
// 35 IntConst
struct IntConstNode : public BaseNode {
	using BaseNode::BaseNode;
	std::shared_ptr<DecimalConstNode> decimal_const;

	int val = 0;
};
// 36 decimal_const
struct DecimalConstNode : public BaseNode {
	using BaseNode::BaseNode;
	std::shared_ptr<NonzeroDigitNode> non_zero_digit;

	int val = 0;
};
// 37 decimal_const
struct NonzeroDigitNode : public BaseNode {
	using BaseNode::BaseNode;

};
// 38 UnaryOp
struct UnaryOpNode : public BaseNode {
	using BaseNode::BaseNode;

};
// 39 ConstInitVal
struct ConstInitValNode : public BaseNode {
	using BaseNode::BaseNode;

};
// 40 ConstDef
struct ConstDefNode : public BaseNode {
	using BaseNode::BaseNode;

};


