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
	vector <int> tmp;
	int * return_value;
	while (!variable->post_order_expressions.empty() ){
		pair <Type, string> ele;
		ele = variable->post_order_expressions.top();

		if(ele.first == num){
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
			for (int i =0; i < variables.size(); i++){
				if (variables[i]->name == ele.second){
					if  (variables[i]->status == "solved"){
						variable->post_order_expressions.pop();
						tmp.push_back(variables[i]->value);
					}
					else if (variable[i].status == "broken") {
						variable->status = "broken";
						signal = "broken";
						return NULL;
					}
					else{
						variable->status = "unsolved";
						signal ="unsolved";
						return NULL;
					}
				}
			}
		}
		else if (ele.first == opt){
			string operant;
			operant = ele.second;
			int option = convert(operant);
			variable->post_order_expressions.pop();
			int n = tmp.size();
			pair <Type, string> p;
			p.first = num;

			switch(option){
				case 1: 
					p.second = tmp[n-2]+tmp[n-1];
					variable->post_order_expressions.push(p);
					tmp.pop_back();tmp.pop_back(); break;
				case 2: 
					p.second = tmp[n-2]-tmp[n-1];
					variable->post_order_expressions.push(p);
					tmp.pop_back();tmp.pop_back(); break;
				case 3: 
					p.second = tmp[n-2]*tmp[n-1];
					variable->post_order_expressions.push(p);
					tmp.pop_back();tmp.pop_back(); break;
				case 4: 
					p.second = tmp[n-2]/tmp[n-1];
					variable->post_order_expressions.push(p);
					tmp.pop_back();tmp.pop_back(); break;
				case 5: 
					p.second = 0-tmp[n-1];
					variable->post_order_expressions.push(p);
					tmp.pop_back(); break;
				case 6: 
					p.second =tmp[n-2]%tmp[n-1];
					variable->post_order_expressions.push(p);
					tmp.pop_back(); tmp.pop_back(); break;
				case 7: 
					p.second = tmp[n-1] * tmp[n-1];
					variable->post_order_expressions.push(p);
					tmp.pop_back(); break;
				case 8: 
					p.second =tmp[n-1] +1;
					variable->post_order_expressions.push(p);
					tmp.pop_back(); break;
				case 9: 
					p.second = tmp[n-1] -1;
					variable->post_order_expressions.push(p); 
					tmp.pop_back(); break;
			}
		}
	}

}

int MyCalc::convert(string & st){
	if (st=="+") return 1;
	if (st=="-") return 2;
	if (st=="*") return 3;
	if (st=="/") return 4;
	if (st=="~") return 5;
	if (st=="%") return 6;
	if (st=="**") return 7;
	if (st=="++") return 8;
	if (st=="--") return 9;
}
