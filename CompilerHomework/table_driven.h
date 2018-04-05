#ifndef TABLE_DRIVEN_H
#define TABLE_DRIVEN_H
 
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


typedef int State;

class SymbolSet {

private:
	
	enum token_type {
		INTEGER,
		STRING
	};

	std::string name;
	int enumValue;

	/*토큰이 문자나 문자열일 경우*/
	std::vector<std::string> tokens;

	/*토큰이 일정 범위의 숫자일 경우*/
	int start;
	int end;

	int type;

	void init(std::string name, int enumValue);

public:

	SymbolSet(std::string name, int enumValue, std::vector<std::string> tokens);

	SymbolSet(std::string name, int enumValue, int start, int end);

	bool is_in_set(std::string input);

	int get_enumValue();

	std::string get_name();
};


class Table {

private:
 bool accept[11];
	std::vector<SymbolSet> symbolSet;
	
	/*
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
	*/

	std::vector < std::vector < State > > table;

	int rowSize;
	int colSize;

	int startState;
 
	int get_symbol_set(std::string input);

public:

	Table();

	Table(const State arr[], int row, int col);

	~Table();

	void add_symbol(SymbolSet symbol);

	State get_next(State state, std::string token);
	
	void set_start_state(int startState);
	State start_state();

	bool is_accept(State state);

	void print_table();
};



#endif
