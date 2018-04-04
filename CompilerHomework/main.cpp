#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

typedef int State;


enum symbols {
	MONTH,
	NUM1,
	NUM2,
	REST,
	NOT_TOKEN
};

enum states {
	DT_START,
	DT_MONTH,
	DT_COM1,
	DT_COM2,
	DT_COM3,
	DT_NUM1,
	DT_NUM2,

	DT_ACCEPT_1_2,
	DT_ACCEPT_3,
	DT_ACCEPT_4,
	DT_ACCEPT_5,
};

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

	int get_symbolset(string token) {

		bool isNum = false;
		bool isMonth = false;

		const char *str = token.c_str();

		int size = token.length();

		for (int i = 0; i < size; i++) {
			if (isdigit(str[i])) {
				isNum = true;
			}
		}

		size = month.size();

		for (int i = 0; i < size; i++) {
			if (token == month[i]) {
				isMonth = true;
			}
		}

		if (isMonth) {
			return MONTH;
		}

		else if (token == ",") {
			return REST;
		}

		else if (isNum) {

			int num = atoi(str);

			if (num > 0 && num <= 31) {
				return NUM1;
			}
			else if (num > 31 && num < 3000) {
				return NUM2;
			}
		}

		return NOT_TOKEN;

	}

public:

	Table() {

		for (int i = DT_START; i <= DT_ACCEPT_5; i++) {
			if (i >= DT_ACCEPT_1_2 && i <= DT_ACCEPT_5) {
				Accept[i] = true;
			}
			else {
				Accept[i] = false;
			}
		}

	}

	State getNext(State state, string token) {
		int symbol = get_symbolset(token);

		return table[state][symbol];
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
	DFA(vector<string> tokens) {
		this->tokens = tokens;
	}

	void Run() {

		int size = tokens.size();

		State state = DT_START;

		string temp = "";

		for (int i = 0; i < size; i++) {

			State new_state = table.getNext(state, tokens[i]);

			if (new_state != DT_START) {
				if (new_state != DT_ACCEPT_5) {
					temp.append(tokens[i]);
				}
			}

			else {
				if (table.is_Accept(state)) {
					result.push_back(temp);
				}
				temp.clear();
			}

			if (table.is_Accept(new_state) && !temp.empty()) {
				/*마지막 쉼표 떼기*/
				string last_letter = temp.substr(temp.length() - 1, temp.length());
				if (last_letter == ",") {
					temp.pop_back();
				}
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
		cout <<  result.size() << endl;
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

	ofstream fout;
	int size = tokens.size();

	DFA dfa(tokens);

	dfa.Run();

	dfa.print_result();
	dfa.write_to_file("date.txt");

	getchar();
	return 0;

}