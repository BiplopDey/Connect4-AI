#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
ROOT_DIR=$(cd "${SCRIPT_DIR}/.." && pwd)
ENGINE_DIR="${ROOT_DIR}/engine"
CONFIG_PATH="${ENGINE_DIR}/config.h"
ENGINE_BIN="${ENGINE_DIR}/engine"
BOARD_INPUT="${SCRIPT_DIR}/board_empty.txt"
RESULTS_FILE="${SCRIPT_DIR}/depth_benchmark_results.txt"

if [[ ! -f "${BOARD_INPUT}" ]]; then
  echo "missing board input: ${BOARD_INPUT}" >&2
  exit 1
fi

DEFAULT_DEPTH=$(awk '/ENGINE_DEFAULT_MAX_DEPTH/ {print $3}' "${CONFIG_PATH}" | tail -n 1)

make -C "${ENGINE_DIR}" clean >/dev/null
make -C "${ENGINE_DIR}" engine >/dev/null

: >"${RESULTS_FILE}"

printf "Benchmarking depths on input %s\n" "${BOARD_INPUT}" | tee -a "${RESULTS_FILE}"
printf "Default depth: %s\n\n" "${DEFAULT_DEPTH:-unknown}" | tee -a "${RESULTS_FILE}"

for depth in 6 7 8 9 10; do
  printf "depth=%s\n" "${depth}" | tee -a "${RESULTS_FILE}"
  ENGINE_MAX_DEPTH="${depth}" /usr/bin/time -f "%E real, %M KB maxrss" \
    "${ENGINE_BIN}" < "${BOARD_INPUT}" 2> >(tee -a "${RESULTS_FILE}" >&2) |
    tee -a "${RESULTS_FILE}"
  printf "\n" | tee -a "${RESULTS_FILE}"
done
