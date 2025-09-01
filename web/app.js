const N = 7; // must match backend
let board = Array.from({ length: N }, () => Array(N).fill(0));
let locking = false;
let gameOver = false;

const CELL = 72, GAP = 8, PAD = 8, PIECE = 56, OFFSET = (CELL - PIECE) / 2; // px

const elBoard = document.getElementById('board');
const elStatus = document.getElementById('status');
const elNewGame = document.getElementById('new-game');

function render() {
  elBoard.innerHTML = '';
  for (let i = 0; i < N; i++) {
    for (let j = 0; j < N; j++) {
      const v = board[i][j];
      const cell = document.createElement('div');
      cell.className = 'cell' + (v === 1 ? ' human' : v === 2 ? ' ai' : '');
      cell.dataset.col = j;
      cell.dataset.row = i;
      cell.addEventListener('click', onClickCol);
      elBoard.appendChild(cell);
    }
  }
}

function findDropRow(b, col) {
  for (let i = N - 1; i >= 0; i--) {
    if (b[i][col] === 0) return i;
  }
  return -1;
}

function isColFull(col) { return board[0][col] !== 0; }

function cellLeftPx(col) { return PAD + col * (CELL + GAP) + OFFSET; }
function cellTopPx(row) { return PAD + row * (CELL + GAP) + OFFSET; }

function animateDrop(col, row, player) {
  return new Promise(resolve => {
    const piece = document.createElement('div');
    piece.className = 'piece-floating ' + (player === 1 ? 'human' : 'ai');
    piece.style.left = cellLeftPx(col) + 'px';
    piece.style.setProperty('--target', cellTopPx(row) + 'px');
    // duration slightly depends on distance for realism
    const dist = cellTopPx(row) + 120; // start height ~120px above
    const duration = Math.max(550, Math.min(1000, 400 + dist * 0.35));
    piece.style.animationDuration = duration + 'ms';
    piece.addEventListener('animationend', () => {
      piece.remove();
      resolve();
    }, { once: true });
    elBoard.appendChild(piece);
  });
}

async function newGame() {
  locking = false;
  gameOver = false;
  try {
    const res = await fetch('/api/new', { method: 'POST' });
    const data = await res.json();
    board = data.board;
    elStatus.textContent = 'Your move.';
    render();
  } catch (e) {
    console.error(e);
  }
}

async function onClickCol(e) {
  if (locking) return;
  if (gameOver) return;
  const col = Number(e.currentTarget.dataset.col);
  if (!Number.isInteger(col) || col < 0 || col >= N) return;
  if (isColFull(col)) return;
  locking = true;
  elStatus.textContent = 'AI thinking…';

  // Predict human drop row and animate immediately
  const hRow = findDropRow(board, col);
  const moveReq = fetch('/api/human-move', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ board, column: col }),
  });

  await animateDrop(col, hRow, 1);
  board[hRow][col] = 1;
  render();

  let data;
  try {
    const res = await moveReq;
    data = await res.json();
    if (!res.ok) throw new Error(data?.detail || 'Move failed');
  } catch (err) {
    console.error(err);
    elStatus.textContent = 'Error: ' + err.message;
    locking = false;
    return;
  }

  const hasAi = Number.isInteger(data.aiColumn);
  if (hasAi) {
    // Delay AI response for a more natural feel
    const AI_DELAY_MS = 700;
    await new Promise(r => setTimeout(r, AI_DELAY_MS));
    const aiCol = data.aiColumn;
    const aiRow = findDropRow(board, aiCol);
    await animateDrop(aiCol, aiRow, 2);
  }

  // Sync with server state and render final position
  board = data.board;
  render();

  if (data.result === 2) { elStatus.textContent = 'AI wins! 🤖'; if (Array.isArray(data.winLine)) highlightWin(data.winLine); gameOver = true; }
  else if (data.result === 1) { elStatus.textContent = 'You win! 🎉'; if (Array.isArray(data.winLine)) highlightWin(data.winLine); gameOver = true; }
  else if (data.result === 3) { elStatus.textContent = 'Draw.'; gameOver = true; }
  else { elStatus.textContent = 'Your move.'; }

  locking = false;
}

elNewGame.addEventListener('click', newGame);
newGame();

// Responsive: scale board to fit viewport
function updateScale() {
  const BOARD_W = PAD*2 + N*CELL + (N-1)*GAP;
  const BOARD_H = PAD*2 + N*CELL + (N-1)*GAP;
  const availW = Math.max(320, window.innerWidth - 24);
  const availH = Math.max(320, window.innerHeight - 220); // leave room for header/controls
  const scale = Math.min(1, availW / BOARD_W, availH / BOARD_H);
  elBoard.style.transform = `scale(${scale})`;
}

window.addEventListener('resize', updateScale);
// adjust after each render to recalc size
const _render = render;
render = function() { _render(); updateScale(); };
updateScale();

function highlightWin(line) {
  // line: array of [row, col]
  const cells = elBoard.querySelectorAll('.cell');
  for (const [r, c] of line) {
    const idx = r * N + c;
    const cell = cells[idx];
    if (cell) cell.classList.add('win');
  }
}
