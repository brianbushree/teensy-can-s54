CXX=g++
RM=rm -f
CPPFLAGS=-std=c++14 # Teensy supports c++14
LDFLAGS=
LDLIBS=

ms43CanHead=$(wildcard ./ms43-can/*.h)
ms43CanImpl=$(wildcard ./ms43-can/*.cpp)
sampleDataTestImpl=$(wildcard ./sample-data-test/*.cpp)

build: bin/sampleDataTest

bin/sampleDataTest: $(sampleDataTestImpl) $(ms43CanHead) $(ms43CanImpl)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -o bin/sampleDataTest $(sampleDataTestImpl) $(ms43CanImpl) $(LDLIBS)

test: build
	bin/sampleDataTest

clean:
	$(RM) bin/sampleDataTest
