CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=
LDFLAGS=
LDLIBS=

build: bin/sampleDataTest

bin/sampleDataTest: ./sample-data-test/sampleDataTest.cpp ./ms43-can/MS43Can.cpp ./ms43-can/MS43Can.h
	$(CXX) $(LDFLAGS) -o bin/sampleDataTest ./sample-data-test/sampleDataTest.cpp ./ms43-can/MS43Can.cpp $(LDLIBS)

clean:
	$(RM) bin/sampleDataTest
