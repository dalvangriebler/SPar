.PHONY: all

all: fastflow mydir

fastflow:
	svn checkout svn://svn.code.sf.net/p/mc-fastflow/code/ff libraries/ff

mydir:
	pwd > bin/.cincle_dir

