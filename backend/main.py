from fastapi import FastAPI, Request
from fastapi.middleware.cors import CORSMiddleware
import subprocess
import json

app = FastAPI()

# Allow frontend access
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # Replace with frontend URL if deploying
    allow_methods=["*"],
    allow_headers=["*"],
)

@app.post("/move/")
async def make_move(request: Request):
    data = await request.json()
    move = data.get("move")  # e.g., "e2-e4"
    fen = data.get("gamePosition")

    print(f"Received move: {move}")
    try:
        # Call your compiled C++ chess bot (make sure 'chess_bot' is in the same dir)
        result = subprocess.run(
            ["./chess", move, fen],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=5  # Prevent hanging
        )

        if result.stderr:
            return {"valid": False, "error": result.stderr.strip()}

        output = result.stdout.strip()
        print(f"Output from C++: {output}")
        return json.loads(output)  # Expecting JSON string from C++
    
    except Exception as e:
        return {"valid": False, "error": str(e)}
