import React, { useState, useEffect, useRef } from 'react';
import { Chessboard } from 'react-chessboard';
import { toPng } from 'html-to-image'

export default function App() {
  const initialFEN = 'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1';
  const [gamePosition, setGamePosition] = useState(initialFEN);
  const [timeLeft, setTimeLeft] = useState(60);
  const [darkMode, setDarkMode] = useState(false);
  const [gameStarted, setGameStarted] = useState(false);
  const [gameOver, setGameOver] = useState(false);
  const timerRef = useRef(null);
  const [moveHistory, setMoveHistory] = useState([]);
  const chessboardRef = useRef(null)
  const moveSound = new Audio('src/assets/move.mp3')
  const timerActiveRef = useRef(false); 

useEffect(() => {
  if (!gameStarted || gameOver) return;

  setTimeLeft(60);
  let called = false; 

  clearInterval(timerRef.current);

  timerRef.current = setInterval(() => {
    setTimeLeft(prev => {
      if (prev <= 1) {
        clearInterval(timerRef.current);
        if (!called) {
          called = true;
          timerActiveRef.current = false;
          alert("⏰ Time's up! Bot will play now.");
          botMove();
        }
        return 60;
      }
      return prev - 1;
    });
  }, 1000);

  timerActiveRef.current = true;

  return () => {
    clearInterval(timerRef.current);
    timerActiveRef.current = false;
  };
}, [gamePosition, gameStarted, gameOver]);


  const startGame = () => {
    setGamePosition(initialFEN);
    setGameOver(false);
    setGameStarted(true);
    console.log("Game started");
  };

  const restartGame = () => {
    setGamePosition(initialFEN);
    setGameOver(false);
    setTimeLeft(60);
    setGameStarted(false);
    clearInterval(timerRef.current);
    setMoveHistory([]);
  };

  const exportChessboardToPng = async () => {
    if (!chessboardRef.current) return

    try {
      const dataUrl = await toPng(chessboardRef.current)
      const link = document.createElement('a')
      link.download = 'chessboard.png'
      link.href = dataUrl
      link.click()
    } catch (err) {
      console.error('Failed to export chessboard:', err)
    }
  }

  const exportMoveHistoryToTxt = () => {
    let output = ''
    for (let i = 0; i < moveHistory.length; i += 2) {
      const whiteMove = moveHistory[i] || ''
      const blackMove = moveHistory[i + 1] || ''
      output += `${Math.floor(i / 2) + 1}. White: ${whiteMove}${blackMove ? `\n   Black: ${blackMove}` : ''}\n`
    }

    const blob = new Blob([output], { type: 'text/plain' })
    const link = document.createElement('a')
    link.download = 'move_history.txt'
    link.href = URL.createObjectURL(blob)
    link.click()
}

  async function botMove() {
    try {
      const response = await fetch('http://localhost:8000/move/', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ move: 'no-ne', gamePosition }), 
      });
      const data = await response.json();
      if (data.valid) {
        setGamePosition(data.fen);
        moveSound.play();
        setMoveHistory(prev => [...prev, "Black: " + data.aiMove]);
        if (data.isCheckmate) {
          alert("Checkmate! You lose.");
          setGameOver(true);
        } else if (data.isStalemate) {
          alert("Stalemate! It's a draw.");
          setGameOver(true);
        }
      }
    } catch (error) {
      alert('Failed to get bot move');
    }
  }

  async function onDrop(sourceSquare, targetSquare) {
    if (!gameStarted || gameOver) return false;

    const move = `${sourceSquare}-${targetSquare}`;

    try {
      const response = await fetch('http://localhost:8000/move/', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ move, gamePosition }),
      });

      const data = await response.json();

      if (!response.ok || !data.valid) {
        alert('Invalid move: ' + (data.error || 'Unknown error'));
        return false;
      }

      setGamePosition(data.fen);
      moveSound.play();
      setMoveHistory(prev => [...prev, "White: " + move, "Black: " + data.aiMove]); 


      if (data.isCheckmate) {
        alert("Checkmate! You lose.");
        setGameOver(true);
      } else if (data.isStalemate) {
        alert("Stalemate! It's a draw.");
        setGameOver(true);
      }

      return true;
    } catch (err) {
      console.error('Failed to connect to backend:', err);
      alert('Failed to connect to backend');
      return false;
    }
  }

  return (
    <div
      style={{
        padding: '20px',
        minHeight: '100vh',
        backgroundColor: darkMode ? '#121212' : '#f0f0f0',
        color: darkMode ? '#ffffff' : '#000000',
        transition: 'background-color 0.3s, color 0.3s',
        display: 'flex',
        flexDirection: 'column',
        alignItems: 'center',
        justifyContent: 'center',
        textAlign: 'center',
      }}
    >
      <h2>Play Against Your C++ Chess Bot</h2>

      <button
        onClick={() => setDarkMode(!darkMode)}
        style={{
          padding: '8px 16px',
          marginBottom: '10px',
          cursor: 'pointer',
          borderRadius: '5px',
          backgroundColor: darkMode ? '#333' : '#ddd',
          color: darkMode ? '#fff' : '#000',
          border: 'none',
        }}
      >
        Toggle {darkMode ? 'Light' : 'Dark'} Mode
      </button>

      <div style={{ marginBottom: '10px' }}>
        <button
          onClick={startGame}
          style={{ padding: '10px', marginRight: '10px', cursor: 'pointer' }}
        >
          Start Game
        </button>
        <button
          onClick={restartGame}
          style={{ padding: '10px', cursor: 'pointer' }}
        >
          Restart Game
        </button>
      </div>

      {gameStarted && (
        <p style={{ fontSize: '18px', fontWeight: 'bold' }}>
          🕒 Time left: {timeLeft}s
        </p>
      )}

      <div style={{ display: 'flex', justifyContent: 'center', marginTop: '20px' }}>
        {/* Chessboard */}
        <div ref={chessboardRef}>
        <Chessboard
          key={gameStarted ? 'started' : 'not-started'} 
          position={gamePosition}
          onPieceDrop={gameOver || !gameStarted ? () => false : onDrop}
          boardWidth={500}
          boardOrientation="white"
          boardStyle={{
            borderRadius: '10px',
            boxShadow: darkMode ? '0 0 10px #888' : '0 0 10px #aaa',
          }}
          darkSquareStyle={{ backgroundColor: darkMode ? '#888' : '#769656' }}
          lightSquareStyle={{ backgroundColor: darkMode ? '#aaa' : '#eeeed2' }}
        />
        </div>

      <div
        style={{
        marginLeft: '30px',
        maxHeight: '400px',
        overflowY: 'auto',
        width: '250px',
        textAlign: 'left',
        padding: '10px',
        backgroundColor: darkMode ? '#1e1e1e' : '#ffffff',
        borderRadius: '10px',
        boxShadow: darkMode ? '0 0 10px #555' : '0 0 10px #ccc',
        }}
      >
      <h3 style={{ marginBottom: '10px' }}>Move History</h3>
      <ul style={{ listStyleType: 'none', paddingLeft: 0 }}>
      {moveHistory.map((move, idx) => (
        <li key={idx}>
        {idx + 1}. {move}
        </li>
      ))}
    </ul>
  </div>
</div>

      <div style={{ marginTop: '20px' }}>
  <button onClick={exportChessboardToPng} style={{ marginRight: '10px' }}>
    Export Chessboard (PNG)
  </button>
  <button onClick={exportMoveHistoryToTxt}>Export Move History (TXT)</button>
</div>

    </div>
  );
}