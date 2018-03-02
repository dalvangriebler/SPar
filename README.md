/*
	Author: Dalvan Griebler
	Description: Basic tutorial how to use SPar DSL.
	Last update: 25/05/2016
*/


SPar means an Embedded C++ Domain-Specific Language for Stream Parallelism

SPar is composed by 5 attributes in total, where there is 2 annotation types. Annotation are identifier (ID) keywords while others are auxiliary (AUX). They are declared using the C++ 11/14 standard annotation mechanism:
	[[<attr-list>]]

Spar syntax is such follow. The first attribute of the list defines the type of the annotation followed by its auxiliary attributes:

	[[ID, AUX, ...]]

	* Obs: the auxiliary attributes possibly associated to the ID attribute are not necessary ordered.

ID:
	- ToStream
		to denote that a given C++ program region is going to provide stream parallelism. A 'ToStream' annotation my only be used to annotate a compound statement or iteration statement. SPar requires that an annotated region must contain at least one SPar stage.

	- Stage
		is used to annotate a computing phase where operations are computed over the stream items. If you imagine that you are in an assembly line, 'Stage' is a workstation in the production line. Inside a 'ToStream' region, SPar supports any number of Stages. Also, it may be used to annotate any C++ statement (including a compound statement in brackets) and it may be equipped with AUX attributes to further specify the input/output data needed to compute the stage as well as the parallelism degree (with 'Replicate' attribute) to be used to implement the stage.

AUX:
	- Input()
		this attribute is used to express the input data in both ID attribute annotations' list. It is a way to explicitly say what a given region will consume. 

	- Output() 
		this attribute is used to express the output data stream in both ID attribute annotations' list. In contrast to input attributes, it means what a given region will produce.

	* Obs:
		= Their arguments will be parsed to build the stream of tasks (data items) that will flow inside the 'ToStream' or 'Stage' region.
		= When using these attributes, at least one argument should be given. It can be a variable and literals are not accepted.
	
	- Replicate()
		replicates as many as specified/assigned workers to a given stage region. No more than a single argument is accepted to represent the number of replicas (workers) in a given stage. This argument can be an integer literal or an integer variable. If no argument is passed, SPar gets the number of workers from the 'SPAR_NUM_WORKERS' environment variable.

__________________________________________________________________________________________

$ Root directory description

.bin 
	contains the 'spar' compiler, which is responsible for recognizing SPar annotation and generate stream-based parallel code targeting multi-core systems with FastFlow library (which builds on top of Pthreads)

.doc
	contains all documentation

.examples
	examples using SPar annotations

.libraries
	are the libraries used by SPar

For compiling a program with spar, please type:

	$ bin/spar -std=c++1y -spar_file <filename>.cpp -o <filename> 


_______________________________________________________________________

One may use customized spar flags. The options are the following:


#used to print in the screen the generated code
	-spar_print

#used to generate ondmand scheduler
	-spar_ondemand [<queue_size>]

#used to preserve the stream elements' ordered
	-spar_ordered

#used to activate the blocking mode of FastFlow
	-spar_blocking

#used to generate code for cluster
	-spar_cluster


* Obs: you may use '-h' to see more options







