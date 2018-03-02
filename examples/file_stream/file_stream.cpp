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
 *  Description: This program read from a file (input.txt), computes over stream elements (strings),
 *	and produces an output (output.txt). Different kinds of annotations are exemplified in the source code.
 *  Please, uncomment in the main function the desired function to compile and execute.
 *  File Name: file_stream.cpp
 *  Version: 1.0 (03/06/2016)
 *  Compilation Command: spar -std=c++1y -spar_file file_stream.cpp -o exe
 ****************************************************************************
*/

#include <iostream>
#include <cstdio>
#include <fstream>
#include <cstring>

std::ofstream stream_out;
std::ifstream stream_in;

//just read a string from the file
void read_in(std::string &stream_element){
	stream_in >> stream_element;
}
//just add bracket before and after the string
void compute(std::string &stream_element){
	stream_element = "[" + stream_element + "]\n";	
}
//just write a string into the file
void write_out(std::string &stream_element){
	stream_out << stream_element;
}
/*

This annotation schema in proc_spar1() produces an activity graph such as: 
	 (read)
	    | 
		|
    (compute) 
    	|
		|
	 (write)
 */
void proc_spar1(){
	[[spar::ToStream]] while(1){
		std::string stream_element;
		read_in(stream_element);
		if(stream_in.eof()) break;
		[[spar::Stage,spar::Input(stream_element),spar::Output(stream_element)]]
		{ compute(stream_element); }
		[[spar::Stage,spar::Input(stream_element)]]
		{ write_out(stream_element); }
	}
}
/*

This annotation schema in proc_spar2() produces an activity graph such as: 
	          (read)
	     _______||______ 
		| 				|
    (compute)  ...  (compute)
    	|_______  ______|
			    ||
			 (write)

ps. you may use 'spar_ordered'  flag to write strings ordered in this annotation schema
 */
void proc_spar2(){
	[[spar::ToStream]] while(1){
		std::string stream_element;
		read_in(stream_element);
		if(stream_in.eof()) break;
		[[spar::Stage,spar::Input(stream_element),spar::Output(stream_element),spar::Replicate()]]
		{ compute(stream_element); }
		[[spar::Stage,spar::Input(stream_element)]]
		{ write_out(stream_element); }
	}
}
/*

This annotation schema in proc_spar3() produces an activity graph such as: 
	             (read)
	     __________||_________ 
		| 					|
 (compute,write) ... (compute,write)

 */
void proc_spar3(){
	[[spar::ToStream]] while(1){
		std::string stream_element;
		read_in(stream_element);
		if(stream_in.eof()) break;
		[[spar::Stage,spar::Input(stream_element),spar::Replicate()]]
		{ compute(stream_element); 
		  write_out(stream_element); }
	}
}
/*

This annotation schema in proc_spar4()  produces an activity graph such as: 
	          (read)
	     _______||______ 
		| 				|
    (compute)  ...  (compute)
    	|		 		|
		|				|
	 (write)   ...   (write)
			 
 */
void proc_spar4(){
	[[spar::ToStream]] while(1){
		std::string stream_element;
		read_in(stream_element);
		if(stream_in.eof()) break;
		[[spar::Stage,spar::Input(stream_element),spar::Output(stream_element),spar::Replicate()]]
		{ compute(stream_element); }
		[[spar::Stage,spar::Input(stream_element),spar::Replicate()]]
		{ write_out(stream_element); }
	}
}
//main function
int main(int argc, char const *argv[]){
	stream_in.open("input.txt",std::ios::in);
	if (stream_in.fail()){
		std::cerr << "Error in: " << "input.txt"  << std::endl;
		stream_in.close();
		return 1;
	}
	stream_out.open("ouput.txt",std::ios::out);
	if (stream_out.fail()){
		std::cerr << "Error in: " << "ouput.txt"  << std::endl;
		stream_out.close();
		return 1;
	}
	//uncomment the desired function call
	//proc_spar1();
	proc_spar2();
	//proc_spar3();
	//proc_spar4();
	stream_in.close();
	stream_out.close();
	return 0;
}