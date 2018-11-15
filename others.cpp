#include "mycalc.h"


Variable::Variable (std::string name, std::string status)
:name(name),status(status),message(""){}


void MyCalc::evaluation(std::vector <Variable *> & variables){
	//put all variables except the broken one and the solved one into the queue
	for (int i = 0; i < variables.size(); i++){
		if (variables[i]->status =="unsolved"){
			my_queue.push(variables[i]);
		}
	}

	while (!my_queue.empty()){
		string  result;
	 //to save whether unable to calculate becasue of broken or unresolved variable
		result = calculate(*my_queue.front());
		if (result == "broken"){
			my_queue.front()->status ="broken";
			my_queue.pop();
			std::cout <<"we have found a broken"<<endl;
			}
		else if (result =="unsolved"){
			my_queue.push(my_queue.front());
			my_queue.pop();
			std::cout <<"somethingw we cannot resolve now"<<endl;

		}
		else {
			my_queue.front()->value = stod(result);
			my_queue.front()->status ="solved";
			std::cout <<"we have figured out that "<<my_queue.front()->name <<" is "<<my_queue.front()->value<<endl;

			my_queue.pop();
		}
	}

}



string MyCalc::calculate(Variable variable){//copy is needed
	std::cout <<"-----now we calculate " << variable.name <<"-----"<<endl;
	vector <double> tmp;

	
	while (!variable.post_order_expressions.empty() ){
	// while (true){
		pair <Type, string> ele;
		ele = variable.post_order_expressions.top();
		 //ele is the pair under process
		//if ele is of num
		if(ele.first == num){
			std::cout <<"it is a number"<<endl;
			//if the variable is solved
			if (variable.post_order_expressions.size()==1){
				std::cout <<"exit requirement found"<<endl;

				std::cout <<"we will return " << ele.second <<endl;
				return ele.second;
			}
			else{
				tmp.push_back( std::stod(ele.second) );
				variable.post_order_expressions.pop();				
			}
		}

		else if (ele.first == var){
			std::cout <<"it is a var, and it is called "<<ele.second<< endl;
			for (int i =0; i < variables.size(); i++){
				//compare the name with each of the variables
				if (variables[i]->name == ele.second || variables[i]->name == "~"+ele.second){
					std::cout <<"found the one that corresponds"<<endl;
					if  (variables[i]->status == "solved"){
						std::cout<<"and it has been solved"<<endl;
						variable.post_order_expressions.pop();
						std::cout <<"the value found is"<<variables[i]->value<<endl;
						tmp.push_back(variables[i]->value);
						break;
					}
					else if (variables[i]->status == "broken") {
						variable.post_order_expressions.pop();
						variable.status = "broken";
						return "broken";
					}
					else{
						variable.post_order_expressions.pop();
						return "unsolved";
		
					}
				}

			}

		}
		else if (ele.first == opt){
			std::cout <<"it is a opt"<<endl;
			string operant;

			operant = ele.second;
			std::cout <<"the operant is"<<operant<<endl;
			variable.post_order_expressions.pop();
			int n = tmp.size();
			std::cout <<"now the size of tmp is"<<n <<endl;
			pair <Type, string> p;
			p.first = num;

			if (operant =="+"){
				p.second =std::to_string( tmp[n-2]+tmp[n-1]);
				variable.post_order_expressions.push(p);
				tmp.pop_back();tmp.pop_back();

			}
			else if (operant =="-"){
				if (tmp.size()==1){
					std::cout <<"it is negative"<<endl;

					p.second = std::to_string(0 - tmp[n-1]);
					std::cout <<"p.second is "<<0-tmp[n-1] ;
					variable.post_order_expressions.push(p);
					tmp.clear();

				}
				else{
					std::cout <<"it is minus"<<endl;
					p.second = std::to_string(tmp[n-2]-tmp[n-1]);
					variable.post_order_expressions.push(p);
					tmp.pop_back();tmp.pop_back();
				}

			}
			else if (operant =="*"){
				p.second = std::to_string(tmp[n-2]*tmp[n-1]);
				variable.post_order_expressions.push(p);
				tmp.pop_back();tmp.pop_back();

			}
			else if (operant =="/"){
				p.second = std::to_string(tmp[n-2]/tmp[n-1]);
				variable.post_order_expressions.push(p);
				tmp.pop_back();tmp.pop_back();

			}

			else if (operant =="%"){
				p.second =std::to_string((int)tmp[n-2]% (int)tmp[n-1]);
				variable.post_order_expressions.push(p);
				tmp.pop_back();tmp.pop_back();

			}
			else if (operant =="**"){
				p.second = std::to_string(tmp[n-1] * tmp[n-1]);
				variable.post_order_expressions.push(p);
				tmp.pop_back();
			}
			else if (operant =="++"){
				p.second =std::to_string(tmp[n-1] +1);
				variable.post_order_expressions.push(p);
				tmp.pop_back();

			}
			else if (operant =="--"){
				p.second = std::to_string(tmp[n-1] -1);
				variable.post_order_expressions.push(p); 
				tmp.pop_back();
			}

			std::cout << "the result of this calc is"<< p.second << endl;
		}
	}

}

void MyCalc::generate_output(char * ofile){
	ofstream outfile;
	outfile.open(ofile);

	// std::cout <<"after the operation, the result is------"<<std::endl;

	for (int i = 0; i < variables.size(); i++){
		if (variables[i]->status =="solved"){
			outfile <<variables[i]->name<<" = "<< variables[i]->value <<std::endl;
		}
		else{
			outfile <<variables[i]->name<< " : unable to calculate";
			if (variables[i]->message !=""){
				outfile <<"--"<<variables[i]->message;
			}
			outfile<<std::endl;
		}
		
	}
	outfile.close();

}
