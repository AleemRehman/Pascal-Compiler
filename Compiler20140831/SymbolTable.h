#pragma once
#include "stdafx.h"

namespace swd
{

	class Node;

	class SymbolTable
	{
	public:
		string name;
		int tableIndex;
		SymbolTable* outer;
		vector<SymbolTable*> inner;
		vector<string> builtInFunctions;
		//methods-------------------------------
		SymbolTable();
		SymbolTable(string name);
		void initFunctions();
		bool lookupFunction(string funcName);
		std::map<std::string, swd::Node* > dict;
		bool add(std::string key, Node* val);
		bool addInnerTable(SymbolTable *innerTable);
		SymbolTable* findInnerTable(string name);
		Node* lookup(std::string key);
		Node* lookupInScope(std::string key);
	};
}