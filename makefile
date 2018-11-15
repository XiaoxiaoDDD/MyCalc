
mycalc: mycalc.o pre_process.o calculation.o
	g++ mycalc.o pre_process.o calculation.o -o mycalc


mycalc.o: mycalc.cpp
	g++ -std=c++11 -c mycalc.cpp

	
pre_process.o: pre_process.cpp
	g++ -std=c++11 -c pre_process.cpp

others.o: calculation.cpp
	g++ -std=c++11 -c calculation.cpp

clean:
	rm *.o mycalc *.txt