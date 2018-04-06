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
	std::vector<bool> accept;
	std::vector<SymbolSet> symbolSet;

	std::vector < std::vector < State > > table;

	int rowSize;
	int colSize;

	int startState;
	int not_token;

	int get_symbol_set(std::string input);

public:

	Table();

	Table(const State arr[], int row, int col);

	~Table();

	void default_init(int row, int col);

	void add_symbol(SymbolSet symbol);

	State get_next(State state, std::string token);

	void set_start_state(int startState);
	State start_state();

	void set_accept(int arr[], int size);
	bool is_accept(State state);

	void print_table();
};



#endif
