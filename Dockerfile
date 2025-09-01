## Multi-stage build: compile in builder, run in slim image

FROM debian:bookworm-slim AS builder

RUN apt-get update \
 && apt-get install -y --no-install-recommends \
      clang make build-essential ca-certificates \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy source
COPY . /app

# Build the binary using the provided Makefile
RUN make clean || true \
 && make main


FROM debian:bookworm-slim AS runtime

WORKDIR /app

# Copy the built binary from builder stage
COPY --from=builder /app/main /app/main

# Default to interactive play
ENTRYPOINT ["/app/main"]
