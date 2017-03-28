#include "SymbolTable.h"
#include "PascalAst.h"
namespace swd
{
	SymbolTable::SymbolTable()
	{
		tableIndex = 0;
		this->name = "";
		outer = NULL;
	}
	SymbolTable::SymbolTable(string name)
	{
		tableIndex = 0;
		this->name = name;
		outer = NULL;
	}
	void SymbolTable::initFunctions()
	{
		builtInFunctions.push_back("read");
		builtInFunctions.push_back("write");
		builtInFunctions.push_back("calcsin");
		builtInFunctions.push_back("calcsinh");
		//cos
		builtInFunctions.push_back("calccos");
		builtInFunctions.push_back("calccosh");
		//tan
		builtInFunctions.push_back("calctan");
		//log
		builtInFunctions.push_back("calclog");
		builtInFunctions.push_back("calclogb");
		builtInFunctions.push_back("calclog10");
		//exponents
		builtInFunctions.push_back("calcexp2");
		builtInFunctions.push_back("calcexp");
		//squared
		builtInFunctions.push_back("calcpow");
		builtInFunctions.push_back("calccubic");
		//square root
		builtInFunctions.push_back("calcsqrt");
	}
	bool SymbolTable::addInnerTable(SymbolTable *innerTable)
	{
		if (innerTable->name == "")
		{
			innerTable->name = to_string(tableIndex);
		}
		tableIndex++;
		inner.push_back(innerTable);
		return true;
	}
	SymbolTable* SymbolTable::findInnerTable(string name)
	{
		if (inner.size() > 0)
		{
			for (auto item : inner)
			{
				if (item->name == name)
				{
					return item;
				}
			}
		}
	}
	bool SymbolTable::add(std::string key, Node* val)
	{
		if (dict.find(key)==dict.end())
		{
			dict.insert(std::pair<string, Node*>(key, val));
			return true;
		}
		return false;
	}
	Node* SymbolTable::lookup(std::string key)
	{
		SymbolTable *env = this;
		while (env != NULL)
		{
			if (env->dict.find(key)!=env->dict.end())
			{
				return env->dict[key];
			}
			if (env->outer != NULL)
			{
				env = env->outer;
			}
			else
				break;
		}
		return NULL;
	}
	Node* SymbolTable::lookupInScope(std::string key)
	{
		if (dict.find(key) != dict.end())
		{
			return dict[key];
		}
		return NULL;
	}
	bool SymbolTable::lookupFunction(string funcName)
	{
		for (auto item : builtInFunctions)
		{
			if (item == funcName)
			{
				return true;
			}
		}
		return false;
	}
}