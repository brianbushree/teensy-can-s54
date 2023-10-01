CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=
LDFLAGS=
LDLIBS=

ms43CanHead=$(wildcard ./ms43-can/*.h)
ms43CanImpl=$(wildcard ./ms43-can/*.cpp)
sampleDataTestImpl=$(wildcard ./sample-data-test/*.cpp)

build: bin/sampleDataTest

bin/sampleDataTest: $(sampleDataTestImpl) $(ms43CanHead) $(ms43CanImpl)
	$(CXX) $(LDFLAGS) -o bin/sampleDataTest $(sampleDataTestImpl) $(ms43CanImpl) $(LDLIBS)

clean:
	$(RM) bin/sampleDataTest
