#include <string>
#include <vector>
#include <iostream>
#include "table_driven.h"

using namespace std;
 
/* ----------Table -------------*/
Table::Table(){

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
			cout << symbolSet[i].get_name() << endl;
			return symbolSet[i].get_enumValue();
		}
	}
	return not_token;
}

void Table::print_table() {

	for (int i = 0; i < rowSize; i++) {
		for (int j = 0; j < colSize; j++) {
			cout << to_string(table[i][j]) + " ";
		}
		cout << endl;
	}

}

void Table::map_state(State domain, int symbolEnum, State codomain) {

	table[domain][symbolEnum] = codomain;

}

void Table::map_other(State domain, int notOther, State codomain) {

	for (int j = 0; j < colSize; j++) {
		if (j != notOther) {
			table[domain][j] = codomain;
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