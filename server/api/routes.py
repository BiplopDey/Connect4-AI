from __future__ import annotations

from fastapi import APIRouter, Depends, HTTPException
from fastapi.responses import FileResponse

from server.config import get_settings
from server.dependencies import get_game_service
from server.schemas import MoveRequest, MoveResponse, NewGameResponse
from server.services.engine import EngineError
from server.services.game import GameService, InvalidMoveError

router = APIRouter()


@router.post("/api/new", response_model=NewGameResponse)
def new_game(human_starts: bool = True, service: GameService = Depends(get_game_service)):
    state = service.new_game(human_starts)
    return NewGameResponse.from_state(state)


@router.post(
    "/api/human-move",
    response_model=MoveResponse,
    response_model_exclude_none=True,
)
def human_move(request: MoveRequest, service: GameService = Depends(get_game_service)):
    try:
        outcome = service.human_move(request.board, request.column)
    except InvalidMoveError as exc:
        raise exc
    except EngineError as exc:
        raise HTTPException(status_code=500, detail=str(exc)) from exc
    return MoveResponse.from_outcome(outcome)


@router.get("/health")
def health():
    settings = get_settings()
    engine_path = settings.engine_path
    if not engine_path.exists():
        raise HTTPException(status_code=503, detail="engine binary not found")
    return {"status": "ok", "enginePath": str(engine_path)}


@router.get("/")
def index():
    settings = get_settings()
    index_path = settings.static_dir / "index.html"
    return FileResponse(index_path)


__all__ = ["router"]
