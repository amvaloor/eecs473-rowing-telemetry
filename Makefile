# Root Makefile for Rowing Telemetry Monorepo
# Makefile courtesy of Gemini
PIO_DIR = firmware

.PHONY: help all build-oar build-central build-coach upload-oar upload-central upload-coach flash-oar flash-central flash-coach monitor clean compiledb setup

help:
	@echo "Available commands:"
	@echo "  make all             - Build all target environments"
	@echo "  make build-oar       - Build the Oar node firmware"
	@echo "  make build-central   - Build the Central node firmware"
	@echo "  make build-coach     - Build the Coach node firmware"
	@echo "  make upload-oar      - Upload firmware to Oar target"
	@echo "  make upload-central  - Upload firmware to Central target"
	@echo "  make upload-coach    - Upload firmware to Coach target"
	@echo "  make flash-oar       - Upload and immediately monitor Oar"
	@echo "  make flash-central   - Upload and immediately monitor Central"
	@echo "  make flash-coach     - Upload and immediately monitor Coach"
	@echo "  make monitor         - Open PlatformIO serial monitor"
	@echo "  make compiledb       - Regenerate compile_commands.json for LSP"
	@echo "  make clean           - Clean build artifacts"

# Build Targets
all:
	pio run -d $(PIO_DIR)

build-oar:
	pio run -d $(PIO_DIR) -e oar

build-central:
	pio run -d $(PIO_DIR) -e central

build-coach:
	pio run -d $(PIO_DIR) -e coach

# Upload Targets
upload-oar:
	pio run -d $(PIO_DIR) -e oar -t upload

upload-central:
	pio run -d $(PIO_DIR) -e central -t upload

upload-coach:
	pio run -d $(PIO_DIR) -e coach -t upload

# Combined Upload + Monitor Targets
flash-oar:
	pio run -d $(PIO_DIR) -e oar -t upload -t monitor

flash-central:
	pio run -d $(PIO_DIR) -e central -t upload -t monitor

flash-coach:
	pio run -d $(PIO_DIR) -e coach -t upload -t monitor

# Utilities
monitor:
	pio device monitor -d $(PIO_DIR)

clean:
	pio run -d $(PIO_DIR) -t clean

compiledb:
	pio run -d $(PIO_DIR) -t compiledb
	ln -sf firmware/compile_commands.json compile_commands.json
