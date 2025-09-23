const N = 7; // must match backend
let board = Array.from({ length: N }, () => Array(N).fill(0));
let locking = false;
let gameOver = false;

const CELL = 72, GAP = 8, PAD = 8, PIECE = 56, OFFSET = (CELL - PIECE) / 2; // px

const elBoard = document.getElementById('board');
const elStatus = document.getElementById('status');
const elNewGame = document.getElementById('new-game');

const BASE_CELL_CLASSES = [
  'cell',
  'relative',
  'flex',
  'items-center',
  'justify-center',
  'w-[72px]',
  'h-[72px]',
  'rounded-full',
  'bg-gradient-to-b',
  'from-slate-900',
  'to-slate-950',
  'shadow-[inset_0_0_0_2px_rgba(55,65,81,1)]',
  'cursor-pointer',
  'transition-transform',
  'duration-150',
  'hover:-translate-y-0.5',
].join(' ');

const BASE_TOKEN_CLASSES = [
  'token',
  'w-[56px]',
  'h-[56px]',
  'rounded-full',
  'bg-slate-800',
  'transition-colors',
  'duration-150',
].join(' ');

const HUMAN_TOKEN_CLASSES = ['bg-token-human', 'shadow-lg'];
const AI_TOKEN_CLASSES = ['bg-token-ai', 'shadow-lg'];

function render() {
  elBoard.innerHTML = '';
  for (let i = 0; i < N; i++) {
    for (let j = 0; j < N; j++) {
      const v = board[i][j];
      const cell = document.createElement('div');
      cell.className = BASE_CELL_CLASSES;
      cell.dataset.col = j;
      cell.dataset.row = i;
      cell.addEventListener('click', onClickCol);
      const token = document.createElement('div');
      token.className = BASE_TOKEN_CLASSES;
      if (v === 1) {
        token.classList.add(...HUMAN_TOKEN_CLASSES);
      } else if (v === 2) {
        token.classList.add(...AI_TOKEN_CLASSES);
      }
      cell.appendChild(token);
      elBoard.appendChild(cell);
    }
  }
}

function cellLeftPx(col) { return PAD + col * (CELL + GAP) + OFFSET; }
function cellTopPx(row) { return PAD + row * (CELL + GAP) + OFFSET; }

function animateDrop(col, row, player) {
  return new Promise(resolve => {
    const piece = document.createElement('div');
    piece.className = 'piece-floating';
    piece.dataset.player = player === 1 ? 'human' : 'ai';
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
  locking = true;
  elStatus.textContent = 'AI thinking…';

  const moveReq = fetch('/api/human-move', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ board, column: col }),
  });

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

  // Animate human move as confirmed by backend
  if (Number.isInteger(data.humanRow)) {
    await animateDrop(col, data.humanRow, 1);
    board[data.humanRow][col] = 1;
    render();
  }

  const hasAi = Number.isInteger(data.aiColumn) && Number.isInteger(data.aiRow);
  if (hasAi) {
    // Delay AI response for a more natural feel
    const AI_DELAY_MS = 700;
    await new Promise(r => setTimeout(r, AI_DELAY_MS));
    await animateDrop(data.aiColumn, data.aiRow, 2);
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
    if (!cell) continue;
    cell.classList.add('ring-4', 'ring-yellow-300', 'ring-offset-2', 'ring-offset-slate-900');
    cell.style.boxShadow = '0 0 18px rgba(253, 224, 71, 0.65)';
  }
}
