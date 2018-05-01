#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include "table_driven.h" 

using namespace std;
 
/* 파일을 문자열의 벡터로 바꾸어주는 클래스*/
class Tokenizer {

 private:
	vector <string> tokens;
	string filePath;
 public:

	Tokenizer(string filePath) {
		this->filePath = filePath;
	}

	void ToTokens() {

		ifstream stream(filePath, ios::in);
		string word;

		while (stream >> word) {
			tokens.push_back(word);
		}

		stream.close();
	}

	vector<string> GetTokens() {
		return tokens;
	}
};
 

class DFA {

private:
	vector<string> tokens;
	Table table;
	vector<string> result;
	int count_result;

public:

	DFA(vector<string> tokens, Table table) {
		this->tokens = tokens;
		this->table = table;
	}

	~DFA() {

	}

	void run() {

		int size = tokens.size();
		State state = table.start_state();
		string temp = "";

		for (int i = 0; i < size; i++) {

			State newState = table.get_next(state, tokens[i]);

			if (newState != table.start_state()) {
				temp.append(tokens[i]);
				temp.append(" "); //공백 추가
			}

			else {//new_state == START
				if (table.is_accept(state)) {
					result.push_back(temp);
				}
				temp.clear();
			}

			if (table.is_accept(newState) && !temp.empty()) {
				result.push_back(temp);
				temp.clear();
			}

			state = newState;
		}

	}

	void print_result() {

		if (result.empty()) {
			cout << "result is empty" << endl;
			return;
		}

		int size = result.size();

		for (int i = 0; i < size; i++) {
			cout << result[i] << endl;
		}
		cout << result.size() << endl;
	}

	void write_to_file(string filePath) {

		ofstream stream;
		stream.open(filePath, ios::trunc);

		int size = result.size();

		int j = 1;

		for (int i = 0; i < size; i++) {

			if (j > 20) {
				break;
			}

			string temp = "(" + to_string(j) + ")";

			if (!result[i].empty()) {
				stream << temp + result[i] << endl;
				j++;
			}
		}
		string temp = "total data pattern: ";
		stream << temp + to_string(result.size());
	}
};


enum SYMBOLS {
	ADD, SUB, MUL, DIV, MOD,
	EQUAL, MORE, LESS, AND, OR,
	SEMI, COM,
	LEFT_PAREN, RIGHT_PAREN, LEFT_CURL, RIGHT_CURL, LEFT_SQUARE, RIGHT_SQUARE,

	ALPHABET, LETTER,
	DIGIT, NON_ZERO, HEX,
	BACK_SLASH, QUOTE, DOUBLE_QUOTE

};

enum STATES {
	IN_ASSIGN, IN_MORE, IN_LESS, IN_NOT, IN_ID,
	IN_CHAR, IN_CHAR2, ESCAPE_CHAR,
	IN_STRING, ESCAPE_STRING,
	IN_ZERO, IN_DECIMAL, IN_HEX,

	ACC_ADD, ACC_SUB, ACC_MUL, ACC_DIV, ACC_MOD,
	ACC_EQUAL, ACC_MORE, ACC_EQUAL_MORE, ACC_LESS, ACC_EQUAL_LESS,
	ACC_NOT, ACC_NOT_EQUAL, ACC_AND, ACC_OR,

	ACC_LEFT_PAREN, ACC_RIGHT_PAREN, ACC_LEFT_CURL, ACC_RIGHT_CURL, ACC_LEFT_SQUARE, ACC_RIGHT_SQUARE,

	ACC_ID, ACC_CHAR, ACC_STRING,
	ACC_DECIMAL, ACC_HEX, ACC_ZERO
};

int main() {


	Tokenizer tokenizer("lexer_test.txt");
	tokenizer.ToTokens();

	vector<string> tokens = tokenizer.GetTokens();
 
	const vector<string> specialSymbol = { "+", "-", "/", "%",
		"=", ">", "<", "&", "|",
		";", ",",
		"(", ")", "{", "}", "[", "]"
	};

	const vector<string> symbolName = {
		"ADD", "SUB", "MUL", "DIV", "MOD",
		"EQUAL", "MORE", "LESS", "AND", "OR",
		"SEMI", "COM",
		"LEFT_PAREN", "RIGHT_PAREN", "LEFT_CURL", "RIGHT_CURL", "LEFT_SQUARE", "RIGHT_SQUARE",

		"ALPHABET", "LETTER",
		"DIGIT", "NON_ZERO", "HEX",
		"BACK_SLASH", "QUOTE", "DOUBLE_QUOTE"

	};

	vector <SymbolSet>  symbolSets;

	for (int i = SYMBOLS::ADD; i < SYMBOLS::RIGHT_SQUARE; i++) {
		SymbolSet temp(symbolName[i], i, specialSymbol[i]);
		symbolSets.push_back(temp);
	}

	vector<string> alphabetSymbols;
	vector<string> digitSymbols;
	vector<string> nonzeroSymbols;

	for (char i = 'A'; i <= 'Z'; i++) {

		string capital(1, i);
		string small(1, i + ' ');

		alphabetSymbols.push_back(capital);
		alphabetSymbols.push_back(small);
		
	}

	for (int i = 0; i <= 9; i++) {
		char c = i - '0';
		string s(1, c);

		digitSymbols.push_back(s);

		if (i > 0) {
			nonzeroSymbols.push_back(s);
		}
	}

	vector<string> letterSymbols(digitSymbols.size() + alphabetSymbols.size());
	letterSymbols.insert(letterSymbols.end(), alphabetSymbols.begin(), alphabetSymbols.end());
	letterSymbols.insert(letterSymbols.end(), digitSymbols.begin(), digitSymbols.end());


	getchar();


}
