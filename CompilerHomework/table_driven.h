#ifndef TABLE_DRIVEN_H
#define TABLE_DRIVEN_H

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
enum SYMBOLS {
	ADD, SUB, MUL, DIV, MOD,
	EQUAL, MORE, LESS, AND, OR, NOT,
	SEMI, COM, SMALL_X, LARGE_X,
	LEFT_PAREN, RIGHT_PAREN, LEFT_CURL, RIGHT_CURL, LEFT_SQUARE, RIGHT_SQUARE,
	BACK_SLASH, QUOTE, DOUBLE_QUOTE,

	ALPHABET, ZERO, NON_ZERO, HEX,
	NOT_TOKEN

};

enum STATES {
	START,
	IN_ASSIGN, IN_MORE, IN_LESS, IN_NOT, IN_ID, IN_AND, IN_OR,
	IN_CHAR, IN_CHAR2, ESCAPE_CHAR,
	IN_STRING, ESCAPE_STRING,
	IN_ZERO, IN_DECIMAL, IN_HEX,

	ACC_ADD, ACC_SUB, ACC_MUL, ACC_DIV, ACC_MOD,
	ACC_ASSIGN, ACC_EQUAL, ACC_MORE, ACC_EQUAL_MORE, ACC_LESS, ACC_EQUAL_LESS,
	ACC_NOT, ACC_NOT_EQUAL, ACC_AND, ACC_OR,

	ACC_SEMI, ACC_COM,
	ACC_LEFT_PAREN, ACC_RIGHT_PAREN, ACC_LEFT_CURL, ACC_RIGHT_CURL, ACC_LEFT_SQUARE, ACC_RIGHT_SQUARE,

	ACC_ID, ACC_CHAR, ACC_STRING,
	ACC_DECIMAL, ACC_HEX, ACC_ZERO
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
	SymbolSet(std::string name, int enumValue, std::string token);
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

	State startState;
	int not_token;

	int get_symbol_set(State state, std::string input);
	std::vector<std::string> statesName;


public:

	Table();

	Table(const State arr[], int row, int col);
	Table(int row, int col, State startState);
	~Table();

	void default_init(int row, int col);

	void map_state(State domain, int symbolEnum, State codomain);
	void map_state(State domain, std::vector<int> symbolEnum, State codomain);

	void map_other(State domain, int notOther, State codomain);
	void map_other(State domain, std::vector<State> notOthers, State codomain);
	void add_symbol(SymbolSet symbol);

	State get_next(State state, std::string token);

	void set_start_state(int startState);
	State start_state();

	void set_accept(State arr[], int size);
	bool is_accept(State state);
	void set_not_token(int enumValue);

	void print_table();
};



#endif
