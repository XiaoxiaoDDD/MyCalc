#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <string>
#include <fstream>
#include <ctype.h>
#include <locale>
#include <cstdlib>


using namespace std;

class Variable;

enum Type
{
    var, num, opt, bra1, bra2
    
};

class MyCalc{
public:
	MyCalc(char *, char *);

private:
	std::vector<std::string> lines;
	std::vector <Variable *> variables;
	//std::vector <Variable *> working_unsolved_variable;
	std::queue <Variable *> my_queue; //the queue of all the variables to be calculated
	void read_input(char *);
	void process_line(std::string &);
	std::stack<std::pair<Type,std::string> > in_a_pre(std::vector<std::pair<Type,std::string> > &);
    int sequence(std::string);
    bool inferior(std::string , std::string );
    void evaluation(std::vector <Variable *> &);
    string calculate(Variable );
    std::vector< std::pair<Type,std::string> > sort_out(std::string &,std::string &);
    bool is_operator(char &);
	void generate_output();
  
};


class Variable{
public:
	std::string name;
	std::string status; //solved, unsolved, broken
	std::string instruction;
	std::string message;
	double value;
	Variable(std::string name, std::string status);
	stack< std::pair<Type,std::string> > pre_order_expressions;
	stack< std::pair<Type,std::string> > post_order_expressions;

private:
	

};
