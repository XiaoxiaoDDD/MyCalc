#include "mycalc.h"


Variable::Variable (std::string name, std::string status)
:name(name),status(status){}


void MyCalc::evaluation(std::vector <Variable *> & variables){
	//put all variables except the broken one and the solved one into the queue
	for (int i = 0; i < variables.size(); i++){
		if (variables[i]->status =="unsolved"){
			my_queue.push(variables[i]);
		}
	}

	while (!my_queue.empty()){
		int * result;
		string signal =""; //to save whether unable to calculate becasue of broken or unresolved variable
		result = calculate(my_queue.front(), signal);
		if (result == NULL){
			if (signal == "broken"){
				my_queue.front()->status ="broken";
				my_queue.pop();
			}
			else if (signal =="unsolved"){
				my_queue.push(my_queue.front());
				my_queue.pop();
			}
			else{
				std::cout << "error here" <<std::endl;
			}
		}
		else {
			my_queue.front()->value = *result;
			my_queue.pop();
		}
	}
	std::cout <<"after the operation, the result is------"<<std::endl;
	for (int i = 0; i < variables.size(); i++){
		// if (variables[i]->status =="solved"){
		// 	std::cout <<variables[i]->name<<" = "<< variables[i]->value <<std::endl;
		// }
		// else{
		// 	std::cout <<variables[i]->name<< " : " << variables[i]->status<<endl;
		// }
		std::cout <<variables[i]->name<< " : " << variables[i]->status<<endl;
		
	}
}



int * MyCalc::calculate(Variable* variable,string& signal){
	std::cout <<"now we calculate " << variable->name <<","<<endl;
	vector <int> tmp;

	int * return_value;
	while (!variable->post_order_expressions.empty() ){
	// while (true){
		pair <Type, string> ele;
		ele = variable->post_order_expressions.top(); //ele is the pair under process
		//if ele is of num
		if(ele.first == num){
			std::cout <<"it is a number"<<endl;
			//if the variable is solved
			if (variable->post_order_expressions.size()==1){
				*return_value= std::stoi(ele.second);
				return return_value;
			}
			else{
				tmp.push_back( std::stoi(ele.second) );
				variable->post_order_expressions.pop();				
			}
		}
		else if (ele.first == var){
			std::cout <<"it is a var"<<endl;
			for (int i =0; i < variables.size(); i++){
				//compare the name with each of the variables
				if (variables[i]->name == ele.second){
					std::cout <<"there is one that corresponds"<<endl;
					if  (variables[i]->status == "solved"){
						std::cout<<"and it has been solved"<<endl;
						variable->post_order_expressions.pop();
						std::cout <<"the value found is"<<variables[i]->value<<endl;
						tmp.push_back(variables[i]->value);
						break;
					}
					else if (variable[i].status == "broken") {
						variable->status = "broken";
						signal = "broken";
						return NULL;
					}
					else{
						signal ="unsolved";
						return NULL;
					}
				}
			}
		}
		else if (ele.first == opt){
			std::cout <<"it is a opt"<<endl;
			string operant;

			operant = ele.second;
			std::cout <<"the operant is"<<operant<<endl;
			variable->post_order_expressions.pop();
			int n = tmp.size();
			std::cout <<"now the size of tmp is"<<n <<endl;
			pair <Type, string> p;
			p.first = num;

			if (operant =="+"){
				p.second =std::to_string( tmp[n-2]+tmp[n-1]);
				variable->post_order_expressions.push(p);
				tmp.pop_back();tmp.pop_back();

			}
			else if (operant =="-"){
				if (tmp.size()==1){
					std::cout <<"it is negative"<<endl;
					std::cout <<"yeha: "<<tmp[0]<<endl;
					p.second = std::to_string(0 - tmp[0]);
					std::cout <<"p.second is "<<0-tmp[0] ;
					variable->post_order_expressions.push(p);
					tmp.clear();

				}
				else{
					std::cout <<"it is minus"<<endl;
					p.second = std::to_string(tmp[0]-tmp[1]);
					variable->post_order_expressions.push(p);
					tmp.clear();
				}

			}
			else if (operant =="*"){
				p.second = std::to_string(tmp[0]*tmp[1]);
				variable->post_order_expressions.push(p);
				tmp.clear();

			}
			else if (operant =="/"){
				p.second = std::to_string(tmp[0]/tmp[1]);
				variable->post_order_expressions.push(p);
				tmp.clear();

			}

			else if (operant =="%"){
				p.second =std::to_string(tmp[0]%tmp[1]);
				variable->post_order_expressions.push(p);
				tmp.clear();

			}
			else if (operant =="**"){
				p.second = std::to_string(tmp[0] * tmp[0]);
				variable->post_order_expressions.push(p);
				tmp.clear();

			}
			else if (operant =="++"){
				p.second =std::to_string(tmp[0] +1);
				variable->post_order_expressions.push(p);
				tmp.clear();

			}
			else if (operant =="--"){
				p.second = std::to_string(tmp[0] -1);
				variable->post_order_expressions.push(p); 
				tmp.clear();

			}

			std::cout << "the result of this calc is"<< p.second << endl;
		}
	}

}

