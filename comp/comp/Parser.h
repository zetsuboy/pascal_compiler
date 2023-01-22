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
		if (getCurLex().value == word) {
			tokenId++;
		}
		else {
			throw std::runtime_error("Syntax error! Expected " + word);
		}
	}

	Lexem& lookForward() {
		if (tokenId + 1 >= lexems.size())
			throw std::runtime_error("Error! Can't look forward - end of lexems");
		
		return lexems[tokenId + 1];
	}

	Node* analyze() {
		Node* root = Program();
		return root;
	}

	Node* Program() {
		eat("program");
		Lexem program_name = getCurLex();

		eat(Lexem::Type::IDENTIFIER);
		eat(";");

		std::vector<Node*> functions;
		while (getCurLex().value == "procedure" || getCurLex().value == "function") {
			functions.emplace_back(FuncDef());
			eat(";");
		}

		Node* block = Block();
		eat(".");
		ProgramNode *prog = new ProgramNode(program_name, functions, block);
		return prog;
	}

	Node* FuncDef() {
		Lexem funcLex = getCurLex();
		if (funcLex.value == "procedure")
			eat("procedure");
		else
			eat("function");

		Lexem name = getCurLex();
		eat(Lexem::Type::IDENTIFIER);
		eat("(");

		std::vector<Node*> params;
		while (getCurLex().value != ")") {
			std::vector<VarDeclNode*> sup = VarDecl();
			for (auto ptr : sup)
				params.emplace_back(ptr);
			if (getCurLex().value == ",")
				eat(",");
		}
		eat(")");

		Node* returnType;
		if (funcLex.value == "function") {
			eat(":");
			returnType = TypeSpec();
		}
		else {
			Lexem typeLex = { "void" };
			returnType = new TypeSpecNode(typeLex);
		}
		eat(";");
		Node* body = Block();

		FunctionNode* node = new FunctionNode(name, params, returnType, body);
		return node;
	}

	Node* Block() {
		std::vector<Node*> consts = Consts();
		std::vector<Node*> decls = Vars();
		Node* compound = CompoundSt();

		BlockNode* node = new BlockNode({ "BLOCK" }, consts, decls, compound);
		return node;
	}

	std::vector<Node*> Consts() {
		std::vector<Node*> cons;
		if (getCurLex().value == "const") {
			eat("const");
			while (getCurLex().lex_type == Lexem::Type::IDENTIFIER) {
				cons.emplace_back(ConstDecl());
				eat(";");
			}
		}
		return cons;
	}

	Node* ConstDecl() {
		Node* name = new Node(getCurLex().value);
		eat(Lexem::Type::IDENTIFIER);
		eat("=");

		Lexem val = getCurLex();
		Node* valPtr = Expression();

		ConstNode* node = new ConstNode(val, name, valPtr);
		return node;
	}

	std::vector<Node*> Vars() {
		std::vector<Node*> vars;
		if (getCurLex().value == "var") {
			eat("var");
			while (getCurLex().lex_type == Lexem::Type::IDENTIFIER) {
				std::vector<VarDeclNode*> varDeclList = VarDecl();
				for (auto varDeclPTR : varDeclList)
					varDeclList.emplace_back(varDeclPTR);
				eat(";");
			}
		}
		return vars;
	}

	std::vector<VarDeclNode*> VarDecl() {
		std::vector<Node*> varsList;
		Lexem tok = getCurLex();
		VarNode* var = new VarNode(getCurLex());
		varsList.emplace_back(var);
		eat(Lexem::Type::IDENTIFIER);

		while (getCurLex().value == ",") {
			eat(",");
			Lexem tok = getCurLex();
			VarNode* var = new VarNode(getCurLex());
			varsList.emplace_back(var);
			eat(Lexem::Type::IDENTIFIER);
		}
		eat(":");

		Node* typeNode = TypeSpec();

		std::vector<VarDeclNode*> varDeclsList;
		for (auto var : varsList) {
			VarDeclNode* tmp = new VarDeclNode({ "var_decl" }, var, typeNode);
			varDeclsList.emplace_back(tmp);
		}
		return varDeclsList;
	}

	Node* TypeSpec() {
		Lexem typeTok = getCurLex();

		if (typeTok.value != "array") {
			eat(Lexem::Type::TYPE_SPEC);
			Node* node = new Node(typeTok);
			return node;
		}
		else {
			Lexem arrTok = getCurLex();
			eat("array");
			eat("[");
			Lexem lHandTok = getCurLex();
			eat(Lexem::Type::INTEGER_CONST);
			eat(".");
			eat(".");
			Lexem rHandTok = getCurLex();
			if (rHandTok.lex_type == Lexem::Type::INTEGER_CONST)
				eat(Lexem::Type::INTEGER_CONST);
			else
				eat(Lexem::Type::IDENTIFIER);

			eat("]");
			eat("of");

			Node* subType = TypeSpec();
			ArrSpecNode* node = new ArrSpecNode(arrTok, lHandTok, rHandTok, subType);
			return node;
		}
	}

	Node* CompoundSt() {
		Lexem beginTok = getCurLex();
		eat("begin");
		std::vector<Node*> stList = StList();
		eat("end");

		CompoundNode* node = new CompoundNode(beginTok, stList);
		return node;
	}

	std::vector<Node*> StList() {
		Node* st = St();
		std::vector<Node*> stList;
		stList.emplace_back(st);

		while (getCurLex().value == ";") {
			eat(";");
			stList.emplace_back(St());
		}

		return stList;
	}

	Node* St() {
		if (getCurLex().value == "begin") {
			return CompoundSt();
		}
		if (getCurLex().lex_type == Lexem::Type::IDENTIFIER) {
			if (lookForward().value == ":=" || lookForward().value == "[") {
				return AssignSt();
			}
			else if (lookForward().value == "(") {
				return CallSt();
			}
			else {
				eat(Lexem::Type::IDENTIFIER);
				eat(":=");
			}
		}
		if (getCurLex().value == "while")
			return WhileSt();
		if (getCurLex().value == "if")
			return IfSt();
		if (getCurLex().value == "for")
			return ForSt();
		if (getCurLex().value == "return")
			return ReturnSt();

		return EmptySt();
	}

	Node* EmptySt() {
		Node* node = new Node({ "EMPTY" });
		return node;
	}

	Node* AssignSt() {
		Node* lValue;
		if (lookForward().value == "[")
			lValue = SelectSt();
		else
			lValue = Variable();

		Lexem assignTok = getCurLex();
		eat(":=");

		Node* toAssign = Expression();
		AssignNode* node = new AssignNode(assignTok, lValue, toAssign);
		return node;
	}

	Node* IfSt() {
		Lexem ifTok = getCurLex();
		eat("if");
		Node* condition = Expression();
		eat("then");

		Node* body = St();
		Node* elseBody = nullptr;
		if (getCurLex().value == "else") {
			eat("else");
			elseBody = St();
		}
		IfNode* node = new IfNode(ifTok, condition, body, elseBody);
		return node;
	}

	Node* WhileSt() {
		Lexem whileTok = getCurLex();
		eat("while");
		Node* condition = Expression();
		eat("do");
		Node* body = St();
		WhileNode* node = new WhileNode(whileTok, condition, body);
		return node;
	}

	Node* ForSt() {
		Lexem tok = getCurLex();
		eat("for");
		Node* iterSt = IterSt();
		eat("do");
		Node* body = St();
		ForNode* node = new ForNode(tok, iterSt, body);
		return node;
	}

	Node* CallSt() {
		Lexem tok = getCurLex();
		eat(Lexem::Type::IDENTIFIER);
		eat("(");
		std::vector<Node*> params;
		if (getCurLex().value != ")") {
			Node* param = Expression();
			params.emplace_back(param);
		}
		while (getCurLex().value == ",") {
			eat(",");
			Node* param = Expression();
			params.emplace_back(param);
		}

		eat(")");
		CallFuncNode* node = new CallFuncNode(tok, params);
		return node;
	}

	Node* ReturnSt() {
		Lexem tok = getCurLex();
		eat("return");
		Node* toReturn = Expression();
		ReturnNode* node = new ReturnNode(tok, toReturn);
		return node;
	}

	Node* Variable() {
		VarNode* node = new VarNode(getCurLex());
		eat(Lexem::Type::IDENTIFIER);
		return node;
	}

	Node* SelectSt() {
		Lexem tok = getCurLex();
		Node* var = Variable();
		std::vector<Node*> selections;
		while (getCurLex().value == "[") {
			eat("[");
			Node* index = Expression();
			eat("]");
			selections.emplace_back(index);
		}
		if (selections.size() != 1) {
			for (int i = 0; i < selections.size() - 1; i++) {
				var = new SelectNode(tok, var, selections[i]);
			}
		}
		SelectNode *node = new SelectNode(tok, var, selections[selections.size() - 1]);
		return node;
	}

	Node* Expression() {
		Node* simpleExpession = SimpleExpression();
		Lexem token;

		std::string operators[] = { "<", "<=", "<>", "=", ">", ">=" };

		if (std::find(std::begin(operators), std::end(operators), getCurLex().value) != std::end(operators)) {
			token = getCurLex();
			eat(Lexem::Type::OPERATOR);
			BinOpNode* node = new BinOpNode(token, simpleExpession, SimpleExpression());
		}
		else {
			return simpleExpession;
		}
	}

	Node* SimpleExpression() {
		Node* term = Term();

		std::string operators[] = { "+", "-", "or"};

		while (std::find(std::begin(operators), std::end(operators), getCurLex().value) != std::end(operators)) {
			Lexem tok = getCurLex();
			eat(tok.lex_type);

			term = new BinOpNode(tok, term, Term());
		}

		return term;
	}

	Node* Term() {
		Node* factor = Factor();

		std::string operators[] = { "/", "*", "and" };

		while (std::find(std::begin(operators), std::end(operators), getCurLex().value) != std::end(operators)) {
			Lexem tok = getCurLex();
			eat(tok.value);
			factor = new BinOpNode(tok, factor, Factor());
		}

		return factor;
	}

	Node* Factor() {
		Lexem tok = getCurLex();
		if (tok.lex_type == Lexem::Type::INTEGER_CONST || tok.lex_type == Lexem::Type::REAL_CONST || tok.lex_type == Lexem::Type::STRING_CONST) {
			eat(tok.lex_type);
			Node* node = new Node(tok);
			return node;
		}
		if (tok.value == "(") {
			eat("(");
			Node* node = Expression();
			eat(")");
			return node;
		}
		if (tok.value == "+" || tok.value == "-" || tok.value == "not") {
			eat(tok.value);
			UnOpNode *node = new UnOpNode(tok, Factor());
			return node;
		}
		if (tok.lex_type == Lexem::Type::IDENTIFIER) {
			if (lookForward().value == "(") {
				return CallSt();
			}
			else if (lookForward().value == "[") {
				return SelectSt();
			}
			else {
				return Variable();
			}
		}

		throw std::runtime_error("Unexpected token! " + tok.value);
		return nullptr;
	}

	Node* IterSt() {
		Lexem varTok = getCurLex();

		Node* initSt = AssignSt();

		Lexem toTok = getCurLex();

		if (toTok.value == "downto")
			eat("downto");
		else
			eat("to");

		Lexem finTok = getCurLex();
		if (finTok.lex_type != Lexem::Type::INTEGER_CONST && finTok.lex_type != Lexem::Type::REAL_CONST
			&& finTok.lex_type != Lexem::Type::STRING_CONST && finTok.lex_type != Lexem::Type::IDENTIFIER)
			throw std::runtime_error("Expected const or variable as end value of cycle!");

		Factor();

		Lexem condOp = { "<=" };
		Node* var_cond = new Node(varTok);
		Node* fin_cond = new Node(finTok);
		Node* condition = new BinOpNode(condOp, var_cond, fin_cond);

		Lexem postOp = { "+" };
		if (toTok.value == "downto")
			postOp = { "-" };
		Lexem forOp = { "1" };
		Node* var_postOp = new Node(varTok);
		Node* fin_postOp = new Node(forOp);
		Node* postAction = new BinOpNode(postOp, var_postOp, fin_postOp);

		IterationNode* node = new IterationNode(toTok, initSt, condition, postAction);
		return node;
	}
};