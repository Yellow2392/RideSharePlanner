import { useState } from 'react'
import reactLogo from './assets/react.svg'
import viteLogo from '/vite.svg'
import './App.css'
import Planner from './Planner';


export default function App() {
  return (
    <main style={{padding:'1.5rem',backgroundColor: 'rgba(255, 255, 255, 0.7)',borderRadius: '10px'}}>
      <h1>🚗 POKE RideShare Planner</h1>
      <Planner />
    </main>
  );
}