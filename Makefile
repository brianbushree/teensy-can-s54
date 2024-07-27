PIO=pio
ARDUINO_CLI=arduino-cli
ARDUINO_CLI_BOARD=teensy:avr:teensy40
ARDUINO_CLI_FIND_PORT=`arduino-cli board list --json | jq -e -r '.detected_ports[] | select(has("matching_boards") and (.matching_boards[] | select(.fqbn == "$(ARDUINO_CLI_BOARD)")) ) | .port.address'`
ARDUINO_CLI_FIND_PORT_LABEL=`arduino-cli board list --json | jq -e -r '.detected_ports[] | select(has("matching_boards") and (.matching_boards[] | select(.fqbn == "$(ARDUINO_CLI_BOARD)")) ) | .port.label'`

.PHONY: build
build:
	$(PIO) run -e teensy40

.PHONY: upload
upload:
	$(PIO) run -e teensy40 -t upload

# TODO: update this to use '$(PIO) device monitor'
.PHONY: monitor
monitor:
	export PORT=$(ARDUINO_CLI_FIND_PORT_LABEL) ;\
	[[ "$$PORT" == "" ]] && { echo "ERROR: Board not found" ; exit 1; } ; \
	printf "\n\nNote: Type command '~.' to exit\n\n" && \
	sudo cu -s 9600 -l /dev/cu.usbmodem142113501

.PHONY: test
test:
	$(PIO) test -e test -v

.PHONY: clean
clean:
	$(PIO) run -e teensy40 -t clean
	$(PIO) run -e test -t clean
