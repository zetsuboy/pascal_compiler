#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Lexem.h"
#include <map>

class Lexer {
private:
	std::vector<Lexem> lexems;
	std::vector<char> HexadecimalChars = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
	std::vector<char> DecimalChars = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
	std::vector<char> OctalChars = { '0', '1', '2', '3', '4', '5', '6', '7' };
	std::vector<char> BinaryChars = { '0', '1' };
	std::map<char, int> CharsToInt = { {'0', 0}, {'1', 1}, {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7}, {'8', 8},  {'9', 9}, {'a', 10}, {'b', 11},  {'c', 12},  {'d', 13},  {'e', 14},  {'f', 15} };

	std::string input = "";
	std::string buf = "";
	std::vector<char> current_system = DecimalChars;
	int current_base = 10;
	long long num_buf = 0, double_counter = 1;
	double double_buf = 0;
	int lex_id = 0;
	int line_id = 1;
	int line_skip_count = 0;
	int i = 0;
	enum States { S, NUM, DOUBLE, STR, SYMBOL, ID, ER, SINGLE_COM, MULTI_COM };

	std::map<std::string, Lexem::Type> DelimiterToType = { {"(", Lexem::Type::DELIMITER}, {")", Lexem::Type::DELIMITER},
		{"{", Lexem::Type::DELIMITER}, {"}", Lexem::Type::DELIMITER}, {"[", Lexem::Type::DELIMITER}, {"]", Lexem::Type::DELIMITER},
		{"'", Lexem::Type::DELIMITER}, {".", Lexem::Type::DELIMITER}, {";", Lexem::Type::DELIMITER}, {",", Lexem::Type::DELIMITER},
		{":", Lexem::Type::DELIMITER} };
	std::map<std::string, Lexem::Type> OperatorToType = { {"+", Lexem::Type::OPERATOR}, {"-", Lexem::Type::OPERATOR}, {"*", Lexem::Type::OPERATOR},
		{"/", Lexem::Type::OPERATOR}, {"=", Lexem::Type::OPERATOR}, {"!=", Lexem::Type::OPERATOR}, {"<", Lexem::Type::OPERATOR}, {"<=", Lexem::Type::OPERATOR},
		{">", Lexem::Type::OPERATOR}, {">=", Lexem::Type::OPERATOR}, {":=", Lexem::Type::OPERATOR} };
	std::map<std::string, Lexem::Type> WordToType = { {"begin", Lexem::Type::WORD}, {"end", Lexem::Type::WORD}, {"return", Lexem::Type::WORD},
		{"program", Lexem::Type::WORD}, {"var", Lexem::Type::WORD}, {"const", Lexem::Type::WORD}, {"while", Lexem::Type::WORD},
		{"do", Lexem::Type::WORD}, {"for", Lexem::Type::WORD}, {"range", Lexem::Type::WORD}, {"if", Lexem::Type::WORD}, {"then", Lexem::Type::WORD},
		{"else", Lexem::Type::WORD}, {"and", Lexem::Type::WORD}, {"not", Lexem::Type::WORD}, {"or", Lexem::Type::WORD}, {"array", Lexem::Type::WORD},
		{"function", Lexem::Type::WORD}, {"procedure", Lexem::Type::WORD}, {"integer", Lexem::Type::WORD},
		{"double", Lexem::Type::WORD}, {"string", Lexem::Type::WORD} };
	
	int state = S;

public:
	Lexer(std::string path) {
		std::ifstream infile(path);
		if (infile) {
			std::stringstream str;
			str << infile.rdbuf();
			input = str.str();
			input += " ";
			infile.close();
		}
	}

	std::vector<Lexem> getLexems() {
		while (i < input.size()) {
			switch (state) {
			case S: {
				lex_id = i + 1;
				if (input[i] == ' ' || input[i] == '\t' || input[i] == '\0' || input[i] == '\r')
					i++;
				else if (input[i] == '\n') {
					line_id++;
					i++;
					line_skip_count = i;
				}
				else if (isalpha(input[i])) {
					buf = input[i];
					state = ID;
					i++;
				}
				else if (isdigit(input[i])) {
					buf = input[i];
					state = NUM;
					current_system = DecimalChars;
					current_base = 10;
					num_buf = (int)(input[i] - '0');
					i++;
				}
				else if (input[i] == '$') {
					state = NUM;
					buf = input[i];
					current_system = HexadecimalChars;
					current_base = 16;
					num_buf = 0;
					i++;
				}
				else if (input[i] == '&') {
					state = NUM;
					buf = input[i];
					current_system = OctalChars;
					current_base = 8;
					num_buf = 0;
					i++;
				}
				else if (input[i] == '%') {
					state = NUM;
					buf = input[i];
					current_system = BinaryChars;
					current_base = 2;
					num_buf = 0;
					i++;
				}
				else if (input[i] == '{') {
					state = MULTI_COM;
					i++;
				}
				else if (input[i] == '/' && input[i + 1] == '/') {
					state = SINGLE_COM;
					i += 2;
				}
				else if (input[i] == '\'') {
					state = STR;
					i++;
					buf = "";
				}
				else {
					state = SYMBOL;
				}
				break;
			}
			case ID: {
				if (isalnum(input[i]) || input[i] == '_') {
					buf += input[i];
					i++;
				}
				else {
					state = S;
					std::map<std::string, Lexem::Type>::iterator it = WordToType.find(buf);
					if (it != WordToType.end()) {
						lexems.push_back({ lex_id - line_skip_count, line_id, it->second, buf });
					}
					else {
						lexems.push_back({ lex_id - line_skip_count, line_id, Lexem::Type::IDENTIFIER, buf });
					}
				}
				break;
			}
			case NUM: {
				std::string sss(1, input[i]);
				if (isdigit(input[i])) {
					num_buf = num_buf * current_base + CharsToInt[input[i]];
					buf += input[i];
					if (num_buf > INT_MAX && !(buf == "2147483648" && lexems[lexems.size() - 1].value == "-")) {
						state = ER;
					}
					i++;
				}
				else if (input[i] == '.') {
					if (current_base != 10) {
						state = ER;
						break;
					}
					try {
						if (input[i + 1] == '.') {
							lexems.push_back({ lex_id - line_skip_count, line_id, Lexem::Type::INTEGER_CONST, std::to_string(num_buf), buf });
							state = S;
							break;
						}
					}
					catch (std::exception) {}
					double_buf = num_buf;
					i++;
					state = DOUBLE;
					double_counter = 10;
				}
				else {
					lexems.push_back({ lex_id - line_skip_count, line_id, Lexem::Type::INTEGER_CONST, std::to_string(num_buf), buf });
					state = S;
				}
				break;
			}
			case DOUBLE: {
				std::string sss(1, input[i]);
				if (isdigit(input[i])) {
					double_buf = double_buf + (double)(input[i] - '0') / double_counter;
					double_counter *= 10;
					i++;
				}
				else {
					state = S;
					lexems.push_back({ lex_id - line_skip_count, line_id, Lexem::Type::REAL_CONST, std::to_string(double_buf) });
				}
				break;
			}
			case STR: {
				while (input[i] != '\'') {
					buf += input[i];
					i++;
				}
				lexems.push_back({ lex_id - line_skip_count, line_id, Lexem::Type::STRING_CONST, buf });
				i++;
				state = S;
				break;
			}
			case SYMBOL: {
				state = S;
				buf = input[i];
				std::string double_operator_buf = buf;
				i++;
				double_operator_buf += input[i];
				std::map<std::string, Lexem::Type>::iterator it = OperatorToType.find(double_operator_buf);
				if (it != OperatorToType.end()) {
					buf += input[i];
					i++;
					lexems.push_back({ lex_id - line_skip_count, line_id, it->second, double_operator_buf });
					break;
				}
				else {
					if ((it = OperatorToType.find(buf)) != OperatorToType.end())
						lexems.push_back({ lex_id - line_skip_count, line_id, it->second, buf });
					else if ((it = DelimiterToType.find(buf)) != DelimiterToType.end())
						lexems.push_back({ lex_id - line_skip_count, line_id, it->second, buf });
					else
						state = ER;
					break;
				}
			}
			case SINGLE_COM: {
				if (input[i] == '\n')
				{
					state = S;
					line_id++;
				}
				i++;
				break;
			}
			case MULTI_COM: {
				if (input[i] == '}')
					state = S;
				if (input[i] == '\n')
					line_id++;
				i++;
				break;
			}
			case ER: {
				std::string error_string = "Lexer error at line " + std::to_string(line_id) + ":" + std::to_string(lex_id - line_skip_count) + " buf: " + buf;
				throw std::runtime_error(error_string);
				exit(1);
			}
			}
		}
		return lexems;
	}
};