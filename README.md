# Connect Four :robot:

`Connect Four (also known as Four Up, Plot Four, Find Four, Captain's Mistress, Four in a Row, Drop Four, and Gravitrips in the Soviet Union) is a two-player connection board game, in which the players choose a color and then take turns dropping colored tokens into a seven-column, six-row vertically suspended grid. The pieces fall straight down, occupying the lowest available space within the column`[https://en.wikipedia.org/wiki/Connect_Four].

<img src="https://upload.wikimedia.org/wikipedia/commons/a/ad/Connect_Four.gif" width="400" height="250" />


## Stack
- C core with MiniMax + alpha-beta pruning
- Headless engine binary for AI move selection
- FastAPI backend exposing HTTP endpoints
- Static web UI (vanilla JS) served by the backend

## How to Play
Run locally with Docker (below) and open the web UI. Click a column to drop your token (human = O, AI = X). The AI responds automatically.

### Quick Start (Docker)

Build the image and run the web server:

```
docker build -t connect4-ai .
docker run --rm -p 8000:8000 connect4-ai
```

Open http://localhost:8000 and play in the browser.

CLI build is still available inside the image (`/app/main`) but the default container entry runs the web server.

### API Overview
- `POST /api/new` → `{ board: number[7][7], next: "human"|"ai" }`
- `POST /api/human-move` with body `{ board, column }` → `{ board, aiColumn?, result }`
  - `result`: 1 = human win, 2 = AI win, 3 = draw, 4 = continue

Example:

```
curl -X POST localhost:8000/api/new
curl -X POST localhost:8000/api/human-move \
  -H 'Content-Type: application/json' \
  -d '{"board": [[0,0,0,0,0,0,0], [0,0,0,0,0,0,0], [0,0,0,0,0,0,0], [0,0,0,0,0,0,0], [0,0,0,0,0,0,0], [0,0,0,0,0,0,0], [0,0,0,0,0,0,0]], "column": 3}'
```

### Local Development (without Docker)

Build C binaries:

```
make clean && make engine && make main
```

Run the web server:

```
pip install fastapi uvicorn[standard]
export CONNECT4_ENGINE=$(pwd)/engine   # or set in your shell
uvicorn server.app:app --reload --port 8000
```

Open http://localhost:8000

### Configuration
- Board size: currently fixed to 7x7 (see `connect4.h` `#define N 7`).
- AI depth: `connect4.h` `#define K 6`. Increase for stronger AI (slower), rebuild.
- Engine path: set `CONNECT4_ENGINE` env var (default `/app/engine` in Docker).

### Deploy Online
- Single container: `docker run -d -p 80:8000 connect4-ai` then point DNS to the host.
- Behind a proxy: put behind Nginx/Traefik and proxy to `:8000`. Add TLS at the proxy.

## Enjoy playing 
And let me know if you have won against the AI.
