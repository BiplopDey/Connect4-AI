#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
ROOT_DIR=$(cd "${SCRIPT_DIR}/.." && pwd)
HEADER_PATH="${ROOT_DIR}/engine/connect4.h"
ENGINE_DIR="${ROOT_DIR}/engine"
ENGINE_BIN="${ENGINE_DIR}/engine"
BOARD_INPUT="${SCRIPT_DIR}/board_empty.txt"
RESULTS_FILE="${SCRIPT_DIR}/depth_benchmark_results.txt"

if [[ ! -f "${BOARD_INPUT}" ]]; then
  echo "missing board input: ${BOARD_INPUT}" >&2
  exit 1
fi

original_define=$(grep -E '^#define K [0-9]+' "${HEADER_PATH}" || true)
if [[ -z "${original_define}" ]]; then
  echo "could not locate '#define K <value>' in ${HEADER_PATH}" >&2
  exit 1
fi

original_k=$(awk '{print $3}' <<<"${original_define}")

restore_k() {
  if [[ -n "${original_k}" ]]; then
    perl -0pi -e "s/#define K \\d+/#define K ${original_k}/" "${HEADER_PATH}"
    make -C "${ENGINE_DIR}" clean >/dev/null
    make -C "${ENGINE_DIR}" engine >/dev/null
  fi
}

trap restore_k EXIT

>"${RESULTS_FILE}"

printf "Benchmarking depths on input %s\n" "${BOARD_INPUT}" | tee -a "${RESULTS_FILE}"
printf "Original depth: K=%s\n\n" "${original_k}" | tee -a "${RESULTS_FILE}"

for k in 6 7 8 9 10; do
  perl -0pi -e "s/#define K \\d+/#define K ${k}/" "${HEADER_PATH}"
  make -C "${ENGINE_DIR}" clean >/dev/null
  make -C "${ENGINE_DIR}" engine >/dev/null

  printf "K=%s\n" "${k}" | tee -a "${RESULTS_FILE}"
  /usr/bin/time -f "%E real, %M KB maxrss" "${ENGINE_BIN}" < "${BOARD_INPUT}" 2> >(tee -a "${RESULTS_FILE}" >&2) | tee -a "${RESULTS_FILE}"
  printf "\n" | tee -a "${RESULTS_FILE}"
done
