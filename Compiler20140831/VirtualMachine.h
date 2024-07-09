#pragma once
#include "CodeGen.h"
#define PI 3.14159265359

using namespace compiler;
namespace swd
{
	class Error;
	class SymbolTable;
}
namespace vm
{
	class StackItem
	{
	public:
		string name;
		string value;
		Tag type;
	};

	class RuntimeInfo
	{
	public:
		string lastScope;
		string currentScope;
		int currentPc;//current pc
		int vStackItems;
		vector<int> varStackItems;
		int constStackItems;
		int paramNum;//function params
	};

	class VirtualMachine
	{
	protected:
		int programCounter;//pc
		bool reg_flag;//flag register
		map<string, int> labelPos;//<Label,pos>, new iterator=vector.begin()+pos
		vector<shared_ptr<IRCode>>::iterator it;
		vector<Error> errList;
		//stack
		vector<StackItem> constStack;
		vector<StackItem> vStack;
		map<string, string> varStack;
		//code generator
		compiler::IRCodeGen *generator;
		swd::SymbolTable *symTable;
		RuntimeInfo rtInfo;
		swd::SymbolTable *currentTable;
	public:
		VirtualMachine();
		VirtualMachine(compiler::IRCodeGen *generator);
		VirtualMachine(compiler::IRCodeGen *generator, swd::SymbolTable *symTable);
		void initRuntimeInfo();
		void labelScan();
		void scan();
		void move();
		void run();
		void compute(char op);
		void functionExec(string funcName, StackItem *params, int args);
		//built-in ...something wrong
		template<typename T>
		void write(T stringOrNum);
		template<typename T>
		void read(T* varName);
		template<typename T>
		void calcsin(T stringorNum);
		template<typename T>
		void calcsinh(T stringorNum);
		template<typename T>
		void calccos(T stringorNum);
		template<typename T>
		void calccosh(T stringorNum);
		template<typename T>
		void calctan(T stringorNum);
		template<typename T>
		void calclog(T stringorNum);
		template<typename T>
		void calclogb(T stringorNum);
		template<typename T>
		void calclog10(T stringorNum);
		template<typename T>
		void calcexp2(T stringorNum);
		template<typename T>
		void calcexp(T stringorNum);
		template<typename T>
		void calcpow(T stringorNum);
		template<typename T>
		void calccubic(T stringorNum);
		template<typename T>
		void calcsqrt(T stringorNum);
	};
}