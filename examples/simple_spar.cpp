


#include <iostream>
#include <vector>
#include <list>
#include <array>
using namespace std;

int main(int argc, char const *argv[]){

	std::array<int, 3> a2 = {1, 2, 3};
	std::string dalvan = "dalvan griebler";
	int N = 11;
	int **x;
	int i=0;
	[[spar::ToStream, spar::Input(x[10][10],N,i,a2[3]), spar::Output(i)]]
	for (i = 1; i < N; ++i){
		[[spar::Stage, spar::Input(i,a2[3]), spar::Output(i)]]
		{
			std::cout << "Happy " << i << std::endl;
		}
		[[spar::Stage, spar::Input(i), spar::Output(i)]]
		{
			std::cout << "New " << i << std::endl;
		}
		[[spar::Stage, spar::Input(i), spar::Output(i)]]
		{
			std::cout << "Year " << i << std::endl;
		}
		[[spar::Stage, spar::Input(i), spar::Output(i), spar::Replicate()]]
		{
			std::cout << ":D ";
		}
	}
	std::cout << "\nLast I=" << i+2005 << std::endl;

	return 0;
}
