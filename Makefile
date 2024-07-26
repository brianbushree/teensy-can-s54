CXX=g++
RM=rm -f
CPPFLAGS=-std=c++14 # Teensy supports c++14
LDFLAGS=
LDLIBS=
CP=cp
MKDIR=mkdir
ARDUINO_CLI=arduino-cli
ARDUINO_CLI_BOARD=teensy:avr:teensy40
ARDUINO_CLI_FIND_PORT=`arduino-cli board list --json | jq -e -r '.detected_ports[] | select(has("matching_boards") and (.matching_boards[] | select(.fqbn == "$(ARDUINO_CLI_BOARD)")) ) | .port.address'`
ARDUINO_CLI_FIND_PORT_LABEL=`arduino-cli board list --json | jq -e -r '.detected_ports[] | select(has("matching_boards") and (.matching_boards[] | select(.fqbn == "$(ARDUINO_CLI_BOARD)")) ) | .port.label'`

ms43CanHead=$(wildcard ./ms43-can/*.h)
ms43CanImpl=$(wildcard ./ms43-can/*.cpp)
ms43CanTestImpl=$(wildcard ./ms43-can/test/*.cpp)

nextionHead=$(wildcard ./nextion/*.h)
nextionImpl=$(wildcard ./nextion/*.cpp)
nextionTestImpl=$(wildcard ./nextion/test/*.cpp)

inoLibs=$(wildcard ./teensy-controller/libraries/ms43-can/*.h) $(wildcard ./teensy-controller/libraries/nextion/*.h) \
	$(wildcard ./teensy-controller/libraries/ms43-can/*.cpp) $(wildcard ./teensy-controller/libraries/nextion/*.cpp)

bin/ms43CanTest: $(ms43CanTestImpl) $(ms43CanHead) $(ms43CanImpl)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -o bin/ms43CanTest $(ms43CanTestImpl) $(ms43CanImpl) $(LDLIBS)

bin/nextionTest: $(nextionTestImpl) $(nextionHead) $(nextionImpl)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -o bin/nextionTest $(nextionTestImpl) $(nextionImpl) $(LDLIBS)

teensy-controller/libraries/%: $(ms43CanHead) $(ms43CanImpl) $(nextionHead) $(nextionImpl)
	$(MKDIR) -p teensy-controller/libraries/ms43-can/ && $(CP) $(ms43CanHead) teensy-controller/libraries/ms43-can/ && \
    $(CP) $(ms43CanImpl) teensy-controller/libraries/ms43-can/
	$(MKDIR) -p teensy-controller/libraries/nextion/ &&	$(CP) $(nextionHead) teensy-controller/libraries/nextion/

teensy-controller/build/teensy-controller%: teensy-controller/libraries/% teensy-controller/teensy-controller.ino
	cd teensy-controller/ && $(ARDUINO_CLI) compile -b $(ARDUINO_CLI_BOARD) --libraries=libraries --output-dir build/

build: buildCpp buildIno

buildCpp: bin/ms43CanTest bin/nextionTest

buildIno: teensy-controller/build/teensy-controller.ino.hex

upload: buildIno
	export PORT=$(ARDUINO_CLI_FIND_PORT) ;\
	[[ "$$PORT" == "" ]] && { echo "ERROR: Board not found" ; exit 1; } ; \
	cd teensy-controller/ && $(ARDUINO_CLI) upload -b $(ARDUINO_CLI_BOARD) -p $$PORT

monitor:
	export PORT=$(ARDUINO_CLI_FIND_PORT_LABEL) ;\
	[[ "$$PORT" == "" ]] && { echo "ERROR: Board not found" ; exit 1; } ; \
	printf "\n\nNote: Type command '~.' to exit\n\n" && \
	sudo cu -s 9600 -l /dev/cu.usbmodem142113501

test: buildCpp
	bin/ms43CanTest
	bin/nextionTest

clean:
	$(RM) bin/ms43CanTest bin/nextionTest
	$(RM) -r teensy-controller/build/
	$(RM) -r teensy-controller/libraries/