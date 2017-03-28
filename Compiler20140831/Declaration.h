#pragma once
#include "stdafx.h"
#include "lexer.h"
namespace swd
{

	enum class DeclaredType
	{
		Constant, Enumeration,
		Type, Variable, Record,
		Program, Procedure, Function,
		Undefined, 
		INT, FLOAT, STRING, USERTYPE 
	};

	class Declaration
	{
	public:
		DeclaredType type;
		string name;
	};

	class ConstantDecl :public Declaration
	{
	public:
		// const i=10
		Token value;
	};

	class VariableDecl :public Declaration
	{
	public:
		// var i:integer
		Token identity;

	};

	class TypeDecl :public Declaration
	{
	public:
		vector<shared_ptr<VariableDecl> > vars;
	};

	class FunctionDecl :public Declaration
	{
	public:
		std::vector<shared_ptr<VariableDecl> > vars;
		std::string returnType;
	};
}