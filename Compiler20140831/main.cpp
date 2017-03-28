#include "parser.h"
#include "SemanticAnalyzer.h"
#include "CodeGen.h"
#include "SymbolTable.h"
#include "VirtualMachine.h"
#include <time.h>
#include <fstream>

void clear();

int main() {
	int filelines = 0;
	string filename;
	string filecontent;
	char choice;
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //check memory leaks
	{
		clear();
		cout << "Enter Filename: " << endl;
		cin >> filename; //input of the filename
		clear();
		ifstream openfile(filename);
		cout << "SourceCode" << endl;
		if (openfile.is_open()) {
			while (!openfile.eof()) {
				getline(openfile, filecontent);
				filelines++;
				cout << filecontent << endl;
			}
			cout << "Number of lines in code: " << filelines << endl;
			cout << endl;
		}
		openfile.close();
		int start_s = clock();
		Lexer lex_analyzer; //create instance of the lexer
		lex_analyzer.readFILE(filename); //read from thw file name
		lex_analyzer.scan_source();
		SymbolTable *symStack = new swd::SymbolTable();
		Parser parser(&lex_analyzer);

		parser.parseProgram();
		if (parser.listError.size() >= 1) {
			for (auto errors : parser.listError)
			{
				errors.errorPrint();
			}
		}
		else
		{
			cout << "Success" << endl;
		}
		std::cout << filename << endl;
		parser.root->print();//print synatatic tree
		swd::SemanticAnalyzer semanticAnalyzer;
		parser.root->accept(&semanticAnalyzer);
		symStack = semanticAnalyzer.symTable;//defined above the parser object--
		compiler::IRCodeGen codeGen;
		parser.root->genCode(&codeGen);

		//output IR code in a file
		ofstream f("out.txt");
		f << "Assembly Code" << endl;
		for (auto item : codeGen.IRCodeFile)
		{
			f << item->toString() << endl;
		}

		/*
		cout << "Assembly Code" << endl;
		for (auto item : codeGen.IRCodeFile)
		{
		cout << item->toString() << endl;
		}*/

		vm::VirtualMachine virtualRUN(&codeGen, symStack);
		int stop_s = clock();
		std::cout << "execution time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000 << endl;;
		std::cout << "compiled" << endl;
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		virtualRUN.run();
		SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN);
		std::cout << "press y to restart / n to exit." << endl;
		std::cin >> choice;
		if (choice == 'y' || choice == 'Y') {
			main();
		}
		else {
			exit(0);
		}
	}
	getchar();
	return 0;
}

void clear() {
	system("cls");
}