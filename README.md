SPar is an internal C++ Domain-Specific Language (DSL) for expressing high-level stream parallelism.



# Root directory description

## .bin 

contains the 'spar' compiler, which is responsible for recognizing SPar annotation and generate stream-based parallel code targeting multi-core systems with FastFlow library (which builds on top of Pthreads)

## .doc

contains all documentation

## .examples

examples using SPar annotations

## .libraries

are the libraries used by SPar

# For compiling a program with spar, please type:

	$ bin/spar -std=c++1y -spar_file <filename>.cpp -o <filename> 

# SPar compilation flags

## Used to print in the screen the generated code
	-spar_print

## used to generate ondmand scheduler
	-spar_ondemand [<queue_size>]

## used to preserve the stream elements' ordered
	-spar_ordered

## used to activate the blocking mode of FastFlow
	-spar_blocking


*Obs: you may use '-h' to see more options*







