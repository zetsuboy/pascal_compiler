#pragma once
#include <vector>
#include <iostream>
#include "Lexem.h"

class Node {
public:
	Lexem token;

	Node(Lexem tok) {
		token = tok;
	}

	virtual void print(const std::string& prefix) = 0;
};

class ProgramNode : public Node {
public:
	std::vector<std::unique_ptr<Node>> functions;
	std::unique_ptr<Node> block;

	ProgramNode(Lexem tok, std::vector<std::unique_ptr<Node>> func, std::unique_ptr<Node> bl) : Node(tok) {
		functions = std::move(func);
		block = std::move(bl);
	}

	void print(const std::string& prefix) {
		std::cout << token.value << std::endl;

		if (functions.size()) {
			std::cout << "|-- " << "FUNCTIONS" << std::endl;
		}
		for (auto &func : functions) {
			func->print("|   ");
		}

		block->print("");
	}
};

class FunctionNode : public Node {
public:
	std::vector<std::unique_ptr<Node>> params;
	std::unique_ptr<Node> returnType;
	std::unique_ptr<Node> body;

	FunctionNode(Lexem tok, std::vector<std::unique_ptr<Node>> pars, std::unique_ptr<Node> reT, std::unique_ptr<Node> bd) : Node(tok) {
		params = std::move(pars);
		returnType = std::move(reT);
		body = std::move(bd);
	}

	void print(const std::string& prefix) {
		std::cout << prefix;
		std::cout << "|-- ";
		std::cout << token.value << std::endl;

		returnType->print(prefix + "|   ");
		body->print(prefix + "|   ");

		for (auto &par : params) {
			par->print(prefix + "|   ");
		}
	}
};

class BlockNode : public Node {
public:
	std::vector<std::unique_ptr<Node>> consts;
	std::vector<std::unique_ptr<Node>> declarations;
	std::unique_ptr<Node> compound;

	BlockNode(Lexem tok, std::vector<std::unique_ptr<Node>> con, std::vector<std::unique_ptr<Node>> decl, std::unique_ptr<Node> com) : Node(tok) {
		consts = std::move(con);
		declarations = std::move(decl);
		compound = std::move(com);
	}

	void print(const std::string& prefix) {
		std::cout << prefix;
		std::cout << "|-- ";
		std::cout << token.value << std::endl;

		for (auto &par : consts) {
			par->print(prefix + "|   ");
		}

		for (auto &par : declarations) {
			par->print(prefix + "|   ");
		}

		compound->print(prefix + "|   ");
	}
};

class VarDeclNode : public Node {
public:
	std::unique_ptr<Node> var;
	std::unique_ptr<Node> type;

	VarDeclNode(Lexem tok, std::unique_ptr<Node> v, std::unique_ptr<Node> t) : Node(tok) {
		var = std::move(v);
		type = std::move(t);
	}

	void print(const std::string& prefix) {
		std::cout << prefix;
		std::cout << "|-- ";
		std::cout << token.value << std::endl;

		var->print(prefix + "|   ");
		type->print(prefix + "|   ");
	}
};

class TypeSpecNode : public Node {
public:
	TypeSpecNode(Lexem tok) : Node(tok) {};

	void print(const std::string& prefix) {
		std::cout << prefix;
		std::cout << "|-- ";
		std::cout << token.value << std::endl;
	}
};

class ArrSpecNode : public Node {
public:
	Lexem lHandTok;
	Lexem rHandTok;
	std::unique_ptr<Node> subType;

	ArrSpecNode(Lexem tok, Lexem l, Lexem r, std::unique_ptr<Node> s) : Node(tok) {
		lHandTok = l;
		rHandTok = r;
		subType = std::move(s);
	}

	void print(const std::string& prefix) {
		std::cout << prefix;
		std::cout << "|-- ";
		std::cout << token.value << std::endl;

		std::cout << prefix + "|-- " << lHandTok.value << std::endl;
		std::cout << prefix + "|-- " << rHandTok.value << std::endl;
		subType->print(prefix);
	}
};

class ConstNode : public Node {
public:
	std::unique_ptr<Node> constName;
	std::unique_ptr<Node> constValue;

	ConstNode(Lexem tok, std::unique_ptr<Node> name, std::unique_ptr<Node> value) : Node(tok) {
		constName = std::move(name);
		constValue = std::move(value);
	}

	void print(const std::string& prefix) {
		std::cout << prefix;
		std::cout << "|-- ";
		std::cout << token.value << std::endl;

		constName->print(prefix + "|   ");
		constValue->print(prefix + "|   ");
	}
};

class CompoundNode : public Node {
public:
	std::vector<std::unique_ptr<Node>> children;
	
	CompoundNode(Lexem tok) : Node(tok) {};

	CompoundNode(Lexem tok, std::vector<std::unique_ptr<Node>> ch) : Node(tok) {
		children = std::move(ch);
	};

	void print(const std::string& prefix) {
		std::cout << prefix;
		std::cout << "|-- ";
		std::cout << token.value << std::endl;

		for (auto &ch : children) {
			ch->print(prefix + "|   ");
		}
	}
};

class NumberNode : public Node {
public:
	NumberNode(Lexem tok) : Node(tok) {};

	void print(const std::string& prefix) {
		std::cout << prefix;
		std::cout << "|-- ";
		std::cout << token.value << std::endl;
	}
};

class StringNode : public Node {
public:
	StringNode(Lexem tok) : Node(tok) {};

	void print(const std::string& prefix) {
		std::cout << prefix;
		std::cout << "|-- ";
		std::cout << token.value << std::endl;
	}
};

class BinOpNode : public Node {
public:
	std::unique_ptr<Node> left;
	std::unique_ptr<Node> right;

	BinOpNode(Lexem tok, std::unique_ptr<Node> l, std::unique_ptr<Node> r) : Node(tok) {
		left = std::move(l);
		right = std::move(r);
	}

	void print(const std::string& prefix) {
		std::cout << prefix;
		std::cout << "|-- ";
		std::cout << token.value << std::endl;

		left->print(prefix + "|   ");
		right->print(prefix + "|   ");
	}
};

class UnOpNode : public Node {
public:
	std::unique_ptr<Node> down;

	UnOpNode(Lexem tok, std::unique_ptr<Node> d) : Node(tok) {
		down = std::move(d);
	}

	void print(const std::string& prefix) {
		std::cout << prefix;
		std::cout << "|-- ";
		std::cout << token.value << std::endl;

		down->print(prefix + "|   ");
	}
};

class NoOpNode : public Node {
public:
	NoOpNode(Lexem tok) : Node(tok) {};

	void print(const std::string& prefix) {}
};

class AssignNode : public Node {
public:
	std::unique_ptr<Node> var;
	std::unique_ptr<Node> val;

	AssignNode(Lexem tok, std::unique_ptr<Node> vr, std::unique_ptr<Node> vl) : Node(tok) {
		var = std::move(vr);
		val = std::move(vl);
	}

	void print(const std::string& prefix) {
		std::cout << prefix;
		std::cout << "|-- ";
		std::cout << token.value << std::endl;

		var->print(prefix + "|   ");
		val->print(prefix + "|   ");
	}
};

class VarNode : public Node {
public:
	VarNode(Lexem tok) : Node(tok) {};

	void print(const std::string& prefix) {
		std::cout << prefix;
		std::cout << "|-- ";
		std::cout << token.value << std::endl;
	}
};

class SelectNode : public Node {
public:
	std::unique_ptr<Node> from;
	std::unique_ptr<Node> index;

	SelectNode(Lexem tok, std::unique_ptr<Node> f, std::unique_ptr<Node> i) : Node(tok) {
		from = std::move(f);
		index = std::move(i);
	}

	void print(const std::string& prefix) {
		std::cout << prefix;
		std::cout << "|-- ";
		std::cout << token.value << std::endl;

		from->print(prefix + "|   ");
		index->print(prefix + "|   ");
	}
};

class CallFuncNode : public Node {
public:
	std::vector<std::unique_ptr<Node>> params;

	CallFuncNode(Lexem tok, std::vector<std::unique_ptr<Node>> pars) : Node(tok) {
		params = std::move(pars);
	}

	void print(const std::string& prefix) {
		std::cout << prefix;
		std::cout << "|-- ";
		std::cout << token.value << std::endl;

		for (auto &p : params) {
			p->print(prefix + "|   ");
		}
	}
};

class ReturnNode : public Node {
public:
	std::unique_ptr<Node> toReturn;

	ReturnNode(Lexem tok, std::unique_ptr<Node> to) : Node(tok) {
		toReturn = std::move(to);
	}

	void print(const std::string& prefix) {
		std::cout << prefix;
		std::cout << "|-- ";
		std::cout << token.value << std::endl;

		toReturn->print(prefix + "|   ");
	}
};

class IfNode : public Node {
public:
	std::unique_ptr<Node> condition;
	std::unique_ptr<Node> body;
	std::unique_ptr<Node> elseBody;

	IfNode(Lexem tok, std::unique_ptr<Node> c, std::unique_ptr<Node> b, std::unique_ptr<Node> e) : Node(tok) {
		condition = std::move(c);
		body = std::move(b);
		elseBody = std::move(e);
	}

	void print(const std::string& prefix) {
		std::cout << prefix;
		std::cout << "|-- ";
		std::cout << token.value << std::endl;

		condition->print(prefix + "|   ");
		body->print(prefix + "|   ");
		elseBody->print(prefix + "|   ");
	}
};

class WhileNode : public Node {
public:
	std::unique_ptr<Node> condition;
	std::unique_ptr<Node> body;

	WhileNode(Lexem tok, std::unique_ptr<Node> c, std::unique_ptr<Node> b) : Node(tok) {
		condition = std::move(c);
		body = std::move(b);
	}

	void print(const std::string& prefix) {
		std::cout << prefix;
		std::cout << "|-- ";
		std::cout << token.value << std::endl;

		condition->print(prefix + "|   ");
		body->print(prefix + "|   ");
	}
};

class ForNode : public Node {
public:
	std::unique_ptr<Node> iterStatement;
	std::unique_ptr<Node> body;

	ForNode(Lexem tok, std::unique_ptr<Node> i, std::unique_ptr<Node> b) : Node(tok) {
		iterStatement = std::move(i);
		body = std::move(b);
	}

	void print(const std::string& prefix) {
		std::cout << prefix;
		std::cout << "|-- ";
		std::cout << token.value << std::endl;

		iterStatement->print(prefix + "|   ");
		body->print(prefix + "|   ");
	}
};

class IterationNode : public Node {
public:
	std::unique_ptr<Node> assign;
	std::unique_ptr<Node> condition;
	std::unique_ptr<Node> postAction;

	IterationNode(Lexem tok, std::unique_ptr<Node> a, std::unique_ptr<Node> c, std::unique_ptr<Node> p) : Node(tok) {
		assign = std::move(a);
		condition = std::move(c);
		postAction = std::move(p);
	}

	void print(const std::string& prefix) {
		std::cout << prefix;
		std::cout << "|-- ";
		std::cout << token.value << std::endl;

		condition->print(prefix + "|   ");
		postAction->print(prefix + "|   ");
	}
};