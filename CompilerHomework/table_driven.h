#ifndef TABLE_DRIVEN_H
#define TABLE_DRIVEN_H
 
typedef int State;

class SymbolSet {

private:
	enum token_type {
		INTEGER,
		STRING
	};

	std::string name;
	int enumValue;

	/*��ū�� ���ڳ� ���ڿ��� ���*/
	std::vector<std::string> tokens;

	/*��ū�� ���� ������ ������ ���*/
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


class Table {

private:

	bool Accept[11];
	std::vector<SymbolSet> symbolSet;
	
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

	/* Accept ���θ� Ȯ���ϱ� ���� boolean �迭*/


	int get_symbolset(std::string input);

public:

	Table();

	Table(std::vector<SymbolSet> symbolSet);

	void add_symbol(SymbolSet symbol);

	State getNext(State state, std::string token);

	State start_state();

	bool is_Accept(State state);
};



#endif