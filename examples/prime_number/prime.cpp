/* ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  As a special exception, you may use this file as part of a free software
 *  library without restriction.  Specifically, if other files instantiate
 *  templates or use macros or inline functions from this file, or you compile
 *  this file and link it with other files to produce an executable, this
 *  file does not by itself cause the resulting executable to be covered by
 *  the GNU General Public License.  This exception does not however
 *  invalidate any other reasons why the executable file might be covered by
 *  the GNU General Public License.
 *
 ****************************************************************************
 *  Authors: Dalvan Griebler <dalvangriebler@gmail.com>
 *         
 *  Copyright: GNU General Public License
 *  Description: Application that counts the number of primes between 1 and N (argument [...] are optional).
 *  File Name: prime.cpp
 *  Version: 1.0 (17/07/2016)
 *  Compilation Command: spar -std=c++1y -spar_file prime.cpp -spar_ondemand -o exe
 *	Execution Command: ./exe -h
*/

#include <cstdlib>
#include <cstdio>
#include <chrono>
#include <iostream>
#include <getopt.h>
#include <cstring>

int workers=1;

int update_args(int argc, char *argv[], int &n){
	int option;
	while ((option = getopt(argc, argv, "n:r:h")) != -1){
		switch(option){
			case 'n':
				n = atoi(optarg);
				break;
			case 'r':
				workers = atoi (optarg);
				break;
			case 'h':
				std::cout << "\nUsage: " << argv[0] << " [options]...\n\n" <<
				"Options:\n" <<
				 " -n <integer>             it will find all prime numbers from 1 to this number\n" <<
				 " -r <integer>             it will set the number of replicas\n" << std::endl;
				return -1;
				break;
			case '?':
				std::cerr << strerror(errno) << " Run  " << argv[0] << " -h for usage. "  << std::endl;
				return -1;
				break;
		}
	}
	if (optind != argc){
		std::cerr << strerror(errno) << " Too many arguments. Run  " << argv[0] << " -h for usage. "  << std::endl;
		return -1;
	}
	return 0;
}
int prime_number ( int n ){
	int total = 0;
	[[spar::ToStream, spar::Input(total,n)]]
	for (int i = 2; i <= n; i++ ){
		int prime = 1;
		[[spar::Stage,spar::Input(i,prime), spar::Output(prime), spar::Replicate(workers)]]
		for (int j = 2; j < i; j++ ){
			if ( i % j == 0 ){
				prime = 0;
				break;
			}
		}
		[[spar::Stage, spar::Input(prime), spar::Output(total)]]
		{total = total + prime;}
	}
	return total;
}
int main ( int argc, char *argv[]){
	int n=500000;
	if(update_args(argc,argv,n)) { exit(3); }
	std::cout << "\n PRIME APP (CPP version) \n Author: Dalvan Griebler <dalvangriebler@gmail.com>" << std::endl;
	std::cout << " Number of Replicas: " << workers << std::endl;
	std::cout << "\nLast Value(N): " << n << std::endl;

	auto t_start = std::chrono::high_resolution_clock::now();

	int total_primes = prime_number( n );
	
	auto t_end = std::chrono::high_resolution_clock::now();

	std::cout << "\n PRIME APP (CPP version) \n Normal end of execution." << std::endl;

	std::cout << "Execution Time(seconds): "  <<  std::chrono::duration<double>(t_end-t_start).count() << " Primes: " << total_primes << std::endl;

	return 0;
}
/******************************************************************************/



