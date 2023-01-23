#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Lexem.h"
#include <map>
#include "Node.h"
#include "Lexem.h"
#include "NodeFactory.h"

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
			throw std::runtime_error("Syntax error! Expected " + getCurLex().TypeToString[(int)type] + " at " + 
				std::to_string(getCurLex().line_id) + ":" + std::to_string(getCurLex().id));
		}
	}

	void eat(std::string word) {
		if (getCurLex().value == word) {
			tokenId++;
		}
		else {
			throw std::runtime_error("Syntax error! Expected " + word + " at " +
				std::to_string(getCurLex().line_id) + ":" + std::to_string(getCurLex().id));
		}
	}

	Lexem& lookForward() {
		if (tokenId + 1 >= lexems.size())
			throw std::runtime_error("Error! Can't look forward - end of lexems");
		
		return lexems[tokenId + 1];
	}

	std::unique_ptr<Node> analyze() {
		std::unique_ptr<Node> root = std::move(Program());
		return std::move(root);
	}

	std::unique_ptr<Node> Program() {
		Lexem program_name;
		if (getCurLex().value == "program") {
			eat("program");
			program_name = getCurLex();
			eat(Lexem::Type::IDENTIFIER);
			eat(";");
		}
		else {
			program_name = { "UNKNOWN_NAME" };
		}

		std::vector<std::unique_ptr<Node>> functions;
		while (getCurLex().value == "procedure" || getCurLex().value == "function") {
			functions.emplace_back(std::move(FuncDef()));
			eat(";");
		}

		std::unique_ptr<Node> block = std::move(Block());
		eat(".");
		program_name.lex_type = Lexem::Type::PROGRAM_NAME;
		return NodeFactory::createNode(program_name, std::move(functions), std::move(block));
	}

	std::unique_ptr<Node> FuncDef() {
		Lexem funcLex = getCurLex();
		if (funcLex.value == "procedure")
			eat("procedure");
		else
			eat("function");

		Lexem name = getCurLex();
		eat(Lexem::Type::IDENTIFIER);
		eat("(");

		std::vector<std::unique_ptr<Node>> params;
		while (getCurLex().value != ")") {
			std::vector<std::unique_ptr<Node>> sup = std::move(VarDecl());
			for (auto &ptr : sup)
				params.emplace_back(std::move(ptr));
			if (getCurLex().value == ";")
				eat(";");
		}
		eat(")");

		std::unique_ptr<Node> returnType;
		if (funcLex.value == "function") {
			eat(":");
			returnType = std::move(TypeSpec());
		}
		else {
			Lexem typeLex = { "void" };
			typeLex.lex_type = Lexem::Type::TYPE_SPEC;
			returnType = NodeFactory::createNode(typeLex);
		}
		eat(";");
		std::unique_ptr<Node> body = std::move(Block());

		name.lex_type = Lexem::Type::FUNCTION_NAME;
		return NodeFactory::createNode(name, std::move(params), std::move(returnType), std::move(body));
	}

	std::unique_ptr<Node> Block() {
		std::vector<std::unique_ptr<Node>> consts = std::move(Consts());
		std::vector<std::unique_ptr<Node>> decls = std::move(Vars());
		std::unique_ptr<Node> compound = std::move(CompoundSt());

		return NodeFactory::createNode({ "BLOCK" }, std::move(consts), std::move(decls), std::move(compound));
	}

	std::vector<std::unique_ptr<Node>> Consts() {
		std::vector<std::unique_ptr<Node>> cons;
		if (getCurLex().value == "const") {
			eat("const");
			while (getCurLex().lex_type == Lexem::Type::IDENTIFIER) {
				cons.emplace_back(std::move(ConstDecl()));
				eat(";");
			}
		}
		return cons;
	}

	std::unique_ptr<Node> ConstDecl() {
		std::unique_ptr<Node> name = NodeFactory::createNode(getCurLex());
		eat(Lexem::Type::IDENTIFIER);
		eat("=");

		Lexem val = getCurLex();
		std::unique_ptr<Node> valPtr = std::move(Expression());

		val.value = "CONSTDECL";

		return NodeFactory::createNode(val, std::move(name), std::move(valPtr));
	}

	std::vector<std::unique_ptr<Node>> Vars() {
		std::vector<std::unique_ptr<Node>> vars;
		if (getCurLex().value == "var") {
			eat("var");
			while (getCurLex().lex_type == Lexem::Type::IDENTIFIER) {
				std::vector<std::unique_ptr<Node>> varDeclList = std::move(VarDecl());
				for (auto &varDeclPTR : varDeclList)
					vars.emplace_back(std::move(varDeclPTR));
				eat(";");
			}
		}
		return vars;
	}

	std::vector<std::unique_ptr<Node>> VarDecl() {
		std::vector<std::unique_ptr<Node>> varsList;
		Lexem tok = getCurLex();
		varsList.emplace_back(NodeFactory::createNode(tok));
		eat(Lexem::Type::IDENTIFIER);

		while (getCurLex().value == ",") {
			eat(",");
			Lexem tok = getCurLex();
			varsList.emplace_back(NodeFactory::createNode(tok));
			eat(Lexem::Type::IDENTIFIER);
		}
		eat(":");

		std::unique_ptr<Node> typeNode = std::move(TypeSpec());

		std::vector<std::unique_ptr<Node>> varDeclsList;
		for (auto &var : varsList) {
			varDeclsList.emplace_back(NodeFactory::createNode({ "VAR_DECL" }, std::move(var), NodeFactory::copyType(typeNode.get())));
		}
		return varDeclsList;
	}

	std::unique_ptr<Node> TypeSpec() {
		Lexem typeTok = getCurLex();

		if (typeTok.value != "array") {
			eat(Lexem::Type::TYPE_SPEC);
			return NodeFactory::createNode(typeTok);
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

			std::unique_ptr<Node> subType = std::move(TypeSpec());
			return NodeFactory::createNode(arrTok, lHandTok, rHandTok, std::move(subType));
		}
	}

	std::unique_ptr<Node> CompoundSt() {
		Lexem beginTok = getCurLex();
		eat("begin");
		std::vector<std::unique_ptr<Node>> stList = std::move(StList());
		eat("end");
		
		beginTok.value = "COMPOUND";
		return NodeFactory::createNode(beginTok, std::move(stList));
	}

	std::vector<std::unique_ptr<Node>> StList() {
		std::unique_ptr<Node> st = std::move(St());
		std::vector<std::unique_ptr<Node>> stList;
		stList.emplace_back(std::move(st));

		while (getCurLex().value == ";") {
			eat(";");
			stList.emplace_back(std::move(St()));
		}

		return stList;
	}

	std::unique_ptr<Node> St() {
		if (getCurLex().value == "begin") {
			return std::move(CompoundSt());
		}
		if (getCurLex().lex_type == Lexem::Type::IDENTIFIER) {
			if (lookForward().value == ":=" || lookForward().value == "[") {
				return std::move(AssignSt());
			}
			else if (lookForward().value == "(") {
				return std::move(CallSt());
			}
			else {
				eat(Lexem::Type::IDENTIFIER);
				eat(":=");
			}
		}
		if (getCurLex().value == "while")
			return std::move(WhileSt());
		if (getCurLex().value == "if")
			return std::move(IfSt());
		if (getCurLex().value == "for")
			return std::move(ForSt());
		if (getCurLex().value == "return")
			return std::move(ReturnSt());

		return std::move(EmptySt());
	}

	std::unique_ptr<Node> EmptySt() {
		return NodeFactory::createNode({ "EMPTY" });
	}

	std::unique_ptr<Node> AssignSt() {
		std::unique_ptr<Node> lValue;
		if (lookForward().value == "[")
			lValue = std::move(SelectSt());
		else
			lValue = std::move(Variable());

		Lexem assignTok = getCurLex();
		eat(":=");

		std::unique_ptr<Node> toAssign = std::move(Expression());
		return NodeFactory::createNode(assignTok, std::move(lValue), std::move(toAssign));
	}

	std::unique_ptr<Node> IfSt() {
		Lexem ifTok = getCurLex();
		eat("if");
		std::unique_ptr<Node> condition = std::move(Expression());
		eat("then");

		std::unique_ptr<Node> body = std::move(St());
		std::unique_ptr<Node> elseBody = nullptr;
		if (getCurLex().value == "else") {
			eat("else");
			elseBody = std::move(St());
		}
		return NodeFactory::createNode(ifTok, std::move(condition), std::move(body), std::move(elseBody));
	}

	std::unique_ptr<Node> WhileSt() {
		Lexem whileTok = getCurLex();
		eat("while");
		std::unique_ptr<Node> condition = std::move(Expression());
		eat("do");
		std::unique_ptr<Node> body = std::move(St());
		return NodeFactory::createNode(whileTok, std::move(condition), std::move(body));
	}

	std::unique_ptr<Node> ForSt() {
		Lexem tok = getCurLex();
		eat("for");
		std::unique_ptr<Node> iterSt = std::move(IterSt());
		eat("do");
		std::unique_ptr<Node> body = std::move(St());
		return NodeFactory::createNode(tok, std::move(iterSt), std::move(body));
	}

	std::unique_ptr<Node> CallSt() {
		Lexem tok = getCurLex();
		eat(Lexem::Type::IDENTIFIER);
		eat("(");
		std::vector<std::unique_ptr<Node>> params;
		if (getCurLex().value != ")") {
			std::unique_ptr<Node> param = std::move(Expression());
			params.emplace_back(std::move(param));
		}
		while (getCurLex().value == ",") {
			eat(",");
			std::unique_ptr<Node> param = std::move(Expression());
			params.emplace_back(std::move(param));
		}

		eat(")");
		tok.lex_type = Lexem::Type::FUNCTION_NAME;
		return NodeFactory::createNode(tok, std::move(params));
	}

	std::unique_ptr<Node> ReturnSt() {
		Lexem tok = getCurLex();
		eat("return");
		std::unique_ptr<Node> toReturn = std::move(Expression());
		return NodeFactory::createNode(tok, std::move(toReturn));
	}

	std::unique_ptr<Node> Variable() {
		std::unique_ptr<Node> node = NodeFactory::createNode(getCurLex());
		eat(Lexem::Type::IDENTIFIER);
		return std::move(node);
	}

	std::unique_ptr<Node> SelectSt() {
		Lexem tok = getCurLex();
		tok.lex_type = Lexem::Type::SELECT;
		std::unique_ptr<Node> var = std::move(Variable());
		std::vector<std::unique_ptr<Node>> selections;
		while (getCurLex().value == "[") {
			eat("[");
			std::unique_ptr<Node> index = std::move(Expression());
			eat("]");
			selections.emplace_back(std::move(index));
		}
		if (selections.size() != 1) {
			for (int i = 0; i < selections.size() - 1; i++) {
				var = NodeFactory::createNode(tok, std::move(var), std::move(selections[i]));
			}
		}

		return NodeFactory::createNode(tok, std::move(var), std::move(selections[selections.size() - 1]));
	}

	std::unique_ptr<Node> Expression() {
		std::unique_ptr<Node> simpleExpession = std::move(SimpleExpression());
		Lexem token;

		std::string operators[] = { "<", "<=", "<>", "=", ">", ">=" };

		if (std::find(std::begin(operators), std::end(operators), getCurLex().value) != std::end(operators)) {
			token = getCurLex();
			eat(Lexem::Type::OPERATOR);
			return NodeFactory::createNode(token, std::move(simpleExpession), std::move(SimpleExpression()));
		}
		else {
			return std::move(simpleExpession);
		}
	}

	std::unique_ptr<Node> SimpleExpression() {
		std::unique_ptr<Node> term = std::move(Term());

		std::string operators[] = { "+", "-", "or"};

		while (std::find(std::begin(operators), std::end(operators), getCurLex().value) != std::end(operators)) {
			Lexem tok = getCurLex();
			eat(tok.lex_type);

			term = NodeFactory::createNode(tok, std::move(term), std::move(Term()));
		}

		return std::move(term);
	}

	std::unique_ptr<Node> Term() {
		std::unique_ptr<Node> factor = std::move(Factor());

		std::string operators[] = { "/", "*", "and" };

		while (std::find(std::begin(operators), std::end(operators), getCurLex().value) != std::end(operators)) {
			Lexem tok = getCurLex();
			eat(tok.value);

			factor = NodeFactory::createNode(tok, std::move(factor), std::move(Factor()));
		}

		return std::move(factor);
	}

	std::unique_ptr<Node> Factor() {
		Lexem tok = getCurLex();
		if (tok.lex_type == Lexem::Type::INTEGER_CONST || tok.lex_type == Lexem::Type::REAL_CONST || tok.lex_type == Lexem::Type::STRING_CONST) {
			eat(tok.lex_type);
			std::unique_ptr<Node> node = NodeFactory::createNode(tok);
			return std::move(node);
		}
		if (tok.value == "(") {
			eat("(");
			std::unique_ptr<Node> node = std::move(Expression());
			eat(")");
			return std::move(node);
		}
		if (tok.value == "+" || tok.value == "-" || tok.value == "not") {
			eat(tok.value);
			std::unique_ptr<Node> node = NodeFactory::createNode(tok, std::move(Factor()));
			return std::move(node);
		}
		if (tok.lex_type == Lexem::Type::IDENTIFIER) {
			if (lookForward().value == "(") {
				return std::move(CallSt());
			}
			else if (lookForward().value == "[") {
				return std::move(SelectSt());
			}
			else {
				return std::move(Variable());
			}
		}

		throw std::runtime_error("Unexpected token! " + tok.value);
		//return nullptr;
	}

	std::unique_ptr<Node> IterSt() {
		Lexem varTok = getCurLex();

		std::unique_ptr<Node> initSt = std::move(AssignSt());

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
		std::unique_ptr<Node> var_cond = NodeFactory::createNode(varTok);
		std::unique_ptr<Node> fin_cond = NodeFactory::createNode(finTok);
		std::unique_ptr<Node> condition = NodeFactory::createNode(condOp, std::move(var_cond), std::move(fin_cond));

		Lexem postOp = { "+" };
		if (toTok.value == "downto")
			postOp = { "-" };
		Lexem forOp = { "1" };
		std::unique_ptr<Node> var_postOp = NodeFactory::createNode(varTok);
		std::unique_ptr<Node> fin_postOp = NodeFactory::createNode(forOp);
		std::unique_ptr<Node> postAction = NodeFactory::createNode(postOp, std::move(var_postOp), std::move(fin_postOp));

		return NodeFactory::createNode(toTok, std::move(initSt), std::move(condition), std::move(postAction));
	}
};