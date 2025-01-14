from fastapi import FastAPI, WebSocket, Request
from fastapi.middleware.cors import CORSMiddleware
from fastapi.staticfiles import StaticFiles
from fastapi.templating import Jinja2Templates
import os
from pathlib import Path

app = FastAPI()

# กำหนด path สำหรับ static files และ templates
BASE_DIR = Path(__file__).resolve().parent
STATIC_DIR = BASE_DIR / "static"
TEMPLATES_DIR = BASE_DIR / "templates"

# สร้างโฟลเดอร์ถ้ายังไม่มี
STATIC_DIR.mkdir(exist_ok=True)
TEMPLATES_DIR.mkdir(exist_ok=True)

# Mount static files
app.mount("/static", StaticFiles(directory=str(STATIC_DIR)), name="static")

# Setup templates
templates = Jinja2Templates(directory=str(TEMPLATES_DIR))

# CORS middleware
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# WebSocket connections
websocket_connections = set()

@app.websocket("/ws")
async def websocket_endpoint(websocket: WebSocket):
    await websocket.accept()
    websocket_connections.add(websocket)
    try:
        while True:
            data = await websocket.receive_text()
            # Broadcast to all connected clients
            for connection in websocket_connections:
                await connection.send_text(data)
    except:
        websocket_connections.remove(websocket)

@app.get("/")
async def root(request: Request):
    return templates.TemplateResponse("index.html", {"request": request})

@app.post("/sensor_data")
async def receive_sensor_data(data: dict):
    # Broadcast to all WebSocket clients
    for connection in websocket_connections.copy():
        try:
            await connection.send_json(data)
        except:
            websocket_connections.remove(connection)
    return {"status": "ok"}

if __name__ == "__main__":
    import uvicorn
    print(f"Starting server...")
    print(f"Static files directory: {STATIC_DIR}")
    print(f"Templates directory: {TEMPLATES_DIR}")
    uvicorn.run(app, host="0.0.0.0", port=8000)