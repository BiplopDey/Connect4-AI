## Multi-stage build: compile in builder, run in slim image

FROM debian:bookworm-slim AS builder

RUN apt-get update \
 && apt-get install -y --no-install-recommends \
      clang make build-essential ca-certificates \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy source
COPY . /app

# Build engine + cli
RUN make -C engine clean || true \
 && make -C engine engine \
 && make -C engine main


FROM python:3.11-slim AS runtime

WORKDIR /app

# Copy binaries and app
COPY --from=builder /app/engine /app/engine
COPY server /app/server
COPY web /app/web

ENV CONNECT4_ENGINE=/app/engine/engine

RUN pip install --no-cache-dir fastapi uvicorn[standard]

EXPOSE 8000

CMD ["uvicorn", "server.app:app", "--host", "0.0.0.0", "--port", "8000"]
