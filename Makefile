LIBS_RYSY_PATH=$(HOME)/libs/rysy/rysy2

export LIBS_RYSY_PATH


all:
	cd libs_detector && make -j4

	cd detector && make -j4
	cd detector_test && make -j4
	cd train && make -j4


clean:
	cd libs_detector && make clean

	cd detector && make clean
	cd detector_test && make clean
	cd train && make clean
