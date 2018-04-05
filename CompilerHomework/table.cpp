 #include <string>
#include <vector>
#include <iostream>
#include "table_driven.h"

using namespace std;
 
/* ----------Table -------------*/
Table::Table(){

}

Table::Table(const State arr[], int row, int col) {

	//vector < vector <State> > temp;
	
	for (int i = 0; i < row; i++) {
		std::vector<State> elem;
		elem.resize(col);
		table.push_back(elem);
	}

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			table[i][j] = arr[(i*col) + j];
		}
	}
	
	for (int i = 0; i < 11; i++) {
		accept[i] = { false };
	}
	for (int i = DT_ACCEPT_1_2; i <= DT_ACCEPT_5; i++) {
		accept[i] = { true };
	}

	//this->table =temp;
	startState = 0;
	this->rowSize = row;
	this->colSize = col;
}

Table::~Table() {

}

void Table::set_start_state(int startState) {

	this->startState = startState;

}

void Table::add_symbol(SymbolSet symbol) {
	symbolSet.push_back(symbol);
}

State Table::get_next(State state, string token) {

	int symbol = get_symbol_set(token);

	return table[state][symbol];
}

State Table::start_state() {
	return startState;
}

bool Table::is_accept(State state) {
	return accept[state];
}

int Table::get_symbol_set(string input) {

	int size = symbolSet.size();

	for (int i = 0; i < size; i++) {
		if (symbolSet[i].is_in_set(input)) {
			return symbolSet[i].get_enumValue();
		}
	}
	return NOT_TOKEN;
}

void Table::print_table() {

	for (int i = 0; i < rowSize; i++) {
		for (int j = 0; j < colSize; j++) {
			cout << to_string(table[i][j]) + " ";
		}
		cout << endl;
	}

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