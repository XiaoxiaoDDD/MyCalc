#include "mycalc.h"
#include <vector>

void read_input(char *);
void generate_ouput();

int main(int argc, char* argv[]){
	char * ifile = "default";
	char * ofile = "default";


	for (int i = 0; i < argc; i++){
		if (strcmp(argv[i],"-i") == 0){
			ifile = argv[++i];
		}
		else if (strcmp(argv[i], "-o") == 0){
			ofile = argv[++i];
		}
	}

	if (ifile == "default" || ofile == "default"){
		std::cout << "mymachine-promt >> ./mycalc -i <inputfile> -o <outfile>"<<std::endl;
		return EXIT_FAILURE;
	}


	MyCalc mycalc(ifile, ofile);


	return 0;

}

