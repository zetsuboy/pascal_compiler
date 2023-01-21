#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <fstream>
#include <algorithm>
#include <map>
#include <iterator>
#include <sstream>
#include <windows.h>
#include <cstdlib>
#include <stdexcept>
#include "Lexer.h"
#include "Parser.h"


//class SyntaxAFull {
//private:
//	std::vector<Lexem> lexes;
//	int token = 0;
//
//	enum Types { INTEGER, REAL, STRING, IDENTIFIER, WORDS, COMMENT, OPERATOR, DELIMITER, END };
//	std::string TypesName[9] = { "Integer", "Real", "String", "Identifier", "Reserved word", "Comment", "Operator", "Delimiter", "End" };
//	
//	class Node {};
//
//	class NodeExpression : public Node {};
//
//	class ProgramNode : public Node {
//		void print() {
//
//		}
//	};
//
//	class NodeStatement : public Node {};
//
//	class WhileStmt : public NodeStatement {
//	public:
//		NodeExpression *condition;
//		NodeStatement* body;
//	};
//
//	class IfStmt : public NodeStatement {
//	public:
//		NodeExpression* condition;
//		NodeStatement* ifPart;
//		NodeStatement* elsePart;
//	};
//
//	class AssignStmt : public NodeStatement {
//	public:
//		NodeExpression* left;
//		NodeExpression* right;
//	};
//
//	class BlockStmt : public NodeStatement {
//	public:
//		std::vector<NodeStatement*> body;
//	};
//
//	NodeExpression* expression() {
//		NodeExpression* expr = new NodeExpression();
//		expr->left = term();
//		if (expr != nullptr && token < lexes.size() - 1) {
//			while (lexes[token].value == "+" || lexes[token].value == "-") {
//				NodeExpression* parent_node = new NodeExpression();
//				parent_node->val = lexes[token].value;
//				parent_node->left = expr;
//				token++;
//				Node* right_node = term();
//				parent_node->right = right_node;
//				expr = parent_node;
//				if (token >= lexes.size() - 1) break;
//			}
//			return expr;
//		}
//		else
//			return expr;
//	}
//
//	Node* term() {
//		Node* left_node = factor();
//		if (left_node != nullptr && token < lexes.size() - 1) {
//			while (lexes[token].value == "*" || lexes[token].value == "/") {
//				Node* parent_node = new Node();
//				parent_node->left = left_node;
//				parent_node->val = lexes[token].value;
//				token++;
//				Node* right_node = factor();
//				parent_node->right = right_node;
//				left_node = parent_node;
//				if (token >= lexes.size() - 1) break;
//			}
//			return left_node;
//		}
//		else
//			return left_node;
//	}
//
//	Node* factor() {
//		if (lexes[token].lex_type == "Integer" || lexes[token].lex_type == "Real") {
//			Node* node = new Node();
//			node->val = lexes[token].value;
//			token++;
//			return node;
//		}
//		if (lexes[token].lex_type == "Identifier") {
//			Node* node = new Node();
//			node->val = lexes[token].value;
//			token++;
//			return node;
//		}
//		if (lexes[token].value == "(") {
//			token++;
//			Node* node = expression();
//			if (lexes[token].value != ")") {
//				throw std::exception("No right bracket");
//				return nullptr;
//			}
//			token++;
//			return node;
//		}
//		else {
//			throw std::exception("Factor exception");
//			return nullptr;
//		};
//	}
//
//	NodeStatement* parseStatement() {
//		std::string s = lexes[token].default_value;
//		NodeStatement* node;
//		if (s == "begin") {
//
//		}
//		else if (s == "if") {
//
//		}
//		else if (s == "while") {
//			node = parseWhile();
//		}
//		else if (s == "var") {
//
//		}
//		else if (s == "for") {
//
//		}
//		else if (s == "break") {
//
//		}
//		else if (s == "end" || s == ";") {
//
//		}
//		else if (s == "readln") {
//
//		}
//		else if (s == "writeln") {
//
//		}
//		else if (lexes[token].lex_type == TypesName[IDENTIFIER]) {
//
//		}
//
//		return node;
//	}
//
//	NodeStatement* parseWhile() {
//		WhileStmt* stmt = new WhileStmt();
//		stmt->condition = expression();
//		if (lexes[token].default_value != "do") {
//			std::cout << "Error: 'do' expected";
//			return nullptr;
//		}
//
//		token++;
//		stmt->body = parseStatement();
//		return stmt;
//	}
//	public:
//		SyntaxAFull(std::vector<Lexem> l) {
//			lexes = l;
//		}
//
//		/*void printBT(const std::string& prefix, const Node* node, bool isLeft)
//		{
//			if (node != nullptr)
//			{
//				std::cout << prefix;
//				std::cout << "|--";
//
//				std::cout << node->val << std::endl;
//
//				printBT(prefix + (isLeft ? "|   " : "    "), node->left, true);
//				printBT(prefix + (isLeft ? "|   " : "    "), node->right, false);
//			}
//		}
//
//		void printBT()
//		{
//			printBT("", parseStatement(), false);
//		}*/
//};

//int main(int argc, char* argv[]) {
int main() {
	//Lexer s(argv[1]);
	//l.readFile("tests/test5.txt");

	Lexer s("tests/test5.txt");
	std::vector<Lexem> lexes;
	try {
		lexes = s.getLexems();
	}
	catch (std::runtime_error e) {
		std::cout << e.what();
	}
	
	for (auto l : lexes) {
		std::cout << l.line_id << "\t" << l.id << "\t" << l.TypeToString[(int)l.lex_type] << "\t" << l.default_value << "\t" << l.value << "\n";
	}

	SimpleParser parser(lexes);
	parser.printBT();

	return 0;
}