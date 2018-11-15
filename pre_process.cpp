#include "mycalc.h"




MyCalc::MyCalc(char * input_file, char * output_file){
	std::cout <<"---test:"<<isalpha(' ')<<endl;
	read_input(input_file);
	for (int i = 0; i < lines.size(); i++){
		process_line(lines[i]);
	}

	//debug
	std::cout <<"Before operation, all the variables are"<<std::endl;
	for (int i = 0; i < variables.size(); i++){
		std::cout <<variables[i]->name << ";" << variables[i]->status <<std::endl;
		if (variables[i]->status=="solved"){
			std::cout <<"the value is "<< variables[i]->value <<std::endl;
		}
	}

	evaluation(variables);
	generate_output(output_file);

}


void MyCalc::read_input(char * file_name){ //read each line, break it into the name of the variable and its values, and sanity check
	std::string line;
	std::string processed_line;
	std::stack<char> opening_bracket; //this stack is to do the sanity check that the brackets are balanced

	std::ifstream input(file_name);

	if (input.is_open()){
		while (std::getline(input,line)){
			// line =line.substr(0, line.length() - 1);
			for (std::string::iterator it = line.begin(); it != line.end(); it++){ //to purge the unnecessary symbols and spaces
				if ( (ispunct(*it)||isalnum(*it)) && *it != ';'){
				//if (isgraph(*it)|| *it !=' ' || *it != ';' || (bool)std::iscntrl(*it) ==0 )
					processed_line.push_back(*it);
					if (*it =='('){
						opening_bracket.push('(');

					}
					else if (*it ==')'){
						opening_bracket.pop();
					}
				}
			}
			if (!opening_bracket.empty()){
				processed_line = '~'+ processed_line;
				std::cout << "The barcketes is not balanced in this line: "<< line << std::endl;

			}
			lines.push_back(processed_line);
			// //debug: 
			// std::cout << processed_line <<std::endl;
			processed_line = "";
			while (!opening_bracket.empty()){
				opening_bracket.pop();
			}
		}
	}
	else{
		std::cerr <<"mymachine-promt >> ./mycalc -i <inputfile> -o <outfile>";
	}
	input.close();
}

void MyCalc::process_line(std::string& line){ //for each line, return the root of the tree
	std::string name;
	int position;  //of '='
	for (int i = 0; i < line.length(); i++){
		if (line[i] == '='){
			position = i;
			break;
		}
	}

	name = line.substr(0, position);
	if (name[0] == '~'){
		Variable * broken_variable = new Variable (name.substr(1,name.length()-1),"broken");
		broken_variable->message ="unbalanced brackets";
		variables.push_back(broken_variable);
	}
	else{
		std::cout << name <<std::endl;
		Variable * good_variable = new Variable (name,"unsolved");
		variables.push_back(good_variable);

		std::string instru;
		instru = line.substr(position+1,line.length()-position-1);
		good_variable->instruction = instru;

		std::vector< std::pair<Type,std::string> > elements;
		elements = sort_out(good_variable->name, good_variable->instruction);



		if (good_variable->name[0] == '~'){
			good_variable->status ="broken";
			good_variable->name = good_variable->name.substr(1,good_variable->name.length()-1);
		}
		else{

			if (elements.size() ==1 && elements[0].first == num){
				good_variable->status = "solved";
				good_variable->value = std::stod(elements[0].second);
			}

			else{
				good_variable->status = "unsolved"; //this contain all the unbroken but unsolved variables
			}

			stack< std::pair<Type,std::string> > pre_order;
			good_variable->pre_order_expressions = in_a_pre(elements);

			//convert pre-order to post-order
			while (!good_variable->pre_order_expressions.empty()){
				good_variable->post_order_expressions.push(good_variable->pre_order_expressions.top());
				good_variable->pre_order_expressions.pop();
			}
		}
	}


}

stack<std::pair<Type,std::string> > MyCalc::in_a_pre(std::vector<std::pair<Type,std::string> > & elements){ //convert from inorder to preorder
	//pre-order is the final list
	stack< pair <Type, string> > pre_order;
	std::pair<Type,std::string> p;
	p.first = bra2;
	p.second =")";
	elements.push_back(p);

	//tmp is a intermediate container
	stack< pair <Type, string> > tmp;
	std::pair<Type,std::string> q;
	q.first = bra1;
	q.second ="(";
	tmp.push(q);

	int i = 0; 
	while (!tmp.empty()){
		std::pair <Type, string> ele = elements[i++];
		if (ele.first ==num || ele.first == var){
			pre_order.push(ele);
		}
		else if (ele.first == bra1 ){
			tmp.push(ele);
		}
		else if (ele.first == opt){
			while (tmp.top().first == opt && inferior(ele.second, tmp.top().second)){
				pre_order.push(tmp.top());
				tmp.pop(); 
			}
			tmp.push(ele);
		}
		else if (ele.first == bra2 ){
			while (tmp.top().first!= bra1){
				pre_order.push(tmp.top());
				tmp.pop();
			}
			tmp.pop();
		}
	}
	return pre_order;
}

int MyCalc::sequence(string o){
	if (o =="+" || o =="-") return 1;
	else if (o =="*" || o =="/"|| o =="%") return 2;
	else if (o =="++" || o =="--"|| o =="**") return 3;
	else return 0;
}

bool MyCalc::inferior(string a, string b){
	if (sequence(a)<= sequence(b)) return 1;
	else return 0;
}


std::vector< std::pair<Type,std::string> > MyCalc::sort_out(std::string& name, std::string & instructions){ //take the characters and sort them into var, num, baracketes and  operators
	//and return a pre-order list of all the elements

	std::string tmp;
	tmp = "";
	std::vector<std::pair<Type,std::string> > instruction_list;
	for (std::string::iterator it = instructions.begin(); it!=instructions.end(); it++){
		//put the subsequent characters into a unit
		if (isalpha(*it)){
			while (isalpha(*it)){
				tmp.push_back(*it);
                it++;
			}
            it--;
			if (tmp !="mod"){
				if (tmp.size()>3 && tmp.substr(tmp.size()-3, 3)=="mod"){
					std::pair<Type,std::string> p;
					p.first = var;
					p.second = tmp.substr(0,3);
					instruction_list.push_back(p);
					p.first = opt;
					p.second = '%';
					instruction_list.push_back(p);
					tmp="";


				}


				else{
					while (tmp.back()==' '){
						tmp.erase(tmp.back());
					}
	                std::pair<Type,std::string> p;
					p.first = var;
					p.second = tmp;
					instruction_list.push_back(p);
					tmp = "";
				}
			}
			else{
                std::pair<Type,std::string> p;
				p.first = opt;
				p.second = '%';
				instruction_list.push_back(p);
				tmp = "";
			}
		}
		else if (isdigit(*it)){
			//put the subsequent digits into a unit
			while (isdigit(*it)){
				tmp.push_back(*it);
                it++;
			}
            it--;
            std::pair<Type,std::string> p;
			p.first = num;
			p.second = tmp;
			instruction_list.push_back(p);
			tmp = "";

		}
		else if (is_operator(*it)){
			//put the operators characters into a unit, just in case there are ++, --, **
			while(is_operator(*it)){
				char c = *it;
				tmp.push_back(*it);
                it++;
                if (*it != c) break;
			}
            it--;

            // //identify unary and binary -
            // std::string::iterator rator;
            // rator = it--;

            // if (tmp =="-" && !( isdigit(*rator) || isalpha(*rator) )){
            // 	tmp = "~";
            // }

            std::pair<Type,std::string> p;
			p.first = opt;
			p.second = tmp;
			instruction_list.push_back(p);
			tmp = "";
		}

		else {
			if (*it=='('){
                std::pair<Type,std::string> p;
				p.first = bra1;
				p.second = "(";
				instruction_list.push_back(p);
				tmp = "";	
			}
			else if (*it== ')'){
                std::pair<Type,std::string> p;
				p.first = bra2;
				p.second = ")";
				instruction_list.push_back(p);
				tmp = "";
			}	
			// else if (isblank(*it)){
			// 	tmp = "";
			// }	
			else{
				std::cout <<"the abnormalty is "<< *it <<"...."<<std::endl;

				std::cout << "ambiguous expression in line: "<< name<<" = "<< instructions <<";"<< std::endl;
				name = "~"+name;

			}
		}
	}


	// //for debug: print out each units of elements
	for (int i =0; i< instruction_list.size(); i++){
		std::cout <<instruction_list[i].second <<" ";
	}
	std::cout <<std::endl;
	return instruction_list; //a list of pairs, which info about its type and its content
}


bool MyCalc::is_operator(char & o){
	if (o =='+' || o =='-' || o =='*' || o =='/' || o =='%') return 1;
	else return 0;
}
