#!/usr/bin/env bash
# This script is courtesy of Gemini

# Resolve project root relative to script location
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

TARGET_DIR="${PROJECT_ROOT}/firmware/components"
ARDUINO_LIB_DIR="${HOME}/Arduino/libraries"

# Ensure Arduino libraries folder exists
mkdir -p "${ARDUINO_LIB_DIR}"

# Create symlinks
for component in "${TARGET_DIR}"/*; do
    if [ -d "${component}" ]; then
        ln -sf "${component}" "${ARDUINO_LIB_DIR}/"
    fi
done

echo "Arduino library symlinks created successfully!"
