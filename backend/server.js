import express from 'express';
import { spawn } from 'child_process';
import path from 'path';
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

app.listen(port, () =>
  console.log(`✅ Backend en http://localhost:${port}/run`)
);
