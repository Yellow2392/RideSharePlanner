import { useState } from 'react';
import type { CSSProperties } from 'react';

const styles: Record<string, CSSProperties> = {
  button: {
    padding: '6px 12px',
    margin: '0 4px 4px 0',
    border: '1px solid #333',
    borderRadius: '4px',
    backgroundColor: '#007bff',
    color: 'white',
    cursor: 'pointer',
    fontWeight: 'bold',
  },
  buttonRow: { marginBottom: '1rem' },

  table: {
    borderCollapse: 'collapse',
    marginBottom: '1rem',
    backgroundColor: 'white',
    width: '100%',
  },
  th: {
    backgroundColor: '#f2f2f2',
    padding: '6px 10px',
    border: '1px solid #ccc',
  },
  td: {
    padding: '6px 10px',
    border: '1px solid #ccc',
    textAlign: 'center',
  },
  pre: {
    whiteSpace: 'pre-wrap',
    background: '#1e1e1e',
    color: '#0f0',
    padding: '1rem',
    marginTop: '1rem',
    borderRadius: '6px',
    fontSize: '0.9rem',
    overflowX: 'auto',
  },
};

const rand = (min: number, max: number): number =>
  Math.floor(Math.random() * (max - min + 1)) + min;

export default function Planner() {
  const [reqs, setReqs] = useState<any[]>([]);
  const [vehs, setVehs] = useState<any[]>([]);
  const [out,  setOut]  = useState('—');

  /* ------------------- DEMO ------------------- */
  const addReq = () => {
    const pick    = [rand(0, 20), rand(0, 20)];
    const drop    = [rand(0, 20), rand(0, 20)];
    const release = rand(0, 10);
    const dist    = Math.abs(pick[0] - drop[0]) + Math.abs(pick[1] - drop[1]);
    const deadline = release + dist + rand(Math.floor(dist*0.3), Math.floor(dist*0.7));

    setReqs(r => [...r, {
      id: r.length + 1,
      pick, drop, release, deadline,
      passengers: 1,
      payment: rand(5, 10)
    }]);
  };

  const addVeh = () =>
    setVehs(v => [...v, {
      id: v.length + 1,
      start: [rand(0, 20), rand(0, 20)],
      capacity: 4,
    }]);
  /* -------------------------------------------- */

  // 🆕 ------------- DESCARGAR JSON -------------
  const downloadJson = () => {
    const data = { requests: reqs, vehicles: vehs };
    const json = JSON.stringify(data, null, 2); 
    const blob = new Blob([json], { type: 'application/json' });
    const url  = URL.createObjectURL(blob);

    const a = document.createElement('a');
    const ts = new Date().toISOString().replace(/[:.]/g, '-');
    a.href = url;
    a.download = `rideshare_${ts}.json`;
    a.click();
    URL.revokeObjectURL(url);
  };
  // --------------------------------------------

  const run = async () => {
    try {
      const res  = await fetch('/run', {
        method:'POST',
        headers:{'Content-Type':'application/json'},
        body: JSON.stringify({ requests:reqs, vehicles:vehs })
      });
      const text = await res.text();
      setOut(text);
    } catch (err) {
      console.error('Error al llamar /run', err);
    }
  };

  return (
    <div style={{ fontFamily:'monospace',backgroundColor: '##e0f7fa', minHeight: '100vh', padding: '1rem' }}>
      <div style={styles.buttonRow}>
        <button onClick={addReq} style={styles.button}>+Request</button>
        <button onClick={addVeh} style={styles.button}>+Vehicle</button>
        <button onClick={run}     style={styles.button}>Ejecutar</button>
        <button onClick={downloadJson} style={styles.button}>Descargar JSON</button>
      </div>

      {/* ---------- tabla Requests ---------- */}
      <h3>Requests</h3>
      {reqs.length === 0 ? (
        <p>(vacío)</p>
      ) : (
        <table style={styles.table}>
          <thead>
            <tr>
              <th style={styles.th}>ID</th>
              <th style={styles.th}>Pick</th>
              <th style={styles.th}>Drop</th>
              <th style={styles.th}>Release</th>
              <th style={styles.th}>Deadline</th>
              <th style={styles.th}>Pax</th>
              <th style={styles.th}>Pay</th>
            </tr>
          </thead>
          <tbody>
            {reqs.map(r => (
              <tr key={r.id}>
                <td style={styles.td}>{r.id}</td>
                <td style={styles.td}>{r.pick[0]},{r.pick[1]}</td>
                <td style={styles.td}>{r.drop[0]},{r.drop[1]}</td>
                <td style={styles.td}>{r.release}</td>
                <td style={styles.td}>{r.deadline}</td>
                <td style={styles.td}>{r.passengers}</td>
                <td style={styles.td}>{r.payment}</td>
              </tr>
            ))}
          </tbody>
        </table>
      )}

      {/* ---------- tabla Vehicles ---------- */}
      <h3 style={{ marginTop:'1rem' }}>Vehicles</h3>
      {vehs.length === 0 ? (
        <p>(vacío)</p>
      ) : (
       <table style={styles.table}>
          <thead>
            <tr>
              <th style={styles.th}>ID</th>
              <th style={styles.th}>Start</th>
              <th style={styles.th}>Cap</th>
            </tr>
          </thead>
          <tbody>
            {vehs.map(v => (
              <tr key={v.id}>
                <td style={styles.td}>{v.id}</td>
                <td style={styles.td}>{v.start[0]},{v.start[1]}</td>
                <td style={styles.td}>{v.capacity}</td>
              </tr>
            ))}
          </tbody>
        </table>
      )}

      {/* ---------- salida del algoritmo ---------- */}
    <pre style={styles.pre}>{out}</pre>
    </div>
  );
}
