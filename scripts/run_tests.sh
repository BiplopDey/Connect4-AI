#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
cd "${ROOT_DIR}"

export PYTHONPATH="${ROOT_DIR}:${PYTHONPATH:-}"

if ! command -v pytest >/dev/null 2>&1; then
  echo "[error] pytest is not installed; run 'pip install -r requirements-dev.txt'" >&2
  exit 1
fi

if ! command -v make >/dev/null 2>&1; then
  echo "[error] make is required; install build-essential." >&2
  exit 1
fi

set -x
make -C engine search_tests board_tests engine
./engine/search_tests
./engine/board_tests
pytest
