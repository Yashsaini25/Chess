import React, { useState, useEffect } from 'react'
import { Chessboard } from 'react-chessboard'

export default function App() {
  const [gamePosition, setGamePosition] = useState(
    'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1'
  ) // initial FEN
  const [timeLeft, setTimeLeft] = useState(60);
  const [darkMode, setDarkMode] = useState(false)

    useEffect(() => {
    setTimeLeft(60); // Reset timer on each move

    const timer = setInterval(() => {
      setTimeLeft(prev => {
        if (prev <= 1) {
          clearInterval(timer);
          alert("⏰ Time's up!");
          return 0;
        }
        return prev - 1;
      });
    }, 1000);

    return () => clearInterval(timer); // Clean up on re-render
  }, [gamePosition]);

  async function onDrop(sourceSquare, targetSquare) {
    const move = `${sourceSquare}-${targetSquare}`

    try {
      const response = await fetch('http://localhost:8000/move/', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ move, gamePosition }),
      })

      const data = await response.json()

      if (!response.ok || !data.valid) {
        alert('Invalid move: ' + (data.error || 'Unknown error'))
        return false
      }

      setGamePosition(data.fen) // <-- use 'fen', not 'bot_fen'
      return true
    } catch (err) {
      console.error('Failed to connect to backend:', err)
      alert('Failed to connect to backend')
      return false
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
      justifyContent: 'center', // centers vertically
      textAlign: 'center', // centers text content
    }}
  >
    <h2>Play Against Your C++ Chess Bot</h2>

    <button
      onClick={() => setDarkMode(!darkMode)}
      style={{
        padding: '8px 16px',
        marginBottom: '20px',
        cursor: 'pointer',
        borderRadius: '5px',
        backgroundColor: darkMode ? '#333' : '#ddd',
        color: darkMode ? '#fff' : '#000',
        border: 'none',
      }}
    >
      Toggle {darkMode ? 'Light' : 'Dark'} Mode
    </button>

    <p style={{ fontSize: '18px', fontWeight: 'bold' }}>🕒 Time left: {timeLeft}s</p>

    <div style={{ marginTop: '20px' }}>
      <Chessboard
        position={gamePosition}
        onPieceDrop={onDrop}
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
  </div>
)
}

