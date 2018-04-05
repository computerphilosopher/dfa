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
	Table *table;
	vector<string> result;
	int count_result;

public:

	DFA(vector<string> tokens, Table table) {
		
		this->tokens = tokens;
		this->table = &table;
 
	}

	~DFA(){

	}

	void Run() {

		int size = tokens.size();

		State state = table->start_state();

		string temp = "";

		for (int i = 0; i < size; i++) {

			State new_state = table->get_next(state, tokens[i]);

			if (new_state != table->start_state()) {
				temp.append(tokens[i]);
			}

			else {//new_state == DT_START
				if (table->is_accept(state)){
					result.push_back(temp);
				}
				temp.clear();
			}

			if (table->is_accept(new_state) && !temp.empty()) {
				result.push_back(temp);
				temp.clear();
			}

			state = new_state;
		}

	}

	void PrintResult() {

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

	void WriteToFile(string filePath) {

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

	vector<string> tokens = tokenizer.GetTokens();

	vector<string> month_tokens = {
		"Jan.", "January", "Feb.", "February", "Mar.", "March",
		"Apr.", "April", "May", "May", "June", "June",
		"July", "July", "Aug.", "August", "Sep.", "September",
		"Oct.", "October", "Nov.", "November", "Dec.", "December"
	};

	SymbolSet symbolSet[3] = { SymbolSet("month", symbols::MONTH, month_tokens), SymbolSet("NUM1", symbols::NUM1, 1, 31), SymbolSet("NUM2", symbols::NUM1, 32, 2999) };

	//DFA dfa(tokens);


	getchar();

	return 0;
}