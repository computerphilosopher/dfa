#include <string>
#include <vector>
#include <iostream>
#include "table_driven.h"

using namespace std;


/* ----------Table -------------*/
Table::Table() {

}
void Table::default_init(int row, int col) {

	this->rowSize = row;
	this->colSize = col;

	startState = 0;
	not_token = col - 1;

	for (int i = 0; i < row; i++) {
		std::vector<State> elem;
		elem.resize(col);
		table.push_back(elem);
	}
	
	vector <string> Name = {
		"START",
		"IN_ASSIGN", "IN_MORE", "IN_LESS", "IN_NOT", "IN_ID", "IN_AND", "IN_OR",
		"IN_CHAR", "IN_CHAR2", "ESCAPE_CHAR",
		"IN_STRING", "ESCAPE_STRING",
		"IN_ZERO", "IN_DECIMAL", "IN_HEX",

		"ACC_ADD", "ACC_SUB", "ACC_MUL", "ACC_DIV", "ACC_MOD",
		"ACC_ASSIGN", "ACC_EQUAL", "ACC_MORE", "ACC_EQUAL_MORE", "ACC_LESS", "ACC_EQUAL_LESS",
		"ACC_NOT", "ACC_NOT_EQUAL", "ACC_AND", "ACC_OR",

		"ACC_SEMI", "ACC_COM",
		"ACC_LEFT_PAREN", "ACC_RIGHT_PAREN", "ACC_LEFT_CURL", "ACC_RIGHT_CURL", "ACC_LEFT_SQUARE", "ACC_RIGHT_SQUARE",

		"ACC_ID", "ACC_CHAR", "ACC_STRING",
		"ACC_DECIMAL", "ACC_HEX", "ACC_ZERO"
	};

	statesName = Name;
}

Table::Table(const State arr[], int row, int col) {

	default_init(row, col);

	/*
	for (int i = 0; i < row; i++) {
		std::vector<State> elem;
		elem.resize(col);
		table.push_back(elem);
	}
	*/

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			table[i][j] = arr[(i*col) + j];
		}
	}

	accept.resize(table.size(), false);
	fill(accept.begin(), accept.end(), false);
}

Table::Table(int row, int col, State startState) {
	default_init(row, col);

	set_start_state(startState);

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {

			table[i][j] = startState;

		}
	}
	accept.resize(table.size(), false);
	fill(accept.begin(), accept.end(), false);
}

Table::~Table() {

}

void Table::set_accept(State arr[], int size) {

	for (int i = 0; i < size; i++) {
		accept[arr[i]] = true;
	}

}

void Table::set_start_state(State startState) {

	this->startState = startState;

}

void Table::add_symbol(SymbolSet symbol) {
	symbolSet.push_back(symbol);
}

void Table::set_not_token(int enumValue) {

	not_token = enumValue;

}

State Table::get_next(State state, string token) {

	int symbol = get_symbol_set(state, token);

	return table[state][symbol];
}

State Table::start_state() {
	return startState;
}

bool Table::is_accept(State state) {
	return accept[state];
}

int Table::get_symbol_set(State state, string input) {

	int size = symbolSet.size();

	for (int i = 0; i < size; i++) {
		if (symbolSet[i].is_in_set(input)) {

			string name = symbolSet[i].get_name();
			return symbolSet[i].get_enumValue();
		}
	}
	return not_token;
}

void Table::print_table() {

	for (int i = 0; i < rowSize; i++) {
		for (int j = 0; j < colSize; j++) {
			cout << statesName[table[i][j]] + " ";
		}
		cout << endl;
	}

}

void Table::map_state(State domain, int symbolEnum, State codomain) {

	table[domain][symbolEnum] = codomain;

}

void Table::map_state(State domain, vector<int> symbolEnum, State codomain) {

	for (State i = 0; i < symbolEnum.size(); i++) {

		table[domain][i] = codomain;
	}

}



void Table::map_other(State domain, int notOther, State codomain) {

	for (int j = 0; j < colSize; j++) {
		if (j != notOther) {
			table[domain][j] = codomain;
		}
	}

}
void Table::map_other(State domain, vector<State> notOthers, State codomain) {

	bool isException = false;
	for (int i = 0; i < colSize; i++) {
		for (int j = 0; j < notOthers.size(); j++) {
			if (i == notOthers[j]) {
				isException = true;
			}
		}
		if (!isException) {
			table[domain][i] = codomain;
		}
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

SymbolSet::SymbolSet(string name, int enumValue, string token) {
	init(name, enumValue);
	this->tokens.push_back(token);
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