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
 *  Description: This program read integer events (digits) from the terminal (std::cin),
 *  multiplies a digit from 1 until 10, and prints the result in the screen.
 *  File Name: reactive_stream.cpp
 *  Version: 1.0 (03/06/2016)
 *  Compilation Command: spar -std=c++1y -spar_file reactive_stream.cpp -o exe
 ****************************************************************************
*/


#include <iostream>
#include <cstdio>
#include <fstream>
#include <cstring>
/*
This annotation schema in proc_spar1() produces an activity graph such as: 
	  (cin)
	    | 
		|
      (cout) 
 */
void proc_spar1(){
	[[spar::ToStream]] while(1){
		int stream_source;
		std::cout << "Enter a digit: ";
		std::cin >> stream_source;	
		[[spar::Stage,spar::Input(stream_source)]] for (int i = 1; i < 11; i++){	
			std::cout << stream_source*i << std::endl;
		}
	}
}
/*

This annotation schema in proc_spar2() produces an activity graph such as: 
	             			 (cin)
	             			   |
	    1_____2______3______4__|___5______6_______7______8_____9______10 
		|* 	  |*	 |*		|*	   |*     |*	  |*	 |*	   |*	  |*
 	 (cout) (cout) (cout) (cout) (cout) (cout) (cout) (cout) (cout)	(cout)

 */
void proc_spar2(){
	[[spar::ToStream]] while(1){
		int stream_source;
		std::cout << "Enter a digit: ";
		std::cin >> stream_source;	
		for (int i = 1; i < 11; i++){
			[[spar::Stage,spar::Input(i,stream_source),spar::Replicate(10)]]
			{std::cout << stream_source*i << std::endl;}
		}
	}
}
/*
This annotation schema in proc_spar3() produces an activity graph such as: 
	  (cin)
	    | 
    (multiply)
		|
      (cout) 
 */
void proc_spar3(){
	[[spar::ToStream]] while(1){
		int stream_source;
		std::cout << "Enter a digit: ";
		std::cin >> stream_source;	
		for (int i = 1; i < 11; i++){
			[[spar::Stage,spar::Input(stream_source,i),spar::Output(stream_source)]]{
				stream_source = stream_source*i;
			}
			[[spar::Stage,spar::Input(stream_source)]]{
				std::cout << stream_source << std::endl;
			}			
		}
	}
}
/*

This annotation schema in proc_spar4() produces an activity graph such as: 
	     				     (cin)
							   |	      
		1_____2______3______4__|___5______6_______7______8_____9______10 
		|* 	  |*	 |*		|*	   |*     |*	  |*	 |*	   |*	  |*
		|_____|______|______|______|______|_______|______|_____|______|
    						   |
			   				 (cout)

ps. you may use 'spar_ordered'  flag to print the results ordered in this annotation schema
 */
void proc_spar4(){
	[[spar::ToStream]] while(1){
		int stream_source;
		std::cout << "Enter a digit: ";
		std::cin >> stream_source;	
		for (int i = 1; i < 11; i++){
			[[spar::Stage,spar::Input(stream_source,i),spar::Output(stream_source),spar::Replicate(10)]]{
				stream_source = stream_source*i;
			}
			[[spar::Stage,spar::Input(stream_source)]]{
				std::cout << stream_source << std::endl;
			}			
		}
	}
}
int main(int argc, char const *argv[]){
	std::cout << "This program gives the multipliers of the digit (integer) from 1 until 10!!" << std::endl;
	std::cout << "Press 'Crtl+c'  to exit." << std::endl;
	//uncomment the desired function call
	//proc_spar1();
	//proc_spar2();
	//proc_spar3();
	proc_spar4();
	return 0;
}