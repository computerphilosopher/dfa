 #include <string>
#include <vector>
#include "table_driven.h"
/* ----------Table -------------*/

using namespace std;


Table::Table(){
	for (int i = 0; i < 11; i++) {
		accept[i] = { false };
	}
	for (int i = DT_ACCEPT_1_2; i <= DT_ACCEPT_5; i++) {
		accept[i] = { true };
	}

}

Table::Table(vector<SymbolSet> symbolSet){

}

void Table::add_symbol(SymbolSet symbol) {
	symbolSet.push_back(symbol);
}

State Table::get_next(State state, string token) {

	int symbol = get_symbolset(token);

	return table[state][symbol];
}
State Table::start_state() {
	return DT_START;
}

bool Table::is_accept(State state) {
	return accept[state];
}

int Table::get_symbolset(string input) {

	int size = symbolSet.size();

	for (int i = 0; i < size; i++) {
		if (symbolSet[i].is_in_set(input)) {
			return symbolSet[i].get_enumValue();
		}
	}
	return NOT_TOKEN;
}

/* ---------- SymbolSet-------------*/

void SymbolSet::init(string name, int enumValue) {
	this->name = name;
	this->enumValue = enumValue;
}

SymbolSet::SymbolSet(string name, int enumValue, vector<string> tokens) {
	init(name, enumValue);
	this->tokens = tokens;
	this->type = STRING;
}

SymbolSet::SymbolSet(string name, int enumValue, int start, int end) {
	init(name, enumValue);
	this->start = start;
	this->end = end;
	this->type = INTEGER;
}

bool SymbolSet::is_in_set(string input) {
	if (type == STRING) {

		int size = tokens.size();

		for (int i = 0; i < size; i++) {
			if (tokens[i] == input) {
				return true;
			}
		}

	}
	else if (type == INTEGER) {
		const char *temp = input.c_str();
		int t = atoi(temp);

		if (t >= start && t <= end) {
			return true;
		}
	}
	return false;
}


int SymbolSet::get_enumValue() {
	return enumValue;
}

string SymbolSet::get_name() {
	return name;
}