# Performance Benchmarks

This directory holds a simple depth benchmark for the C minimax engine.

- `board_empty.txt`: 7x7 empty board input fed to the engine.
- `depth_benchmark.sh`: Runs the engine for K depths 6–10, rebuilding between each run. The script restores the original `K` value defined in `engine/connect4.h` when it finishes.
- `depth_benchmark_results.txt`: Captured output from `depth_benchmark.sh` as executed on this host (shared CPU, Render-like). Includes the engine's column choice and `/usr/bin/time` statistics.

To reproduce, run from the repo root:

```bash
performance/depth_benchmark.sh
```

The script writes fresh results back to `depth_benchmark_results.txt` each time.
