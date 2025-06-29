import express from 'express';
import { spawn } from 'child_process';
import path from 'path';
import fs from 'fs';
import { fileURLToPath } from 'url';
const __dirname = path.dirname(fileURLToPath(import.meta.url));

const app  = express();
const port = 3001;

app.use(express.json({ limit: '1mb' }));

app.post('/run', (req, res) => {
  const exe = path.resolve(__dirname, '..', 'planner.exe');            // ruta al binario
  const child = spawn(exe, [], { stdio: ['pipe', 'pipe', 'pipe'] });

  // envía JSON al stdin del C++
  child.stdin.write(JSON.stringify(req.body));
  child.stdin.end();

  let out = '', err = '';
  child.stdout.on('data', d => out += d);
  child.stderr.on('data', d => err += d);

  child.on('close', code => {
    if (code !== 0) return res.status(500).send(err || `Exit ${code}`);
    res.type('text/plain').send(out);
  });
});

app.post('/save', (req, res) => {
  const data = req.body;
  const filename = `data/data.json`;
  const filepath = path.join(__dirname, filename);

  fs.mkdirSync(path.dirname(filepath), { recursive: true });
  fs.writeFile(filepath, JSON.stringify(data, null, 2), (err) => {
    if (err) {
      console.error('❌ Error al guardar:', err);
      return res.status(500).send('Error al guardar el archivo');
    }
    console.log('✅ Guardado en:', filename);
    res.json({ ok: true, file: filename });
  });
});


app.listen(port, () =>
  console.log(`✅ Backend en http://localhost:${port}/run`)
);
