LIBS_RYSY_PATH=$(HOME)/libs/rysy

export LIBS_RYSY_PATH


all:
	cd libs && make -j4
	cd libs_detector && make -j4

	cd detector && make -j4
	cd detector_test && make -j4
	cd tests && make -j4


clean:
	cd libs && make clean
	cd libs_detector && make clean

	cd detector && make clean
	cd detector_test && make clean
	cd tests && make clean
