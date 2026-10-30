/**
 * Proxy OpenSky Network (ADS-B công khai, miễn phí, giới hạn tốc độ).
 * GET /api/opensky -> states trong bounding box Việt Nam
 */
module.exports = async function handler(req, res) {
  res.setHeader('Access-Control-Allow-Origin', '*');
  res.setHeader('Cache-Control', 'public, max-age=15');
  if (req.method === 'OPTIONS') { res.statusCode = 204; return res.end(); }
  if (req.method !== 'GET') {
    res.statusCode = 405;
    res.setHeader('Content-Type', 'application/json');
    return res.end(JSON.stringify({ ok: false, error: 'GET only' }));
  }
  try {
    // Việt Nam bbox
    const url = 'https://opensky-network.org/api/states/all?lamin=8.0&lomin=102.0&lamax=24.0&lomax=110.5';
    const r = await fetch(url, { headers: { 'User-Agent': 'SkyportStudentDemo/1.0' } });
    if (!r.ok) {
      res.statusCode = 200;
      res.setHeader('Content-Type', 'application/json');
      return res.end(JSON.stringify({
        ok: false,
        source: 'opensky',
        error: 'OpenSky HTTP ' + r.status,
        states: [],
        time: null,
      }));
    }
    const data = await r.json();
    const states = (data.states || []).map(s => ({
      icao24: s[0],
      callsign: (s[1] || '').trim(),
      country: s[2],
      lon: s[5],
      lat: s[6],
      baro_altitude: s[7],
      on_ground: s[8],
      velocity: s[9],
      true_track: s[10],
      geo_altitude: s[13],
    })).filter(x => x.lat != null && x.lon != null && !x.on_ground);
    res.statusCode = 200;
    res.setHeader('Content-Type', 'application/json');
    res.end(JSON.stringify({
      ok: true,
      source: 'opensky',
      time: data.time || null,
      count: states.length,
      states,
    }));
  } catch (e) {
    res.statusCode = 200;
    res.setHeader('Content-Type', 'application/json');
    res.end(JSON.stringify({ ok: false, source: 'opensky', error: String(e && e.message || e), states: [] }));
  }
};
