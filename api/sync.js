/**
 * Skyport shared state — Vercel Blob
 * GET  /api/sync  — mọi máy đọc
 * POST /api/sync  — ghi (header x-skyport-secret)
 * Env: BLOB_READ_WRITE_TOKEN (bắt buộc), SYNC_SECRET (tuỳ chọn, mặc định khớp client)
 */
const BLOB_PATH = 'skyport-shared-state.json';

function mergeByMa(prevArr, nextArr, limit) {
  const map = new Map();
  (prevArr || []).forEach(x => { if (x && x.ma != null) map.set(String(x.ma), x); });
  (nextArr || []).forEach(x => { if (x && x.ma != null) map.set(String(x.ma), x); });
  return [...map.values()].slice(0, limit || 2000);
}
function mergeObj(prev, next) {
  return Object.assign({}, prev || {}, next || {});
}
function mergeLogs(prev, next, limit) {
  const seen = new Set();
  const out = [];
  [...(next || []), ...(prev || [])].forEach(x => {
    if (!x) return;
    const k = x.id || (String(x.at) + '|' + String(x.hanhDong || '') + '|' + String(x.chiTiet || ''));
    if (seen.has(k)) return;
    seen.add(k);
    out.push(x);
  });
  return out.slice(0, limit || 300);
}


function cors(res) {
  res.setHeader('Access-Control-Allow-Origin', '*');
  res.setHeader('Access-Control-Allow-Methods', 'GET, POST, OPTIONS');
  res.setHeader('Access-Control-Allow-Headers', 'Content-Type, x-skyport-secret');
  res.setHeader('Cache-Control', 'no-store, no-cache, must-revalidate');
}

function okSecret(req) {
  const expect = process.env.SYNC_SECRET || 'SkyportSync2024';
  const got = req.headers['x-skyport-secret'] || req.headers['X-Skyport-Secret'] || '';
  return got === expect;
}

async function readJsonBody(req) {
  if (req.body != null) {
    if (typeof req.body === 'object' && !Buffer.isBuffer(req.body)) return req.body;
    if (typeof req.body === 'string') {
      try { return JSON.parse(req.body || '{}'); } catch (e) { return {}; }
    }
  }
  const chunks = [];
  for await (const chunk of req) chunks.push(chunk);
  const raw = Buffer.concat(chunks).toString('utf8');
  if (!raw) return {};
  try { return JSON.parse(raw); } catch (e) { return {}; }
}

async function readState() {
  const token = process.env.BLOB_READ_WRITE_TOKEN;
  if (!token) return { ok: false, reason: 'NO_BLOB', data: null };
  try {
    const { list } = await import('@vercel/blob');
    const listed = await list({ prefix: 'skyport-shared-state', token, limit: 20 });
    const hit = (listed.blobs || []).find(b =>
      (b.pathname || '').includes('skyport-shared-state')
    ) || (listed.blobs || [])[0];
    if (!hit || !hit.url) {
      return {
        ok: true,
        data: {
          version: 0,
          updatedAt: 0,
          flight_status: {},
          extra_flights: [],
          events: [],
        },
      };
    }
    // cache-bust read
    const url = hit.url + (hit.url.includes('?') ? '&' : '?') + 't=' + Date.now();
    const r = await fetch(url, { cache: 'no-store' });
    if (!r.ok) return { ok: false, reason: 'FETCH_' + r.status, data: null };
    const data = await r.json();
    return { ok: true, data: data || {} };
  } catch (e) {
    return { ok: false, reason: String(e && e.message || e), data: null };
  }
}

async function writeState(data) {
  const token = process.env.BLOB_READ_WRITE_TOKEN;
  if (!token) return { ok: false, reason: 'NO_BLOB' };
  try {
    const { put, list, del } = await import('@vercel/blob');
    // Xóa các bản skyport cũ (tránh đầy hạn mức Blob)
    try {
      const listed = await list({ prefix: 'skyport-shared-state', token, limit: 100 });
      const urls = (listed.blobs || [])
        .map(b => b.url)
        .filter(Boolean);
      if (urls.length) {
        // xóa hết bản cũ trước khi ghi 1 file mới
        await del(urls, { token });
      }
    } catch (e) {
      console.warn('blob cleanup', e && e.message);
    }
    await put(BLOB_PATH, JSON.stringify(data), {
      access: 'public',
      addRandomSuffix: false,
      contentType: 'application/json',
      token,
      allowOverwrite: true,
    });
    return { ok: true };
  } catch (e) {
    return { ok: false, reason: String(e && e.message || e) };
  }
}

module.exports = async function handler(req, res) {
  cors(res);
  if (req.method === 'OPTIONS') {
    res.statusCode = 204;
    return res.end();
  }

  if (req.method === 'GET') {
    // Giải phóng Blob: /api/sync?cleanup=1  + header x-skyport-secret
    const q = req.url && req.url.includes('?') ? new URL(req.url, 'http://x').searchParams : null;
    if (q && q.get('cleanup') === '1') {
      if (!okSecret(req)) {
        res.statusCode = 401;
        res.setHeader('Content-Type', 'application/json');
        return res.end(JSON.stringify({ ok: false, error: 'Unauthorized' }));
      }
      const token = process.env.BLOB_READ_WRITE_TOKEN;
      if (!token) {
        res.statusCode = 503;
        res.setHeader('Content-Type', 'application/json');
        return res.end(JSON.stringify({ ok: false, error: 'NO_BLOB' }));
      }
      try {
        const { list, del } = await import('@vercel/blob');
        const listed = await list({ prefix: 'skyport', token, limit: 1000 });
        const urls = (listed.blobs || []).map(b => b.url).filter(Boolean);
        if (urls.length) await del(urls, { token });
        res.statusCode = 200;
        res.setHeader('Content-Type', 'application/json');
        return res.end(JSON.stringify({ ok: true, deleted: urls.length, message: 'Đã xóa ' + urls.length + ' file Blob.' }));
      } catch (e) {
        res.statusCode = 500;
        res.setHeader('Content-Type', 'application/json');
        return res.end(JSON.stringify({ ok: false, error: String(e && e.message || e) }));
      }
    }
    const st = await readState();
    if (!st.ok && st.reason === 'NO_BLOB') {
      res.statusCode = 200;
      res.setHeader('Content-Type', 'application/json');
      return res.end(JSON.stringify({
        ok: false,
        configured: false,
        message: 'Chưa cấu hình BLOB_READ_WRITE_TOKEN trên Vercel → Project → Settings → Environment Variables.',
        version: 0,
        updatedAt: 0,
        flight_status: {},
        extra_flights: [],
        events: [],
        ves: [],
        users_extra: [],
        activity_log: [],
        hh_map: {},
      }));
    }
    if (!st.ok) {
      res.statusCode = 500;
      res.setHeader('Content-Type', 'application/json');
      return res.end(JSON.stringify({ ok: false, configured: true, error: st.reason }));
    }
    res.statusCode = 200;
    res.setHeader('Content-Type', 'application/json');
    return res.end(JSON.stringify({
      ok: true,
      configured: true,
      version: st.data.version || 0,
      updatedAt: st.data.updatedAt || 0,
      flight_status: st.data.flight_status || {},
      extra_flights: st.data.extra_flights || [],
      events: st.data.events || [],
      ves: st.data.ves || [],
      users_extra: st.data.users_extra || [],
      activity_log: st.data.activity_log || [],
      hh_map: st.data.hh_map || {},
      airports: st.data.airports,
      planes: st.data.planes,
    }));
  }

  if (req.method === 'POST') {
    if (!okSecret(req)) {
      res.statusCode = 401;
      res.setHeader('Content-Type', 'application/json');
      return res.end(JSON.stringify({ ok: false, error: 'Unauthorized — sai x-skyport-secret' }));
    }
    const body = await readJsonBody(req);
    const prev = await readState();
    const prevData = (prev.ok && prev.data) ? prev.data : {};
    const prevVer = prevData.version || 0;
    // GỘP với state cũ — không cho máy trống đè mất vé/user máy khác
    const flight_status = mergeObj(prevData.flight_status, body.flight_status);
    const extra_flights = mergeByMa(prevData.extra_flights, body.extra_flights, 500);
    const ves = mergeByMa(prevData.ves, body.ves, 2000);
    const users_extra = mergeByMa(prevData.users_extra, body.users_extra, 500);
    const activity_log = mergeLogs(prevData.activity_log, body.activity_log, 300);
    const hh_map = mergeObj(prevData.hh_map, body.hh_map);
    const eventsIn = Array.isArray(body.events) ? body.events.slice(0, 50) : [];
    const prevEvents = Array.isArray(prevData.events) ? prevData.events : [];
    const events = [...eventsIn, ...prevEvents].slice(0, 40);
    const data = {
      version: prevVer + 1,
      updatedAt: Date.now(),
      flight_status,
      extra_flights,
      events,
      ves,
      users_extra,
      activity_log,
      hh_map,
    };
    if (Array.isArray(body.airports) && body.airports.length) data.airports = body.airports.slice(0, 200);
    else if (prevData.airports) data.airports = prevData.airports;
    if (Array.isArray(body.planes) && body.planes.length) data.planes = body.planes.slice(0, 200);
    else if (prevData.planes) data.planes = prevData.planes;
    const w = await writeState(data);
    if (!w.ok) {
      res.statusCode = w.reason === 'NO_BLOB' ? 503 : 500;
      res.setHeader('Content-Type', 'application/json');
      return res.end(JSON.stringify({
        ok: false,
        error: w.reason === 'NO_BLOB' ? 'Chưa cấu hình BLOB_READ_WRITE_TOKEN' : w.reason,
      }));
    }
    res.statusCode = 200;
    res.setHeader('Content-Type', 'application/json');
    return res.end(JSON.stringify({ ok: true, version: data.version, updatedAt: data.updatedAt }));
  }

  res.statusCode = 405;
  res.setHeader('Content-Type', 'application/json');
  res.end(JSON.stringify({ ok: false, error: 'Method not allowed' }));
};
