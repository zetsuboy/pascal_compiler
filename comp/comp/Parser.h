#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Lexem.h"
#include <map>
#include "Node.h"
#include "Lexem.h"

class SimpleParser {
private:
	std::vector<Lexem> lexes;
	int token = 0;

	class Node {
	public:
		std::string val;
		Node* left = nullptr;
		Node* right = nullptr;
	};

	Node* expression() {
		Node* left_node = term();
		if (left_node != nullptr && token < lexes.size() - 1) {
			while (lexes[token].value == "+" || lexes[token].value == "-") {
				Node *parent_node = new Node();
				parent_node->val = lexes[token].value;
				parent_node->left = left_node;
				token++;
				Node* right_node = term();
				parent_node->right = right_node;
				left_node = parent_node;
				if (token >= lexes.size() - 1) break;
			}
			return left_node;
		}
		else
			return left_node;
	}

	Node* term() {
		Node *left_node = factor();
		if (left_node != nullptr && token < lexes.size() - 1) {
			while (lexes[token].value == "*" || lexes[token].value == "/") {
				Node* parent_node = new Node();
				parent_node->left = left_node;
				parent_node->val = lexes[token].value;
				token++;
				Node* right_node = factor();
				parent_node->right = right_node;
				left_node = parent_node;
				if (token >= lexes.size() - 1) break;
			}
			return left_node;
		}
		else
			return left_node;
	}

	Node* factor() {
		if (lexes[token].lex_type == Lexem::Type::INTEGER_CONST || lexes[token].lex_type == Lexem::Type::REAL_CONST) {
			Node *node = new Node();
			node->val = lexes[token].value;
			token++;
			return node;
		}
		if (lexes[token].lex_type == Lexem::Type::IDENTIFIER) {
			Node* node = new Node();
			node->val = lexes[token].value;
			token++;
			return node;
		}
		if (lexes[token].value == "(") {
			token++;
			Node* node = expression(); 
			if (lexes[token].value != ")") {
				throw std::exception("No right bracket");
				return nullptr;
			}
			token++;
			return node;
		}
		else {
			throw std::exception("Factor exception");
			return nullptr;
		};
	}
public:
	SimpleParser(std::vector<Lexem> l) {
		lexes = l;
	}

	void printBT(const std::string& prefix, const Node* node, bool isLeft)
	{
		if (node != nullptr)
		{
			std::cout << prefix;
			std::cout << "|--";

			std::cout << node->val << std::endl;

			printBT(prefix + (isLeft ? "|   " : "    "), node->left, true);
			printBT(prefix + (isLeft ? "|   " : "    "), node->right, false);
		}
	}

	void printBT()
	{
		printBT("", expression(), false);
	}

};

class Parser {
private:
	std::vector<Lexem> lexems;
	unsigned int tokenId = 0;
public:
	Parser(std::vector<Lexem> &l) {
		lexems = l;
	}

	Lexem& getCurLex() {
		return lexems[tokenId];
	}

	void eat(Lexem::Type type) {
		if (getCurLex().lex_type == type) {
			tokenId++;
		}
		else {
			throw std::runtime_error("Syntax error! Expected " + getCurLex().TypeToString[(int)type]);
		}
	}

	void eat(std::string word) {
		if (getCurLex().default_value == word) {
			tokenId++;
		}
		else {
			throw std::runtime_error("Syntax error! Expected " + word);
		}
	}


};