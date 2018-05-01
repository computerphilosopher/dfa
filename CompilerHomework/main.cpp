#include <iostream>
#include <fstream>
#include <streambuf>
#include <vector>
#include <string>
#include <cstring>
#include "table_driven.h" 

using namespace std;
 
/* 파일을 문자열의 벡터로 바꾸어주는 클래스*/
class Tokenizer {

 private:
	vector <string> tokens;
	vector <string> str;
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
	vector<string> words;
	Table table;
	vector<string> result;
	int count_result;

public:

	DFA(vector<string> words, Table table) {
		this->words = words;
		this->table = table;
	}

	~DFA() {

	}

	void run() {

		int wordCount = words.size();
		State state = table.start_state();
		string temp = "";

		for (int i = 0; i < wordCount; i++) {

			int wordLength = words[i].length();

			for (int j = 0; j < wordLength; j++) {

				string s(words[i], j, i);
				State newState = table.get_next(state, s);

				if (newState != table.start_state()) {
					temp.append(s);
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
	EQUAL, MORE, LESS, AND, OR, NOT,
	SEMI, COM, SMALL_X, LARGE_X,
	LEFT_PAREN, RIGHT_PAREN, LEFT_CURL, RIGHT_CURL, LEFT_SQUARE, RIGHT_SQUARE,
	BACK_SLASH, QUOTE, DOUBLE_QUOTE,

	ALPHABET, LETTER,
	DIGIT, ZERO, NON_ZERO, HEX,
	NOT_TOKEN

};

enum STATES {
	START,
	IN_ASSIGN, IN_MORE, IN_LESS, IN_NOT, IN_ID, IN_AND, IN_OR,
	IN_CHAR, IN_CHAR2, ESCAPE_CHAR,
	IN_STRING, ESCAPE_STRING,
	IN_ZERO, IN_DECIMAL, IN_HEX,

	ACC_ADD, ACC_SUB, ACC_MUL, ACC_DIV, ACC_MOD,
	ACC_ASSIGN, ACC_EQUAL, ACC_MORE, ACC_EQUAL_MORE, ACC_LESS, ACC_EQUAL_LESS,
	ACC_NOT, ACC_NOT_EQUAL, ACC_AND, ACC_OR,

	ACC_SEMI, ACC_COM,
	ACC_LEFT_PAREN, ACC_RIGHT_PAREN, ACC_LEFT_CURL, ACC_RIGHT_CURL, ACC_LEFT_SQUARE, ACC_RIGHT_SQUARE,

	ACC_ID, ACC_CHAR, ACC_STRING,
	ACC_DECIMAL, ACC_HEX, ACC_ZERO
};

void table_set(Table &table) {

	for (int i = SYMBOLS::ADD; i <= NOT_TOKEN; i++) {

		State range = START;

		switch (i) {

		case ADD:
			range = ACC_ADD;
			break;
		case SUB:
			range = ACC_SUB;
			break;
		case MUL:
			range = ACC_MUL;
			break;
		case DIV:
			range = ACC_DIV;
			break;
		case MOD:
			range = ACC_MOD;
			break;
		case EQUAL:
			range = IN_ASSIGN;
			break;
		case MORE:
			range = IN_MORE;
			break;
		case LESS:
			range = IN_LESS;
			break;
		case NOT:
			range = IN_NOT;
			break;
		case AND:
			range = IN_AND;
			break;
		case OR:
			range = IN_OR;
			break;
		case SEMI:
			range = ACC_SEMI;
			break;
		case COM:
			range = COM;
			break;
		case LEFT_PAREN:
			range = ACC_LEFT_PAREN;
			break;
		case RIGHT_PAREN:
			range = ACC_LEFT_PAREN;
			break;
		case LEFT_CURL:
			range = ACC_LEFT_CURL;
			break;
		case RIGHT_CURL:
			range = ACC_RIGHT_CURL;
			break;
		case LEFT_SQUARE:
			range = ACC_LEFT_SQUARE;
			break;
		case RIGHT_SQUARE:
			range = ACC_RIGHT_SQUARE;
			break;
		case QUOTE:
			range = IN_CHAR;
			break;
		case DOUBLE_QUOTE:
			range = IN_STRING;
			break;
		case ALPHABET:
			range = IN_ID;
			break;
		case ZERO:
			range = IN_ZERO;
		case NON_ZERO:
			range = IN_DECIMAL;
			break;
		}
		table.map_state(START, i, range);

	}

	table.map_other(STATES::IN_ASSIGN, SYMBOLS::EQUAL, STATES::ACC_ASSIGN);
	table.map_state(STATES::IN_ASSIGN, SYMBOLS::EQUAL, STATES::ACC_EQUAL);

	table.map_other(STATES::IN_MORE, SYMBOLS::EQUAL, STATES::ACC_MORE);
	table.map_state(STATES::IN_MORE, SYMBOLS::EQUAL, STATES::ACC_EQUAL_MORE);

	table.map_other(STATES::IN_LESS, SYMBOLS::EQUAL, STATES::ACC_LESS);
	table.map_state(STATES::IN_LESS, SYMBOLS::EQUAL, STATES::ACC_EQUAL_LESS);

	table.map_other(STATES::IN_NOT, SYMBOLS::EQUAL, STATES::ACC_NOT);
	table.map_state(STATES::IN_NOT, SYMBOLS::EQUAL, STATES::ACC_NOT_EQUAL);

	table.map_state(STATES::IN_AND, SYMBOLS::AND, STATES::ACC_AND);
	table.map_state(STATES::IN_OR, SYMBOLS::OR, STATES::ACC_OR);

	table.map_other(STATES::IN_ID, SYMBOLS::LETTER, STATES::ACC_ID);
	table.map_state(STATES::IN_ID, SYMBOLS::LETTER, STATES::IN_ID);

	table.map_state(STATES::IN_CHAR, SYMBOLS::LETTER, STATES::IN_CHAR2);

	table.map_state(STATES::IN_CHAR2, SYMBOLS::BACK_SLASH, STATES::ESCAPE_CHAR);
	table.map_state(STATES::IN_CHAR2, SYMBOLS::QUOTE, STATES::ACC_CHAR);

	table.map_state(STATES::ESCAPE_CHAR, SYMBOLS::LETTER, IN_CHAR2);

	table.map_state(STATES::IN_STRING, SYMBOLS::BACK_SLASH, STATES::ESCAPE_STRING);
	table.map_state(STATES::IN_STRING, SYMBOLS::DOUBLE_QUOTE, STATES::ACC_STRING);
	table.map_state(STATES::ESCAPE_STRING, SYMBOLS::LETTER, STATES::IN_STRING);

	table.map_other(STATES::IN_DECIMAL, SYMBOLS::DIGIT, STATES::ACC_DECIMAL);
	table.map_state(STATES::IN_DECIMAL, SYMBOLS::DIGIT, STATES::IN_DECIMAL);

	table.map_state(STATES::IN_ZERO, SYMBOLS::SMALL_X, STATES::IN_HEX);
	table.map_state(STATES::IN_ZERO, SYMBOLS::LARGE_X, STATES::IN_HEX);

	table.map_other(STATES::IN_HEX, SYMBOLS::HEX, ACC_HEX);
	table.map_state(STATES::IN_HEX, SYMBOLS::HEX, STATES::IN_HEX);

	State arr[29] = { ACC_ADD, ACC_SUB, ACC_MUL, ACC_DIV, ACC_MOD,
		ACC_ASSIGN, ACC_EQUAL, ACC_MORE, ACC_EQUAL_MORE, ACC_LESS, ACC_EQUAL_LESS,
		ACC_NOT, ACC_NOT_EQUAL, ACC_AND, ACC_OR,

		ACC_SEMI, ACC_COM,
		ACC_LEFT_PAREN, ACC_RIGHT_PAREN, ACC_LEFT_CURL, ACC_RIGHT_CURL, ACC_LEFT_SQUARE, ACC_RIGHT_SQUARE,

		ACC_ID, ACC_CHAR, ACC_STRING,
		ACC_DECIMAL, ACC_HEX, ACC_ZERO };


	table.set_accept(arr, 29);
	table.set_start_state(START);

}

void add_symbolSet() {

}

int main() {

	Tokenizer tokenizer("lexer_test.txt");
	tokenizer.ToTokens();

	vector<string> tokens = tokenizer.GetTokens();

	const vector<string> specialSymbol = { "+", "-", "*", "/", "%",
		"=", ">", "<", "&", "|", "!",
		";", ",", "x", "X",
		"(", ")", "{", "}", "[", "]",
		"\\", "'", "\""
	};

	const vector<string> symbolName = {
		"ADD", "SUB", "MUL", "DIV", "MOD",
		"EQUAL", "MORE", "LESS", "AND", "OR", "NOT",
		"SEMI", "COM", "SMALL_X", "LARGE_X",
		"LEFT_PAREN", "RIGHT_PAREN", "LEFT_CURL", "RIGHT_CURL", "LEFT_SQUARE", "RIGHT_SQUARE",
		"BACK_SLASH", "QUOTE", "DOUBLE_QUOTE",

		"ALPHABET", "LETTER",
		"DIGIT", "ZERO", "NON_ZERO", "HEX",

	};

	vector <SymbolSet>  specialSymbolSet;

	for (int i = SYMBOLS::ADD; i <= SYMBOLS::DOUBLE_QUOTE; i++) {
		SymbolSet temp(symbolName[i], i, specialSymbol[i]);
		specialSymbolSet.push_back(temp);
	}

	vector<string> alphabetString;
	vector<string> digitString;
	vector<string> hexString;
	vector<string> nonzeroString;

	for (int i = 0; i <= 9; i++) {
		char c = i - '0';
		string s(1, c);

		digitString.push_back(s);
		hexString.push_back(s);

		if (i > 0) {
			nonzeroString.push_back(s);
		}
	}


	for (char i = 'A'; i <= 'Z'; i++) {

		string capital(1, i);
		string small(1, i + ' ');

		alphabetString.push_back(capital);

		if (i <= 'F') {
			hexString.push_back(capital);
		}
		alphabetString.push_back(small);

	}

	/* letter = (alphabet | digit) */
	vector<string> letterString(digitString.size() + alphabetString.size());
	letterString.insert(letterString.end(), alphabetString.begin(), alphabetString.end());
	letterString.insert(letterString.end(), digitString.begin(), digitString.end());

	SymbolSet alphabet("ALPHABET", SYMBOLS::ALPHABET, alphabetString);
	SymbolSet digit("DIGIT", SYMBOLS::DIGIT, digitString);
	SymbolSet letter("LETTER", SYMBOLS::LETTER, letterString);
	SymbolSet nonZero("NON_ZERO", SYMBOLS::NON_ZERO, nonzeroString);
	SymbolSet hex("HEX", SYMBOLS::HEX, hexString);
	SymbolSet zero("ZERO", SYMBOLS::ZERO, "0");

	Table table(STATES::ACC_ZERO + 1, SYMBOLS::NOT_TOKEN + 1, STATES::START);

	for (int i = 0; i < specialSymbolSet.size(); i++) {
		table.add_symbol(specialSymbolSet[i]);
	}

	table.add_symbol(alphabet);
	table.add_symbol(letter);
	table.add_symbol(digit);
	table.add_symbol(nonZero);
	table.add_symbol(hex);
	table.add_symbol(zero);

	table_set(table);

	DFA dfa(tokens, table);
	dfa.run();
	dfa.print_result();

	getchar();

}

