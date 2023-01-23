#pragma once
#include "Lexem.h"
#include "Node.h"

class NodeFactory {
public:
	static std::unique_ptr<Node> createNode(Lexem lex) {
		if (lex.lex_type == Lexem::Type::TYPE_SPEC)
			return std::unique_ptr<Node>(new TypeSpecNode(lex));
		if (lex.lex_type == Lexem::Type::STRING_CONST)
			return std::unique_ptr<Node>(new StringNode(lex));
		if (lex.lex_type == Lexem::Type::INTEGER_CONST)
			return std::unique_ptr<Node>(new NumberNode(lex));
		if (lex.lex_type == Lexem::Type::REAL_CONST)
			return std::unique_ptr<Node>(new NumberNode(lex));
		if (lex.lex_type == Lexem::Type::IDENTIFIER)
			return std::unique_ptr<Node>(new VarNode(lex));
		if (lex.value == "EMPTY")
			return std::unique_ptr<Node>(new NoOpNode(lex));

		throw std::runtime_error("Factory class exception at " +
			std::to_string(lex.line_id) + ":" + std::to_string(lex.id));
	}

	static std::unique_ptr<Node> createNode(Lexem lex, std::unique_ptr<Node> first) {
		if (lex.lex_type == Lexem::Type::OPERATOR)
			return std::unique_ptr<Node>(new UnOpNode(lex, std::move(first)));
		if (lex.value == "return")
			return std::unique_ptr<Node>(new ReturnNode(lex, std::move(first)));

		throw std::runtime_error("Factory class exception at " +
			std::to_string(lex.line_id) + ":" + std::to_string(lex.id));
	}

	static std::unique_ptr<Node> createNode(Lexem lex, std::unique_ptr<Node> first, std::unique_ptr<Node> second) {
		if (lex.value == ":=")
			return std::unique_ptr<Node>(new AssignNode(lex, std::move(first), std::move(second)));
		if (lex.value == "while")
			return std::unique_ptr<Node>(new WhileNode(lex, std::move(first), std::move(second)));
		if (lex.value == "for")
			return std::unique_ptr<Node>(new ForNode(lex, std::move(first), std::move(second)));
		if (lex.lex_type == Lexem::Type::OPERATOR)
			return std::unique_ptr<Node>(new BinOpNode(lex, std::move(first), std::move(second)));
		if (lex.value == "VAR_DECL")
			return std::unique_ptr<Node>(new BinOpNode(lex, std::move(first), std::move(second)));
		if (lex.lex_type == Lexem::Type::SELECT) {
			lex.value = "SELECT";
			return std::unique_ptr<Node>(new VarDeclNode(lex, std::move(first), std::move(second)));
		}
		if (lex.value == "CONSTDECL") {
			return std::unique_ptr<Node>(new VarDeclNode(lex, std::move(first), std::move(second)));
		}

		throw std::runtime_error("Factory class exception at " +
			std::to_string(lex.line_id) + ":" + std::to_string(lex.id));
	}

	static std::unique_ptr<Node> createNode(Lexem lex, std::unique_ptr<Node> first, std::unique_ptr<Node> second, std::unique_ptr<Node> third) {
		if (lex.value == "if")
			return std::unique_ptr<Node>(new IfNode(lex, std::move(first), std::move(second), std::move(third)));
		if (lex.value == "to")
			return std::unique_ptr<Node>(new IterationNode(lex, std::move(first), std::move(second), std::move(third)));
		if (lex.value == "downto")
			return std::unique_ptr<Node>(new IterationNode(lex, std::move(first), std::move(second), std::move(third)));

		throw std::runtime_error("Factory class exception at " +
			std::to_string(lex.line_id) + ":" + std::to_string(lex.id));
	}

	static std::unique_ptr<Node> createNode(Lexem lex, std::vector<std::unique_ptr<Node>> params) {
		if (lex.value == "COMPOUND") {
			return std::unique_ptr<CompoundNode>(new CompoundNode(lex, std::move(params)));
		}
		if (lex.lex_type == Lexem::Type::FUNCTION_NAME) {
			return std::unique_ptr<Node>(new CallFuncNode(lex, std::move(params)));
		}

		throw std::runtime_error("Factory class exception at " +
			std::to_string(lex.line_id) + ":" + std::to_string(lex.id));
	}

	static std::unique_ptr<Node> createNode(Lexem lex, std::vector<std::unique_ptr<Node>> params, std::unique_ptr<Node> first) {
		if (lex.lex_type == Lexem::Type::PROGRAM_NAME) {
			return std::unique_ptr<ProgramNode>(new ProgramNode(lex, std::move(params), std::move(first)));
		}

		throw std::runtime_error("Factory class exception at " +
			std::to_string(lex.line_id) + ":" + std::to_string(lex.id));
	}

	static std::unique_ptr<Node> createNode(Lexem lex, std::vector<std::unique_ptr<Node>> params, std::unique_ptr<Node> first, std::unique_ptr<Node> second) {
		if (lex.lex_type == Lexem::Type::FUNCTION_NAME)
			return std::unique_ptr<Node>(new FunctionNode(lex, std::move(params), std::move(first), std::move(second)));
		
		throw std::runtime_error("Factory class exception at " +
			std::to_string(lex.line_id) + ":" + std::to_string(lex.id));
	}

	static std::unique_ptr<Node> createNode(Lexem lex, std::vector<std::unique_ptr<Node>> constDecls, std::vector<std::unique_ptr<Node>> varDecls, std::unique_ptr<Node> compound) {
		if (lex.value == "BLOCK")
			return std::unique_ptr<Node>(new BlockNode(lex, std::move(constDecls), std::move(varDecls), std::move(compound)));

		throw std::runtime_error("Factory class exception at " +
			std::to_string(lex.line_id) + ":" + std::to_string(lex.id));
	}

	static std::unique_ptr<Node> createNode(Lexem firstTok, Lexem secondTok, Lexem thirdTok, std::unique_ptr<Node> first) {
		if (firstTok.value == "array")
			return std::unique_ptr<Node>(new ArrSpecNode(firstTok, secondTok, thirdTok, std::move(first)));
		throw std::runtime_error("Factory class exception!");
	}

	static std::unique_ptr<Node> copyType(Node* ptr) {
		if (ptr->token.lex_type == Lexem::Type::TYPE_SPEC)
			return createNode(ptr->token);
		if (ptr->token.value == "array") {
			ArrSpecNode* rawPtr = static_cast<ArrSpecNode*>(ptr);
			return std::unique_ptr<Node>(createNode(rawPtr->token, rawPtr->lHandTok, rawPtr->rHandTok, copyType(rawPtr->subType.get())));
		}

		throw std::runtime_error("Using copyType() for non-type node!");
	}
};