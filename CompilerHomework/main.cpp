#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include "table_driven.h" 

using namespace std;

typedef int State;
 
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

	vector<string> getTokens() {
		return tokens;
	}
};

class SymbolSet{

private:
	enum token_type {
		INTEGER,
		STRING
	};

	string name;
	int enumValue;

	/*토큰이 문자나 문자열일 경우*/
	vector<string> tokens;

	/*토큰이 일정 범위의 숫자일 경우*/
	int start;
	int end;

	int type;

	void init(string name, int enumValue) {
		this->name = name;
		this->enumValue = enumValue;
	}

public:

	SymbolSet(string name, int enumValue, vector<string> tokens) {
		init(name, enumValue);
		this->tokens = tokens;
		this->type = STRING;
	}

	SymbolSet(string name, int enumValue, int start, int end) {
		init(name, enumValue);
		this->start = start;
		this->end = end;
		this->type = INTEGER;
	}

	bool is_in_set(string input) {
		if (type == STRING) {
			
			int size = tokens.size();
			
			for (int i = 0; i < size; i++) {
				if (tokens[i] == input) {
					return true;
				}
			}

		}
		else if(type == INTEGER){
			const char *temp = input.c_str();
			int t = atoi(temp);

			if (t >= start && t <= end) {
				return true;
			}
		}
		return false;
	}

	/*
	bool is_in_set(int input) {
		if (type != INTEGER) {
			return false;
		}
		else {
			if (input >= start && input <= end) {
				return true;
			}
		}
		return false;
	}
	*/

	int get_enumValue() {
		return enumValue;
	}

	string get_name() {
		return name;
	}
};


class Table {

private:

	const vector<string> month = {
		"Jan.", "January", "Feb.", "February", "Mar.", "March",
		"Apr.", "April", "May", "May", "June", "June",
		"July", "July", "Aug.", "August", "Sep.", "September",
		"Oct.", "October", "Nov.", "November", "Dec.", "December"
	};

	const State table[11][5] = {
		//month     num1     num2      ,         others 
		{ DT_MONTH, DT_NUM2, DT_START, DT_START, DT_START }, //start
	{ DT_START, DT_NUM1, DT_NUM1, DT_COM1, DT_START }, //month
	{ DT_START, DT_NUM1, DT_NUM1, DT_START, DT_START }, //com1
	{ DT_ACCEPT_4, DT_START, DT_ACCEPT_1_2, DT_ACCEPT_4, DT_START }, //com2
	{ DT_ACCEPT_3, DT_START, DT_START, DT_START, DT_START },//com3
	{ DT_ACCEPT_5, DT_ACCEPT_5, DT_ACCEPT_5, DT_COM2, DT_ACCEPT_5 },//num1
	{ DT_START, DT_START, DT_START, DT_COM3, DT_START },//num2

	{ DT_START, DT_START, DT_START,DT_START, DT_START }, //accpet 1,2
	{ DT_START, DT_START, DT_START,DT_START, DT_START }, //accept 3
	{ DT_START, DT_START, DT_START,DT_START, DT_START }, //accept 4
	{ DT_START, DT_START, DT_START,DT_START, DT_START }, //accept 5

	};

	/* Accept 여부를 확인하기 위한 boolean 배열*/

	bool Accept[11];
	vector<SymbolSet> symbolSet;

	int get_symbolset(string input) {

		int size = symbolSet.size();

		for (int i = 0; i < size; i++) {
			if (symbolSet[i].is_in_set(input)) {
				return symbolSet[i].get_enumValue();
			}
		}
		return NOT_TOKEN;
	}

public:

	Table() {
		for (int i = 0; i < 11; i++) {
			Accept[i] = { false };
		}
		for (int i = DT_ACCEPT_1_2; i <= DT_ACCEPT_5; i++) {
			Accept[i] = { true };
		}

	}

	Table(vector<SymbolSet> symbolSet) {
		this->symbolSet = symbolSet;
	}

	void add_symbol(SymbolSet symbol) {
		symbolSet.push_back(symbol);
	}

	State getNext(State state, string token) {

		int symbol = get_symbolset(token);

		return table[state][symbol];
	}

	State start_state() {
		return DT_START;
	}

	bool is_Accept(State state) {
		return Accept[state];
	}
};

class DFA {

private:
	vector<string> tokens;
	Table table;
	vector<string> result;
	int count_result;

public:

	DFA(vector<string> tokens, SymbolSet set[], int set_size) {
		
		this->tokens = tokens;

		for (int i = 0; i < set_size; i++) {
			table.add_symbol(set[i]);
		}

	}

	void Run() {

		int size = tokens.size();

		State state = table.start_state();

		string temp = "";

		for (int i = 0; i < size; i++) {

			State new_state = table.getNext(state, tokens[i]);

			if (new_state != table.start_state()) {
				temp.append(tokens[i]);
			}

			else {//new_state == DT_START
				if (table.is_Accept(state)){
					result.push_back(temp);
				}
				temp.clear();
			}

			if (table.is_Accept(new_state) && !temp.empty()) {
				result.push_back(temp);
				temp.clear();
			}

			state = new_state;
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

int main() {

	Tokenizer tokenizer("hw2-sample.txt");
	tokenizer.ToTokens();

	vector<string> tokens = tokenizer.getTokens();

	vector<string> month_tokens = {
		"Jan.", "January", "Feb.", "February", "Mar.", "March",
		"Apr.", "April", "May", "May", "June", "June",
		"July", "July", "Aug.", "August", "Sep.", "September",
		"Oct.", "October", "Nov.", "November", "Dec.", "December"
	};

	SymbolSet symbolSet[3] = { SymbolSet("month", symbols::MONTH, month_tokens), SymbolSet("NUM1", symbols::NUM1, 1, 31), SymbolSet("NUM2", symbols::NUM1, 32, 2999) };

	DFA dfa(tokens, symbolSet, 3);

	dfa.Run();
	dfa.print_result();

	getchar();

	return 0;
}