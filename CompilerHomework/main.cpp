#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using namespace std;


class SymbolSet {

private:
	string name;
	vector <string> symbols;
public:
	vector <SymbolSet> subset;

	SymbolSet(string name, vector<string> symbols) {
		this->symbols = symbols;
	}

	void make_subset(SymbolSet child, vector<string> child_symbols) {

		int size = child_symbols.size();

		for (int i = 0; i < size; i++) {
			subset.push_back(child);
		}
	}

	bool is_element(string element) {
		return std::find(symbols.begin(), symbols.end(), element) != symbols.end();
	}

	int subset_count() {
		return subset.size();
	}

	vector<string> get_symbols() {
		return symbols;
	}

	bool Equals(SymbolSet s) {
		return s.get_symbols() == symbols;
	}
};


class State {

private:
	bool accept;
	string name;
	unordered_map <SymbolSet, State> transition;
	

public:

	static State start;

	State() {

	}

	State(string name, bool accept) {
		this->name = name;
		this->accept = accept;
	}
 
	void add_transition(SymbolSet symbol, State state) {
		transition[symbol] = state;
	}

	State get_transition(SymbolSet symbol) {

		if (transition.count(symbol)) {
			return transition[symbol];
		}
		else {
			return start;
		}
	}

	bool empty() {
		return (name.empty() && transition.empty());
	}
	

};


class Automata {

private:
	vector<string> tokens;
	vector<State> states;
	vector<string> pattern;

public:

	Automata(vector<string> tokens, vector<State> states) {
		this->tokens = tokens;
		this->states = states;
	}

	void Run() {

		int size = tokens.size();
		vector<string> temp;
		// State s = states.start;
		for (int i = 0; i < size; i++) {
		}
	}
};


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

	}

	vector<string> getTokens() {
		return tokens;
	}
};

int main() {

	Tokenizer tokenizer("hw-2sample.txt");
	vector <string> t = tokenizer.getTokens();

	int size = t.size();

}