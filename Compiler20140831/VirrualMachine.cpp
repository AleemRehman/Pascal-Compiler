#include "VirtualMachine.h"
#include "parser.h"
#include "SymbolTable.h"
#include "PascalAst.h"
#include "Declaration.h"
using namespace vm;
VirtualMachine::VirtualMachine()
{
	programCounter = 0;
	reg_flag = false;
	//it = generator->IRCodeFile.begin();
	//this->labelScan();
}
VirtualMachine::VirtualMachine(compiler::IRCodeGen *generator)
{
	programCounter = 0;
	reg_flag = false;
	this->generator = generator;
	it = generator->IRCodeFile.begin();
	this->labelScan();
	this->initRuntimeInfo();
	this->symTable->initFunctions();
}
VirtualMachine::VirtualMachine(compiler::IRCodeGen *generator, 
							   swd::SymbolTable *symTable)
{
	programCounter = 0;
	reg_flag = false;
	this->generator = generator;
	this->symTable = symTable;
	currentTable = symTable;
	it = generator->IRCodeFile.begin();
	this->labelScan();
	this->initRuntimeInfo();
	this->symTable->initFunctions();
}

void VirtualMachine::move()
{
	it++;
	programCounter=it - generator->IRCodeFile.begin();
}

void VirtualMachine::initRuntimeInfo()
{
	rtInfo.lastScope = "";
	rtInfo.currentScope = "global";
	rtInfo.vStackItems = vStack.size();
	//rtInfo.varStackItems
	rtInfo.constStackItems=constStack.size();
	rtInfo.paramNum=0;
}

void VirtualMachine::labelScan()
{
	vector<shared_ptr<IRCode>>::iterator iter; 
	int pos=0;
	for (iter = this->generator->IRCodeFile.begin(); 
		 iter != this->generator->IRCodeFile.end(); ++iter)
	{
		pos++;
		if ((*iter)->_opType == OperationType::LABEL || (*iter)->_opType == OperationType::FUNC)
		{
			labelPos.insert(std::pair<string, int>((*iter)->_op1, pos));
		}
	}
}

void VirtualMachine::compute(char op)
{
	double result = 0;
	Tag resultType;
	if ((*it)->Count == 0)
	{
		StackItem item1 = vStack.back();
		vStack.pop_back();
		StackItem item2 = vStack.back();
		vStack.pop_back();

		if (item1.type == Tag::INT && item2.type == Tag::INT)
		{
			resultType = Tag::INT;
		}
		else
		{
			resultType = Tag::FLOAT;
		}

		switch (op)
		{
		case '+':
		result = atof(item1.value.c_str()) + atof(item2.value.c_str());
		break;
		case '-':
		result = atof(item1.value.c_str()) - atof(item2.value.c_str());
		break;
		case '*':
		result = atof(item1.value.c_str()) * atof(item2.value.c_str());
		break;
		case '/':
		result = atof(item1.value.c_str()) / atof(item2.value.c_str());
		break;
		default:
		result = atof(item1.value.c_str()) + atof(item2.value.c_str());

		}

		if (resultType == Tag::INT)
		{
			StackItem sitem = { item1.value + item2.value + "_result", to_string((int)result), Tag::INT };
			vStack.push_back(sitem);
		}
		else
		{
			StackItem sitem = { item1.value + item2.value + "_result", to_string(result), Tag::FLOAT };
			vStack.push_back(sitem);
		}
	}
	else if ((*it)->Count == 2){

		if (varStack.find((*it)->_op1) != varStack.end())
		{
			string val = varStack[(*it)->_op1];
			switch (op)
			{
			case '+':
			varStack[(*it)->_op1] = to_string(atoi(val.c_str()) + atoi((*it)->_op2.c_str()));
			break;
			case '-':
			varStack[(*it)->_op1] = to_string(atoi(val.c_str()) - atoi((*it)->_op2.c_str()));
			break;
			case '*':
			varStack[(*it)->_op1] = to_string(atoi(val.c_str()) * atoi((*it)->_op2.c_str()));
			break;
			case '/':
			varStack[(*it)->_op1] = to_string(atoi(val.c_str()) / atoi((*it)->_op2.c_str()));
			break;
			default:
			varStack[(*it)->_op1] = to_string(atoi(val.c_str()) + atoi((*it)->_op2.c_str()));

			}
			
		}
	}
}
template<typename T>
void VirtualMachine::write(T stringOrNum)
{
	cout << stringOrNum << endl;
}
template<typename T>
void VirtualMachine::read(T* varName)
{
	cin >> *varName;
	cin.get();//consume enter key
}
template<typename T>
void VirtualMachine::calcsin(T num) {
	//float param, result;
	//param = num;
	float rol = strtod(num.c_str(), NULL);
	float result = strtod(num.c_str(), NULL);
	if (rol != NULL) {
		result = sin(rol * PI / 180);
		cout << result << endl;
		//printf(num);
		//return num;
	}
	else
		cout << "code removed for calcsin" << endl;
}
template<typename T>
void VirtualMachine::calcsinh(T num) {
	//float param, result;
	//param = num;
	float rol = strtod(num.c_str(), NULL);
	float result = strtod(num.c_str(), NULL);
	if (rol != NULL) {
		result = sinh(rol * PI / 180);
		cout << result << endl;
	}
	else
		cout << "code reomved for hyperbolic sin" << endl;
	//printf(num);
	//return num;
}
template<typename T>
void VirtualMachine::calccos(T num) {
	//float param, result;
	//param = num;
	float rol = strtod(num.c_str(), NULL);
	float result = strtod(num.c_str(), NULL);
	result = cos(rol * PI / 180);
	cout << result << endl;
	//printf(num);
	//return num;
}
template<typename T>
void VirtualMachine::calccosh(T num) {
	//float param, result;
	//param = num;
	float rol = strtod(num.c_str(), NULL);
	float result = strtod(num.c_str(), NULL);
	result = cosh(rol * PI / 180);
	cout << result << endl;
	//printf(num);
	//return num;
}
template<typename T>
void VirtualMachine::calctan(T num) {
	//float param, result;
	//param = num;
	float rol = strtod(num.c_str(), NULL);
	float result = strtod(num.c_str(), NULL);
	result = tan(rol * PI / 180);
	cout << result << endl;
	//printf(num);
	//return num;
}
template<typename T>
void VirtualMachine::calclog(T num) {
	//float param, result;
	//param = num;
	float rol = strtod(num.c_str(), NULL);
	float result = strtod(num.c_str(), NULL);
	result = log(rol);
	cout << result << endl;
	//printf(num);
	//return num;
}
template<typename T>
void VirtualMachine::calclogb(T num) {
	//float param, result;
	//param = num;
	float rol = strtod(num.c_str(), NULL);
	float result = strtod(num.c_str(), NULL);
	result = logb(rol);
	cout << result << endl;
	//printf(num);
	//return num;
}
template <typename T>
void VirtualMachine::calclog10(T num) {
	//float param, result;
	//param = num;
	float rol = strtod(num.c_str(), NULL);
	//float rol1 = strtod(power.c_str(), NULL);
	float result = strtod(num.c_str(), NULL);
	result = log10(rol);
	cout << result << endl;
	//printf(num);
	//return num;
}
template<typename T>
void VirtualMachine::calcexp2(T num) {
	//float param, result;
	//param = num;
	float rol = strtod(num.c_str(), NULL);
	float result = strtod(num.c_str(), NULL);
	result = exp2(rol);
	cout << result << endl;
	//printf(num);
	//return num;
}
template<typename T>
void VirtualMachine::calcexp(T num) {
	//float param, result;
	//param = num;
	float rol = strtod(num.c_str(), NULL);
	float result = strtod(num.c_str(), NULL);
	result = exp(rol);
	cout << result << endl;
	//printf(num);
	//return num;
}
template<typename T>
void VirtualMachine::calcpow(T num) {
	//float param, result;
	//param = num;
	float rol = strtod(num.c_str(), NULL);
	//float rol1 = strtod(pow.c_str(), NULL);
	float result = strtod(num.c_str(), NULL);
	result = pow(rol, 2);
	cout << result << endl;
	//printf(num);
	//return num;
}
template<typename T>
void VirtualMachine::calccubic(T num) {
	//float param, result;
	//param = num;
	float rol = strtod(num.c_str(), NULL);
	//float rol1 = strtod(power.c_str(), NULL);
	float result = strtod(num.c_str(), NULL);
	result = cbrt(rol);
	cout << result << endl;
	//printf(num);
	//return num;
}
template<typename T>
void VirtualMachine::calcsqrt(T num) {
	//float param, result;
	//param = num;
	float rol = strtod(num.c_str(), NULL);
	//float rol1 = strtod(power.c_str(), NULL);
	float result = strtod(num.c_str(), NULL);
	result = sqrt(rol);
	cout << result << endl;
	//printf(num);
	//return num;
}


void VirtualMachine::functionExec(string funcName, StackItem *params, int args)
{
	if (funcName == "read"&& args==1)
	{
		string s;
		read(&s);
		params[0].value = s;


		varStack[params[0].name]=params[0].value;
	}
	else if (funcName == "calcsin"&& args == 1)
	{
		calcsin(params[0].value);
	}
	else if (funcName == "calcsinh"&& args == 1)
	{
		calcsinh(params[0].value);
	}
	else if (funcName == "calctan"&& args == 1)
	{
		calctan(params[0].value);
	}
	else if (funcName == "calccubic"&& args == 1)
	{
		calccubic(params[0].value);
	}
	else if (funcName == "calccos"&& args == 1)
	{
		calccos(params[0].value);
	}
	else if (funcName == "calccosh"&& args == 1)
	{
		calccosh(params[0].value);
	}
	else if (funcName == "calclog"&& args == 1)
	{
		calclog(params[0].value);
	}
	else if (funcName == "calclogb"&& args == 1)
	{
		calclogb(params[0].value);
	}
	else if (funcName == "calclog10"&& args == 1)
	{
		calclog10(params[0].value);
	}
	else if (funcName == "calcexp2"&& args == 1)
	{
		calcexp2(params[0].value);
	}
	else if (funcName == "calcexp"&& args == 1)
	{
		calcexp(params[0].value);
	}
	else if (funcName == "calcpow"&& args == 1)
	{
		calcpow(params[0].value);
	}
	else if (funcName == "calcsqrt"&& args == 1)
	{
		calcsqrt(params[0].value);
	}
	else if (funcName == "write" && args == 1)
	{
		write(params[0].value);
	}
	else
	{
		//
	}
}

/*
MOV,ADD,SUB,MUL,DIV,*^
CMP,*
JMP,JMPF,JE,JMPT,*
STORE,LOAD,*
PUSH,POP,*
EQ, UNEQ,*
LT,GT,^
LE,GE,^
AND,OR,^
LABEL,FUNC,PARAM,RET,CALL,*
IConst,FConst*
*/
#define GENE_CONSTITEM(TAG_TYPE)                                  \
    if ((*it)->Count == 2)                                        \
	{                                                             \
	StackItem sitem = { (*it)->_op1, (*it)->_op2, TAG_TYPE };     \
	constStack.push_back(sitem);                                  \
	}else if ((*it)->Count == 1)                                  \
	{                                                             \
		StackItem sitem = { (*it)->_op1, "", TAG_TYPE };          \
		constStack.push_back(sitem);                              \
	}                                                             

#define GENE_CMP_OP(_CMP_OP_)                 \
	StackItem item1 = vStack.back();          \
	vStack.pop_back();                        \
	StackItem item2 = vStack.back();          \
	vStack.pop_back();                        \
	if (item2.value _CMP_OP_ item1.value)     \
	{                                         \
	reg_flag = true;                          \
	}                                         \
	else                                      \
	{reg_flag=false;} 

void VirtualMachine::scan()
{
	static bool isJmpOrCall=false;
	
	switch ((*it)->_opType)
	{
	case OperationType::IConst:
	{
		GENE_CONSTITEM(Tag::INT);
		break;
	}
	case OperationType::FConst:
	{
		GENE_CONSTITEM(Tag::FLOAT);
		break;
	}
	case OperationType::PUSH:
	{
		StackItem sitem = { "", (*it)->_op1, Tag::INT };
		vStack.push_back(sitem);
		break;
	}
	case OperationType::POP:
	{
		vStack.pop_back();
		break;
	}
	case OperationType::STORE:
	{
		StackItem aitem = vStack.back();
		vStack.pop_back();
		if (varStack.find((*it)->_op1) != varStack.end() && varStack[(*it)->_op1] != aitem.value)
		{
			varStack[(*it)->_op1] = aitem.value;
		}
		else
			varStack.insert(std::pair<string, string>((*it)->_op1, aitem.value));
		break;
	}
	case OperationType::LOAD:
	{
		string varValue="";
		Node *varNode=currentTable->lookup((*it)->_op1);
		Tag t = Tag::INT;
		if (varNode != NULL)
		{
			t = varNode->value.tag;//NodeΪConstantStmt,TypeStmt,VariableStmt
			switch (t)
			{
			case Tag::CONSTANT:
				t = ((ConstantStmt*)varNode)->constDeclare->value.tag;
				varValue = ((ConstantStmt*)varNode)->constDeclare->value.value;
				break;
			case Tag::VAR:
				varValue = varStack[(*it)->_op1];
				t = ((VariableStmt*)varNode)->varDeclare->identity.tag;
				break;
			case Tag::TYPE:
				t = Tag::TYPE;
				varValue = "";
				break;
			}
		}
		StackItem sitem = { (*it)->_op1, varValue, t };
		vStack.push_back(sitem);
		break;
	}
	case OperationType::ADD:
	{
		compute('+');
		break;
	}
	case OperationType::SUB:
	{
		compute('-');
		break;
	}
	case OperationType::MUL:
	{
		compute('*');
		break;
	}
	case OperationType::DIV:
	{
		compute('/');
		break;
	}
	case OperationType::JMP:
	{
		if (labelPos.find((*it)->_op1) != labelPos.end())
		{
			isJmpOrCall = true;
			it = generator->IRCodeFile.begin() + labelPos[(*it)->_op1];
		}
		break;
	}
	case OperationType::JMPT:
	{
		if (reg_flag && labelPos.find((*it)->_op1) != labelPos.end())
		{
			isJmpOrCall = true;
			it = generator->IRCodeFile.begin() + labelPos[(*it)->_op1];
		}
		break;
	}
	case OperationType::JE:
	{
		isJmpOrCall = true;
		StackItem item1 = vStack.back();
		vStack.pop_back();
		StackItem item2 = vStack.back();
		vStack.pop_back();
		if (atof(item1.value.c_str()) - atof(item2.value.c_str()) == 0)//
		
		{
			reg_flag = true;
		}
		if (reg_flag && labelPos.find((*it)->_op1) != labelPos.end())
		{
			it = generator->IRCodeFile.begin() + labelPos[(*it)->_op1];
		}
		break;
	}
	case OperationType::JMPF:
	{
		if (!reg_flag && labelPos.find((*it)->_op1) != labelPos.end())
		{
			isJmpOrCall = true;
			it = generator->IRCodeFile.begin() + labelPos[(*it)->_op1];
		}
		break;
	}
	case OperationType::EQ:
	{
		GENE_CMP_OP( == );
		break;
	}
	case OperationType::UNEQ:
	{
		GENE_CMP_OP( != );
		break;
	}
	case OperationType::LT:
	{
		GENE_CMP_OP(< );
		break;
	}
	case OperationType::GT:
	{
		GENE_CMP_OP(> );
		break;
	}
	case OperationType::LE:
	{
		GENE_CMP_OP(<= );
		break;
	}
	case OperationType::GE:
	{
		GENE_CMP_OP(>= );
		break;
	}
	case OperationType::CMP:
	{
		StackItem item1 = vStack.back();
		vStack.pop_back();
		StackItem item2 = vStack.back();
		vStack.pop_back();
		if (atof(item1.value.c_str()) - atof(item2.value.c_str()) == 0)
		{
			reg_flag = true;
		}
		break;
	}
	case OperationType::LABEL:
	{
		break;//nothing to do with label instruction
	}
	case OperationType::FUNC:
	{
		if (rtInfo.currentScope == "global")
		{
			it = generator->IRCodeFile.begin() + labelPos["__Main__"];
			isJmpOrCall = true;
		}
		break;//nothing to do with FUNC instruction
	}
	case OperationType::PARAM:
	{
		static bool isTableFound=false;
		
		StackItem item = vStack.back();
		vStack.pop_back();
		if (!isTableFound)
		{
			currentTable = currentTable->findInnerTable(rtInfo.currentScope);
			isTableFound = true;
		}
		//Tag t = currentTable->lookupInScope((*it)->_op1)->value.tag;
		rtInfo.paramNum++;//
		varStack.insert(std::pair<string, string>((*it)->_op1, item.value));
		//distance 
		rtInfo.varStackItems.push_back(distance(varStack.find((*it)->_op1),varStack.begin()));
		break;
	}
	case OperationType::RET:
	{
		isJmpOrCall = true;
		if (labelPos.find(rtInfo.currentScope + "_call") != labelPos.end())
		{
			it = generator->IRCodeFile.begin() + labelPos[rtInfo.currentScope + "_call"];
		}
		string tmp = rtInfo.lastScope;
		rtInfo.lastScope = rtInfo.currentScope;
		rtInfo.currentScope = tmp;
		int vStackClear= vStack.size() - rtInfo.vStackItems-rtInfo.paramNum;
		
		int constStackClear = constStack.size() - rtInfo.constStackItems;
		while (vStackClear>0)//clear stack
		{
			vStack.pop_back();
			vStackClear--;
		}
		while (constStackClear > 0)
		{
			constStack.pop_back();
			constStackClear--;
		}
		while (rtInfo.varStackItems.size()>0)
		{
			int item = rtInfo.varStackItems.back();
			
			map<string,string>::iterator iter= varStack.begin();
			advance(iter, item);
			varStack.erase(iter);
			rtInfo.varStackItems.pop_back();
		}
		rtInfo.constStackItems = constStack.size();
		currentTable = currentTable->outer;
		
		break;
	}
	case OperationType::CALL:
	{
		if (labelPos.find((*it)->_op1) != labelPos.end())
		{
			rtInfo.lastScope = rtInfo.currentScope;
			rtInfo.currentScope = (*it)->_op1;
			rtInfo.vStackItems = vStack.size();
			rtInfo.constStackItems = constStack.size();
			isJmpOrCall = true;
			labelPos.insert(std::pair<string, int>((*it)->_op1 + "_call", programCounter + 1));
			it = generator->IRCodeFile.begin() + labelPos[(*it)->_op1];
		}
		else if (symTable->lookupFunction((*it)->_op1))//built-in functions
		{
			isJmpOrCall = false;
			StackItem item = vStack.back();
			vStack.pop_back();
			//built-in function call
			functionExec((*it)->_op1, &item, 1);
		}
		break;
	}
	}
	if (!isJmpOrCall)
	{
		move();
	}
	else
	{
		isJmpOrCall = false;
		programCounter = it - generator->IRCodeFile.begin();
	}
}
void VirtualMachine::run()
{
	while (it != generator->IRCodeFile.end())
	{
		scan();
	}
}