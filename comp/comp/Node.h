#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Lexem.h"
#include <map>

class Node {
public:
	Lexem token;

	Node(Lexem tok) {
		token = tok;
	}
};

class ProgramNode : public Node {
public:
	std::vector<Node*> functions;
	Node* block;

	ProgramNode(Lexem tok, std::vector<Node*> func, Node* bl) : Node(tok) {
		functions = func;
		block = bl;
	}
};

class FunctionNode : public Node {
public:
	std::vector<Node*> params;
	Node* returnType;
	Node* body;

	FunctionNode(Lexem tok, std::vector<Node*> pars, Node* reT, Node* bd) : Node(tok) {
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

	BlockNode(Lexem tok, std::vector<Node*> con, std::vector<Node*> decl, Node* com) : Node(tok) {
		consts = con;
		declarations = decl;
		compound = com;
	}
};

class VarDeclNode : public Node {
public:
	Node* var;
	Node* type;

	VarDeclNode(Lexem tok, Node* v, Node* t) : Node(tok) {
		var = v;
		type = t;
	}
};

class TypeSpecNode : public Node {
public:
	TypeSpecNode(Lexem tok) : Node(tok) {};
};

class ArrSpecNode : public Node {
public:
	Lexem lHandTok;
	Lexem rHandTok;
	Node* subType;

	ArrSpecNode(Lexem tok, Lexem l, Lexem r, Node* s) : Node(tok) {
		lHandTok = l;
		rHandTok = r;
		subType = s;
	}
};

class ConstNode : public Node {
public:
	Node* constName;
	Node* constValue;

	ConstNode(Lexem tok, Node* name, Node* value) : Node(tok) {
		constName = name;
		constValue = value;
	}
};

class CompoundNode : public Node {
public:
	std::vector<Node*> children;
	
	CompoundNode(Lexem tok) : Node(tok) {};

	CompoundNode(Lexem tok, std::vector<Node*> ch) : Node(tok) {
		children = ch;
	};

	void addChild(Node* child) {
		children.push_back(child);
	}
};

class NumberNode : public Node {
public:
	NumberNode(Lexem tok) : Node(tok) {};
};

class StringNode : public Node {
public:
	StringNode(Lexem tok) : Node(tok) {};
};

class BinOpNode : public Node {
public:
	Node* left;
	Node* right;

	BinOpNode(Lexem tok, Node* l, Node* r) : Node(tok) {
		left = l;
		right = r;
	}
};

class UnOpNode : public Node {
public:
	Node* down;

	UnOpNode(Lexem tok, Node* d) : Node(tok) {
		down = d;
	}
};

class AssignNode : public Node {
public:
	Node* var;
	Node* val;

	AssignNode(Lexem tok, Node* vr, Node* vl) : Node(tok) {
		var = vr;
		val = vl;
	}
};

class VarNode : public Node {
public:
	VarNode(Lexem tok) : Node(tok) {};
};

class SelectNode : public Node {
public:
	Node* from;
	Node* index;

	SelectNode(Lexem tok, Node* f, Node* i) : Node(tok) {
		from = f;
		index = i;
	}
};

class CallFuncNode : public Node {
public:
	std::vector<Node*> params;

	CallFuncNode(Lexem tok, std::vector<Node*> pars) : Node(tok) {
		params = pars;
	}
};

class ReturnNode : public Node {
public:
	Node* toReturn;

	ReturnNode(Lexem tok, Node* to) : Node(tok) {
		toReturn = to;
	}
};

class IfNode : public Node {
public:
	Node* condition;
	Node* body;
	Node* elseBody;

	IfNode(Lexem tok, Node* c, Node* b, Node* e) : Node(tok) {
		condition = c;
		body = b;
		elseBody = e;
	}
};

class WhileNode : public Node {
public:
	Node* condition;
	Node* body;

	WhileNode(Lexem tok, Node* c, Node* b) : Node(tok) {
		condition = c;
		body = b;
	}
};

class ForNode : public Node {
public:
	Node* iterStatement;
	Node* body;

	ForNode(Lexem tok, Node* i, Node* b) : Node(tok) {
		iterStatement = i;
		body = b;
	}
};

class IterationNode : public Node {
public:
	Node* assign;
	Node* condition;
	Node* postAction;

	IterationNode(Lexem tok, Node* a, Node* c, Node* p) : Node(tok) {
		assign = a;
		condition = c;
		postAction = p;
	}
};