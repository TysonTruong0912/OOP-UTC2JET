// WebDemo.cpp — P5
// PHAN 5: WEB DEMO (khong thuoc UML) - may chu HTTP nho chi goi ham PUBLIC cua 34 lop o tren
// ======================================================================
#if BAT_WEB_DEMO
#ifdef _WIN32
  typedef SOCKET sock_t;
  #define DONG_SOCK closesocket
#else
  typedef int sock_t;
  #define DONG_SOCK close
  #define INVALID_SOCKET (-1)
#endif

namespace web {
const std::string& trangChu() {
    static const std::string s = std::string(R"HTML(<!DOCTYPE html>
<html lang="vi">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Airline System Management  Demo BTL OOP T04</title>
<style>
:root{--bg:#f4f7fb;--card:#fff;--tx:#14213d;--mut:#64748b;--line:#e2e8f0;--pri:#1d4ed8;--pri2:#dbeafe;--ok:#15803d;--okbg:#dcfce7;--warn:#b45309;--warnbg:#fef3c7;--bad:#b91c1c;--badbg:#fee2e2;--tg:#7c3aed;--r:12px}
*{box-sizing:border-box}
body{margin:0;font:15px/1.5 system-ui,-apple-system,"Segoe UI",Roboto,sans-serif;background:var(--bg);color:var(--tx)}
header{background:linear-gradient(135deg,#1e3a8a,#1d4ed8 60%,#0ea5e9);color:#fff;padding:14px 24px;display:flex;align-items:center;gap:16px;flex-wrap:wrap}
header h1{margin:0;font-size:20px;letter-spacing:.3px}header small{opacity:.85;display:block;font-weight:400;font-size:12px}
.sp{flex:1}.chip{background:rgba(255,255,255,.18);padding:6px 12px;border-radius:99px;font-size:13px}
main{max-width:1120px;margin:20px auto;padding:0 16px}
.card{background:var(--card);border:1px solid var(--line);border-radius:var(--r);padding:18px;margin-bottom:16px;box-shadow:0 1px 2px rgba(0,0,0,.04)}
h2{margin:0 0 12px;font-size:17px}h3{margin:14px 0 8px;font-size:15px}
nav{display:flex;gap:6px;flex-wrap:wrap;margin-bottom:16px}
nav button{border:1px solid var(--line);background:#fff;padding:9px 14px;border-radius:99px;cursor:pointer;font-size:14px;color:var(--tx)}
nav button.on{background:var(--pri);border-color:var(--pri);color:#fff}
button.b{background:var(--pri);color:#fff;border:0;padding:9px 16px;border-radius:8px;cursor:pointer;font-size:14px;font-weight:600}
button.b:hover{filter:brightness(1.08)}button.b:disabled{opacity:.5;cursor:not-allowed}
button.b.g{background:#fff;color:var(--pri);border:1px solid var(--pri)}button.b.r{background:var(--bad)}button.b.s{padding:5px 10px;font-size:13px}
label{display:block;font-size:12px;color:var(--mut);margin-bottom:3px}
input,select{width:100%;padding:8px 10px;border:1px solid #cbd5e1;border-radius:8px;font-size:14px;background:#fff}
.grid{display:grid;gap:12px;grid-template-columns:repeat(auto-fit,minmax(160px,1fr));align-items:end}
table{width:100%;border-collapse:collapse;font-size:14px}th{text-align:left;color:var(--mut);font-weight:600;font-size:12px;text-transform:uppercase;padding:8px;border-bottom:2px solid var(--line)}
td{padding:10px 8px;border-bottom:1px solid var(--line);vertical-align:middle}tr:hover td{background:#f8fafc}
.tag{display:inline-block;padding:2px 9px;border-radius:99px;font-size:12px;font-weight:600}
.t-tg{background:#ede9fe;color:var(--tg)}.t-pt{background:var(--pri2);color:var(--pri)}.t-tk{background:var(--okbg);color:var(--ok)}
.t-ok{background:var(--okbg);color:var(--ok)}.t-wait{background:var(--warnbg);color:var(--warn)}
.acc{display:grid;grid-template-columns:repeat(auto-fit,minmax(210px,1fr));gap:12px}
.acc button{text-align:left;background:#fff;border:1px solid var(--line);border-radius:var(--r);padding:16px;cursor:pointer;font-size:14px}
.acc button:hover{border-color:var(--pri);box-shadow:0 4px 14px rgba(29,78,216,.15)}
.acc b{display:block;font-size:16px}.acc span{color:var(--mut);font-size:13px}
.two{display:grid;grid-template-columns:1.5fr 1fr;gap:16px}@media(max-width:820px){.two{grid-template-columns:1fr}}
.seatmap{overflow-x:auto}.cabin{margin-bottom:14px}.cabin-title{font-weight:700;margin-bottom:6px}.cabin-title small{color:var(--mut);font-weight:400}
.row{display:flex;align-items:center;gap:5px;margin-bottom:5px}.rn{width:22px;text-align:right;color:var(--mut);font-size:12px;margin-right:4px}
.aisle{width:22px}
.seat{width:34px;height:34px;border-radius:7px 7px 4px 4px;border:1.5px solid #86efac;background:var(--okbg);color:var(--ok);font-weight:700;font-size:13px;cursor:pointer;padding:0}
.seat.held{background:var(--warnbg);border-color:#fcd34d;color:var(--warn);cursor:not-allowed}
.seat.taken{background:#e2e8f0;border-color:#cbd5e1;color:#94a3b8;cursor:not-allowed}
.seat.sel{background:var(--pri);border-color:var(--pri);color:#fff}.seat.cur{outline:3px solid #f59e0b}
.legend{display:flex;gap:14px;font-size:12px;color:var(--mut);margin:8px 0;flex-wrap:wrap}.legend i{display:inline-block;width:12px;height:12px;border-radius:3px;margin-right:4px;vertical-align:-1px}
.kv{display:grid;grid-template-columns:auto 1fr;gap:4px 12px;font-size:14px;margin:10px 0}.kv span{color:var(--mut)}
.big{font-size:26px;font-weight:800}.cd{font-size:30px;font-weight:800;color:var(--warn);font-variant-numeric:tabular-nums}
.hood{display:none!important;background:#0f172a;color:#cbd5e1;border-radius:10px;padding:10px 14px;margin-top:12px;font:12.5px/1.7 ui-monospace,Consolas,monospace}
.hood b{color:#7dd3fc}.hood em{color:#86efac;font-style:normal}
.ticket{border:1px solid var(--line);border-radius:var(--r);padding:14px;margin-bottom:12px;background:#fff;border-left:5px solid var(--pri)}
.ticket.tg{border-left-color:var(--tg)}.ticket.tk{border-left-color:var(--ok)}
.ticket .h{display:flex;gap:10px;align-items:center;flex-wrap:wrap;margin-bottom:6px}.ticket .h b{font-size:16px}
.acts{display:flex;gap:8px;margin-top:10px;flex-wrap:wrap}
.leg{display:flex;align-items:center;gap:10px;padding:8px 0;border-bottom:1px dashed var(--line);flex-wrap:wrap}.leg:last-child{border:0}
.path{border:1px solid var(--line);border-radius:var(--r);padding:12px 14px;margin-bottom:10px}
.chips{display:flex;gap:6px;flex-wrap:wrap}.chips .tag{background:var(--pri2);color:var(--pri);cursor:default}
.empty{color:var(--mut);padding:18px;text-align:center}
.bar{height:22px;border-radius:6px;background:var(--pri);color:#fff;font-size:12px;padding:0 8px;line-height:22px;min-width:26px}
.stat{display:grid;grid-template-columns:repeat(auto-fit,minmax(190px,1fr));gap:12px}.stat .card{margin:0;text-align:center}.stat small{color:var(--mut)}
#toast{position:fixed;right:18px;bottom:18px;display:flex;flex-direction:column;gap:8px;z-index:99}
#toast div{padding:11px 16px;border-radius:10px;color:#fff;box-shadow:0 6px 20px rgba(0,0,0,.2);max-width:380px;font-size:14px}
.m-ok{background:var(--ok)}.m-ok,.m-err{white-space:pre-line;max-width:min(420px,92vw);line-height:1.45}.m-err{background:var(--bad)}
.modal{position:fixed;inset:0;background:rgba(15,23,42,.55);display:flex;align-items:center;justify-content:center;z-index:50;padding:16px}
.modal>div{background:#fff;border-radius:var(--r);padding:20px;max-width:560px;width:100%;max-height:90vh;overflow:auto}
pre.bp{background:#0f172a;color:#e2e8f0;padding:14px;border-radius:10px;font:13px/1.5 ui-monospace,Consolas,monospace;overflow:auto}
footer{text-align:center;color:var(--mut);font-size:12px;padding:20px}
.right{text-align:right}.mut{color:var(--mut)}.mb0{margin-bottom:0}
</style>
</head>
<body>
<header>
  <div><h1>Airline System Management <small>Demo BTL OOP DT04 – Quản lý đặt vé &amp; điều hành chuyến bay</small></h1></div>
  <div class="sp"></div>
  <div id="who"></div>
</header>
<main id="app"></main>
<footer>Airline System Management — Demo BTL OOP</footer>
<div id="toast"></div>
<div id="modal"></div>

<script>
const $ = (s, r = document) => r.querySelector(s);
const $$ = (s, r = document) => [...r.querySelectorAll(s)];
const esc = s => String(s ?? '').replace(/[&<>"']/g, c => ({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&#39;'}[c]));
const vnd = n => Number(n).toLocaleString('vi-VN') + ' ';
const hm = s => { const [d, t] = s.split(' '); const [y, m, dd] = d.split('-'); return `${t}  ${dd}/${m}`; };
const dur = p => `${Math.floor(p / 60)}g${String(p % 60).padStart(2, '0')}`;
const HANG = { TG: 'Thương gia', PT: 'Phổ thông', TK: 'Tiết kiệm' };
const HESO = { TG: 2.5, PT: 1, TK: 0.8 };
const tagH = h => `<span class="tag t-${h.toLowerCase()}">${HANG[h]}</span>`;
let token = sessionStorage.getItem('token'), user = null, tab = null, meta = { sanBay: [], mayBay: [] };
const S = { flights: [], sel: null, ma: null, ghe: [], hold: null, timer: null, legs: [], hanh: 'PT' };

async function api(path, params = {}, method = 'GET') {
  const p = new URLSearchParams();
  for (const [k, v] of Object.entries(params)) if (v !== undefined && v !== null && v !== '') p.append(k, v);
  if (token) p.set('token', token);
  let url = '/api/' + path; const opt = { method };
  if (method === 'GET') { const q = p.toString(); if (q) url += '?' + q; }
  else { opt.headers = { 'Content-Type': 'application/x-www-form-urlencoded' }; opt.body = p.toString(); }
  const r = await fetch(url, opt); const d = await r.json();
  if (!d.ok) throw new Error(d.loi || 'Lỗi không xác định');
  return d;
}
const post = (p, a) => api(p, a, 'POST');
function toast(msg, ok = true) {
  const d = document.createElement('div'); d.className = ok ? 'm-ok' : 'm-err'; d.textContent = msg;
  $('#toast').appendChild(d); setTimeout(() => d.remove(), ok ? 3500 : 10000);
}
const guard = fn => async (...a) => { try { await fn(...a); } catch (e) { toast(e.message, false); } };
function modal(html) { $('#modal').innerHTML = `<div class="modal" onclick="if(event.target===this)closeModal()"><div>${html}</div></div>`; }
function closeModal() { $('#modal').innerHTML = ''; }
const hood = lines => '';

/* ---------------- ĐĂNG NHẬP ---------------- */
function loginView() {
  $('#who').innerHTML = '';
  $('#app').innerHTML = `
  <div class="card"><h2>Chọn tài khoản có sẵn</h2>
    <div class="acc">
      <button data-u="AD01|admin123"><b>Quản trị viên</b><span>Tài khoản: AD01 — Mật khẩu: admin123</span></button>
      <button data-u="DL01|daily123"><b>Đại lý Sao Việt</b><span>Tài khoản: DL01 — Mật khẩu: daily123</span></button>
      <button data-u="DL02|daily123"><b>Vietnam Booking</b><span>Tài khoản: DL02 — Mật khẩu: daily123</span></button>
      <button data-u="DL03|daily123"><b>BestPrice Travel</b><span>Tài khoản: DL03 — Mật khẩu: daily123</span></button>
      <button data-u="DL04|daily123"><b>Sao Mai Travel</b><span>Tài khoản: DL04 — Mật khẩu: daily123</span></button>
      <button data-u="DL05|daily123"><b>Hong Ngoc Ha Travel</b><span>Tài khoản: DL05 — Mật khẩu: daily123</span></button>
    </div>
    <p class="mut">Hành khách: đăng ký tài khoản mới ở form bên dưới.</p>
  </div>
  <div class="card"><h2>Hoặc đăng nhập thủ công</h2>
    <form id="lf" class="grid">
      <div><label>Mã người dùng</label><input name="ma" autocomplete="off"></div>
      <div><label>Mật khẩu</label><input name="mk" type="password"></div>
      <div><button class="b" type="submit">Đăng nhập</button></div>
    </form></div>
  <div class="card"><h2>Đăng ký Hành khách (tự tạo tài khoản)</h2>
    <p class="mut" style="margin-top:0">Chỉ Hành khách được tự đăng ký. Admin và Đại lý chỉ dùng tài khoản có sẵn.</p>
    <form id="rf" class="grid">
      <div><label>Mã (vd HK01)</label><input name="ma" required></div>
      <div><label>Họ tên</label><input name="ten" required></div>
      <div><label>Mật khẩu</label><input name="mk" type="password" required></div>
      <div><label>CCCD (9 hoặc 12 số)</label><input name="cccd" required placeholder="001234567890"></div>
      <div><label>Số điện thoại</label><input name="sdt" required placeholder="0901234567"></div>
      <div><label>Email</label><input name="email" required placeholder="ten@gmail.com"></div>
      <div><button class="b" type="submit">Đăng ký</button></div>
    </form></div>`;
  $$('.acc button').forEach(b => b.onclick = guard(() => { const [ma, mk] = b.dataset.u.split('|'); return login(ma, mk); }));
  $('#lf').onsubmit = guard(e => { e.preventDefault(); const f = new FormData(e.target); return login(f.get('ma'), f.get('mk')); });
  $('#rf').onsubmit = guard(async e => {
    e.preventDefault();
    const f = Object.fromEntries(new FormData(e.target));
    await post('dangky', f);
    toast('Đăng ký thành công! Hãy đăng nhập bằng mã ' + f.ma);
    e.target.reset();
  });
}
async function login(ma, mk) {
  const d = await post('login', { ma, mk }); token = d.token; user = d.user; sessionStorage.setItem('token', token);
  toast('Xin chào ' + user.ten); await boot();
}
async function logout() { try { await post('logout'); } catch (e) {} token = null; user = null; sessionStorage.removeItem('token'); clearInterval(S.timer); S.hold = null; loginView(); }

/* ---------------- KHUNG CHNH ---------------- */
const VAITRO = {HK:"Hành khách",DL:"Đại lý",AD:"Admin"};
const TABS = {
  HK: [['tim', 'Tìm chuyến'], ['noi', 'Nối chuyến'], ['dat', 'Đặt vé'], ['ht', 'Hành trình (Builder)'], ['ss', 'So sánh đại lý'], ['ve', 'Vé của tôi']],
  DL: [['tim', 'Tìm chuyến'], ['noi', 'Nối chuyến'], ['dat', 'Đặt vé hộ'], ['ht', 'Hành trình (Builder)'], ['ve', 'Vé đã bán'], ['tk', 'Hoa hồng']],
  AD: [['qly', 'Chuyến bay'], ['ve', 'Tất cả vé'], ['daily', 'Quản lý đại lý'], ['tk', 'Thống kê']]
};
async function boot() {
  if (!user) { try { user = (await api('me')).user; } catch (e) { token = null; sessionStorage.removeItem('token'); return loginView(); } }
  meta = await api('meta');
  $('#who').innerHTML = `<span class="chip">${VAITRO[user.vaiTro]} — ${esc(user.ten)} (${esc(user.ma)})</span> <button class="b g s" id="lo">Đăng xuất</button>`;
  $('#lo').onclick = logout;
  $('#app').innerHTML = `<nav id="nav"></nav><div id="panel"></div>`;
)HTML") +
        R"HTML(  $('#nav').innerHTML = TABS[user.vaiTro].map(([k, t]) => `<button data-t="${k}">${t}</button>`).join('');
  $$('#nav button').forEach(b => b.onclick = () => go(b.dataset.t));
  await go(TABS[user.vaiTro][0][0]);
}
async function go(t) {
  tab = t; $$('#nav button').forEach(b => b.classList.toggle('on', b.dataset.t === t));
  clearInterval(S.timer);
  try { await ({ tim: pTim, noi: pNoi, dat: pDat, ht: pHT, ve: pVe, tk: pTK, qly: pQly, ss: pSS, daily: pDaily })[t](); } catch (e) { toast(e.message, false); }
}
const apOpts = (all = true) => (all ? '<option value="">Tất cả</option>' : '') + meta.sanBay.map(s => `<option value="${s.ma}">${s.ma}  ${esc(s.tp)}</option>`).join('');

/* ---------------- TÌM CHUYẾN ---------------- */
function flightRows(list, action) {
  if (!list.length) return '<div class="empty">Không có chuyến bay phù hợp</div>';
  return `<table><tr><th>Mã</th><th>Tuyến</th><th>Khởi hành</th><th>Đến</th><th>Bay</th><th class="right">Giá cơ bản</th><th>Ghế trống</th><th></th></tr>` + list.map(c => `
    <tr><td><b>${c.ma}</b><br><small class="mut">${c.mayBay}</small></td><td>${c.di} → ${c.den}<br><small class="mut">${esc(c.diTen)} → ${esc(c.denTen)}</small></td>
    <td>${hm(c.gioDi)}</td><td>${hm(c.gioDen)}</td><td>${dur(c.phut)}</td><td class="right"><b>${vnd(c.gia)}</b></td>
    <td><span class="tag t-tg">TG ${c.TG.trong}/${c.TG.tong}</span> <span class="tag t-pt">PT ${c.PT.trong}/${c.PT.tong}</span> <span class="tag t-tk">TK ${c.TK.trong}/${c.TK.tong}</span></td>
    <td class="right">${action(c)}</td></tr>`).join('') + '</table>';
}
async function pTim() {
  const today = new Date(); const ymd = today.toISOString().slice(0,10);
  $('#panel').innerHTML = `<div class="card"><h2>Tìm chuyến bay</h2>
    <p class="mut">Danh sách chuyến theo ngày thực tế (giờ hệ thống đã căn theo "bây giờ"). Chọn điểm đi/đến rồi bấm Tìm.</p>
    <form id="f" class="grid">
    <div><label>Điểm đi</label><select name="di">${apOpts()}</select></div>
    <div><label>Điểm đến</label><select name="den">${apOpts()}</select></div>
    <div><label>Ngày bay</label><input type="date" name="ngay" value="${ymd}"></div>
    <div><label>Sắp xếp</label><select name="sapxep"><option value="gio">Theo giờ đi</option><option value="gia">Theo giá</option></select></div>
    <div><button class="b" type="submit">Tìm chuyến</button></div></form></div>
    <div class="card" id="res"><div class="empty">Đang tải danh sách…</div></div>`;
  const run = guard(async () => {
    const f = Object.fromEntries(new FormData($('#f')));
    const d = await api('chuyenbay', f);
    if (!d.chuyenBay.length) {
      $('#res').innerHTML = '<div class="empty">Không có chuyến phù hợp. Thử bỏ lọc ngày hoặc đổi điểm đi/đến.</div>';
      return;
    }
    $('#res').innerHTML = `<div class="mut" style="margin-bottom:8px">Có <b>${d.chuyenBay.length}</b> chuyến — chọn "Chọn ghế / Đặt vé" để đặt.</div>` +
      flightRows(d.chuyenBay, c => `<button class="b s" data-m="${c.ma}">Chọn ghế / Đặt vé</button>`);
    $$('#res [data-m]').forEach(b => b.onclick = () => { S.ma = b.dataset.m; go('dat'); });
  });
  $('#f').onsubmit = e => { e.preventDefault(); run(); }; run();
}

/* ---------------- NỐI CHUYẾN ---------------- */
async function pNoi() {
  const today = new Date(); const ymd = today.toISOString().slice(0,10);
  $('#panel').innerHTML = `<div class="card"><h2>Tìm hành trình nối chuyến</h2>
    <p class="mut">Hiện <b>tất cả</b> phương án nối chuyến (tối đa 3 chặng, chờ tối thiểu giữa các chặng). Chọn một chặng để đặt vé.</p>
    <form id="f" class="grid">
    <div><label>Điểm đi</label><select name="di">${apOpts(false)}</select></div>
    <div><label>Điểm đến</label><select name="den">${apOpts(false)}</select></div>
    <div><label>Ngày bay</label><input type="date" name="ngay" value="${ymd}"></div>
    <div><label>Chờ tối thiểu (phút)</label><input type="number" name="cho" value="45" min="0"></div>
    <div><button class="b" type="submit">Tìm nối chuyến</button></div></form></div>
    <div id="res"><div class="card empty">Chọn điểm đi/đến rồi bấm tìm.</div></div>`;
  const sel = $('#f [name=den]'); if (sel.options.length > 1) sel.selectedIndex = 1;
  const run = guard(async () => {
    const f = Object.fromEntries(new FormData($('#f')));
    const d = await api('noichuyen', f);
    if (!d.hanhTrinh.length) {
      $('#res').innerHTML = '<div class="card empty">Không có hành trình nối chuyến phù hợp. Thử giảm thời gian chờ hoặc đổi ngày.</div>';
      return;
    }
    $('#res').innerHTML = `<div class="mut" style="margin:8px 0">Có <b>${d.hanhTrinh.length}</b> phương án nối chuyến:</div>` +
      d.hanhTrinh.map((h, i) => {
        const rows = (h.chang || []).map((c, j) => {
          const tg = (c.TG && c.TG.trong != null) ? c.TG.trong : '?';
          const pt = (c.PT && c.PT.trong != null) ? c.PT.trong : '?';
          const tk = (c.TK && c.TK.trong != null) ? c.TK.trong : '?';
          return `<tr>
            <td>Chặng ${j+1}</td><td><b>${c.ma}</b></td>
            <td>${c.di} → ${c.den}<br><small class="mut">${esc(c.diTen||'')} → ${esc(c.denTen||'')}</small></td>
            <td>${hm(c.gioDi)}</td><td>${hm(c.gioDen)}</td>
            <td class="right">${vnd(c.gia)}</td>
            <td>${tg}/${pt}/${tk}</td>
            <td><button class="b s" data-m="${c.ma}">Đặt chặng này</button></td></tr>`;
        }).join('');
        return `<div class="card path"><b>Phương án ${i+1}</b> — ${(h.chang||[]).length} chặng —
          Tổng giá CB: <b>${vnd(h.tongGia || 0)}</b>
          <table style="margin-top:8px"><tr><th></th><th>Mã</th><th>Tuyến</th><th>Đi</th><th>Đến</th><th class="right">Giá</th><th>Ghế TG/PT/TK</th><th></th></tr>${rows}</table></div>`;
      }).join('');
    $$('#res [data-m]').forEach(b => b.onclick = () => { S.ma = b.dataset.m; go('dat'); });
  });
  $('#f').onsubmit = e => { e.preventDefault(); run(); }; run();
}


function seatMap(ghe, opt) {
  opt = opt || {};
  const sel = opt.sel || null, off = !!opt.off;
  const byHang = { TG: [], PT: [], TK: [] };
  (ghe || []).forEach(g => { if (byHang[g.hang]) byHang[g.hang].push(g); });
  const labels = { TG: 'Thương gia', PT: 'Phổ thông', TK: 'Tiết kiệm' };
  let h = '<div class="seatmap">';
  ['TG','PT','TK'].forEach(hg => {
    const list = byHang[hg];
    if (!list || !list.length) return;
    h += `<div class="cabin"><div class="cabin-title">${labels[hg]} <small>(${list.filter(x=>x.tt==='T').length} trống)</small></div>`;
    const rows = {};
    list.forEach(g => {
      const m = String(g.so).match(/^(\d+)([A-Z])$/);
      if (!m) return;
      const rn = m[1], col = m[2];
      if (!rows[rn]) rows[rn] = {};
      rows[rn][col] = g;
    });
    Object.keys(rows).sort((a,b)=>+a-+b).forEach(rn => {
      h += `<div class="row"><span class="rn">${rn}</span>`;
      const cols = Object.keys(rows[rn]).sort();
      const mid = Math.ceil(cols.length / 2);
      cols.forEach((col, i) => {
        const g = rows[rn][col];
        const c = ['seat'];
        if (g.tt === 'T') c.push('free');
        else if (g.tt === 'H') c.push('hold');
        else c.push('taken');
        if (sel === g.so) c.push('sel');
        const ok = g.tt === 'T' && !off;
        h += `<button type="button" class="${c.join(' ')}" data-so="${g.so}" ${ok?'':'disabled'}>${col}</button>`;
        if (i === mid - 1) h += '<span class="aisle"></span>';
      });
      h += '</div>';
    });
    h += '</div>';
  });
  h += '</div><div class="legend"><span><i style="background:#dcfce7;border:1px solid #86efac"></i>Trống</span>'
    + '<span><i style="background:#fef3c7;border:1px solid #fcd34d"></i>Giữ chỗ</span>'
    + '<span><i style="background:#e2e8f0"></i>Đã đặt</span>'
    + '<span><i style="background:#1d4ed8"></i>Đang chọn</span></div>';
  return h;
}

/* ---------------- ĐẶT VÉ ---------------- */
async function pDat() {
  const all = await api('chuyenbay', { sapxep: 'gio' }); S.flights = all.chuyenBay;
  S.sel = null; S.hold = null;
  const dl = user.vaiTro === 'DL';
  const daChon = S.ma && S.flights.find(c => c.ma === S.ma);
  const tenDl = S.tenDaiLy || S.maDaiLy || '';
  const banner = S.maDaiLy
    ? `<div class="card" style="border-color:#93c5fd;background:#eff6ff"><h2 style="margin:0 0 6px">Đặt vé hộ qua đại lý: ${esc(tenDl)} <small class="mut">(${esc(S.maDaiLy)})</small></h2>
         <p class="mut" style="margin:0">Bạn đang đặt vé thông qua đại lý này (hoa hồng ghi nhận cho đại lý). <a href="#" id="boDl">Bỏ đại lý — đặt tại quầy</a></p></div>`
    : (dl ? '' : `<p class="mut">Đặt trực tiếp tại quầy (không qua đại lý).</p>`);

  $('#panel').innerHTML = `
    ${banner}
    <div class="card"><h2>Đặt vé — tìm chuyến</h2>
      <p class="mut">Chọn điểm đi và điểm đến, bấm Tìm chuyến, rồi chọn 1 chuyến trong list để đặt ghế.</p>
      <form id="fTimDat" class="grid">
        <div><label>Điểm đi</label><select name="di" id="diDat">${apOpts(true)}</select></div>
        <div><label>Điểm đến</label><select name="den" id="denDat">${apOpts(true)}</select></div>
        <div><label>&nbsp;</label><button class="b" type="submit">Tìm chuyến</button></div>
      </form>
      <div id="listDat" style="margin-top:14px"></div>
    </div>
    <div class="two" id="datArea" style="display:none;margin-top:14px">
      <div class="card"><h2>${S.maDaiLy ? 'Chọn ghế — đặt hộ «' + esc(tenDl) + '»' : 'Chọn ghế'} — <span id="maDatLabel"></span></h2>
        <button type="button" class="b ghost" id="btnDoiChuyen" style="margin-bottom:10px">← Đổi chuyến</button>
        <div id="map"></div>
      </div>
      <div class="card"><h2>${dl ? 'Đặt vé hộ khách' : (S.maDaiLy ? 'Thanh toán qua đại lý' : 'Đặt vé')}</h2><div id="box"></div></div>
    </div>`;

  const bo = $('#boDl'); if (bo) bo.onclick = e => { e.preventDefault(); S.maDaiLy = ''; S.tenDaiLy = ''; S.fromSS = false; pDat(); };

  const hienList = (ds) => {
    const box = $('#listDat');
    if (!ds.length) {
      box.innerHTML = '<div class="empty">Không có chuyến bay phù hợp. Thử đổi điểm đi / điểm đến.</div>';
      return;
    }
    box.innerHTML = `<table><tr><th>STT</th><th>Mã</th><th>Tuyến</th><th>Giờ đi</th><th>Giờ đến</th><th class="right">Giá CB</th><th>Ghế TG/PT/TK</th><th></th></tr>
      ${ds.map((c,i) => `<tr>
        <td>${i+1}</td><td><b>${c.ma}</b></td>
        <td>${c.di}→${c.den}</td>
        <td>${hm(c.gioDi)}</td><td>${hm(c.gioDen)}</td>
        <td class="right">${vnd(c.gia)}</td>
        <td>${(c.TG&&c.TG.trong)||0}/${(c.PT&&c.PT.trong)||0}/${(c.TK&&c.TK.trong)||0}</td>
        <td><button type="button" class="b" data-m="${c.ma}">Chọn đặt</button></td>
      </tr>`).join('')}</table>`;
    $$('#listDat [data-m]').forEach(b => b.onclick = guard(async () => {
      S.ma = b.dataset.m; S.sel = null; S.hold = null;
      $('#datArea').style.display = '';
      $('#maDatLabel').textContent = S.ma;
      await loadMap();
    }));
  };

  $('#fTimDat').onsubmit = e => {
    e.preventDefault();
    const di = $('#diDat').value, den = $('#denDat').value;
    let ds = S.flights.slice();
    if (di) ds = ds.filter(c => c.di === di);
    if (den) ds = ds.filter(c => c.den === den);
    $('#datArea').style.display = 'none';
    S.ma = null; S.sel = null; S.hold = null;
    hienList(ds);
  };
  $('#btnDoiChuyen').onclick = () => {
    $('#datArea').style.display = 'none';
    S.ma = null; S.sel = null; S.hold = null;
  };

  // Tu So sanh dai ly: mo thang chon ghe cho chuyen da chon
  if (daChon) {
    const c = S.flights.find(x => x.ma === S.ma);
    if (c) {
      $('#diDat').value = c.di;
      $('#denDat').value = c.den;
      hienList(S.flights.filter(x => x.di === c.di && x.den === c.den));
      $('#datArea').style.display = '';
      $('#maDatLabel').textContent = S.ma;
      await loadMap();
    }
  }
}


async function loadMap() {
  if (!S.ma) { $('#map').innerHTML = '<div class="empty">Chưa có chuyến bay</div>'; return; }
  const d = await api('sodoghe', { ma: S.ma }); S.ghe = d.ghe; S.cb = d.chuyenBay; drawMap(); drawBox();
}
function drawMap() {
  $('#map').innerHTML = seatMap(S.ghe, { sel: S.sel, off: !!S.hold });
  $$('#map .seat').forEach(b => b.onclick = () => { S.sel = b.dataset.so; drawMap(); drawBox(); });
}
function drawBox() {
  const dl = user.vaiTro === 'DL', box = $('#box'); if (!box) return;
  if (S.hold) return holdBox();
  const g = S.ghe.find(x => x.so === S.sel);
  if (!g) { box.innerHTML = '<div class="empty">Bấm vào một ghế trống trên sơ đồ để đặt</div>'; return; }
  box.innerHTML = `<div class="kv">
      <span>Chuyến</span><b>${S.cb.ma}  ${S.cb.di}→${S.cb.den}</b>
      <span>Khởi hành</span><b>${hm(S.cb.gioDi)}</b>
      <span>Ghế</span><b>${g.so}</b>
      <span>Hạng vé</span><b>${tagH(g.hang)}</b></div>
    <div class="mut">Giá dự kiến (giá cơ bản × ${HESO[g.hang]})</div>
    <div class="big">${vnd(S.cb.gia * HESO[g.hang])}</div>
    ${dl ? '<div style="margin-top:10px"><label>Mã hành khách</label><input id="mk" placeholder="VD: HK01"></div>' : ''}
    <label style="margin-top:10px"><input type="checkbox" id="fast" style="width:auto"> Demo nhanh: giữ chỗ chỉ <b>15 giây</b> (xem hết hạn)</label>
    <div class="acts"><button class="b" id="gc">Giữ chỗ</button></div>`;
  $('#gc').onclick = guard(async () => {
    const d = await post('giucho', { maCB: S.ma, soGhe: S.sel, maKhach: dl ? $('#mk').value : '', maDaiLy: (!dl && S.maDaiLy) ? S.maDaiLy : '', giay: $('#fast').checked ? 15 : '' });
    S.hold = { ...d, until: Date.now() + d.conLai * 1000, maKhach: dl ? $('#mk').value : '' };
    toast('Đã giữ ghế ' + d.soGhe); await loadMap(); startTimer();
  });
}
function startTimer() { clearInterval(S.timer); S.timer = setInterval(tick, 250); tick(); }
function tick() {
  const e = $('#cd'); if (!e || !S.hold) return clearInterval(S.timer);
  const left = Math.max(0, Math.ceil((S.hold.until - Date.now()) / 1000));
  e.textContent = `${Math.floor(left / 60)}:${String(left % 60).padStart(2, '0')}`;
  if (left === 0) { $('#st').innerHTML = '<span class="tag t-wait">Hết thời gian giữ chỗ</span>'; }
}
function holdBox() {
  const h = S.hold;
  $('#box').innerHTML = `<div class="kv"><span>Chuyến</span><b>${h.maCB}</b><span>Ghế</span><b>${h.soGhe}</b><span>Hạng vé</span><b>${tagH(h.hang)}</b></div>
    <div class="mut">Thời gian giữ chỗ còn lại</div><div class="cd" id="cd">--:--</div><div id="st"><span class="tag t-ok">Đang giữ chỗ</span></div>
    <div class="acts"><button class="b" id="pay">Thanh toán</button><button class="b g" id="cancel">Hủy giữ chỗ</button></div>`;
  $('#pay').onclick = guard(async () => {
    const id = S.hold.holdId; clearInterval(S.timer);
    try { const d = await post('thanhtoan', { holdId: id }); S.hold = null; S.sel = null; toast('Đặt vé thành công ' + d.ve.ma); await loadMap(); $('#box').innerHTML = ticketHtml(d.ve, true) + '<div class="acts"><button class="b g" onclick="go(\'ve\')">Xem vé</button></div>'; }
    catch (e) { S.hold = null; S.sel = null; await loadMap(); throw e; }
  });
  $('#cancel').onclick = guard(async () => { clearInterval(S.timer); await post('huygiucho', { holdId: S.hold.holdId }); S.hold = null; S.sel = null; toast('Đã hủy giữ chỗ'); await loadMap(); });
  startTimer();
}

/* ---------------- V ---------------- */
function chain(){ return ""; }

function ticketHtml(v, withHood, acts) {
  const dv = v.dichVu.map(d => `<div>+ ${esc(d)}</div>`).join('');
  return `<div class="ticket ${v.hang === 'TG' ? 'tg' : v.hang === 'TK' ? 'tk' : ''}"><div class="h"><b>${v.ma}</b>${tagH(v.hang)}
      ${v.checkIn ? '<span class="tag t-ok">✓ check-in</span>' : '<span class="tag t-wait">Chưa check-in</span>'}<span class="sp"></span><b>${vnd(v.gia)}</b></div>
    <div><b>${v.maCB}</b>  ${v.di} → ${v.den}  ${hm(v.gioDi)} → ${hm(v.gioDen)}</div>
)HTML" +
        R"HTML(    <div class="mut">Ghế <b>${v.ghe}</b>  ${esc(v.hanhKhach.ten)} (${v.hanhKhach.ma})  giá vé gốc ${vnd(v.giaGoc)}</div>${dv ? `<div class="mut">${dv}</div>` : ''}
    ${withHood ? chain(v) : ''}${acts || ''}</div>`;
}
async function pVe() {
  const d = await api('ve'); const ro = user.vaiTro === 'AD';
  $('#panel').innerHTML = `<div class="card"><h2>${ro ? 'Tất cả vé trong hệ thống' : user.vaiTro === 'DL' ? 'Vé đã bán' : 'Vé của tôi'} (${d.ve.length})</h2>` +
    (d.ve.length ? d.ve.map(v => ticketHtml(v, true, ro ? '' : `<div class="acts"><button class="b s" data-c="${v.ma}">Check-in / Đổi chỗ</button>
      <button class="b g s" data-b="${v.ma}">Boarding pass</button><button class="b r s" data-h="${v.ma}">Hủy vé</button></div>`)).join('') : '<div class="empty">Chưa có vé nào</div>') + '</div>';
  $$('[data-c]').forEach(b => b.onclick = guard(() => checkInDlg(d.ve.find(v => v.ma === b.dataset.c))));
  $$('[data-b]').forEach(b => b.onclick = guard(async () => { const r = await api('boardingpass', { maVe: b.dataset.b }); modal(`<h2>Boarding pass ${r.ve.ma}</h2><pre class="bp">${esc(r.text)}</pre><div class="acts"><button class="b" onclick="closeModal()">Đóng</button></div>`); }));
  $$('[data-h]').forEach(b => b.onclick = guard(async () => { if (!confirm('Hủy vé ' + b.dataset.h + '? Ghế sẽ được trả lại.')) return; await post('huyve', { maVe: b.dataset.h }); toast('Đã hủy vé'); await pVe(); }));
}
async function checkInDlg(v) {
  const d = await api('sodoghe', { ma: v.maCB }); let pick = v.ghe;
  const draw = () => { $('#cmap').innerHTML = seatMap(d.ghe.map(g => g.so === v.ghế ? { ...g, tt: 'T' } : g), { sel: pick, cur: v.ghe, hang: v.hang });
    $$('#cmap .seat').forEach(b => b.onclick = () => { pick = b.dataset.so; draw(); $('#pk').textContent = pick; }); };
  modal(`<h2>Check-in vé ${v.ma} — chuyến ${v.maCB}</h2><p class="mut">Ghế hiện tại <b>${v.ghe}</b> (viền cam). Chọn ghế mới cùng hạng ${tagH(v.hang)} hoặc giữ nguyên. Check-in mở trước giờ bay 24 giờ, đóng trước 1 giờ.</p>
    <div id="cmap"></div><p>Ghế đang chọn: <b id="pk">${pick}</b></p>
    
    <div class="acts"><button class="b" id="ok">Xác nhận check-in</button><button class="b g" onclick="closeModal()">Đóng</button></div>`);
  draw();
  $('#ok').onclick = guard(async () => { await post('checkin', { maVe: v.ma, soGhe: pick }); closeModal(); toast('Check-in thành công, ghế ' + pick); await pVe(); });
}

/* ---------------- HÀNH TRÌNH (BUILDER + DECORATOR) ---------------- */
async function pHT() {
  const all = await api('chuyenbay', { sapxep: 'gio' }); S.flights = all.chuyenBay; const dl = user.vaiTro === 'DL';
  $('#panel').innerHTML = `<div class="two"><div class="card"><h2>Đóng hành trình (Builder)</h2>
    <div class="grid"><div><label>Hạng vé</label><select id="hg"><option value="TG">Thương gia (2.5)</option><option value="PT" selected>Phổ thông (1.0)</option><option value="TK">Tiết kiệm (0.8)</option></select></div>
    ${dl ? '<div><label>Mã hành khách</label><input id="mk" value="HK01"></div>' : ''}</div>
    <h3>Các chặng bay</h3><div id="legs"></div>
    <div class="grid" style="margin-top:8px"><div><label>Thêm chặng</label><select id="addsel">${S.flights.map(c => `<option value="${c.ma}">${c.ma}  ${c.di}→${c.den}  ${hm(c.gioDi)}</option>`).join('')}</select></div><div><button class="b g" id="add">+ Thêm chặng</button></div></div>
    <h3>Dịch vụ gia tăng (Decorator)</h3>
    <div class="grid"><div><label>Hành lý thêm (kg)  50.000 /kg</label><input id="kg" type="number" min="0" value="0"></div>
    <div><label>Suất ăn (để trống = không)</label><input id="mon" placeholder="vd: Cơm gà"></div><div><label>Giá suất ăn (VND)</label><input id="gia" type="number" min="0" value="150000"></div></div>
    <div class="acts"><button class="b" id="dat">Đặt hành trình</button></div></div>
    <div class="card"><h2>Kết quả</h2><div id="out"><div class="empty">Chọn các chặng rồi bấm Đặt hành trình</div></div></div></div>`;
  $('#hg').value = S.hanh;
  const drawLegs = () => { $('#legs').innerHTML = S.legs.length ? S.legs.map((m, i) => { const c = S.flights.find(x => x.ma === m); return `<div class="leg"><b>${i + 1}.</b> ${m} ${c ? c.di + ' ? ' + c.den + ' <span class="mut">' + hm(c.gioDi) + '</span>' : ''}<span class="sp"></span><button class="b g s" data-x="${i}">Bỏ</button></div>`; }).join('') : '<div class="mut">Chưa có chặng nào</div>';
    $$('#legs [data-x]').forEach(b => b.onclick = () => { S.legs.splice(b.dataset.x, 1); drawLegs(); }); };
  drawLegs();
  $('#add').onclick = () => { S.legs.push($('#addsel').value); drawLegs(); };
  $('#dat').onclick = guard(async () => {
    S.hanh = $('#hg').value;
    const d = await post('hanhtrinh', { hang: S.hanh, chang: S.legs.join(','), kg: $('#kg').value, mon: $('#mon').value, gia: $('#gia').value, maKhach: dl ? $('#mk').value : '' });
    $('#out').innerHTML = d.ve.map(v => ticketHtml(v, true)).join('') + `<div class="kv"><span>Tổng cộng</span><b class="big">${vnd(d.tong)}</b></div>` +
      hood(['<em>HanhTrinhBuilder</em>.chonHanhKhach().chonHang().themChang()themHanhLy().themSuatAn().<em>build()</em> → <em>HanhTrinh</em>', '<em>QuanLyDatVe::datVeTheoHanhTrinh()</em>: mỗi chặng <em>VeFactory</em> tạo vé, rồi bọc <em>VeCoHanhLy</em> / <em>VeCoSuatAn</em>']);
    S.legs = []; drawLegs(); toast('Đã đặt ' + d.ve.length + ' vé');
  });
}

/* ---------------- THỐNG KÊ ---------------- */
async function pTK() {
  const d = await api('thongke'); const mx = Math.max(1, ...Object.values(d.loaiVe));
  $('#panel').innerHTML = `<div class="stat"><div class="card"><small>Doanh thu vé</small><div class="big">${vnd(d.doanhThu)}</div></div>
    <div class="card"><small>Số vé đã bán</small><div class="big">${d.soVe}</div></div><div class="card"><small>Số chuyến bay</small><div class="big">${d.soChuyen}</div></div>
    ${d.hoaHong !== undefined ? `<div class="card"><small>Hoa hồng đại lý</small><div class="big">${vnd(d.hoaHong)}</div></div>` : ''}</div>
    <div class="card" style="margin-top:16px"><h2>Vé theo hạng</h2>${Object.entries(d.loaiVe).map(([k, n]) => `<div style="display:grid;grid-template-columns:110px 1fr;gap:10px;margin-bottom:8px;align-items:center"><span>${esc(k)}</span><div><div class="bar" style="width:${Math.max(4, n / mx * 100)}%">${n}</div></div></div>`).join('')}
    </div>`;
}

/* ---------------- ADMIN ---------------- */

async function pSS() {
  const all = await api('chuyenbay', { sapxep: 'gio' });
  const dls = await api('daily');
  $('#panel').innerHTML = `<div class="card"><h2>So sánh đại lý & đặt vé hộ</h2>
    <p class="mut">1) Chọn chuyến + hạng vé → So sánh giá &nbsp; 2) Bấm <b>Đặt vé hộ</b> cạnh đại lý → sang giao diện đặt ghế qua đại lý đó.</p>
    <form id="f" class="grid">
      <div><label>Chuyến bay</label><select name="ma">${all.chuyenBay.map(c =>
        `<option value="${c.ma}">${c.ma} ${c.di}→${c.den} ${hm(c.gioDi)} ${vnd(c.gia)}</option>`).join('')}</select></div>
      <div><label>Hạng vé</label><select name="hang">
        <option value="PT">Phổ thông (×1.0)</option>
        <option value="TG">Thương gia (×2.5)</option>
        <option value="TK">Tiết kiệm (×0.8)</option></select></div>
      <div><button class="b" type="submit">So sánh giá</button></div>
    </form>
    <div id="res" style="margin-top:12px"></div></div>`;
  const run = () => {
    const ma = $('#f [name=ma]').value, hang = $('#f [name=hang]').value;
    const c = all.chuyenBay.find(x => x.ma === ma); if (!c) return;
    const heso = hang==='TG'?2.5:hang==='TK'?0.8:1;
    const quay = c.gia * heso;
    let rows = `<tr><td>0</td><td><b>Mua trực tiếp (quầy)</b></td><td>0%</td>
      <td class="right"><b>${vnd(quay)}</b></td>
      <td><button type="button" class="b g" data-dl="" data-ten="Quầy trực tiếp" data-m="${c.ma}">Đặt tại quầy</button></td></tr>`;
    const list = (dls.daiLy||[]).map(d => {
      const gia = quay * (1 - (d.tyLe||0)*0.5);
      return { ...d, gia };
    }).sort((a,b)=>a.gia-b.gia);
    list.forEach((d,i) => {
      rows += `<tr><td>${i+1}</td><td>${esc(d.ten)} <small class="mut">(${d.ma})</small></td>
        <td>${((d.tyLe||0)*100).toFixed(1)}%</td><td class="right">${vnd(d.gia)}</td>
        <td><button type="button" class="b" data-dl="${d.ma}" data-ten="${esc(d.ten)}" data-m="${c.ma}">Đặt vé hộ${i===0?' · rẻ nhất':''}</button></td></tr>`;
    });
    $('#res').innerHTML = `<p>Chuyến <b>${c.ma}</b> ${c.di}→${c.den} — giá cơ bản ${vnd(c.gia)}</p>
      <table><tr><th>STT</th><th>Kênh bán</th><th>HH</th><th class="right">Giá ước tính</th><th>Đặt vé hộ</th></tr>${rows}</table>
      <p class="mut">Sau khi bấm <b>Đặt vé hộ</b>, hệ thống mở màn hình chọn ghế / thanh toán gắn với đại lý tương ứng.</p>`;
    $$('#res [data-m]').forEach(b => b.onclick = () => {
      S.ma = b.dataset.m;
      S.maDaiLy = b.dataset.dl || '';
      S.tenDaiLy = b.dataset.ten || '';
      S.sel = null; S.hold = null;
      S.fromSS = true;
      toast(S.maDaiLy ? ('Chuyển đặt vé hộ qua «' + S.tenDaiLy + '»') : 'Chuyển đặt vé tại quầy');
      go('dat');
    });
  };
  $('#f').onsubmit = e => { e.preventDefault(); run(); }; run();
}

async function pDaily() {
  const d = await api('daily');
  const tk = await api('thongke');
  $('#panel').innerHTML = `<div class="card"><h2>Quản lý đại lý</h2>
    <p class="mut">Doanh thu hệ thống: <b>${vnd(tk.doanhThu||0)}</b> — Tổng vé: <b>${tk.soVe|| (tk.ve?tk.ve:0)}</b></p>
    <p class="mut">Ngày: <b>${(d.daiLy&&d.daiLy[0]&&d.daiLy[0].ngay)||'—'}</b> — HH hôm nay = <b>mức thị trường theo ngày</b> (tăng/giảm so với hôm qua) + <b>bán vé qua ĐL</b> (khi có giao dịch).</p>
    <table><tr><th>Mã</th><th>Tên đại lý</th><th>Tỷ lệ</th><th class="right">HH hôm nay</th><th class="right">Hôm qua</th><th class="right">± so hôm qua</th><th class="right">Tổng HH</th></tr>
    ${(d.daiLy||[]).map(x => {
      const cl = x.chenhLech||0;
      const mau = cl>=0 ? '#059669' : '#dc2626';
      const dau = cl>=0 ? '+' : '';
      return `<tr>
      <td><b>${esc(x.ma)}</b></td><td>${esc(x.ten)}</td>
      <td>${((x.tyLe||0)*100).toFixed(1)}%</td>
      <td class="right">${vnd(x.hoaHongHomNay||0)}<br><small class="mut">TT ${vnd(x.hhThiTruong||0)} + vé ${vnd(x.hhBanVe||0)}</small></td>
      <td class="right">${vnd(x.hoaHongHomQua||0)}</td>
      <td class="right" style="color:${mau};font-weight:600">${dau}${vnd(cl)}</td>
      <td class="right"><b>${vnd(x.tongHoaHong||0)}</b></td></tr>`;
    }).join('')}
    </table>
    <p class="mut">Hoa hồng phản ánh mức độ bán qua từng đại lý — dùng để so sánh cạnh tranh.</p>
  </div>`;
}

async function pQly() {
  const d = await api('chuyenbay', { sapxep: 'gio' });
  const f = (id, title, body) => `<div class="card"><h3 style="margin-top:0">${title}</h3><form id="${id}" class="grid">${body}<div><button class="b">Thêm</button></div></form></div>`;
  $('#panel').innerHTML = `<div class="card"><h2>Danh sách chuyến bay (${d.chuyenBay.length})</h2><div id="tb"></div></div>
    <div class="two">${f('fcb', 'Thêm chuyến bay', `<div><label>Mã chuyến</label><input name="ma" required></div><div><label>Từ</label><select name="di">${apOpts(false)}</select></div><div><label>Đến</label><select name="den">${apOpts(false)}</select></div>
      <div><label>Máy bay</label><select name="mb">${meta.mayBay.map(m => `<option>${esc(m)}</option>`).join('')}</select></div><div><label>Giờ đi</label><input type="datetime-local" name="gioDi" required></div><div><label>Giờ đến</label><input type="datetime-local" name="gioDen" required></div><div><label>Giá cơ bản (VND)</label><input type="number" name="gia" value="1000000" required></div>`)}
    <div>${f('fsb', 'Thêm sân bay', '<div><label>Mã</label><input name="ma" required></div><div><label>Tên</label><input name="ten"></div><div><label>Thành phố</label><input name="tp"></div>')}
    ${f('fmb', 'Thêm máy bay', '<div><label>Số hiệu</label><input name="soHieu" required></div><div><label>Model</label><input name="model"></div><div><label>Ghế TG</label><input type="number" name="tg" value="8"></div><div><label>Ghế PT</label><input type="number" name="pt" value="24"></div><div><label>Ghế TK</label><input type="number" name="tk" value="12"></div>')}</div></div>`;
  $('#tb').innerHTML = flightRows(d.chuyenBay, c => `<button class="b r s" data-d="${c.ma}">Xóa</button>`);
  $$('[data-d]').forEach(b => b.onclick = guard(async () => { if (!confirm('Xóa chuyến ' + b.dataset.d + '?')) return; await post('admin/xoachuyenbay', { ma: b.dataset.d }); toast('Đã xóa'); await pQly(); }));
  const hook = (id, path, msg) => { $('#' + id).onsubmit = guard(async e => { e.preventDefault(); await post(path, Object.fromEntries(new FormData(e.target))); toast(msg); meta = await api('meta'); await pQly(); }); };
  hook('fcb', 'admin/chuyenbay', 'Đã thêm chuyến bay'); hook('fsb', 'admin/sanbay', 'Đã thêm sân bay'); hook('fmb', 'admin/maybay', 'Đã thêm máy bay');
}

(async () => { if (token) await boot(); else loginView(); })();
</script>
</body>
</html>
)HTML";
    return s;
}

using Params = std::map<std::string, std::string>;
const int GIU_CHO_GIAY = 300;   // chi de hien dem nguoc tren giao dien; thoi han that do GiuCho::conHieuLuc() quyet dinh

// ===================== JSON toi gian =====================
std::string esc(const std::string& s) {
    std::string o;
    for (unsigned char c : s) {
        switch (c) {
            case '"': o += "\\\""; break;
            case '\\': o += "\\\\"; break;
            case '\n': o += "\\n"; break;
            case '\r': o += "\\r"; break;
            case '\t': o += "\\t"; break;
            default:
                if (c < 0x20) { char b[8]; std::snprintf(b, sizeof b, "\\u%04x", c); o += b; }
                else o += static_cast<char>(c);
        }
    }
    return o;
}
std::string q(const std::string& s) { return "\"" + esc(s) + "\""; }
std::string num(double v) {
    char b[48];
    if (std::fabs(v - std::floor(v)) < 1e-9 && std::fabs(v) < 1e15) std::snprintf(b, sizeof b, "%lld", static_cast<long long>(v));
    else std::snprintf(b, sizeof b, "%.2f", v);
    return b;
}
std::string mang(const std::vector<std::string>& v) {
    std::string o = "[";
    for (std::size_t i = 0; i < v.size(); ++i) o += (i ? "," : "") + v[i];
    return o + "]";
}
std::string loi(const std::string& m) { return "{\"ok\":false,\"loi\":" + q(m) + "}"; }

// ===================== tien ich =====================
std::string lay(const Params& p, const std::string& k, const std::string& mac = "") {
    auto it = p.find(k);
    return it == p.end() ? mac : it->second;
}
double layThuc(const Params& p, const std::string& k) {
    try { return std::stod(lay(p, k, "0")); } catch (...) { return 0; }
}
std::string cat(std::string s) {
    s.erase(0, s.find_first_not_of(" \t\r\n"));
    s.erase(s.find_last_not_of(" \t\r\n") + 1);
    return s;
}
std::string lamSach(std::string s) {
    for (char& c : s) if (c == ',' || c == ';' || c == '\n' || c == '\r') c = ' ';
    s = cat(s);
    const std::string cach = " + ";
    for (std::size_t vt = s.find(cach); vt != std::string::npos; vt = s.find(cach)) s.replace(vt, cach.size(), " & ");
    return s;
}
std::vector<std::string> tach(const std::string& s, char sep) {
    std::vector<std::string> kq;
    std::stringstream ss(s);
    std::string x;
    while (std::getline(ss, x, sep)) { x = cat(x); if (!x.empty()) kq.push_back(x); }
    return kq;
}
std::string gioStr(time_point tp) {
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    char b[32];
    std::strftime(b, sizeof b, "%Y-%m-%d %H:%M", &tm);
    return b;
}
// "YYYY-MM-DD" (gio mac dinh 12:00) hoac "YYYY-MM-DDTHH:MM" / "YYYY-MM-DD HH:MM"
bool docGio(const std::string& s, time_point& kq) {
    int y, mo, d, h = 12, mi = 0;
    int n = std::sscanf(s.c_str(), "%d-%d-%dT%d:%d", &y, &mo, &d, &h, &mi);
    if (n < 5) n = std::sscanf(s.c_str(), "%d-%d-%d %d:%d", &y, &mo, &d, &h, &mi);
    if (n < 3) return false;
    if (n < 5) { h = 12; mi = 0; }
    std::tm tm{};
    tm.tm_year = y - 1900; tm.tm_mon = mo - 1; tm.tm_mday = d; tm.tm_hour = h; tm.tm_min = mi; tm.tm_isdst = -1;
    kq = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    return true;
}
std::string maHang(HangGhe h) { return h == HangGhe::THUONG_GIA ? "TG" : h == HangGhe::PHO_THONG ? "PT" : "TK"; }
std::string tenHang(HangGhe h) { return h == HangGhe::THUONG_GIA ? "Thương gia" : h == HangGhe::PHO_THONG ? "Phổ thông" : "Tiết kiệm"; }
HangGhe docHang(const std::string& s) {
    if (s == "TG") return HangGhe::THUONG_GIA;
    if (s == "PT") return HangGhe::PHO_THONG;
    if (s == "TK") return HangGhe::TIET_KIEM;
    throw LoiNghiepVu("Hang ve khong hop le (TG/PT/TK)");
}

// ===================== trang thai may chu =====================
struct Giu {
    GiuCho* gc; ChuyenBay* cb; std::string soGhe, chu; DaiLy* dl;
};
// Co the dung bo quan ly noi bo (--web doc lap) hoac gan tu console (dong bo live).
struct TrangThai {
    std::unique_ptr<QuanLyChuyenBay> ownQl;
    std::unique_ptr<QuanLyDatVe> ownDv;
    std::unique_ptr<QuanLyNguoiDung> ownNd;
    std::unique_ptr<FileRepository> ownRepo;
    QuanLyChuyenBay* ql{};
    QuanLyDatVe* dv{};
    QuanLyNguoiDung* nd{};
    FileRepository* repo{};
    std::map<std::string, NguoiDung*> phien;
    std::map<std::string, Giu> giu;
    std::set<std::string> sanBay, mayBay;
    long maxVe = 0;
    int demGiu = 0;
    std::mutex mtx;
    std::atomic<bool> dangChay{false};
    sock_t listenSock = INVALID_SOCKET;

    void dungNoiBo() {
        ownQl = std::make_unique<QuanLyChuyenBay>();
        ownDv = std::make_unique<QuanLyDatVe>(*ownQl);
        ownNd = std::make_unique<QuanLyNguoiDung>();
        ownRepo = std::make_unique<FileRepository>("data");
        ql = ownQl.get(); dv = ownDv.get(); nd = ownNd.get(); repo = ownRepo.get();
    }
    void ganTuConsole(QuanLyChuyenBay& eql, QuanLyDatVe& edv, QuanLyNguoiDung& end, FileRepository& erepo) {
        ownQl.reset(); ownDv.reset(); ownNd.reset(); ownRepo.reset();
        ql = &eql; dv = &edv; nd = &end; repo = &erepo;
    }
} S;

void luu() {
    try { S.repo->ghiNguoiDung(*S.nd); S.repo->ghiChuyenBay(*S.ql); S.repo->ghiVe(*S.dv); }
    catch (const std::exception& e) { std::cerr << "[canh bao] khong luu duoc: " << e.what() << "\n"; }
}

std::string maVeTu(long i) { char b[24]; std::snprintf(b, sizeof b, "VE%05ld", i); return b; }
long soVe(const std::string& ma) {
    try { return ma.size() > 2 ? std::stol(ma.substr(2)) : 0; } catch (...) { return 0; }
}
// UML khong co ham liet ke ve -> do ma VE00001..VE<maxVe> bang timVe() (luon dong bo voi du lieu that)
std::vector<VeMayBay*> tatCaVe() {
    std::vector<VeMayBay*> kq;
    for (long i = 1; i <= S.maxVe; ++i) if (VeMayBay* v = S.dv->timVe(maVeTu(i))) kq.push_back(v);
    return kq;
}
void ghiNhanVe(const VeMayBay* v) { S.maxVe = std::max(S.maxVe, soVe(v->getMaVe())); }

void biet(const ChuyenBay* cb) {
    S.sanBay.insert(cb->getSanBayDi()->getMaSB());
    S.sanBay.insert(cb->getSanBayDen()->getMaSB());
    S.mayBay.insert(cb->getMayBay()->getSoHieu());
}

// ===================== JSON cho doi tuong =====================
std::string chuyenJson(ChuyenBay* cb) {
    cb->donDepGiuCho();
    SoDoGhe& sd = cb->getSoDoGhe();
    MayBay* mb = cb->getMayBay();
    const SanBay* di = cb->getSanBayDi();
    const SanBay* den = cb->getSanBayDen();
    auto bo = [&](HangGhe h) { return "{\"trong\":" + std::to_string(sd.demGheTrong(h)) + ",\"tong\":" + std::to_string(mb->getSoGhe(h)) + "}"; };
    return "{\"ma\":" + q(cb->getMaCB()) + ",\"di\":" + q(di->getMaSB()) + ",\"diTen\":" + q(di->getThanhPho()) +
           ",\"den\":" + q(den->getMaSB()) + ",\"denTen\":" + q(den->getThanhPho()) +
           ",\"gioDi\":" + q(gioStr(cb->getGioDi())) + ",\"gioDen\":" + q(gioStr(cb->getGioDen())) +
           ",\"phut\":" + std::to_string(cb->thoiGianBay().count()) + ",\"gia\":" + num(cb->getGiaCoBan()) +
           ",\"mayBay\":" + q(mb->getSoHieu()) + ",\"TG\":" + bo(HangGhe::THUONG_GIA) + ",\"PT\":" + bo(HangGhe::PHO_THONG) +
           ",\"TK\":" + bo(HangGhe::TIET_KIEM) + "}";
}

std::vector<std::string> dichVuTu(const std::string& moTa) {   // "ma - hang + dv1 + dv2" -> {dv1, dv2}
    std::vector<std::string> kq;
    const std::string cach = " + ";
    std::size_t dau = moTa.find(cach);
    while (dau != std::string::npos) {
        std::size_t tiep = moTa.find(cach, dau + cach.size());
        kq.push_back(moTa.substr(dau + cach.size(), tiep == std::string::npos ? std::string::npos : tiep - dau - cach.size()));
        dau = tiep;
    }
    return kq;
}

std::string veJson(const VeMayBay* v) {
    const ChuyenBay* cb = v->getChuyenBay();
    const VeMayBay* goc = v->veGoc();
    std::string lopGoc = dynamic_cast<const VeThuongGia*>(goc) ? "VeThuongGia" : dynamic_cast<const VePhoThong*>(goc) ? "VePhoThong" : "VeTietKiem";
    std::string hang = lopGoc == "VeThuongGia" ? "TG" : lopGoc == "VePhoThong" ? "PT" : "TK";
    std::vector<std::string> dv = dichVuTu(v->moTa()), dvJson;
    std::string doiTuong = lopGoc;                       // vd VeCoSuatAn(VeCoHanhLy(VePhoThong))
    for (const auto& d : dv) {
        doiTuong = std::string(d.rfind("Hanh ly", 0) == 0 ? "VeCoHanhLy" : "VeCoSuatAn") + "(" + doiTuong + ")";
        dvJson.push_back(q(d));
    }
    const HanhKhach* hk = v->getHanhKhach();
    return "{\"ma\":" + q(v->getMaVe()) + ",\"loai\":" + q(v->loaiVe()) + ",\"hang\":" + q(hang) +
           ",\"moTa\":" + q(v->moTa()) + ",\"ghe\":" + q(v->getSoGhe()) + ",\"maCB\":" + q(cb->getMaCB()) +
           ",\"di\":" + q(cb->getSanBayDi()->getMaSB()) + ",\"den\":" + q(cb->getSanBayDen()->getMaSB()) +
           ",\"gioDi\":" + q(gioStr(cb->getGioDi())) + ",\"gioDen\":" + q(gioStr(cb->getGioDen())) +
           ",\"gia\":" + num(v->tinhGia()) + ",\"giaGoc\":" + num(goc->tinhGia()) +
           ",\"checkIn\":" + (v->isDaCheckIn() ? "true" : "false") + ",\"doiTuong\":" + q(doiTuong) +
           ",\"dichVu\":" + mang(dvJson) + ",\"hanhKhach\":{\"ma\":" + q(hk->getMaND()) + ",\"ten\":" + q(hk->getHoTen()) + "}}";
}

std::string nguoiJson(NguoiDung* u) {
    const char* vt = dynamic_cast<Admin*>(u) ? "AD" : dynamic_cast<DaiLy*>(u) ? "DL" : "HK";
    std::string s = "{\"ma\":" + q(u->getMaND()) + ",\"ten\":" + q(u->getHoTen()) + ",\"vaiTro\":" + q(vt) +
                    ",\"vaiTroTen\":" + q(u->vaiTro());
    if (auto* dl = dynamic_cast<DaiLy*>(u)) s += ",\"hoaHong\":" + num(dl->getTongHoaHong());
    return s + "}";
}

// ===================== xac thuc =====================
NguoiDung* nguoiDung(const Params& p) {
    auto it = S.phien.find(lay(p, "token"));
    if (it == S.phien.end()) throw LoiNghiepVu("Chua dang nhap hoac phien da het han");
    return it->second;
}
void canQuyen(NguoiDung* u, const std::string& ds) {   // ds: vd "AD" hoac "HKDL"
    const char* vt = dynamic_cast<Admin*>(u) ? "AD" : dynamic_cast<DaiLy*>(u) ? "DL" : "HK";
    if (ds.find(vt) == std::string::npos) throw LoiNghiepVu("Vai tro cua ban khong duoc phep thuc hien thao tac nay");
}
VeMayBay* veCuaToi(NguoiDung* u, const std::string& ma) {
    VeMayBay* v = S.dv->timVe(ma);
    if (!v) throw LoiNghiepVu("Khong tim thay ve " + ma);
    if (auto* hk = dynamic_cast<HanhKhach*>(u)) if (v->getHanhKhach() != hk) throw LoiNghiepVu("Ve nay khong phai cua ban");
    return v;
}
// Hanh khach tu dat; dai ly dat ho (maKhach); admin khong duoc dat ve.
void nguoiMua(NguoiDung* u, const Params& p, HanhKhach*& hk, DaiLy*& dl) {
    hk = nullptr; dl = nullptr;
    if (auto* h = dynamic_cast<HanhKhach*>(u)) { hk = h; return; }
    if (auto* d = dynamic_cast<DaiLy*>(u)) {
        dl = d;
        const std::string ma = lay(p, "maKhach");
        hk = dynamic_cast<HanhKhach*>(S.nd->timNguoiDung(ma));
        if (!hk) throw LoiNghiepVu("Khong tim thay hanh khach \"" + ma + "\" (vd HK01)");
        return;
    }
    throw LoiNghiepVu("Admin khong dat ve");
}

// ===================== cac API =====================
std::string hLogin(const Params& p) {
    NguoiDung* u = S.nd->dangNhap(lay(p, "ma"), lay(p, "mk"));
    if (!u) throw LoiNghiepVu("Sai ma nguoi dung hoac mat khau");
    std::random_device rd;
    char tk[40];
    std::snprintf(tk, sizeof tk, "%08x%08x%08x", rd(), rd(), rd());
    S.phien[tk] = u;
    return "{\"ok\":true,\"token\":" + q(tk) + ",\"user\":" + nguoiJson(u) + "}";
}
std::string hDangKy(const Params& p) {
    // Chi cho dang ky Hanh khach - Admin/Dai ly khong tu tao qua web
    const std::string ma = lamSach(lay(p, "ma"));
    const std::string mk = lay(p, "mk");
    const std::string ten = lamSach(lay(p, "ten"));
    const std::string cccd = lamSach(lay(p, "cccd"));
    const std::string sdt = lamSach(lay(p, "sdt"));
    const std::string email = lamSach(lay(p, "email"));
    // Gom tat ca loi de bao cung luc (co dau)
    std::vector<std::string> loi;
    if (ma.empty())
        loi.push_back("Mã người dùng bắt buộc (vd HK01)");
    else if (ma.rfind("AD", 0) == 0 || ma.rfind("DL", 0) == 0)
        loi.push_back("Không được dùng mã AD/DL. Chỉ đăng ký Hành khách (vd HK03)");
    else if (S.nd->timNguoiDung(ma))
        loi.push_back("Mã người dùng đã tồn tại, chọn mã khác");
    if (ten.empty())
        loi.push_back("Họ tên bắt buộc");
    if (mk.size() < 3)
        loi.push_back("Mật khẩu ít nhất 3 ký tự");
    if (cccd.empty())
        loi.push_back("CCCD bắt buộc");
    else if (!((cccd.size() == 9 || cccd.size() == 12) && chiChuSo(cccd)))
        loi.push_back("CCCD không hợp lệ — phải 9 hoặc 12 chữ số (vd 001234567890)");
    if (sdt.empty())
        loi.push_back("Số điện thoại bắt buộc");
    else if (!((sdt.size() == 10 || sdt.size() == 11) && chiChuSo(sdt) && sdt[0] == '0'))
        loi.push_back("Số điện thoại không hợp lệ — phải 10–11 chữ số, bắt đầu bằng 0 (vd 0901234567)");
    if (email.empty())
        loi.push_back("Email bắt buộc");
    else if (!emailHopLe(email))
        loi.push_back("Email không hợp lệ — cần có @ và tên miền (vd ten@gmail.com)");
    if (!loi.empty()) {
        std::string msg = "Có " + std::to_string(loi.size()) + " lỗi:\n";
        for (std::size_t i = 0; i < loi.size(); ++i)
            msg += "• " + loi[i] + (i + 1 < loi.size() ? "\n" : "");
        throw LoiNghiepVu(msg);
    }
    S.nd->themNguoiDung(std::make_unique<HanhKhach>(ma, ten, mk, cccd, sdt, email));
    try { S.repo->ghiNguoiDung(*S.nd); } catch (...) {}
    return "{\"ok\":true,\"thongBao\":\"Dang ky thanh cong, hay dang nhap\",\"ma\":" + q(ma) + "}";
}

std::string hMe(const Params& p) { return "{\"ok\":true,\"user\":" + nguoiJson(nguoiDung(p)) + "}"; }
std::string hLogout(const Params& p) { S.phien.erase(lay(p, "token")); return "{\"ok\":true}"; }

std::string hMeta(const Params&) {
    std::vector<std::string> sb, mb;
    for (const auto& ma : S.sanBay) if (SanBay* s = S.ql->timSanBay(ma))
        sb.push_back("{\"ma\":" + q(ma) + ",\"ten\":" + q(s->getTen()) + ",\"tp\":" + q(s->getThanhPho()) + "}");
    for (const auto& ma : S.mayBay) mb.push_back(q(ma));
    return "{\"ok\":true,\"sanBay\":" + mang(sb) + ",\"mayBay\":" + mang(mb) + "}";
}

std::string hChuyenBay(const Params& p) {
    time_point ngay{};
    const std::string n = lay(p, "ngay");
    if (!n.empty() && !docGio(n, ngay)) throw LoiNghiepVu("Ngay khong hop le (YYYY-MM-DD)");
    std::vector<ChuyenBay*> ds = S.ql->timChuyenBay(lay(p, "di"), lay(p, "den"), ngay);
    if (lay(p, "sapxep") == "gia") S.ql->sapXepTheoGia(ds); else S.ql->sapXepTheoGio(ds);
    std::vector<std::string> out;
    for (ChuyenBay* cb : ds) out.push_back(chuyenJson(cb));
    return "{\"ok\":true,\"chuyenBay\":" + mang(out) + "}";
}

std::string hNoiChuyen(const Params& p) {
    time_point ngay{};
    const std::string n = lay(p, "ngay");
    if (!n.empty() && !docGio(n, ngay)) throw LoiNghiepVu("Ngay khong hop le (YYYY-MM-DD)");
    const int cho = static_cast<int>(layThuc(p, "cho") > 0 ? layThuc(p, "cho") : 60);
    const int toiDa = static_cast<int>(layThuc(p, "toida") > 0 ? layThuc(p, "toida") : 3);
    auto kq = S.ql->timNoiChuyen(lay(p, "di"), lay(p, "den"), ngay, std::chrono::minutes(cho), toiDa);
    std::vector<std::string> out;
    for (const auto& duong : kq) {
        std::vector<std::string> chang;
        double tong = 0;
        for (ChuyenBay* cb : duong) { chang.push_back(chuyenJson(cb)); tong += cb->getGiaCoBan(); }
        const long long phut = std::chrono::duration_cast<std::chrono::minutes>(duong.back()->getGioDen() - duong.front()->getGioDi()).count();
        out.push_back("{\"chang\":" + mang(chang) + ",\"tongGia\":" + num(tong) + ",\"tongPhut\":" + std::to_string(phut) + "}");
    }
    return "{\"ok\":true,\"hanhTrinh\":" + mang(out) + "}";
}

// UML khong co ham liet ke ghe -> do cac nhan ghe co the co (hang 1..80, cot A..F) bang timGhe()
std::string hSoDoGhe(const Params& p) {
    ChuyenBay* cb = S.ql->timChuyenBay(lay(p, "ma"));
    if (!cb) throw LoiNghiepVu("Khong co chuyen bay " + lay(p, "ma"));
    cb->donDepGiuCho();
    std::vector<std::string> ghe;
    for (int h = 1; h <= 80; ++h)
        for (char c = 'A'; c <= 'F'; ++c) {
            const std::string nhan = std::to_string(h) + c;
            Ghe* g = cb->getSoDoGhe().timGhe(nhan);
            if (!g) continue;
            const char* tt = g->getTrangThai() == TrangThaiGhe::TRONG ? "T" : g->getTrangThai() == TrangThaiGhe::GIU_CHO ? "H" : "X";
            ghe.push_back("{\"so\":" + q(nhan) + ",\"hang\":" + q(maHang(g->getHang())) + ",\"tt\":" + q(tt) + "}");
        }
    return "{\"ok\":true,\"chuyenBay\":" + chuyenJson(cb) + ",\"ghe\":" + mang(ghe) + "}";
}

std::string hGiuCho(const Params& p) {
    NguoiDung* u = nguoiDung(p);
    HanhKhach* hk; DaiLy* dl;
    nguoiMua(u, p, hk, dl);
    // Hanh khach co the chon mua qua dai ly (maDaiLy tu man so sanh)
    if (!dl) {
        const std::string maDl = lay(p, "maDaiLy");
        if (!maDl.empty()) {
            dl = dynamic_cast<DaiLy*>(S.nd->timNguoiDung(maDl));
            if (!dl) throw LoiNghiepVu("Khong tim thay dai ly " + maDl);
        }
    }
    // Tuy chon "giay" (5..299): demo het han nhanh - goi thang ChuyenBay::giuGhe voi timeout ngan.
    const int giay = static_cast<int>(layThuc(p, "giay"));
    GiuCho* gc = nullptr;
    if (giay >= 5 && giay < GIU_CHO_GIAY) {
        ChuyenBay* cb = S.ql->timChuyenBay(lay(p, "maCB"));
        if (!cb) throw LoiNghiepVu("Khong co chuyen bay " + lay(p, "maCB"));
        gc = cb->giuGhe(lay(p, "soGhe"), hk, std::chrono::seconds(giay));
    } else {
        gc = S.dv->giuCho(lay(p, "maCB"), lay(p, "soGhe"), hk);
    }
    const int conLai = (giay >= 5 && giay < GIU_CHO_GIAY) ? giay : GIU_CHO_GIAY;
    const std::string id = "G" + std::to_string(++S.demGiu);
    S.giu[id] = Giu{gc, gc->getChuyenBay(), gc->getSoGhe(), u->getMaND(), dl};
    const Ghe* g = gc->getChuyenBay()->getSoDoGhe().timGhe(gc->getSoGhe());
    return "{\"ok\":true,\"holdId\":" + q(id) + ",\"conLai\":" + std::to_string(conLai) + ",\"maCB\":" + q(gc->getChuyenBay()->getMaCB()) +
           ",\"soGhe\":" + q(gc->getSoGhe()) + ",\"hang\":" + q(maHang(g->getHang())) + ",\"hangTen\":" + q(tenHang(g->getHang())) + "}";
}

std::map<std::string, Giu>::iterator timGiu(NguoiDung* u, const Params& p) {
    auto it = S.giu.find(lay(p, "holdId"));
    if (it == S.giu.end()) throw LoiNghiepVu("Khong tim thay phien giu cho (co the da het han hoac da xu ly)");
    if (it->second.chu != u->getMaND()) throw LoiNghiepVu("Phien giu cho nay khong phai cua ban");
    return it;
}

std::string hHuyGiuCho(const Params& p) {
    NguoiDung* u = nguoiDung(p);
    auto it = timGiu(u, p);
    it->second.cb->huyGiu(it->second.soGhe);
    S.giu.erase(it);
    return "{\"ok\":true}";
}

std::string hThanhToan(const Params& p) {
    NguoiDung* u = nguoiDung(p);
    auto it = timGiu(u, p);
    Giu g = it->second;
    S.giu.erase(it);                                            // phien chi dung duoc 1 lan
    const Ghe* ghe = g.cb->getSoDoGhe().timGhe(g.soGhe);
    VeMayBay* ve = S.dv->xacNhanThanhToan(g.gc, ghe->getHang(), g.dl);   // nem LoiGiuChoHetHan neu qua han
    ghiNhanVe(ve);
    luu();
    return "{\"ok\":true,\"ve\":" + veJson(ve) + "}";
}

std::string hHanhTrinh(const Params& p) {
    NguoiDung* u = nguoiDung(p);
    HanhKhach* hk; DaiLy* dl;
    nguoiMua(u, p, hk, dl);
    HanhTrinhBuilder b;
    b.chonHanhKhach(hk).chonHang(docHang(lay(p, "hang")));
    const std::vector<std::string> cacChang = tach(lay(p, "chang"), ',');
    if (cacChang.empty()) throw LoiNghiepVu("Chua chon chang bay nao");
    for (const auto& ma : cacChang) {
        ChuyenBay* cb = S.ql->timChuyenBay(ma);
        if (!cb) throw LoiNghiepVu("Khong co chuyen bay " + ma);
        b.themChang(cb);
    }
    if (layThuc(p, "kg") > 0) b.themHanhLy(layThuc(p, "kg"));
    const std::string mon = lamSach(lay(p, "mon"));
    if (!mon.empty()) b.themSuatAn(mon, layThuc(p, "gia"));
    const HanhTrinh ht = b.build();
    const std::vector<VeMayBay*> ds = S.dv->datVeTheoHanhTrinh(ht, dl);
    std::vector<std::string> out;
    double tong = 0;
    for (const VeMayBay* v : ds) { ghiNhanVe(v); out.push_back(veJson(v)); tong += v->tinhGia(); }
    luu();
    return "{\"ok\":true,\"ve\":" + mang(out) + ",\"tong\":" + num(tong) + "}";
}

std::string hVe(const Params& p) {
    NguoiDung* u = nguoiDung(p);
    std::vector<std::string> out;
    for (const VeMayBay* v : tatCaVe()) {
        if (auto* hk = dynamic_cast<HanhKhach*>(u)) if (v->getHanhKhach() != hk) continue;
        out.push_back(veJson(v));
    }
    return "{\"ok\":true,\"ve\":" + mang(out) + "}";
}

std::string hCheckIn(const Params& p) {
    NguoiDung* u = nguoiDung(p);
    canQuyen(u, "HKDL");
    VeMayBay* v = veCuaToi(u, lay(p, "maVe"));
    const std::string ghe = lay(p, "soGhe").empty() ? v->getSoGhe() : lay(p, "soGhe");
    S.dv->checkIn(v->getMaVe(), ghe);
    luu();
    return "{\"ok\":true,\"ve\":" + veJson(v) + "}";
}

std::string hBoardingPass(const Params& p) {
    NguoiDung* u = nguoiDung(p);
    VeMayBay* v = veCuaToi(u, lay(p, "maVe"));
    std::ostringstream cap;                                     // bat dau ra cua VeMayBay::inBoardingPass() de hien len web
    std::streambuf* cu = std::cout.rdbuf(cap.rdbuf());
    try { S.dv->inBoardingPass(v->getMaVe()); } catch (...) { std::cout.rdbuf(cu); throw; }
    std::cout.rdbuf(cu);
    return "{\"ok\":true,\"text\":" + q(cap.str()) + ",\"ve\":" + veJson(v) + "}";
}

std::string hHuyVe(const Params& p) {
    NguoiDung* u = nguoiDung(p);
    canQuyen(u, "HKDL");
    VeMayBay* v = veCuaToi(u, lay(p, "maVe"));
    S.dv->huyVe(v->getMaVe());
    luu();
    return "{\"ok\":true}";
}


std::string hDaiLy(const Params&) {
    std::vector<std::string> out;
    for (NguoiDung* u : S.nd->layDsNguoiDung()) {
        auto* dl = dynamic_cast<DaiLy*>(u);
        if (!dl) continue;
        dl->dongBoNgayThuc();
        out.push_back("{\"ma\":" + q(dl->getMaND()) + ",\"ten\":" + q(dl->getHoTen())
            + ",\"tyLe\":" + num(dl->getTyLeHoaHong())
            + ",\"tongHoaHong\":" + num(dl->getTongHoaHong())
            + ",\"hoaHongHomNay\":" + num(dl->getHoaHongHomNay())
            + ",\"hoaHongHomQua\":" + num(dl->getHoaHongHomQua())
            + ",\"chenhLech\":" + num(dl->getChenhLechVoiHomQua())
            + ",\"hhBanVe\":" + num(dl->getHoaHongBanVeHomNay())
            + ",\"hhThiTruong\":" + num(dl->getHoaHongThiTruongHomNay())
            + ",\"ngay\":" + q(dl->getNgayHoaHong()) + "}");
    }
    return "{\"ok\":true,\"daiLy\":" + mang(out) + "}";
}

std::string hThongKe(const Params& p) {
    NguoiDung* u = nguoiDung(p);
    canQuyen(u, "ADDL");
    std::map<std::string, int> loai = S.dv->thongKeLoaiVe();
    const std::vector<VeMayBay*> ve = tatCaVe();
    std::string s = "{\"ok\":true,\"doanhThu\":" + num(S.dv->tinhDoanhThu()) + ",\"soVe\":" + std::to_string(ve.size()) +
                    ",\"soChuyen\":" + std::to_string(S.ql->timChuyenBay("", "", time_point{}).size()) + ",\"loaiVe\":{";
    bool dau = true;
    for (const auto& kv : loai) { s += (dau ? "" : ",") + q(kv.first) + ":" + std::to_string(kv.second); dau = false; }
    s += "}";
    if (auto* dl = dynamic_cast<DaiLy*>(u)) s += ",\"hoaHong\":" + num(dl->getTongHoaHong());
    return s + "}";
}

std::string hThemSanBay(const Params& p) {
    canQuyen(nguoiDung(p), "AD");
    const std::string ma = lamSach(lay(p, "ma"));
    if (ma.empty()) throw LoiNghiepVu("Ma san bay khong duoc de trong");
    S.ql->themSanBay(std::make_unique<SanBay>(ma, lamSach(lay(p, "ten")), lamSach(lay(p, "tp"))));
    S.sanBay.insert(ma);
    luu();
    return "{\"ok\":true}";
}
std::string hThemMayBay(const Params& p) {
    canQuyen(nguoiDung(p), "AD");
    const std::string ma = lamSach(lay(p, "soHieu"));
    const int tg = static_cast<int>(layThuc(p, "tg")), pt = static_cast<int>(layThuc(p, "pt")), tk = static_cast<int>(layThuc(p, "tk"));
    if (ma.empty() || tg < 0 || pt < 0 || tk < 0 || tg + pt + tk == 0) throw LoiNghiepVu("Thong tin may bay khong hop le");
    S.ql->themMayBay(std::make_unique<MayBay>(ma, lamSach(lay(p, "model")), tg, pt, tk));
    S.mayBay.insert(ma);
    luu();
    return "{\"ok\":true}";
}
std::string hThemChuyenBay(const Params& p) {
    canQuyen(nguoiDung(p), "AD");
    const std::string ma = lamSach(lay(p, "ma"));
    SanBay* di = S.ql->timSanBay(lay(p, "di"));
    SanBay* den = S.ql->timSanBay(lay(p, "den"));
    MayBay* mb = S.ql->timMayBay(lay(p, "mb"));
    time_point gioDi, gioDen;
    if (ma.empty() || !di || !den || !mb || di == den) throw LoiNghiepVu("Ma chuyen bay / san bay / may bay khong hop le");
    if (!docGio(lay(p, "gioDi"), gioDi) || !docGio(lay(p, "gioDen"), gioDen)) throw LoiNghiepVu("Dinh dang gio khong dung");
    if (gioDen <= gioDi) throw LoiNghiepVu("Giờ đến phai sau gio di");
    if (layThuc(p, "gia") <= 0) throw LoiNghiepVu("Gia phai lon hon 0");
    S.ql->themChuyenBay(std::make_unique<ChuyenBay>(ma, di, den, mb, gioDi, gioDen, layThuc(p, "gia")));
    luu();
    return "{\"ok\":true}";
}
std::string hXoaChuyenBay(const Params& p) {
    canQuyen(nguoiDung(p), "AD");
    ChuyenBay* cb = S.ql->timChuyenBay(lay(p, "ma"));
    S.ql->xoaChuyenBay(lay(p, "ma"));
    for (auto it = S.giu.begin(); it != S.giu.end();) it = (it->second.cb == cb) ? S.giu.erase(it) : std::next(it);
    luu();
    return "{\"ok\":true}";
}

typedef std::string (*Handler)(const Params&);
const std::map<std::string, Handler> TUYEN = {
    {"POST /api/login", hLogin}, {"POST /api/dangky", hDangKy},           {"GET /api/me", hMe},                  {"POST /api/logout", hLogout},
    {"GET /api/meta", hMeta},              {"GET /api/chuyenbay", hChuyenBay},    {"GET /api/noichuyen", hNoiChuyen},
    {"GET /api/sodoghe", hSoDoGhe},        {"POST /api/giucho", hGiuCho},         {"POST /api/huygiucho", hHuyGiuCho},
    {"POST /api/thanhtoan", hThanhToan},   {"POST /api/hanhtrinh", hHanhTrinh},   {"GET /api/ve", hVe},
    {"POST /api/checkin", hCheckIn},       {"GET /api/boardingpass", hBoardingPass}, {"POST /api/huyve", hHuyVe},
    {"GET /api/thongke", hThongKe}, {"GET /api/daily", hDaiLy},        {"POST /api/admin/sanbay", hThemSanBay}, {"POST /api/admin/maybay", hThemMayBay},
    {"POST /api/admin/chuyenbay", hThemChuyenBay}, {"POST /api/admin/xoachuyenbay", hXoaChuyenBay}};

// ===================== HTTP =====================

std::string giaiMa(const std::string& s) {
    std::string o;
    for (std::size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '+') o += ' ';
        else if (s[i] == '%' && i + 2 < s.size() + 0 && std::isxdigit(static_cast<unsigned char>(s[i + 1])) && std::isxdigit(static_cast<unsigned char>(s[i + 2]))) {
            o += static_cast<char>(std::stoi(s.substr(i + 1, 2), nullptr, 16));
            i += 2;
        } else o += s[i];
    }
    return o;
}
void docThamSo(const std::string& s, Params& p) {
    std::stringstream ss(s);
    std::string kv;
    while (std::getline(ss, kv, '&')) {
        const std::size_t e = kv.find('=');
        if (e == std::string::npos) { if (!kv.empty()) p[giaiMa(kv)] = ""; }
        else p[giaiMa(kv.substr(0, e))] = giaiMa(kv.substr(e + 1));
    }
}
std::string dap(int ma, const std::string& loai, const std::string& body) {
    const char* tt = ma == 200 ? "OK" : ma == 404 ? "Not Found" : ma == 400 ? "Bad Request" : "Error";
    return "HTTP/1.1 " + std::to_string(ma) + " " + tt + "\r\nContent-Type: " + loai + "; charset=utf-8\r\nContent-Length: " +
           std::to_string(body.size()) + "\r\nCache-Control: no-store\r\nConnection: close\r\n\r\n" + body;
}
void guiHet(sock_t c, const std::string& s) {
    std::size_t da = 0;
    while (da < s.size()) {
        const int n = static_cast<int>(::send(c, s.data() + da, static_cast<int>(s.size() - da), 0));
        if (n <= 0) return;
        da += static_cast<std::size_t>(n);
    }
}
void phucVu(sock_t c) {
#ifdef _WIN32
    DWORD ms = 1000;
    setsockopt(c, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&ms), sizeof ms);
#else
    timeval tv{1, 0};
    setsockopt(c, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof tv);
#endif
    std::string buf;
    char tmp[4096];
    std::size_t dauHet = std::string::npos;
    while (dauHet == std::string::npos && buf.size() < 65536) {
        const int n = static_cast<int>(::recv(c, tmp, sizeof tmp, 0));
        if (n <= 0) { DONG_SOCK(c); return; }                    // ket noi nhan roi / het gio cho
        buf.append(tmp, static_cast<std::size_t>(n));
        dauHet = buf.find("\r\n\r\n");
    }
    if (dauHet == std::string::npos) { DONG_SOCK(c); return; }
    const std::string head = buf.substr(0, dauHet);
    std::string body = buf.substr(dauHet + 4);
    std::size_t dai = 0;
    {
        std::string thap = head;
        for (char& ch : thap) ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
        const std::size_t vt = thap.find("content-length:");
        if (vt != std::string::npos) dai = static_cast<std::size_t>(std::atol(thap.c_str() + vt + 15));
    }
    while (body.size() < dai && body.size() < 1048576) {
        const int n = static_cast<int>(::recv(c, tmp, sizeof tmp, 0));
        if (n <= 0) break;
        body.append(tmp, static_cast<std::size_t>(n));
    }
    std::istringstream rl(head.substr(0, head.find("\r\n")));
    std::string phuongThuc, dich, giaoThuc;
    rl >> phuongThuc >> dich >> giaoThuc;
    std::string duong = dich, truyVan;
    const std::size_t h = dich.find('?');
    if (h != std::string::npos) { duong = dich.substr(0, h); truyVan = dich.substr(h + 1); }
    Params p;
    docThamSo(truyVan, p);
    if (phuongThuc == "POST") docThamSo(body, p);

    std::string kq;
    {
        if (duong.rfind("/api/", 0) == 0) {
            auto it = TUYEN.find(phuongThuc + " " + duong);
            if (it == TUYEN.end()) kq = dap(404, "application/json", loi("Khong co API " + phuongThuc + " " + duong));
            else {
                try { kq = dap(200, "application/json", it->second(p)); }
                catch (const std::exception& e) { kq = dap(200, "application/json", loi(e.what())); }
            }
        } else if (duong == "/" || duong == "/index.html") {
            kq = dap(200, "text/html", trangChu());
        } else if (duong == "/favicon.ico") {
            kq = "HTTP/1.1 204 No Content\r\nConnection: close\r\n\r\n";
        } else {
            kq = dap(404, "text/plain", "404");
        }
    }
    guiHet(c, kq);
    DONG_SOCK(c);
}

void vongLapChapNhan();
bool batDauTuConsole(QuanLyChuyenBay& ql, QuanLyDatVe& dv, QuanLyNguoiDung& nd, FileRepository& repo, int cong = 8080);

int chayWeb(int argc, char** argv) {
    batUtf8Console();
    int cong = 8080;
    std::string host = "127.0.0.1";
    bool datLai = false;
    for (int i = 1; i < argc; ++i) {
        const std::string a = argv[i];
        if (a == "--port" && i + 1 < argc) cong = std::atoi(argv[++i]);
        else if (a == "--host" && i + 1 < argc) host = argv[++i];
        else if (a == "--reset") datLai = true;
        else if (a == "--web") {}
        else { std::cout << "Dung: chuong_trinh --web [--port 8080] [--host 127.0.0.1|0.0.0.0] [--reset]\n"; return 0; }
    }
    if (datLai) std::filesystem::remove_all("data");
    S.dungNoiBo();
    try {
        if (std::filesystem::exists("data/nguoidung.csv")) {
            S.repo->docNguoiDung(*S.nd);
            S.repo->docChuyenBay(*S.ql);
            S.repo->docVe(*S.dv);
            std::cout << "Đã nạp dữ liệu từ thư mục data/ (dung --reset de bat dau lai tu du lieu mau)\n";
            capNhatGioChuyenTheoThoiGianThuc(*S.ql);
        } else {
            nhapDuLieuMau(*S.ql, *S.nd);
            luu();
            std::cout << "Lần chạy đầu: đã tạo dữ liệu mẫu.\n";
        }
        for (ChuyenBay* cb : S.ql->timChuyenBay("", "", time_point{})) biet(cb);
        for (long i = 1; i <= 5000; ++i) if (VeMayBay* v = S.dv->timVe(maVeTu(i))) ghiNhanVe(v);
    } catch (const std::exception& e) {
        std::cerr << "Loi nap du lieu: " << e.what() << "\n";
        return 1;
    }
#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
    sock_t ls = ::socket(AF_INET, SOCK_STREAM, 0);
    if (ls == INVALID_SOCKET) { std::cerr << "Khong tao duoc socket\n"; return 1; }
    int bat = 1;
    setsockopt(ls, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&bat), sizeof bat);
    sockaddr_in dc{};
    dc.sin_family = AF_INET;
    dc.sin_port = htons(static_cast<unsigned short>(cong));
    if (inet_pton(AF_INET, host.c_str(), &dc.sin_addr) != 1) { std::cerr << "Dia chi khong hop le: " << host << "\n"; return 1; }
    if (::bind(ls, reinterpret_cast<sockaddr*>(&dc), sizeof dc) != 0 || ::listen(ls, 64) != 0) {
        std::cerr << "Khong mo duoc cong " << cong << " (co the dang bi chuong trinh khac dung). Thu: --port 8081\n";
        return 1;
    }
    S.listenSock = ls;
    S.dangChay = true;
    std::cout << "\n  WEB DEMO dang chay:  http://" << (host == "0.0.0.0" ? "localhost" : host) << ":" << cong << "\n"
              << "  Tai khoan: AD01/admin123  DL01/daily123  HK01/hk123  HK02/hk456\n"
              << "  Nhan Ctrl+C de dung.\n\n";
    vongLapChapNhan();
    return 0;
}

void vongLapChapNhan() {
    while (S.dangChay.load()) {
        sock_t c = ::accept(S.listenSock, nullptr, nullptr);
        if (c == INVALID_SOCKET) continue;
        {
            std::lock_guard<std::mutex> lock(S.mtx);
            phucVu(c);
        }
    }
    if (S.listenSock != INVALID_SOCKET) {
        DONG_SOCK(S.listenSock);
        S.listenSock = INVALID_SOCKET;
    }
}

// Goi tu menu console: dung CHUNG bo quan ly -> moi thay doi console hien ngay tren web.

std::vector<std::string> layDiaChiLan() {
    std::vector<std::string> out;
    char host[256]{};
    if (gethostname(host, sizeof host) != 0) return out;
    addrinfo goi{}, *res = nullptr;
    goi.ai_family = AF_INET;
    if (getaddrinfo(host, nullptr, &goi, &res) != 0) return out;
    for (addrinfo* p = res; p; p = p->ai_next) {
        char buf[INET_ADDRSTRLEN]{};
        auto* a = reinterpret_cast<sockaddr_in*>(p->ai_addr);
        if (!a) continue;
        if (inet_ntop(AF_INET, &a->sin_addr, buf, sizeof buf)) {
            std::string ip = buf;
            if (ip.rfind("127.", 0) != 0)
                out.push_back(ip);
        }
    }
    freeaddrinfo(res);
    return out;
}

bool batDauTuConsole(QuanLyChuyenBay& ql, QuanLyDatVe& dv, QuanLyNguoiDung& nd, FileRepository& repo, int cong) {
    if (S.dangChay.load()) {
        std::cout << "Web dang chay san tai http://localhost:" << cong << "\n";
        return true;
    }
    S.ganTuConsole(ql, dv, nd, repo);
    if (S.ql) capNhatGioChuyenTheoThoiGianThuc(*S.ql);
    S.phien.clear();
    S.giu.clear();
    S.sanBay.clear();
    S.mayBay.clear();
    S.maxVe = 0;
    S.demGiu = 0;
    for (ChuyenBay* cb : S.ql->timChuyenBay("", "", time_point{})) biet(cb);
    for (long i = 1; i <= 5000; ++i) if (VeMayBay* v = S.dv->timVe(maVeTu(i))) ghiNhanVe(v);

#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
    sock_t ls = ::socket(AF_INET, SOCK_STREAM, 0);
    if (ls == INVALID_SOCKET) { std::cerr << "Khong tao duoc socket\n"; return false; }
    int bat = 1;
    setsockopt(ls, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&bat), sizeof bat);
    sockaddr_in dc{};
    dc.sin_family = AF_INET;
    dc.sin_port = htons(static_cast<unsigned short>(cong));
    inet_pton(AF_INET, "127.0.0.1", &dc.sin_addr);
    if (::bind(ls, reinterpret_cast<sockaddr*>(&dc), sizeof dc) != 0 || ::listen(ls, 64) != 0) {
        std::cerr << "Khong mo duoc cong " << cong << " (thu cong khac).\n";
        DONG_SOCK(ls);
        return false;
    }
    S.listenSock = ls;
    S.dangChay = true;
    std::thread([]() { vongLapChapNhan(); }).detach();
    std::cout << "\n  *** WEB da mo (dong bo live voi console) ***\n"
              << "  Mở trình duyệt:  http://localhost:" << cong << "\n"
              << "  Moi thao tac dat ve / them chuyen tren console se hien ngay tren web.\n"
              << "  Tai khoan: AD01/admin123  DL01/daily123  HK01/hk123  HK02/hk456\n\n";
    return true;
}
}  // namespace web
#endif  // BAT_WEB_DEMO


// ======================================================================
// HAM MAIN
// ======================================================================


// ===== main (cùng translation unit với helpers anonymous) =====