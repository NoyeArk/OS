#include "../include/my_parser.h"
#include <set>

// 该标志位用于判断是否进行语义分析
static bool isSemantic;


// 以下变量用于保存现场
static std::vector<int> ax_line;
static std::vector<std::string> ax_token;
static std::vector<std::string> ax_name;
static size_t ax_read;
static std::string ax_current_token;
static std::string ax_current_name;

static bool ax_lex_out;
static bool ax_parser_out;
static bool ax_error_out;
static bool ax_is_semantic;


// 判断当前语句中（分号之前）是否有想要的wantStr
bool isHaveWantInStmt(const std::string& wantStr) {
	// 从当前readIndex开始遍历，直到遇到分号
	size_t _readIndex = readIndex + 1;
	while (_tokenVector[_readIndex] != ";") {
		if (_tokenVector[_readIndex] == wantStr) {
			// 有想要的字符
			return true;
		}
		_readIndex++;
		assert(_readIndex < _tokenVector.size());
	}
	// 程序执行到这里，说明该语句中没有想要的字符
	return false;
}

// 判断在遇到下一个,号之前有几个[
size_t IdentType() {
	size_t count = 0;
	size_t idx = readIndex + 1;
	while (_tokenVector[idx] != "," && _tokenVector[idx] != ")"
		&& _tokenVector[idx] != "]" && _tokenVector[idx] != ";") {
		if (_tokenVector[idx] == "[")
			++count;
		++idx;
	}
	return count;
}

inline void printLookHead() { // 往后打印10个数字
	assert(readIndex + 10 < _tokenNameVector.size());
	for (int ii = 0; ii < 10; ii++)
		std::cout << _tokenVector[readIndex + ii] << " ";
	std::cout << std::endl;
}
inline std::string getNextSecondToken() {
	assert(readIndex + 1 < _tokenNameVector.size());
	return _tokenNameVector[readIndex + 1];
}
inline std::string getNextNumToken(const size_t& num) {
	assert(readIndex + num < _tokenNameVector.size());
	return _tokenNameVector[readIndex + num];
}

void getNextToken() { // 同时输出当前匹配的终结符信息到文件中
	if (outputFile.is_open() && LexOut) {
		outputFile << globalCurrentTokenName << " " << _tokenVector[readIndex] << std::endl;
	}
	if (readIndex + 1 < _tokenNameVector.size()) {
		globalCurrentToken = _tokenVector[readIndex + 1];
		globalCurrentTokenName = _tokenNameVector[++readIndex];
	}
}
// 输出语法分析结果
inline void outFileParserResult(const std::string& str) {
	if (!ParserOut) return;
	outputFile << "<" << str << ">" << std::endl;
}

// 输出解释执行结果
inline void outFileSemanticResult(std::string& str, const std::vector<int>& vals) {
	if (vals.empty()) {  // 没有%d

	}
	else {  // 替换%d
		for (const int val : vals) {
			str.replace(str.find("%d"), 2, std::to_string(val));
		}
	}

	// 找到换行符的位置
	size_t newlinePos = str.find('\\\n');

	// 提取两个子字符串
	std::string part1 = str.substr(0, newlinePos);
	if (part1.size() != str.size()) {
		std::string part2 = str.substr(newlinePos + 2);
		std::cout << part1 << std::endl << part2;
	}
	else std::cout << part1 << std::endl;
}

void init(std::vector<std::string>& tokenVector,
	std::vector<int>& lineNumberVector) {

	// 从input.txt文件中读取输入
	std::ifstream inFile("F:/code/os/compiler/input.txt"); // 打开文件

	if (inFile.is_open()) { // 检查文件是否成功打开
		std::string line;
		while (std::getline(inFile, line)) { // 逐行读取文件内容
			std::istringstream iss(line); // 使用字符串流来解析每行数据
			int num;
			while (iss >> num) { // 逐个读取整数
				InputData.push(num);
			}
		}
	}
	inFile.close(); // 关闭input.txt

	outputFile.open("F:/code/os/compiler/output.txt");

	for (size_t ii = 0; ii < tokenVector.size(); ii++) {
		bool isFormat = false;
		size_t start = tokenVector[ii].find('"');
		size_t end = tokenVector[ii].find('"', start + 1);

		if (start != std::string::npos && end != std::string::npos) {
			isFormat = true;
		}
		if (checkSpecialChar(tokenVector[ii]) && !isFormat) { // 如果含有特殊字符则跳过
			continue;
		}
		std::string classification = transName(tokenVector[ii], isFormat); // 得到种别码
		if (classification == "ERROR") {
			continue;
		}
		_tokenVector.push_back(tokenVector[ii]);
		_tokenNameVector.push_back(classification);
		_lineNumberVector.push_back(lineNumberVector[ii]);
	}
	//_lineNumberVector.insert(_lineNumberVector.end(), lineNumberVector.begin(), lineNumberVector.end());

	readIndex = 0;
	globalCurrentTokenName = _tokenNameVector[readIndex];
	globalCurrentToken = _tokenVector[readIndex];

	// 默认先不进行语义分析
	isSemantic = false;
}

void end() {
	outputFile.close();
}

void parsering(std::vector<std::string>& tokenVector, std::vector<int>& lineNumberVector) {
	init(tokenVector, lineNumberVector);
	CompUnit();
	end();
}

// 1-编译单元
std::shared_ptr<CompUnitNode> CompUnit() {
	auto cur = std::make_shared<CompUnitNode>("CompUnitNode");

	while (getNextNumToken(2) != "LPARENT" && getNextSecondToken() != "MAINTK") {
		cur->decl.push_back(Decl());
	}

	while (getNextSecondToken() != "MAINTK")
		cur->func_def.push_back(FuncDef());
	cur->main_func_def = MainFuncDef();

	outFileParserResult("CompUnit");
	return cur;
}
// 2-声明
std::shared_ptr<DeclNode> Decl() {
	auto cur = std::make_shared<DeclNode>("Decl");

	if (globalCurrentTokenName == "CONSTTK")
		cur->const_decl = ConstDecl();
	else if (globalCurrentTokenName == "INTTK" && getNextSecondToken() != "MAINTK")
		cur->var_decl = VarDecl();

	return cur;
}
// 3-函数定义
std::shared_ptr<FuncDefNode> FuncDef() {
	auto cur = std::make_shared<FuncDefNode>("FuncDef");

	insertFlag(emInFunc); // 标志当前为函数声明
	TableSpace::newScope();

	const std::string funcName = _tokenVector[readIndex + 1]; // 得到函数名
	TableSpace::ReturnType retType = (globalCurrentToken == "int") ? TableSpace::Int : TableSpace::Void; // 得到函数返回值类型
	//std::vector<std::string> paramType; // 得到函数参数类型
	//std::vector<std::string> paramsName; // 得到函数实参变量名
	//std::pair<std::vector<std::string>, std::vector<std::string>> typeNamePair = std::make_pair(paramType, paramsName);
	std::vector<std::pair<std::string, std::string>> typeNameVector; // 1:类型 2:变量名

	cur->func_type = FuncType();
	cur->ident = Ident(true);
	matchTokenLparent(); // (
	if (globalCurrentTokenName != "RPARENT") { // )
		cur->func_f_params = FuncFParams(typeNameVector);
	}
	matchTokenRparent(J); // )
	TableSpace::funcInsert(funcName, retType, typeNameVector.size(), typeNameVector); // 插入符号表记录

	int startIndex = readIndex;
	cur->block = Block();  // 函数体部分
	int endIndex = readIndex;
	std::vector<std::string> funcBody(_tokenVector.begin() + startIndex, _tokenVector.begin() + endIndex);
	std::vector<std::string> funcBodyName(_tokenNameVector.begin() + startIndex, _tokenNameVector.begin() + endIndex);

	// 将函数体插入符号表中
	TableSpace::insertFuncBody(funcBody, funcBodyName);

	// 有返回值的函数没有return
	if (retType != TableSpace::Void) {
		if (haveReturn == false) {
			// 从当前行开始逆序遍历找最近的}
			size_t idx = readIndex;
			while (_tokenVector[--idx] != "}") {
				continue;
			}
			errorHandle(G, _lineNumberVector[idx]);
		}
	}

	TableSpace::remove();
	haveReturn = false;
	eraseFlag();  // 去除当前标志
	outFileParserResult("FuncDef");

	return cur;
}
// 4-主函数定义
std::shared_ptr<MainFuncDefNode> MainFuncDef() {
	flags.MainFlag = true;
	auto cur = std::make_shared<MainFuncDefNode>("MainFuncDef");

	// 开启语义分析
	isSemantic = true;

	matchTokenInt(); // int
	matchTokenMain(); // main
	matchTokenLparent(); // (
	matchTokenRparent(); // )
	cur->block = Block();
	if (haveReturn == false) {
		// 从当前行开始逆序遍历找最近的}
		size_t idx = readIndex + 1;
		while (_tokenVector[--idx] != "}") {
			continue;
		}
		errorHandle(G, _lineNumberVector[idx]);
	}
	outFileParserResult("MainFuncDef");

	flags.MainFlag = false;
	return cur;
}
// 5-常量声明
std::shared_ptr<ConstDeclNode> ConstDecl() {
	auto cur = std::make_shared<ConstDeclNode>("ConstDecl");
	matchTokenConst(); // const
	cur->var_decl = VarDecl(true);
	return cur;
}
// 6-变量定义
std::shared_ptr<VarDeclNode> VarDecl(const bool& isConstDecl) {
	auto cur = std::make_shared<VarDeclNode>("VarDecl");

	cur->btype = BType();
	t = cur->btype->emType;
	w = cur->btype->width;
	cur->vardef.push_back(VarDef(isConstDecl));
	// 下面的内容可以重复0次或多次
	while (globalCurrentTokenName == "COMMA") { // ,
		matchTokenComma();
		cur->vardef.push_back(VarDef(isConstDecl));
	}
	matchTokenSemicn(I); // ;
	if (isConstDecl) outFileParserResult("ConstDecl");
	else outFileParserResult("VarDecl");

	return cur;
}
// 7-函数类型
std::shared_ptr<FuncTypeNode> FuncType() {
	auto cur = std::make_shared<FuncTypeNode>("FuncType");

	if (globalCurrentTokenName == "VOIDTK") { // void
		matchTokenVoid(); // void
		cur->funcType = "void";
	}
	else {
		matchTokenInt(); // int
		cur->funcType = "int";
	}
	outFileParserResult("FuncType");
	return cur;
}
// 8-标识符
std::shared_ptr<IdentNode> Ident(const bool& isDecl) {
	auto cur = std::make_shared<IdentNode>("Ident");

	cur->lexeme = globalCurrentToken;
	matchTokenIdent(isDecl);

	return cur;
}
// 9-函数形参表
std::shared_ptr<FuncFParamsNode> FuncFParams(std::vector<std::pair<std::string, std::string>>& typeNameVector) {
	auto cur = std::make_shared<FuncFParamsNode>("FuncFParams");

	std::pair<std::string, std::string> currentTypeNamePair;

	cur->func_f_param.push_back(FuncFParam(currentTypeNamePair));
	typeNameVector.push_back(currentTypeNamePair);

	while (globalCurrentTokenName == "COMMA") { // ,
		currentTypeNamePair.first = "";
		currentTypeNamePair.second = "";

		matchTokenComma(); // ,
		cur->func_f_param.push_back(FuncFParam(currentTypeNamePair));

		typeNameVector.push_back(currentTypeNamePair);
	}

	// 处理这种情况 void testFunc(int a, int a)
	std::set<std::string> secondElements;
	for (const auto& pair : typeNameVector) {
		if (!secondElements.insert(pair.second).second) {
			errorHandle(B, _lineNumberVector[readIndex]);
		}
	}

	outFileParserResult("FuncFParams");
	return cur;
}
// 10-语句块
std::shared_ptr<BlockNode> Block() {
	TableSpace::newScope();
	auto cur = std::make_shared<BlockNode>("Block");

	matchTokenLbrace(); // {
	while (globalCurrentTokenName != "RBRACE") {
		cur->block_item.push_back(BlockItem());
	}
	matchTokenRbrace(); // }
	outFileParserResult("Block");

	TableSpace::remove();
	return cur;
}
// 函数语义分析
std::shared_ptr<BlockNode> FuncBlock(std::vector<std::pair<std::string, std::string>> typeNameVector,
	std::vector<std::shared_ptr<ExpNode>> expVector) {
	TableSpace::newScope();
	// 插入函数形参变量
	for (size_t ii = 0; ii < expVector.size(); ii++) {
		TableSpace::insert(typeNameVector[ii].second, TableSpace::variable, std::to_string(expVector[ii]->val), ""); // 插入符号表记录
	}

	auto cur = std::make_shared<BlockNode>("Block");

	matchTokenLbrace(); // {
	while (globalCurrentTokenName != "RBRACE") {
		cur->block_item.push_back(BlockItem());
	}
	cur->retVal = cur->block_item.back()->retVal;

	matchTokenRbrace(); // }
	outFileParserResult("Block");

	TableSpace::remove();
	return cur;
}
// 11-基本类型
std::shared_ptr<BTypeNode> BType() {
	auto cur = std::make_shared<BTypeNode>("BType");

	EmType type = matchTokenInt();
	cur->emType = type;
	cur->width = 4;

	return cur;
}
// 12-变量定义
std::shared_ptr<VarDefNode> VarDef(const bool& isConstDef) {
	std::shared_ptr<VarDefNode> cur = std::make_shared<VarDefNode>("VarDef");
	std::string identType;

	// ------------------------排除重定义------------------------
	TableSpace::TableItem outcome = TableSpace::query(globalCurrentToken, true);
	if (outcome.kind != TableSpace::null)
		errorHandle(B, _lineNumberVector[readIndex]);
	// ---------------------------------------------------------

	// ------------------------得到当前变量名和类型------------------------
	const std::string identName = globalCurrentToken;
	std::string val = "null";
	TableSpace::Kind kind = (isConstDef == true) ? TableSpace::constant : TableSpace::variable;
	// ---------------------------------------------------------

	cur->ident = Ident(true);
	cur->array = Array();

	if (globalCurrentTokenName == "ASSIGN") { // 匹配终结符=
		matchTokenAssign();
		val = globalCurrentToken;
		cur->initval = InitVal(isConstDef);
	}
	TableSpace::insert(identName, kind, val, identType); // 插入符号表记录

	if (isConstDef) outFileParserResult("ConstDef");
	else outFileParserResult("VarDef");
	return cur;
}
// 12.5-多维数组类型
std::shared_ptr<ArrayNode> Array() {
	auto cur = std::make_shared<ArrayNode>("Array");

	if (globalCurrentTokenName == "LBRACK") {  // C -> [num]C1
		matchTokenLbrack();  // [
		cur->const_exp = ConstExp();  // 此时ConstExp的值就是num的值
		matchTokenRbrack(K);  // ]
		cur->array = Array();
		cur->dimension.push_back(cur->const_exp->val);  // 添加此维度大小
		cur->width = cur->width * cur->const_exp->val;
	}
	else {  // C -> 空
		cur->dimension.push_back(0);  //  不是数组类型
		cur->width = w;  // 宽度为全局变量
	}
	return cur;
}
// 13-函数形参
std::shared_ptr<FuncFParamNode> FuncFParam(std::pair<std::string, std::string>& currentTypeNamePair) {
	auto cur = std::make_shared<FuncFParamNode>("FuncFParam");
	currentTypeNamePair.second = _tokenVector[readIndex + 1]; // 第二个是参数名

	cur->btype = BType();
	cur->ident = Ident(true);
	if (globalCurrentTokenName == "LBRACK") { // [
		matchTokenLbrack(); // [
		currentTypeNamePair.first += "[";
		matchTokenRbrack(K); // ]
		while (globalCurrentTokenName == "LBRACK") { // [
			matchTokenLbrack(); // [
			currentTypeNamePair.first += "[";
			cur->const_exp.push_back(ConstExp());
			matchTokenRbrack(K); // ]
		}
	}
	outFileParserResult("FuncFParam");
	return cur;
}
// 14-语句块项
std::shared_ptr<BlockItemNode> BlockItem() {
	auto cur = std::make_shared<BlockItemNode>("BlockItem");

	if (globalCurrentTokenName == "CONSTTK" || globalCurrentTokenName == "INTTK")  // 选择产生式Decl
		cur->decl = Decl();
	else {
		cur->stmt = Stmt();
		cur->retVal = cur->stmt->retVal;
	}
	return cur;
}
// 15-常量表达式
std::shared_ptr<ConstExpNode> ConstExp() {
	auto cur = std::make_shared<ConstExpNode>("ConstExp");

	cur->add_exp = AddExp();
	outFileParserResult("ConstExp");
	return cur;
}
// 16=变量初值
std::shared_ptr<InitValNode> InitVal(const bool& isConstInitVal) {
	auto cur = std::make_shared<InitValNode>("InitVal");

	if (globalCurrentTokenName == "LBRACE") { // {
		matchTokenLbrace(); // {
		if (globalCurrentTokenName == "RBRACE") { // 判断是否有 }
			matchTokenRbrace(); // }
		}
		else {
			cur->init_val.push_back(InitVal(isConstInitVal));
			while (globalCurrentTokenName == "COMMA") { // ,
				matchTokenComma();
				cur->init_val.push_back(InitVal(isConstInitVal));
			}
			matchTokenRbrace(); // }
		}
	}
	else cur->exp = Exp(isConstInitVal);
	if (isConstInitVal) outFileParserResult("ConstInitVal");
	else outFileParserResult("InitVal");
	return cur;
}
// 17-语句
std::shared_ptr<StmtNode> Stmt() {
	auto cur = std::make_shared<StmtNode>("Stmt");

	if (globalCurrentTokenName == "PRINTFTK") { // printf
		matchTokenPrintf();
		matchTokenLparent(); // (

		cur->format_char = FormatString();  // 要输出的内容
		while (globalCurrentTokenName == "COMMA") { // ,
			matchTokenComma(); // ,
			cur->exp = Exp();  // 需要匹配的参数
			cur->format_char->print_exp.push_back(cur->exp->val);
			--cur->format_char->d_num;
		}

		if (isSemantic) outFileSemanticResult(cur->format_char->out_string, cur->format_char->print_exp);

		if (cur->format_char->d_num) errorHandle(L, _lineNumberVector[readIndex]);
		matchTokenRparent(); // )
		matchTokenSemicn(I); // ;
	}
	else if (globalCurrentTokenName == "IFTK") { // if
		++IfNum;
		// newlabel()
		bool isFirst;

		for (size_t ii = 0; ii < 1; ii++) {
			isFirst = ii == 0 ? true : false;

			if (isFirst) {
				matchTokenIf(); // if
				matchTokenLparent(); // ( 
				cur->cond = Cond();
				matchTokenRparent(); // )

				cur->cond->_true = label();  // label(Cond.true)
				//gen(BUFFER, cur->cond->_true);  // 输出cond中没有输出的中间代码

				bool temp_is_semantic = isSemantic;
				// 如果isSemantic为false,那么不进入
				// 如果isSemantic为true,进入,isSemantic为false
				if (isSemantic && !cur->cond->cond)  // if无效，跳过if
					isSemantic = false;

				cur->stmt1 = Stmt();
				cur->stmt1->next = cur->next;

				if (temp_is_semantic && !cur->cond->cond)  // if无效，跳过if
					isSemantic = true;
			}

			gen(isFirst, GOTO, cur->stmt1->next);

			if (globalCurrentTokenName == "ELSETK") { // else
				bool temp_is_semantic = isSemantic;

				if (isSemantic && cur->cond->cond)   // if有效，跳过else
					isSemantic = false;

				if (isFirst) {
					matchTokenElse(); // else
					cur->cond->_false = label();  // label(Cond.false)
				}

				if (isFirst) cur->stmt2 = Stmt();
				cur->stmt2->next = cur->next;

				if (temp_is_semantic && cur->cond->cond)   // if有效，跳过else
					isSemantic = true;
			}
			cur->next = label();
			//rrGoto(cur->next);
			// 得到false后把缓冲区的内容全部输出
			flushCodeBuffer(cur->cond->_true, cur->cond->_false, IfNum);
			//gen(true, BUFFER, cur->cond->_true, "占位符", cur->cond->_false);  // 输出cond中没有输出的中间代码
			--IfNum;
		}
	}
	else if (globalCurrentTokenName == "WHILETK") { // while
		IfNum++;
		insertFlag(emInLoop);
		int cond_false;
		bool isFirst;

		for (size_t ii = 0; ii < 1; ii++) {
			isFirst = ii == 0 ? true : false;

			if (isFirst) {
				matchTokenWhile(); // while
				cur->begin = label();  // label(stmt.begin)
				//cur->cond->_false = tstmt.next;
				matchTokenLparent(); // (
				cond_false = cur->next;
				cur->cond = Cond(); // cond
				cur->cond->_false = cond_false;
				matchTokenRparent(); // )

				cur->cond->_true = label();  // label(Cond.true)
			}
			//gen(isFirst, BUFFER, cur->cond->_true, "占位符", cur->cond->_false);
			if (isFirst) {
				cur->stmt1 = Stmt();
				cur->stmt1->next = cur->begin;
			}

			gen(isFirst, GOTO, cur->begin);

			cur->next = label();
			flushCodeBuffer(cur->cond->_true, cur->next, IfNum);
		}

		--IfNum;
		eraseFlag();
	}
	else if (globalCurrentTokenName == "BREAKTK") { // break
		matchTokenBreak(); // break
		matchTokenSemicn(I); // ;
	}
	else if (globalCurrentTokenName == "CONTINUETK") { // continue
		matchTokenContinue(); // continue
		matchTokenSemicn(I); // ;
	}
	else if (globalCurrentTokenName == "RETURNTK") { // return
		matchTokenReturn(); // return
		if (globalCurrentTokenName != "SEMICN") {
			if (currentEmFlag == emInFunc) { // 当前是函数声明
				TableSpace::FuncItem funcItem = TableSpace::getLastestFunc();
				if (funcItem.retType == TableSpace::Void)
					errorHandle(F, _lineNumberVector[readIndex]);
			}
			cur->exp = Exp();
			if (isSemantic) cur->retVal = cur->exp->val;
			else cur->retVal = -1;
		}
		else cur->retVal = -1;  // -1代表无返回值
		matchTokenSemicn(I); // ;
		haveReturn = true;
	}
	else if (globalCurrentTokenName == "LBRACE") { // Block
		cur->block = Block();
	}
	else if (isHaveWantInStmt("=")) { // 选择LVal
		if (TableSpace::isConstIdent(globalCurrentToken)) { // 判断常量
			errorHandle(H, _lineNumberVector[readIndex]);
		}

		cur->lval = LVal();
		matchTokenAssign(); // =
		if (globalCurrentTokenName == "GETINTTK") { // 选择getint
			matchTokenGetint(); // getint
			matchTokenLparent(); // (
			matchTokenRparent(); // )
			matchTokenSemicn(I); // ;

			if (isSemantic) {
				// 进行符号表该变量值的更新进行赋值
				TableSpace::update(cur->lval->lexeme, std::to_string(InputData.front()));
				InputData.pop();
			}
		}
		else { // 选择Exp
			cur->exp = Exp();
			matchTokenSemicn(I); // ;
			if (isSemantic) {
				// 进行符号表该变量值的更新进行赋值
				TableSpace::update(cur->lval->lexeme, std::to_string(cur->exp->val));
			}
		}
		//gen(true, ASSIGN, cur->exp->lexeme, cur->lval->ident->lexeme);
	}
	else {
		if (globalCurrentTokenName == "SEMICN") { // ;
			matchTokenSemicn(I);
		}
		else {
			cur->exp = Exp();
			matchTokenSemicn(I);
		}
	}
	outFileParserResult("Stmt");
	return cur;
}
// 18-加减表达式
std::shared_ptr<AddExpNode> AddExp() {
	auto cur = std::make_shared<AddExpNode>("AddExp");

	bool isUnaryOp = true;
	if (globalCurrentTokenName != "PLUS" && globalCurrentTokenName != "MINU") {
		cur->mul_exp = MulExp(); // 进入该if语句，说明不是以正负号开头
		cur->val = cur->mul_exp->val;

		isUnaryOp = false;
		outFileParserResult("AddExp");
	}
	else { // 进入该if语句，说明以正负号开头
		if (_tokenNameVector[readIndex - 1] != "PLUS"
			&& _tokenNameVector[readIndex - 1] != "MINU"
			&& currentUnaryExpCount != 0) { // 判断是否要新产生一个输出变量
			preUnaryExpCount = currentUnaryExpCount;
		}
		currentUnaryExpCount = 1;
		cur->mul_exp = std::make_shared<MulExpNode>("NONE");
		cur->mul_exp->temp_name = "UnaryOp";
		cur->val = 0;
	}
	cur->_add_exp = AddExp_(isUnaryOp);
	if (cur->_add_exp) {
		cur->temp_name = "t" + std::to_string(++TempNum);

		if (cur->mul_exp->temp_name == "UnaryOp") {  // -e+a
			if (cur->_add_exp->_add_exp->op_type == "+")
				cur->val = -1 * cur->_add_exp->mul_exp->val + cur->_add_exp->_add_exp->val;
			else cur->val = -1 * cur->_add_exp->mul_exp->val - cur->_add_exp->_add_exp->val;
		}
		else {
			if (cur->_add_exp->op_type == "+") cur->val += cur->_add_exp->val;
			else if (cur->_add_exp->op_type == "-") {
				if (cur->_add_exp->_add_exp) {  // -a%b+c
					if (cur->_add_exp->_add_exp->op_type == "+")
						cur->val += -1 * cur->_add_exp->mul_exp->val + cur->_add_exp->_add_exp->val;
					else if (cur->_add_exp->_add_exp->op_type == "-")
						cur->val = -1 * cur->_add_exp->mul_exp->val - cur->_add_exp->_add_exp->val;
				}
				else cur->val -= cur->_add_exp->val;
			}
			gen(true, EXP, cur->mul_exp->temp_name, cur->_add_exp->op_type, cur->_add_exp->temp_name, cur->temp_name);
		}

	}
	else cur->temp_name = cur->mul_exp->temp_name;
	return cur;
}
std::shared_ptr<AddExp_Node> AddExp_(const bool& isUnaryOp) {
	auto cur = std::make_shared<AddExp_Node>("AddExp_");
	cur->op_type = globalCurrentToken;

	bool nextIsUnaryOp = false;
	if (globalCurrentTokenName == "PLUS") matchTokenPlus(); // +
	else if (globalCurrentTokenName == "MINU") matchTokenMinu(); // -
	else return nullptr;
	if (isUnaryOp) { // 此时加减代表正负号
		outFileParserResult("UnaryOp");
		bracketNameStack.push("PNSign");
	}
	if (globalCurrentTokenName != "PLUS"
		&& globalCurrentTokenName != "MINU") {
		cur->mul_exp = MulExp();
		cur->val = cur->mul_exp->val;

		outFileParserResult("AddExp");
	}
	else { // 用于处理+-+5的情况0
		nextIsUnaryOp = true;
		++currentUnaryExpCount;
	}
	cur->_add_exp = AddExp_(nextIsUnaryOp);
	if (cur->_add_exp) {
		cur->temp_name = "t" + std::to_string(++TempNum);

		if (cur->_add_exp->op_type == "+") cur->val += cur->_add_exp->val;

		//else if (cur->_add_exp->op_type == "-") cur->val -= cur->_add_exp->val;
		else {
			if (cur->_add_exp->_add_exp) {  // -a%b+c
				if (cur->_add_exp->_add_exp->op_type == "+")
					cur->val += -1 * cur->_add_exp->mul_exp->val + cur->_add_exp->_add_exp->val;
				else if (cur->_add_exp->_add_exp->op_type == "-")
					cur->val = -1 * cur->_add_exp->mul_exp->val - cur->_add_exp->_add_exp->val;
			}
			else cur->val -= cur->_add_exp->val;
		}
		gen(true, EXP, cur->val, cur->_add_exp->op_type, cur->_add_exp->val);
	}
	else {
		cur->temp_name = cur->mul_exp->temp_name;
		cur->val = cur->mul_exp->val;
	}

	return cur;
}
// 19-表达式
std::shared_ptr<ExpNode> Exp(const bool& isConstExp) {
	auto cur = std::make_shared<ExpNode>("Exp");

	cur->add_exp = AddExp();
	cur->val = cur->add_exp->val;
	cur->lexeme = cur->add_exp->temp_name;
	if (isConstExp) outFileParserResult("ConstExp");
	else outFileParserResult("Exp");

	return cur;
}
// 20-左值表达式
std::shared_ptr<LValNode> LVal() {
	auto cur = std::make_shared<LValNode>("LVal");

	cur->ident = Ident(false);
	cur->lexeme = cur->ident->lexeme;
	while (globalCurrentTokenName == "LBRACK") { // [
		matchTokenLbrack(); // [
		bracketNameStack.push("LBRACK");
		cur->exp = Exp();
		matchTokenRbrack(K); // ]
		bracketNameStack.pop();
	}

	// 将ident对应的值取出来
	auto temp = TableSpace::query(cur->lexeme);
	cur->val = std::atoi((temp.value).c_str());

	outFileParserResult("LVal");
	return cur;
}
// 21-条件表达式
std::shared_ptr<CondNode> Cond() {
	auto cur = std::make_shared<CondNode>("Cond");

	if (globalCurrentTokenName == "NOT") {
		matchTokenNot();
		outFileParserResult("UnaryOp");
		++currentUnaryExpCount;
		bracketNameStack.push("NOT");
	}
	cur->lor_exp = LOrExp();
	cur->cond = cur->lor_exp->cond;
	//gen("if" + globalCurrentToken + "goto" + std::to_string(tcond._true));
	outFileParserResult("Cond");

	return cur;
}
// 22-格式字符串终结符
std::shared_ptr<FormatCharNode> FormatString() {
	auto cur = std::make_shared<FormatCharNode>("FormatString");
	std::string searchStr = "%d";
	cur->out_string = globalCurrentToken.substr(1, globalCurrentToken.size() - 2);
	cur->d_num = 0;

	size_t pos = 0;
	while ((pos = globalCurrentToken.find(searchStr, pos)) != std::string::npos) {
		++cur->d_num;
		pos += searchStr.length();
	}

	matchTokenFormat();
	return cur;
}
// 23-乘除模表达式
std::shared_ptr<MulExpNode> MulExp() {
	auto cur = std::make_shared<MulExpNode>("MulExp");

	cur->unary_exp = UnaryExp();

	cur->val = cur->unary_exp->val;
	cur->temp_name = cur->unary_exp->lexeme;

	outFileParserResult("MulExp");

	cur->_mul_exp = MulExp_();
	if (cur->_mul_exp) {
		if (cur->_mul_exp->op_type == "*") cur->val = cur->val * cur->_mul_exp->val;
		else if (cur->_mul_exp->op_type == "/") {
			if (cur->_mul_exp->val != 0)
				cur->val = cur->val / cur->_mul_exp->val;
			else;  // 被除数为0，不计算
		}
		else if (cur->_mul_exp->op_type == "%") cur->val = cur->val % cur->_mul_exp->val;
		gen(true, EXP, cur->val, cur->_mul_exp->op_type, cur->_mul_exp->val);
	}
	return cur;
}
std::shared_ptr<MulExp_Node> MulExp_() {
	auto cur = std::make_shared<MulExp_Node>("MulExp_");
	cur->op_type = globalCurrentToken;

	if (globalCurrentTokenName == "MULT") matchTokenMult(); // *
	else if (globalCurrentTokenName == "DIV") matchTokenDiv(); // /
	else if (globalCurrentTokenName == "MOD") matchTokenMod(); // %
	else return nullptr;
	cur->unary_exp = UnaryExp();
	cur->val = cur->unary_exp->val;

	outFileParserResult("MulExp");
	cur->_mul_exp = MulExp_();

	if (cur->_mul_exp) {  // 有效，根据不同的运算类型来进行运算
		cur->temp_name = "t" + std::to_string(++TempNum);

		if (cur->_mul_exp->op_type == "*") cur->val = cur->val * cur->_mul_exp->val;
		else if (cur->_mul_exp->op_type == "/") cur->val = cur->val / cur->_mul_exp->val;
		else if (cur->_mul_exp->op_type == "%") cur->val = cur->val % cur->_mul_exp->val;
		gen(true, EXP, cur->val, cur->_mul_exp->op_type, cur->_mul_exp->val);
	}
	else cur->temp_name = cur->unary_exp->lexeme;
	return cur;
}
// 24-逻辑或表达式
std::shared_ptr<LOrExpNode> LOrExp() {
	auto cur = std::make_shared<LOrExpNode>("LOrExp");

	cur->land_exp = LAndExp();
	outFileParserResult("LOrExp");
	cur->_lor_exp = LOrExp_();

	if (cur->_lor_exp) {
		if (cur->_lor_exp->op_type == "||" && (cur->land_exp->cond || cur->_lor_exp->cond))
			cur->cond = true;
		else cur->cond = false;
	}
	else cur->cond = cur->land_exp->cond;

	return cur;
}
std::shared_ptr<LOrExp_Node> LOrExp_() {
	auto cur = std::make_shared<LOrExp_Node>("LOrExp_");
	cur->op_type = globalCurrentToken;

	if (globalCurrentTokenName == "OR") matchTokenOr(); // ||
	else return nullptr; // null
	if (globalCurrentTokenName == "NOT") {
		matchTokenNot();
		outFileParserResult("UnaryOp");
		++currentUnaryExpCount;
		bracketNameStack.push("NOT");
	}
	cur->land_exp = LAndExp();
	cur->cond = cur->land_exp->cond;

	outFileParserResult("LOrExp");
	cur->_lor_exp = LOrExp_();

	return cur;
}
// 25-逻辑与表达式
std::shared_ptr<LAndExpNode> LAndExp() {
	auto cur = std::make_shared<LAndExpNode>("LAndExp");

	cur->eq_exp = EqExp();
	outFileParserResult("LAndExp");
	cur->_land_exp = LAndExp_();

	if (cur->_land_exp) {
		if (cur->_land_exp->op_type == "&&" && cur->eq_exp->cond && cur->_land_exp->cond)
			cur->cond = true;
		else cur->cond = false;
	}
	else cur->cond = cur->eq_exp->cond;

	return cur;
}
std::shared_ptr<LAndExp_Node> LAndExp_() {
	auto cur = std::make_shared<LAndExp_Node>("LAndExp_");
	cur->op_type = globalCurrentToken;

	if (globalCurrentTokenName == "AND") matchTokenAnd(); // &&
	else return nullptr; // null
	if (globalCurrentTokenName == "NOT") {
		matchTokenNot();
		outFileParserResult("UnaryOp");
		++currentUnaryExpCount;
		bracketNameStack.push("NOT");
	}
	cur->eq_exp = EqExp();
	cur->cond = cur->eq_exp->cond;  // 先只考虑简单的情况

	outFileParserResult("LAndExp");
	cur->_land_exp = LAndExp_();

	return cur;
}
// 26-相等性表达式
std::shared_ptr<EqExpNode> EqExp() {
	auto cur = std::make_shared<EqExpNode>("EqExp");

	cur->rel_exp = RelExp();

	outFileParserResult("EqExp");
	cur->_eq_exp = EqExp_();

	if (cur->_eq_exp) {
		if (cur->_eq_exp->op_type == "==" && cur->rel_exp->val == cur->_eq_exp->rel_exp->val)
			cur->cond = true;
		else if (cur->_eq_exp->op_type == "!=" && cur->rel_exp->val != cur->_eq_exp->rel_exp->val)
			cur->cond = true;
		else cur->cond = false;
	}
	else cur->cond = cur->rel_exp->cond;

	return cur;
}
std::shared_ptr<EqExp_Node> EqExp_() {
	auto cur = std::make_shared<EqExp_Node>("EqExp_");
	cur->op_type = globalCurrentToken;

	if (globalCurrentTokenName == "EQL") matchTokenEql(); // ==
	else if (globalCurrentTokenName == "NEQ") matchTokenNeq(); // !=
	else return nullptr;
	if (globalCurrentTokenName == "NOT") matchTokenNot();
	cur->rel_exp = RelExp();
	outFileParserResult("EqExp");
	cur->_eq_exp = EqExp_();

	return cur;
}
// 27-一元表达式
std::shared_ptr<UnaryExpNode> UnaryExp() {
	auto cur = std::make_shared<UnaryExpNode>("UnaryExp");

	if (getNextSecondToken() == "LPARENT") { // (，函数调用
		insertFlag(emFuncUse);

		// 得到调用的函数内容
		TableSpace::FuncItem outcome = TableSpace::FuncQuery(globalCurrentToken);
		std::vector<std::pair<std::string, std::string>> typeNameVector = outcome.typeNameVector;
		size_t funParamNum = 0;

		cur->ident = Ident(true); // 此位置的true是为了减少麻烦，但是不容易理解，解决函数名是否提前定义
		matchTokenLparent(); // (
		if (globalCurrentTokenName != "RPARENT") { // )
			// 这里需要对函数形参进行赋值
			cur->func_r_params = FuncRParams(funParamNum, typeNameVector);
		}

		if (flags.MainFlag) {
			// 保护现场
			contextSwitch(outcome.funcBody, outcome.funcBodName);

			// 没有执行对应的函数体
			auto func = FuncBlock(typeNameVector, cur->func_r_params->exp);
			cur->val = func->retVal;

			// 恢复现场
			contextReload();
		}

		// 错误处理-函数参数个数不匹配
		if (funParamNum != outcome.paramNum)
			errorHandle(D, _lineNumberVector[readIndex]);

		matchTokenRparent(J); // )
		eraseFlag();
	}
	/*else if (globalCurrentTokenName == "PLUS" || globalCurrentTokenName == "MIUS"
		|| globalCurrentTokenName == "NOT")
		UnaryOp();*/
	else {
		cur->primary_exp = PrimaryExp();
		cur->val = cur->primary_exp->val;
		cur->lexeme = cur->primary_exp->lexeme;
	}

	// 用于处理输出多个UnaryExp
	if (!bracketNameStack.empty()
		&& bracketNameStack.top() != "LPARENT" && bracketNameStack.top() != "LBRACK") {
		for (size_t ii = 0; ii < currentUnaryExpCount + 1; ii++)
			outFileParserResult("UnaryExp");
		if (preUnaryExpCount != 0) {
			currentUnaryExpCount = preUnaryExpCount;
			preUnaryExpCount = 0;
		}
		else currentUnaryExpCount = 0;
		bracketNameStack.pop();
	}
	else outFileParserResult("UnaryExp");

	return cur;
}
// 28-关系表达式
std::shared_ptr<RelExpNode> RelExp() {
	auto cur = std::make_shared<RelExpNode>("RelExp");

	cur->add_exp = AddExp();

	outFileParserResult("RelExp");
	cur->_rel_exp = RelExp_();

	if (cur->_rel_exp) {
		if (cur->_rel_exp->op_type == "<" && cur->add_exp->val < cur->_rel_exp->add_exp->val)
			cur->cond = true;
		else if (cur->_rel_exp->op_type == ">" && cur->add_exp->val > cur->_rel_exp->add_exp->val)
			cur->cond = true;
		else if (cur->_rel_exp->op_type == "<=" && cur->add_exp->val <= cur->_rel_exp->add_exp->val)
			cur->cond = true;
		else if (cur->_rel_exp->op_type == ">=" && cur->add_exp->val >= cur->_rel_exp->add_exp->val)
			cur->cond = true;
		else cur->cond = false;

		gen(true, IF, cur->add_exp->temp_name, cur->_rel_exp->op_type, cur->_rel_exp->add_exp->temp_name);
	}
	else {
		cur->cond = cur->add_exp->val == 0 ? false : true;  // 只有addexp
		cur->val = cur->add_exp->val;
	}
	return cur;
}
std::shared_ptr<RelExp_Node> RelExp_() {
	auto cur = std::make_shared<RelExp_Node>("RelExp_");
	cur->op_type = globalCurrentToken;

	if (globalCurrentTokenName == "LSS") matchTokenLss(); // <
	else if (globalCurrentTokenName == "GRE") matchTokenGre(); // >
	else if (globalCurrentTokenName == "LEQ") matchTokenLeq(); // <=
	else if (globalCurrentTokenName == "GEQ") matchTokenGeq(); // >=
	else return nullptr;
	if (globalCurrentTokenName == "NOT") matchTokenNot();
	cur->add_exp = AddExp();
	outFileParserResult("RelExp");
	cur->_rel_exp = RelExp_();

	if (cur->_rel_exp) {
		if (cur->_rel_exp->op_type == "<" && cur->add_exp->val < cur->_rel_exp->add_exp->val)
			cur->cond = true;
		else if (cur->_rel_exp->op_type == ">" && cur->add_exp->val > cur->_rel_exp->add_exp->val)
			cur->cond = true;
		else if (cur->_rel_exp->op_type == "<=" && cur->add_exp->val <= cur->_rel_exp->add_exp->val)
			cur->cond = true;
		else if (cur->_rel_exp->op_type == ">=" && cur->add_exp->val >= cur->_rel_exp->add_exp->val)
			cur->cond = true;
		else cur->cond = false;
	}

	return cur;
}
// 29-<Char>
std::shared_ptr<CharNode> Char() {
	auto cur = std::make_shared<CharNode>("Char");

	if (globalCurrentToken[0] == '%') { // 选择FormatChar
		cur->format_char = FormatChar();
	}
	else {
		cur->normal_char = NormalChar();
	}
	return cur;
}
// 30-<FormatChar>
std::shared_ptr<FormatCharNode> FormatChar() {
	auto cur = std::make_shared<FormatCharNode>("FormatChar");

	if (globalCurrentToken == "%d") {
		getNextToken();
	}
	return cur;
}
// 31-<NormalChar>
std::shared_ptr<NormalCharNode> NormalChar() {
	return nullptr;
}
// 32-基本表达式
std::shared_ptr<PrimaryExpNode> PrimaryExp() {
	auto cur = std::make_shared<PrimaryExpNode>("PrimaryExp");

	if (globalCurrentTokenName == "LPARENT") { // 匹配终结符(
		matchTokenLparent(); // (
		bracketNameStack.push("LPARENT");
		cur->exp = Exp();
		cur->val = cur->exp->val;
		matchTokenRparent(); // )
		bracketNameStack.pop();
	}
	else if ((globalCurrentToken >= "A" && globalCurrentToken <= "Z")
		|| (globalCurrentToken >= "a" && globalCurrentToken <= "z")) { // 判断为first(Ident)
		cur->lval = LVal();
		cur->lexeme = cur->lval->lexeme;
		cur->val = cur->lval->val;
	}
	else {
		cur->number = Number();
		cur->val = cur->number->val;
	}
	outFileParserResult("PrimaryExp");

	return cur;
}
// 33-函数实参表
std::shared_ptr<FuncRParamsNode> FuncRParams(size_t& funParamNum, const std::vector<std::pair<std::string, std::string>>& typeNameVector) {
	auto cur = std::make_shared<FuncRParamsNode>("FuncRParams");

	size_t idx = 0;
	std::string identType;
	// 检查是不是函数调用
	if (_tokenVector[readIndex + 1] == "(") {
		auto funcInfo = TableSpace::FuncQuery(globalCurrentToken);
		identType = funcInfo.retType == TableSpace::Int ? "" : "null";
	}
	else {
		// 从符号表中取该变量名的相关信息
		TableSpace::TableItem identInfo = TableSpace::query(globalCurrentToken);
		identType = identInfo.type;

		if (identInfo.kind == TableSpace::null) { // 从变量符号表中没有查到相关信息
			// 判断是不是当前函数形参
			std::vector<std::pair<std::string, std::string>> typeNameVector = TableSpace::getLastestFunc().typeNameVector;
			for (const auto& pair : typeNameVector) {
				if (pair.second == globalCurrentToken) { // 是形参变量
					identType = pair.first;
					break;
				}
			}
		}
	}

	if (identType == "null") {
		errorHandle(E, _lineNumberVector[readIndex]);
	}

	else {
		// identType是变量定义的维数
		size_t count = IdentType(); // count是指函数调用实参的维数，真正传入的维数：定义的维数-调用的维数
		size_t realType = identType.size() - count;
		// 判断该变量类型是否与函数参数类型匹配
		if (identType.size() != 0) { // 是数组类型
			if (realType != typeNameVector[idx].first.size())
				errorHandle(E, _lineNumberVector[readIndex]);
		}
		//if (count != 0) { // 是数组类型
		//	if (count != size(typeNameVector[idx].first) - 1)
		//		errorHandle(E, _lineNumberVector[readIndex]);
		//}
		else { // 不是数组类型
			if ((!typeNameVector.empty() && count != typeNameVector[idx].first.size()))
				errorHandle(E, _lineNumberVector[readIndex]);
		}
	}

	auto exp = Exp();
	//if (exp->lexeme != "") {  // 说明是一个变量
	//	auto identInfo = TableSpace::query(exp->lexeme);
	//	exp->val = std::atoi((identInfo.value).c_str());
	//}
	cur->exp.push_back(exp);
	++funParamNum;
	while (globalCurrentTokenName == "COMMA") { // ,
		matchTokenComma(); // ,
		auto exp = Exp();
		if (exp->lexeme != "") {  // 说明是一个变量
			auto identInfo = TableSpace::query(exp->lexeme);
			exp->val = std::atoi((identInfo.value).c_str());
		}
		cur->exp.push_back(exp);
		++funParamNum;
	}
	outFileParserResult("FuncRParams");

	return cur;
}
// 34-数值
std::shared_ptr<NumberNode> Number() {
	auto cur = std::make_shared<NumberNode>("Number");

	cur->int_const = IntConst();
	cur->val = cur->int_const->val;
	outFileParserResult("Number");
	return cur;
}
// 35-整数
std::shared_ptr<IntConstNode> IntConst() {
	auto cur = std::make_shared<IntConstNode>("IntConst");

	if (globalCurrentToken == "0") { // 匹配终结符0
		matchTokenZero();
	}
	else {
		cur->decimal_const = decimal_const();
		cur->val = cur->decimal_const->val;
	}
	return cur;
}
// 36-十进制数
std::shared_ptr<DecimalConstNode> decimal_const() {
	auto cur = std::make_shared<DecimalConstNode>("decimal_const");
	cur->val = std::stoi(globalCurrentToken);

	size_t currentTokenLen = globalCurrentToken.size();
	if (currentTokenLen == 1) {
		cur->non_zero_digit = nonzero_digit();
	}
	else {
		matchTokenDigit();
	}

	return cur;
}
// 37-非零数字
std::shared_ptr<NonzeroDigitNode> nonzero_digit() {
	auto cur = std::make_shared<NonzeroDigitNode>("nonzero_digit");

	matchTokenNonZeroDigit();
	return cur;
}
// 38-单目运算符
std::shared_ptr<UnaryOpNode> UnaryOp() {
	auto cur = std::make_shared<UnaryOpNode>("UnaryOp");

	if (globalCurrentTokenName == "PLUS")
		matchTokenPlus();
	else if (globalCurrentTokenName == "MINU")
		matchTokenMinu();
	else matchTokenNot();
	outFileParserResult("UnaryOp");

	return cur;
}
// 39-常量初值
std::shared_ptr<ConstInitValNode> ConstInitVal() {
	return nullptr;
}
// 40-常量定义
std::shared_ptr<ConstDefNode> ConstDef() {
	return nullptr;
}

void contextSwitch(const std::vector<std::string>& token,
	const std::vector<std::string>& tokenName) {
	ax_lex_out = LexOut;
	ax_parser_out = ParserOut;
	ax_error_out = ErrorOut;
	ax_is_semantic = isSemantic;

	ax_read = readIndex;
	ax_line = _lineNumberVector;
	ax_token = _tokenVector;
	ax_name = _tokenNameVector;
	ax_current_token = globalCurrentToken;
	ax_current_name = globalCurrentTokenName;

	// ----------------------------
	LexOut = false;
	ParserOut = false;
	ErrorOut = false;
	readIndex = 0;
	_tokenVector = token;
	_tokenNameVector = tokenName;
	globalCurrentToken = _tokenVector[readIndex];
	globalCurrentTokenName = _tokenNameVector[readIndex];

	// 开启语义分析
	isSemantic = true;
}

void contextReload() {
	LexOut = ax_lex_out;
	ParserOut = ax_parser_out;
	ErrorOut = ax_error_out;
	isSemantic = ax_is_semantic;

	_lineNumberVector = ax_line;
	_tokenVector = ax_token;
	_tokenNameVector = ax_name;
	readIndex = ax_read;
	globalCurrentToken = ax_current_token;
	globalCurrentTokenName = ax_current_name;

	// 关闭语义分析
	isSemantic = false;
}
