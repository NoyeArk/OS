#pragma once

#include "define.h"
#include "utils.h"
#include "table.h"
#include "errorHandle.h"
#include "ast.h"

typedef enum {
	emNull = 10,
	emInLoop,   // ѭ����
	emInFunc,   // ��������
	emFuncUse,  // ��������
	emMain      // main����
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
static std::vector<int> _lineNumberVector;  // Դ�ļ���ÿһ��token��Ӧ���к�
static std::vector<std::string> _tokenVector;  // �洢Դ�ļ���ÿһ��token
static std::vector<std::string> _tokenNameVector;  // �洢ÿһ��token��Ӧ���ֱ���

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

// ƥ���ս��int
inline EmType matchTokenInt() {
	if (globalCurrentTokenName == "INTTK") {
		getNextToken();
		return Int;
	}
	else { // ������
		return emTypeNull;
	}
}
// ƥ���ս��main
inline void matchTokenMain() {
	if (globalCurrentTokenName == "MAINTK") {
		getNextToken();
	}
	else { // ������

	}
}
// ƥ���ս��break
inline void matchTokenBreak() {
	if (globalCurrentTokenName == "BREAKTK" && currentEmFlag == emInLoop) {
		getNextToken();
	}
	else { // �ڷ�ѭ����ʹ��break
		errorHandle(M, _lineNumberVector[readIndex]);
		getNextToken();
	}
}
// ƥ���ս��continue
inline void matchTokenContinue() {
	if (globalCurrentTokenName == "CONTINUETK" && currentEmFlag == emInLoop) {
		getNextToken();
	}
	else { // �ڷ�ѭ����ʹ��continue
		errorHandle(M, _lineNumberVector[readIndex]);
		getNextToken();
	}
}
// ƥ���ս��if
inline void matchTokenIf() {
	if (globalCurrentTokenName == "IFTK") {
		getNextToken();
	}
	else { // ������

	}
}
// ƥ���ս��else
inline void matchTokenElse() {
	if (globalCurrentTokenName == "ELSETK") {
		getNextToken();
	}
	else { // ������

	}
}
// ƥ���ս��while
inline void matchTokenWhile() {
	if (globalCurrentTokenName == "WHILETK") {
		getNextToken();
	}
	else { // ������

	}
}
// ƥ���ս��Ident������
inline void matchTokenIdent(const bool& isDecl = false) {

	if (globalCurrentToken == "abcde")
		printf("");

	if (!isDecl) { // ����������������
		bool isFuncParam = false;

		// ����ں��������У����жϺ�����ʵ��
		if (currentEmFlag == emInFunc || isInfunction()) {
			if (TableSpace::isfuncParamExist(globalCurrentToken)) { // ����ƥ��
				isFuncParam = true; // �Ǻ����β�
			}
		}

		if (!isFuncParam) { // ������Ǻ����β�
			TableSpace::TableItem outcome = TableSpace::query(globalCurrentToken);
			if (outcome.kind == TableSpace::null) {
				errorHandle(C, _lineNumberVector[readIndex]);
				getNextToken();
				return;
			}
		}
	}

	if ((globalCurrentToken[0] >= 'a' && globalCurrentToken[0] <= 'z') // �жϺϷ�������
		|| (globalCurrentToken[0] >= 'A' && globalCurrentToken[0] <= 'Z')) {
		if (globalCurrentToken == "change1" && readIndex == 28)
			printf("");
		getNextToken();
	}
}
// ƥ���ս��getint
inline void matchTokenGetint() {
	if (globalCurrentTokenName == "GETINTTK") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: getint" << std::endl;
	}
}
// ƥ���ս��printf
inline void matchTokenPrintf() {
	if (globalCurrentTokenName == "PRINTFTK") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: printf" << std::endl;
	}
}
// ƥ���ս��return
inline void matchTokenReturn() {
	if (globalCurrentTokenName == "RETURNTK") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: return" << std::endl;
	}
}
// ƥ���ս��void
inline void matchTokenVoid() {
	if (globalCurrentTokenName == "VOIDTK") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: void" << std::endl;
	}
}
// ƥ���ս��format
inline void matchTokenFormat() {
	size_t found1 = globalCurrentToken.find("$");
	size_t found2 = globalCurrentToken.find("&");
	if (found1 == std::string::npos && found2 == std::string::npos) {
		getNextToken();
	}
	else { // ���зǷ��ַ�
		errorHandle(A, _lineNumberVector[readIndex]);
		getNextToken();
	}
}
// const
inline void matchTokenConst() {
	if (globalCurrentTokenName == "CONSTTK") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: const" << std::endl;
	}
}


// ƥ���ս��,
inline void matchTokenComma() {
	if (globalCurrentTokenName == "COMMA") {
		getNextToken();
	}
	else { // ������
		printLookHead();
		std::cout << "error: ," << std::endl;
	}
}
// ƥ���ս��;
inline void matchTokenSemicn(const emErrorType& errorType = Null) {
	if (globalCurrentTokenName == "SEMICN") {
		getNextToken();
	}
	else { // ȱ�ٷֺ�
		errorHandle(errorType, _lineNumberVector[readIndex - 1]); // �����к�Ϊ�ֺ�ǰһ�����ս�������кš�
	}
}


// ƥ���ս��=
inline void matchTokenAssign() {
	if (globalCurrentTokenName == "ASSIGN") {
		getNextToken();
	}
	else { // ������
		std::cout << "readIndex:" << readIndex << std::endl;
		std::cout << "error: =" << std::endl;
	}
}
// ƥ���ս��<
inline void matchTokenLss() {
	if (globalCurrentTokenName == "LSS") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: <" << std::endl;
		printLookHead();
	}
}
// ƥ���ս��<=
inline void matchTokenLeq() {
	if (globalCurrentTokenName == "LEQ") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: <=" << std::endl;
	}
}
// ƥ���ս��>
inline void matchTokenGre() {
	if (globalCurrentTokenName == "GRE") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: >" << std::endl;
	}
}
// ƥ���ս��>=
inline void matchTokenGeq() {
	if (globalCurrentTokenName == "GEQ") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: >=" << std::endl;
	}
}
// ƥ���ս��==
inline void matchTokenEql() {
	if (globalCurrentTokenName == "EQL") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: ==" << std::endl;
	}
}
// ƥ���ս��!=
inline void matchTokenNeq() {
	if (globalCurrentTokenName == "NEQ") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: !=" << std::endl;
	}
}
// ƥ���ս��!
inline void matchTokenNot() {
	if (globalCurrentTokenName == "NOT") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: !" << std::endl;
	}
}
// ƥ���ս��&&
inline void matchTokenAnd() {
	if (globalCurrentTokenName == "AND") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: &&" << std::endl;
	}
}
// ƥ���ս��||
inline void matchTokenOr() {
	if (globalCurrentTokenName == "OR") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: ||" << std::endl;
	}
}


// ƥ���ս��+
inline void matchTokenPlus() {
	if (globalCurrentTokenName == "PLUS") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: +" << std::endl;
	}
}
// ƥ���ս��-
inline void matchTokenMinu() {
	if (globalCurrentTokenName == "MINU") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: -" << std::endl;
	}
}
// ƥ���ս��*
inline void matchTokenMult() {
	if (globalCurrentTokenName == "MULT") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: -" << std::endl;
	}
}
// ƥ���ս��/
inline void matchTokenDiv() {
	if (globalCurrentTokenName == "DIV") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: /" << std::endl;
	}
}
// ƥ���ս��%
inline void matchTokenMod() {
	if (globalCurrentTokenName == "MOD") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: %" << std::endl;
	}
}


// ƥ���ս��(
inline void matchTokenLparent() {
	if (globalCurrentTokenName == "LPARENT") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: (" << std::endl;
	}
}
// ƥ���ս��)
inline void matchTokenRparent(const emErrorType& errorType = Null) {
	if (globalCurrentTokenName == "RPARENT") {
		getNextToken();
	}
	else { // ȱ�� )
		errorHandle(errorType, _lineNumberVector[readIndex - 1]); // ��������Ϊǰһ�����ս������λ��
	}
}
// ƥ���ս��{
inline void matchTokenLbrace() {
	if (globalCurrentTokenName == "LBRACE") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: {" << std::endl;
	}
}
// ƥ���ս��}
inline void matchTokenRbrace() {
	if (globalCurrentTokenName == "RBRACE") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: }" << std::endl;
	}
}
// ƥ���ս��[
inline void matchTokenLbrack() {
	if (globalCurrentTokenName == "LBRACK") {
		getNextToken();
	}
	else { // ������

	}
}
// ƥ���ս��]
inline void matchTokenRbrack(const emErrorType& errorType = Null) {
	if (globalCurrentTokenName == "RBRACK") {
		getNextToken();
	}
	else { // ȱ����������
		errorHandle(errorType, _lineNumberVector[readIndex]);
	}
}


// ��������0
inline void matchTokenZero() {
	if (globalCurrentTokenName == "INTCON" && globalCurrentToken >= "0") {
		getNextToken();
	}
	else { // ������

	}
}
// ��������1,2,...,9
inline void matchTokenNonZeroDigit() {
	if (globalCurrentTokenName == "INTCON" && globalCurrentToken >= "1" && globalCurrentToken <= "9") {
		getNextToken();
	}
	else { // ������

	}
}
// ��������0,1,2,...,9
inline void matchTokenDigit() {
	if (globalCurrentTokenName == "INTCON" && globalCurrentToken >= "0") {
		getNextToken();
	}
	else { // ������

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


// ---------------------����Ϊ�����������ʹ��-------------------------------
std::shared_ptr<BlockNode> FuncBlock(std::vector<std::pair<std::string, std::string>> typeNameVector,
	std::vector<std::shared_ptr<ExpNode>> expVector);

void contextSwitch(const std::vector<std::string>& token,
	const std::vector<std::string>& tokenName);  // ���ڱ����ֳ�
void contextReload();  // �ָ��ֳ�