#pragma once

#include "define.h"
#include "utils.h"
#include "table.h"
#include "errorHandle.h"
#include "ast.h"

typedef enum {
	emNull = 10,
	emInLoop,   // 循环体
	emInFunc,   // 函数定义
	emFuncUse,  // 函数调用
	emMain      // main函数
}EmFlagIdent;

struct {
	bool MainFlag;
}flags;

static bool haveReturn = false;

static std::vector<EmFlagIdent> flagStack;
static EmFlagIdent currentEmFlag = emNull;

static size_t currentUnaryExpCount = 0;
static size_t preUnaryExpCount = 0;
static std::stack<std::string> bracketNameStack;

static std::queue<int> InputData;
static std::vector<int> _lineNumberVector;  // 源文件中每一个token对应的行号
static std::vector<std::string> _tokenVector;  // 存储源文件中每一个token
static std::vector<std::string> _tokenNameVector;  // 存储每一个token对应的种别码

static size_t readIndex;
static std::string globalCurrentToken;
static std::string globalCurrentTokenName;

inline void printLookHead();
inline std::string getNextSecondToken();
inline std::string getNextNumToken(const size_t& num);
void getNextToken();
bool isHaveWantInStmt(const std::string& wantStr);
inline void outFileParserResult(const std::string& str);
inline void outFileSemanticResult(std::string& str, const std::vector<int>& val = {});
void init(std::vector<std::string>&, std::vector<int>&);
void end();
void parsering(std::vector<std::string>&, std::vector<int>&);

inline void insertFlag(const EmFlagIdent& flag) {
	if (currentEmFlag != emNull)
		flagStack.push_back(currentEmFlag);
	currentEmFlag = flag;
}

inline void eraseFlag() {
	if (flagStack.empty())
		currentEmFlag = emNull;
	else {
		currentEmFlag = flagStack.back();
		flagStack.pop_back();
	}
}

inline bool isInfunction() {
	for (const EmFlagIdent flag : flagStack) {
		if (flag == emInFunc)
			return true;
	}
	return false;
}

// 匹配终结符int
inline EmType matchTokenInt() {
	if (globalCurrentTokenName == "INTTK") {
		getNextToken();
		return Int;
	}
	else { // 错误处理
		return emTypeNull;
	}
}
// 匹配终结符main
inline void matchTokenMain() {
	if (globalCurrentTokenName == "MAINTK") {
		getNextToken();
	}
	else { // 错误处理

	}
}
// 匹配终结符break
inline void matchTokenBreak() {
	if (globalCurrentTokenName == "BREAKTK" && currentEmFlag == emInLoop) {
		getNextToken();
	}
	else { // 在非循环中使用break
		errorHandle(M, _lineNumberVector[readIndex]);
		getNextToken();
	}
}
// 匹配终结符continue
inline void matchTokenContinue() {
	if (globalCurrentTokenName == "CONTINUETK" && currentEmFlag == emInLoop) {
		getNextToken();
	}
	else { // 在非循环中使用continue
		errorHandle(M, _lineNumberVector[readIndex]);
		getNextToken();
	}
}
// 匹配终结符if
inline void matchTokenIf() {
	if (globalCurrentTokenName == "IFTK") {
		getNextToken();
	}
	else { // 错误处理

	}
}
// 匹配终结符else
inline void matchTokenElse() {
	if (globalCurrentTokenName == "ELSETK") {
		getNextToken();
	}
	else { // 错误处理

	}
}
// 匹配终结符while
inline void matchTokenWhile() {
	if (globalCurrentTokenName == "WHILETK") {
		getNextToken();
	}
	else { // 错误处理

	}
}
// 匹配终结符Ident变量名
inline void matchTokenIdent(const bool& isDecl = false) {

	if (globalCurrentToken == "abcde")
		printf("");

	if (!isDecl) { // 不是声明，是引用
		bool isFuncParam = false;

		// 如果在函数声明中，先判断函数的实参
		if (currentEmFlag == emInFunc || isInfunction()) {
			if (TableSpace::isfuncParamExist(globalCurrentToken)) { // 可以匹配
				isFuncParam = true; // 是函数形参
			}
		}

		if (!isFuncParam) { // 如果不是函数形参
			TableSpace::TableItem outcome = TableSpace::query(globalCurrentToken);
			if (outcome.kind == TableSpace::null) {
				errorHandle(C, _lineNumberVector[readIndex]);
				getNextToken();
				return;
			}
		}
	}

	if ((globalCurrentToken[0] >= 'a' && globalCurrentToken[0] <= 'z') // 判断合法变量名
		|| (globalCurrentToken[0] >= 'A' && globalCurrentToken[0] <= 'Z')) {
		if (globalCurrentToken == "change1" && readIndex == 28)
			printf("");
		getNextToken();
	}
}
// 匹配终结符getint
inline void matchTokenGetint() {
	if (globalCurrentTokenName == "GETINTTK") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: getint" << std::endl;
	}
}
// 匹配终结符printf
inline void matchTokenPrintf() {
	if (globalCurrentTokenName == "PRINTFTK") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: printf" << std::endl;
	}
}
// 匹配终结符return
inline void matchTokenReturn() {
	if (globalCurrentTokenName == "RETURNTK") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: return" << std::endl;
	}
}
// 匹配终结符void
inline void matchTokenVoid() {
	if (globalCurrentTokenName == "VOIDTK") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: void" << std::endl;
	}
}
// 匹配终结符format
inline void matchTokenFormat() {
	size_t found1 = globalCurrentToken.find("$");
	size_t found2 = globalCurrentToken.find("&");
	if (found1 == std::string::npos && found2 == std::string::npos) {
		getNextToken();
	}
	else { // 含有非法字符
		errorHandle(A, _lineNumberVector[readIndex]);
		getNextToken();
	}
}
// const
inline void matchTokenConst() {
	if (globalCurrentTokenName == "CONSTTK") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: const" << std::endl;
	}
}


// 匹配终结符,
inline void matchTokenComma() {
	if (globalCurrentTokenName == "COMMA") {
		getNextToken();
	}
	else { // 错误处理
		printLookHead();
		std::cout << "error: ," << std::endl;
	}
}
// 匹配终结符;
inline void matchTokenSemicn(const emErrorType& errorType = Null) {
	if (globalCurrentTokenName == "SEMICN") {
		getNextToken();
	}
	else { // 缺少分号
		errorHandle(errorType, _lineNumberVector[readIndex - 1]); // 报错行号为分号前一个非终结符所在行号。
	}
}


// 匹配终结符=
inline void matchTokenAssign() {
	if (globalCurrentTokenName == "ASSIGN") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "readIndex:" << readIndex << std::endl;
		std::cout << "error: =" << std::endl;
	}
}
// 匹配终结符<
inline void matchTokenLss() {
	if (globalCurrentTokenName == "LSS") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: <" << std::endl;
		printLookHead();
	}
}
// 匹配终结符<=
inline void matchTokenLeq() {
	if (globalCurrentTokenName == "LEQ") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: <=" << std::endl;
	}
}
// 匹配终结符>
inline void matchTokenGre() {
	if (globalCurrentTokenName == "GRE") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: >" << std::endl;
	}
}
// 匹配终结符>=
inline void matchTokenGeq() {
	if (globalCurrentTokenName == "GEQ") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: >=" << std::endl;
	}
}
// 匹配终结符==
inline void matchTokenEql() {
	if (globalCurrentTokenName == "EQL") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: ==" << std::endl;
	}
}
// 匹配终结符!=
inline void matchTokenNeq() {
	if (globalCurrentTokenName == "NEQ") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: !=" << std::endl;
	}
}
// 匹配终结符!
inline void matchTokenNot() {
	if (globalCurrentTokenName == "NOT") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: !" << std::endl;
	}
}
// 匹配终结符&&
inline void matchTokenAnd() {
	if (globalCurrentTokenName == "AND") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: &&" << std::endl;
	}
}
// 匹配终结符||
inline void matchTokenOr() {
	if (globalCurrentTokenName == "OR") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: ||" << std::endl;
	}
}


// 匹配终结符+
inline void matchTokenPlus() {
	if (globalCurrentTokenName == "PLUS") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: +" << std::endl;
	}
}
// 匹配终结符-
inline void matchTokenMinu() {
	if (globalCurrentTokenName == "MINU") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: -" << std::endl;
	}
}
// 匹配终结符*
inline void matchTokenMult() {
	if (globalCurrentTokenName == "MULT") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: -" << std::endl;
	}
}
// 匹配终结符/
inline void matchTokenDiv() {
	if (globalCurrentTokenName == "DIV") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: /" << std::endl;
	}
}
// 匹配终结符%
inline void matchTokenMod() {
	if (globalCurrentTokenName == "MOD") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: %" << std::endl;
	}
}


// 匹配终结符(
inline void matchTokenLparent() {
	if (globalCurrentTokenName == "LPARENT") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: (" << std::endl;
	}
}
// 匹配终结符)
inline void matchTokenRparent(const emErrorType& errorType = Null) {
	if (globalCurrentTokenName == "RPARENT") {
		getNextToken();
	}
	else { // 缺少 )
		errorHandle(errorType, _lineNumberVector[readIndex - 1]); // 报错行数为前一个非终结符所在位置
	}
}
// 匹配终结符{
inline void matchTokenLbrace() {
	if (globalCurrentTokenName == "LBRACE") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: {" << std::endl;
	}
}
// 匹配终结符}
inline void matchTokenRbrace() {
	if (globalCurrentTokenName == "RBRACE") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: }" << std::endl;
	}
}
// 匹配终结符[
inline void matchTokenLbrack() {
	if (globalCurrentTokenName == "LBRACK") {
		getNextToken();
	}
	else { // 错误处理

	}
}
// 匹配终结符]
inline void matchTokenRbrack(const emErrorType& errorType = Null) {
	if (globalCurrentTokenName == "RBRACK") {
		getNextToken();
	}
	else { // 缺少右中括号
		errorHandle(errorType, _lineNumberVector[readIndex]);
	}
}


// 非零数字0
inline void matchTokenZero() {
	if (globalCurrentTokenName == "INTCON" && globalCurrentToken >= "0") {
		getNextToken();
	}
	else { // 错误处理

	}
}
// 非零数字1,2,...,9
inline void matchTokenNonZeroDigit() {
	if (globalCurrentTokenName == "INTCON" && globalCurrentToken >= "1" && globalCurrentToken <= "9") {
		getNextToken();
	}
	else { // 错误处理

	}
}
// 非零数字0,1,2,...,9
inline void matchTokenDigit() {
	if (globalCurrentTokenName == "INTCON" && globalCurrentToken >= "0") {
		getNextToken();
	}
	else { // 错误处理

	}
}

std::shared_ptr<CompUnitNode> CompUnit();
std::shared_ptr<DeclNode> Decl();
std::shared_ptr<FuncDefNode> FuncDef();
std::shared_ptr<MainFuncDefNode> MainFuncDef();
std::shared_ptr<ConstDeclNode> ConstDecl();
std::shared_ptr<VarDeclNode> VarDecl(const bool& val = false);
std::shared_ptr<FuncTypeNode> FuncType();
std::shared_ptr<IdentNode> Ident(const bool& isDecl = false);
std::shared_ptr<FuncFParamsNode> FuncFParams(std::vector<std::pair<std::string, std::string>>&);
std::shared_ptr<BlockNode> Block();
std::shared_ptr<BTypeNode> BType();
std::shared_ptr<VarDefNode> VarDef(const bool& val = false);
std::shared_ptr<FuncFParamNode> FuncFParam(std::pair<std::string, std::string>&);
std::shared_ptr<BlockItemNode> BlockItem();
std::shared_ptr<ArrayNode> Array();
std::shared_ptr<ConstExpNode> ConstExp();
std::shared_ptr<InitValNode> InitVal(const bool& val = false);
std::shared_ptr<StmtNode> Stmt();
std::shared_ptr<AddExpNode> AddExp();
std::shared_ptr<AddExp_Node> AddExp_(const bool& val = false);
std::shared_ptr<ExpNode> Exp(const bool& val = false);
std::shared_ptr<LValNode> LVal();
std::shared_ptr<CondNode> Cond();
std::shared_ptr<FormatCharNode> FormatString();
std::shared_ptr<MulExpNode> MulExp();
std::shared_ptr<MulExp_Node> MulExp_();
std::shared_ptr<LOrExpNode> LOrExp();
std::shared_ptr<LOrExp_Node> LOrExp_();
std::shared_ptr<LAndExpNode> LAndExp();
std::shared_ptr<LAndExp_Node> LAndExp_();
std::shared_ptr<EqExpNode> EqExp();
std::shared_ptr<EqExp_Node> EqExp_();
std::shared_ptr<UnaryExpNode> UnaryExp();
std::shared_ptr<RelExpNode> RelExp();
std::shared_ptr<RelExp_Node> RelExp_();
std::shared_ptr<CharNode> Char();
std::shared_ptr<FormatCharNode> FormatChar();
std::shared_ptr<NormalCharNode> NormalChar();
std::shared_ptr<PrimaryExpNode> PrimaryExp();
std::shared_ptr<FuncRParamsNode> FuncRParams(size_t&, const std::vector<std::pair<std::string, std::string>>&);
std::shared_ptr<NumberNode> Number();
std::shared_ptr<IntConstNode> IntConst();
std::shared_ptr<DecimalConstNode> decimal_const();
std::shared_ptr<NonzeroDigitNode> nonzero_digit();
std::shared_ptr<UnaryOpNode> UnaryOp();
std::shared_ptr<ConstInitValNode> ConstInitVal();
std::shared_ptr<ConstDefNode> ConstDef();


// ---------------------以下为函数语义分析使用-------------------------------
std::shared_ptr<BlockNode> FuncBlock(std::vector<std::pair<std::string, std::string>> typeNameVector,
	std::vector<std::shared_ptr<ExpNode>> expVector);

void contextSwitch(const std::vector<std::string>& token,
	const std::vector<std::string>& tokenName);  // 用于保存现场
void contextReload();  // 恢复现场