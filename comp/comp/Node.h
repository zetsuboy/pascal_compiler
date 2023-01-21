#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Lexem.h"
#include <map>

class Node {
public:
	std::string val;
};

class ProgramNode : public Node {
public:
	std::vector<Node*> functions;
	Node* block;

	ProgramNode(std::string name, std::vector<Node*> func, Node* bl) {
		val = name;
		functions = func;
		block = bl;
	}
};

class FunctionNode : public Node {
public:
	std::vector<Node*> params;
	Node* returnType;
	Node* body;

	FunctionNode(std::string name, std::vector<Node*> pars, Node* reT, Node* bd) {
		val = name;
		params = pars;
		returnType = reT;
		body = bd;
	}
};

class BlockNode : public Node {
public:
	std::vector<Node*> consts;
	std::vector<Node*> declarations;
	Node* compound;

	BlockNode(std::string name, std::vector<Node*> con, std::vector<Node*> decl, Node* com) {
		val = name;
		consts = con;
		declarations = decl;
		compound = com;
	}
};

class VarDeclNode : public Node {
public:

};