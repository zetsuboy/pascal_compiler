#pragma once
#include <iostream>
#include <regex>


class Lexem {
public:
	enum class Type {
		INTEGER_CONST, REAL_CONST, STRING_CONST, IDENTIFIER, DELIMITER, OPERATOR, WORD, TYPE_SPEC, SELECT, FUNCTION_NAME, PROGRAM_NAME
	};

	const std::string TypeToString[8] = { "Integer_const", "Real_const", "String_const", "Identifier", "Delimiter", "Operator", "Word" , "Type_spec"};

	int id;
	int line_id;
	Type lex_type;
	std::string value;
	std::string default_value = "";

	Lexem &operator=(const Lexem &other) {
		id = other.id;
		line_id = other.line_id;
		lex_type = other.lex_type;
		value = other.value;
		default_value = other.default_value;
		return *this;
	}

	Lexem() {};

	Lexem(std::string val) {
		value = val;
	}

	Lexem(int _id, int _line_id, Type _lex_type, std::string _value) {
		id = _id;
		line_id = _line_id;
		lex_type = _lex_type;
		value = _value;
	}

	Lexem(int _id, int _line_id, Type _lex_type, std::string _value, std::string _default_value) {
		id = _id;
		line_id = _line_id;
		lex_type = _lex_type;
		value = _value;
		default_value = _default_value;
	}
};