LIBS_RYSY_PATH=$(HOME)/libs/rysy

export LIBS_RYSY_PATH


all:
	cd libs && make -j4
	cd tests && make
	cd detector && make -j4


clean:
	cd libs && make clean
	cd tests && make clean
	cd detector && make clean
