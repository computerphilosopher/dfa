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

	~DFA(){

	}

	void run() {

		int size = tokens.size();

		State state = table.start_state();
 
		string temp = "";

		for (int i = 0; i < size; i++) {

			State newState = table.get_next(state, tokens[i]);

			if (newState != table.start_state()) {
				temp.append(tokens[i]);
			}

			else {//new_state == START
				if (table.is_accept(state)){
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

int main(){


	Tokenizer tokenizer("hw2-sample.txt");
	tokenizer.ToTokens();

	vector<string> tokens = tokenizer.GetTokens();

	vector<string> monthTokens = {
		"Jan.", "January", "Feb.", "February", "Mar.", "March",
		"Apr.", "April", "May", "May", "June", "June",
		"July", "July", "Aug.", "August", "Sep.", "September",
		"Oct.", "October", "Nov.", "November", "Dec.", "December"
	};

	SymbolSet symbolSet[3] = { SymbolSet("month", symbols::MONTH, monthTokens), SymbolSet("NUM1", symbols::NUM1, 1, 31), SymbolSet("NUM2", symbols::NUM1, 32, 2999) };

	const State arr [55] = {
		//month     num1     num2      ,         others 
		 DT_MONTH, DT_NUM2, DT_START, DT_START, DT_START , //start
	 DT_START, DT_NUM1, DT_NUM1, DT_COM1, DT_START, //month
	DT_START, DT_NUM1, DT_NUM1, DT_START, DT_START, //com1
	DT_ACCEPT_4, DT_START, DT_ACCEPT_1_2, DT_ACCEPT_4, DT_START, //com2
	DT_ACCEPT_3, DT_START, DT_START, DT_START, DT_START,//com3
	DT_ACCEPT_5, DT_ACCEPT_5, DT_ACCEPT_5, DT_COM2, DT_ACCEPT_5,//num1
	DT_START, DT_START, DT_START, DT_COM3, DT_START, //num2

	DT_START, DT_START, DT_START,DT_START, DT_START, //accpet 1,2
	DT_START, DT_START, DT_START,DT_START, DT_START, //accept 3
	DT_START, DT_START, DT_START,DT_START, DT_START, //accept 4
	DT_START, DT_START, DT_START,DT_START, DT_START //accept 5

	};

	Table table(arr, 11, 5);
	table.set_start_state(DT_START);
	
	for (int i = 0; i < 3; i++) {
		table.add_symbol(symbolSet[i]);
	}


	DFA dfa(tokens, table);

 dfa.run();

	dfa.print_result();
 
	getchar();

	return 0;
}