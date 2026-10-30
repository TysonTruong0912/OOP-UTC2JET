const S = { user:null, flights:[], airports:[], planes:[], airlines:[], users:[], meals:[], book:null, alerts:[] };
const SLOT=[[6,0],[6,40],[7,20],[8,0],[8,45],[9,30],[10,30],[11,30],[12,30],[13,30],[14,45],[16,0],[17,0],[18,0],[19,0],[20,0],[21,0],[22,0]];
const HS={TG:2.5,PT:1.0,TK:0.8}, HL={TG:40,PT:20,TK:15};
const $ = s=>document.querySelector(s), $$=s=>[...document.querySelectorAll(s)];

const CITY_DEST = {
  BMV:{ten:'Buôn Ma Thuột', anh:['img/cities/BMV_1.jpg','img/cities/BMV_2.jpg','img/cities/BMV_3.jpg'], moTa:'Thác Dray Nur · Cà phê · Bản Đôn'},
  CAH:{ten:'Cà Mau', anh:['img/cities/CAH_1.jpg','img/cities/CAH_2.jpg','img/cities/CAH_3.jpg'], moTa:'Mũi Cà Mau · Rừng đước'},
  CXR:{ten:'Nha Trang', anh:['img/cities/CXR_1.jpg','img/cities/CXR_2.jpg','img/cities/CXR_3.jpg'], moTa:'Tháp Bà · Biển Trần Phú · Vinpearl'},
  DAD:{ten:'Đà Nẵng', anh:['img/cities/DAD_1.jpg','img/cities/DAD_2.jpg','img/cities/DAD_3.jpg'], moTa:'Cầu Rồng · Mỹ Khê · Sơn Trà'},
  DIN:{ten:'Điện Biên', anh:['img/cities/DIN_1.jpg','img/cities/DIN_2.jpg','img/cities/DIN_3.jpg'], moTa:'Điện Biên Phủ · Mường Thanh'},
  DLI:{ten:'Đà Lạt', anh:['img/cities/DLI_1.jpg','img/cities/DLI_2.jpg','img/cities/DLI_3.jpg'], moTa:'Hồ Xuân Hương · Đồi chè · Ga cổ'},
  HAN:{ten:'Hà Nội', anh:['img/cities/HAN_1.jpg','img/cities/HAN_2.jpg','img/cities/HAN_3.jpg'], moTa:'Hồ Gươm · Văn Miếu · Phố cổ'},
  HPH:{ten:'Hải Phòng', anh:['img/cities/HPH_1.jpg','img/cities/HPH_2.jpg','img/cities/HPH_3.jpg'], moTa:'Đồ Sơn · Cát Bà · Nhà hát lớn'},
  HUI:{ten:'Huế', anh:['img/cities/HUI_1.jpg','img/cities/HUI_2.jpg','img/cities/HUI_3.jpg'], moTa:'Đại Nội · Chùa Thiên Mụ · Sông Hương'},
  PHA:{ten:'Phan Rang', anh:['img/cities/PHA_1.jpg','img/cities/PHA_2.jpg','img/cities/PHA_3.jpg'], moTa:'Ninh Chữ · Tháp Chàm'},
  PQC:{ten:'Phú Quốc', anh:['img/cities/PQC_1.jpg','img/cities/PQC_2.jpg','img/cities/PQC_3.jpg'], moTa:'Bãi Sao · Sunset Sanato · Grand World'},
  PXU:{ten:'Pleiku', anh:['img/cities/PXU_1.jpg','img/cities/PXU_2.jpg','img/cities/PXU_3.jpg'], moTa:'Biển Hồ · Tây Nguyên'},
  SGN:{ten:'TP. Hồ Chí Minh', anh:['img/cities/SGN_1.jpg','img/cities/SGN_2.jpg','img/cities/SGN_3.jpg'], moTa:'Bitexco · Nhà thờ Đức Bà · Bến Thành'},
  SQH:{ten:'Sơn La', anh:['img/cities/SQH_1.jpg','img/cities/SQH_2.jpg','img/cities/SQH_3.jpg'], moTa:'Na San · Tây Bắc'},
  TBB:{ten:'Tuy Hòa', anh:['img/cities/TBB_1.jpg','img/cities/TBB_2.jpg','img/cities/TBB_3.jpg'], moTa:'Tháp Nhạn · Biển'},
  THD:{ten:'Thanh Hóa', anh:['img/cities/THD_1.jpg','img/cities/THD_2.jpg','img/cities/THD_3.jpg'], moTa:'Sầm Sơn · Thành nhà Hồ'},
  UIH:{ten:'Quy Nhơn', anh:['img/cities/UIH_1.jpg','img/cities/UIH_2.jpg','img/cities/UIH_3.jpg'], moTa:'Eo Gió · Kỳ Co · Ghềnh Ráng'},
  VCA:{ten:'Cần Thơ', anh:['img/cities/VCA_1.jpg','img/cities/VCA_2.jpg','img/cities/VCA_3.jpg'], moTa:'Chợ nổi Cái Răng · Ninh Kiều'},
  VCL:{ten:'Chu Lai', anh:['img/cities/VCL_1.jpg','img/cities/VCL_2.jpg','img/cities/VCL_3.jpg'], moTa:'Biển Quảng Nam · Núi Thành'},
  VCS:{ten:'Côn Đảo', anh:['img/cities/VCS_1.jpg','img/cities/VCS_2.jpg','img/cities/VCS_3.jpg'], moTa:'Nhà tù · Bãi An Hải'},
  VDH:{ten:'Đồng Hới', anh:['img/cities/VDH_1.jpg','img/cities/VDH_2.jpg','img/cities/VDH_3.jpg'], moTa:'Phong Nha · Nhật Lệ'},
  VDO:{ten:'Vân Đồn', anh:['img/cities/VDO_1.jpg','img/cities/VDO_2.jpg','img/cities/VDO_3.jpg'], moTa:'Vịnh Hạ Long · Bãi dài'},
  VII:{ten:'Vinh', anh:['img/cities/VII_1.jpg','img/cities/VII_2.jpg','img/cities/VII_3.jpg'], moTa:'Cửa Lò · Quê Bác · Quảng trường'},
  VKG:{ten:'Rạch Giá', anh:['img/cities/VKG_1.jpg','img/cities/VKG_2.jpg','img/cities/VKG_3.jpg'], moTa:'Biển Kiên Giang · Hà Tiên'},
};
function destGalleryHTML(maDen, maDi){
  const info = CITY_DEST[maDen] || {ten: maDen, anh:[], moTa:'Điểm đến nội địa Việt Nam'};
  const diInfo = CITY_DEST[maDi];
  let imgs = (info.anh||[]).slice(0,3);
  while(imgs.length>0 && imgs.length<2) imgs.push(imgs[0]);
  if(!imgs.length){
    return `<div class="dest-gallery" id="destGallery"><div class="dest-card dest-card-empty"><div class="dest-body">
      <span class="dest-badge">MỘT CHIỀU</span>
      <h3>${esc(diInfo?diInfo.ten:maDi)} – ${esc(info.ten||maDen)}</h3>
      <p class="mut">${esc(info.moTa||'')}</p></div></div></div>`;
  }
  return `<div class="dest-gallery" id="destGallery">
    ${imgs.map((src,i)=>`
      <div class="dest-card">
        <div class="dest-img-wrap">
          <img src="${src}" alt="${esc(info.ten)}" loading="lazy"
            onerror="this.onerror=null;this.src='img/cities/DAD_2.jpg'"/>
          <span class="dest-badge">MỘT CHIỀU</span>
          <span class="dest-tag">Giá tốt chỉ từ</span>
        </div>
        <div class="dest-body">
          <h3>${esc(diInfo?diInfo.ten:(maDi||''))} – ${esc(info.ten||maDen)}</h3>
          <p class="mut">${esc(info.moTa||'')}</p>
        </div>
      </div>`).join('')}
  </div>`;
}

const esc=s=>String(s||'').replace(/[&<>"']/g,c=>({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&#39;'}[c]));
const vnd=n=>(Number(n)||0).toLocaleString('vi-VN')+' ₫';
function isHostMode(){
  try {
    // Tùy chọn: chỉ trên máy bạn có thể gõ ?host=DT04 một lần để bật — link gửi người khác KHÔNG cần/không nên có
    const q = new URLSearchParams(location.search);
    if (q.get('host') === 'DT04') localStorage.setItem('asm_host_mode', '1');
    if (q.get('host') === '0') localStorage.removeItem('asm_host_mode');
  } catch(e) {}
  return localStorage.getItem('asm_host_mode') === '1';
}
function batMayChu(){
  const mk = prompt('Nhập mã máy chủ (chỉ trên máy của bạn):');
  if (mk === 'DT04HOST') {
    localStorage.setItem('asm_host_mode', '1');
    toast('Đã bật chế độ máy chủ trên trình duyệt này');
    setNav(); setWho(); go('home');
  } else if (mk !== null) {
    toast('Sai mã máy chủ', false);
  }
}
function tatMayChu(){
  localStorage.removeItem('asm_host_mode');
  toast('Đã tắt chế độ máy chủ');
  setNav(); setWho(); go('home');
}
const toast=(m,ok=true)=>{const t=$('#toast');t.textContent=m;t.className='show'+(ok?'':' bad');setTimeout(()=>t.className='',3200);};
function pwdField(name, id, extra=''){
  return `<div class="pwd-wrap"><input name="${name}" id="${id}" type="password" ${extra}><button type="button" class="pwd-toggle" data-for="${id}" aria-label="Hiện mật khẩu">👁</button></div>`;
}
function bindPwdToggles(root){
  (root||document).querySelectorAll('.pwd-toggle').forEach(btn=>{
    btn.onclick = () => {
      const inp = document.getElementById(btn.dataset.for);
      if(!inp) return;
      const show = inp.type === 'password';
      inp.type = show ? 'text' : 'password';
      btn.textContent = show ? '🙈' : '👁';
      btn.setAttribute('aria-label', show ? 'Ẩn mật khẩu' : 'Hiện mật khẩu');
    };
  });
}
function ngayISO(offset=0){
  const d=new Date(); d.setHours(0,0,0,0); d.setDate(d.getDate()+offset);
  const p=n=>String(n).padStart(2,'0');
  return `${d.getFullYear()}-${p(d.getMonth()+1)}-${p(d.getDate())}`;
}
function thuVN(d){
  return ['CN','Th 2','Th 3','Th 4','Th 5','Th 6','Th 7'][d.getDay()];
}
function formatNgayVN(iso){
  const [y,m,d]=iso.split('-').map(Number);
  const dt=new Date(y,m-1,d);
  return `${thuVN(dt)} ${String(d).padStart(2,'0')}/${String(m).padStart(2,'0')}`;
}
function giaThapNhat(di,den,ngayIso){
  const list=S.flights.filter(c=>c.di===di&&c.den===den);
  if(!list.length) return 0;
  return Math.min(...list.map(c=>c.gia||0));
}


const store={get:(k,d)=>{try{return JSON.parse(localStorage.getItem(k))??d;}catch(e){return d;}}, set:(k,v)=>localStorage.setItem(k,JSON.stringify(v))};

/** Gộp mảng object theo khóa (ưu tiên bản local nếu trùng) */
function mergeByKey(remote, local, key){
  const map = new Map();
  (remote||[]).forEach(x => { if(x && x[key]!=null) map.set(String(x[key]), x); });
  (local||[]).forEach(x => { if(x && x[key]!=null) map.set(String(x[key]), x); });
  return [...map.values()];
}
function mergeActivity(remote, local){
  const seen = new Set();
  const out = [];
  [...(local||[]), ...(remote||[])].forEach(x => {
    const k = (x && (x.id || (String(x.at)+'|'+String(x.hanhDong||x.action||'')+'|'+String(x.chiTiet||x.detail||''))));
    if(!k || seen.has(k)) return;
    seen.add(k); out.push(x);
  });
  return out.slice(0, 300);
}


/* ===== Đồng bộ chuyến bay đa thiết bị (Vercel /api/sync + polling) ===== */
const SYNC = {
  secret: 'SkyportSync2024',
  lastVersion: 0,
  lastEvents: new Set(),
  timer: null,
  configured: null,
};
async function pushSharedFlights(eventNote){
  try {
    // Kéo cloud trước rồi gộp — tránh máy này đè mất dữ liệu máy kia
    try {
      const r0 = await fetch('/api/sync', { cache: 'no-store' });
      if (r0.ok) {
        const j0 = await r0.json();
        if (j0 && (j0.ok || j0.version != null)) {
          if (Array.isArray(j0.ves)) store.set('ves', mergeByKey(j0.ves, store.get('ves', []), 'ma'));
          if (Array.isArray(j0.users_extra)) {
            const m = mergeByKey(j0.users_extra, store.get('users_extra', []), 'ma');
            store.set('users_extra', m);
            store.set('hk_users', m);
          }
          if (Array.isArray(j0.activity_log)) store.set('activity_log', mergeActivity(j0.activity_log, store.get('activity_log', [])));
          if (j0.hh_map && typeof j0.hh_map === 'object') {
            store.set('hh_map', Object.assign({}, j0.hh_map, store.get('hh_map', {})));
          }
          if (j0.flight_status && typeof j0.flight_status === 'object') {
            store.set('flight_status', Object.assign({}, j0.flight_status, store.get('flight_status', {})));
          }
          if (Array.isArray(j0.extra_flights)) store.set('extra_flights', mergeByKey(j0.extra_flights, store.get('extra_flights', []), 'ma'));
        }
      }
    } catch (e) {}
    const users_extra = store.get('users_extra', store.get('hk_users', []));
    const payload = {
      flight_status: store.get('flight_status', {}),
      extra_flights: store.get('extra_flights', []),
      ves: store.get('ves', []),
      users_extra,
      activity_log: store.get('activity_log', []).slice(0, 200),
      hh_map: store.get('hh_map', {}),
      airports: store.get('airports', null),
      planes: store.get('planes', null),
      events: eventNote ? [{ id: Date.now()+'-'+Math.random().toString(36).slice(2,7), at: Date.now(), text: eventNote }] : [],
    };
    if (!Array.isArray(payload.airports)) delete payload.airports;
    if (!Array.isArray(payload.planes)) delete payload.planes;
    const r = await fetch('/api/sync', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json', 'x-skyport-secret': SYNC.secret },
      body: JSON.stringify(payload),
      cache: 'no-store',
    });
    const j = await r.json().catch(()=>({}));
    if (r.ok && j.ok) {
      SYNC.lastVersion = j.version || SYNC.lastVersion;
      SYNC.configured = true;
      return true;
    }
    SYNC.configured = false;
    console.warn('[Skyport sync push]', j.error || r.status);
    return false;
  } catch (e) {
    SYNC.configured = false;
    console.warn('[Skyport sync push]', e);
    return false;
  }
}
function applySharedState(data, notify){
  if (!data) return false;
  const ver = data.version || 0;
  if (ver && ver <= SYNC.lastVersion) return false;
  const prevStatus = store.get('flight_status', {});
  if (data.flight_status && typeof data.flight_status === 'object') {
    store.set('flight_status', Object.assign({}, store.get('flight_status', {}), data.flight_status));
  }
  if (Array.isArray(data.extra_flights)) {
    store.set('extra_flights', mergeByKey(data.extra_flights, store.get('extra_flights', []), 'ma'));
  }
  if (Array.isArray(data.ves)) {
    store.set('ves', mergeByKey(data.ves, store.get('ves', []), 'ma'));
  }
  if (Array.isArray(data.users_extra)) {
    const m = mergeByKey(data.users_extra, store.get('users_extra', store.get('hk_users', [])), 'ma');
    store.set('users_extra', m);
    store.set('hk_users', m);
    // cập nhật S.users nếu đã load
    if (Array.isArray(S.users) && S.users.length) {
      const base = S.users.filter(u => u.vai !== 'Hanh khach' || !(m.some(x => x.ma === u.ma)));
      // giữ admin/daily + mọi HK từ file + cloud
      const byMa = new Map(S.users.map(u => [u.ma, u]));
      m.forEach(u => byMa.set(u.ma, { ...byMa.get(u.ma), ...u }));
      S.users = [...byMa.values()];
    }
  }
  if (Array.isArray(data.activity_log)) {
    store.set('activity_log', mergeActivity(data.activity_log, store.get('activity_log', [])));
  }
  if (data.hh_map && typeof data.hh_map === 'object') {
    store.set('hh_map', Object.assign({}, store.get('hh_map', {}), data.hh_map));
  }
  if (Array.isArray(data.airports) && data.airports.length) {
    store.set('airports', data.airports);
    S.airports = data.airports;
  }
  if (Array.isArray(data.planes) && data.planes.length) {
    store.set('planes', data.planes);
    S.planes = data.planes;
  }
  // merge into memory
  const stMap = store.get('flight_status', {});
  const base = (S.flights || []).filter(f => !(store.get('extra_flights', []).some(e => e.ma === f.ma)));
  // reload flights list similar to load()
  const extra = store.get('extra_flights', []);
  const fromExtra = extra.map(f => ({
    ...f,
    gioDi: typeof hm==='function' ? hm(f.slot||0,0) : f.gioDi,
    gioDen: typeof hm==='function' ? hm(f.slot||0,f.dur||75) : f.gioDen,
    status: (stMap[f.ma] && stMap[f.ma].status) || f.status || 'BINH_THUONG',
  }));
  // keep original static flights, update status
  // không gán status global theo mã — trạng thái theo ngày khi hiển thị
  S.flights.forEach(f => { delete f.status; });
  // add any extra not present
  fromExtra.forEach(ef => {
    const i = S.flights.findIndex(x => x.ma === ef.ma);
    if (i >= 0) S.flights[i] = { ...S.flights[i], ...ef };
    else S.flights.push(ef);
  });
  if (typeof taoThongBaoChuyen === 'function') taoThongBaoChuyen();
  SYNC.lastVersion = ver || SYNC.lastVersion;
  // notifications for status changes
  if (notify) {
    const cur = store.get('flight_status', {});
    Object.keys(cur).forEach(k => {
      const a = cur[k], b = prevStatus[k];
      const key = k + ':' + (a && a.status) + ':' + (a && a.at);
      if (a && a.status && a.status!=='BINH_THUONG' && (!b || b.status !== a.status || b.at !== a.at) && !SYNC.lastEvents.has(key)) {
        SYNC.lastEvents.add(key);
        if (SYNC.lastEvents.size > 80) {
          const arr = [...SYNC.lastEvents];
          SYNC.lastEvents = new Set(arr.slice(-40));
        }
        const label = (typeof ttLabel === 'function') ? ttLabel(a.status) : a.status;
        const ma = a.ma || k.split('|')[0];
        const day = a.ngay || (k.includes('|')?k.split('|')[1]:'');
        if (typeof toast === 'function') toast('Chuyến ' + ma + (day?(' ('+day+')'):'') + ': ' + label + (a.note ? (' — ' + a.note) : ''), true);
      }
    });
    // Làm mới banner / trang hiện tại để máy khác thấy trễ/hoãn ngay
    try {
      if (notify && S.user && typeof go === 'function' && S.page) {
        setTimeout(() => { try { go(S.page || 'home'); } catch (e) {} }, 80);
      }
    } catch (e) {}
  }
  return true;
}
async function pullSharedFlights(notify){
  try {
    const r = await fetch('/api/sync', { cache: 'no-store' });
    if (!r.ok) return false;
    const j = await r.json();
    SYNC.configured = !!j.configured;
    if (j.ok || j.version != null) return !!applySharedState(j, notify);
    return false;
  } catch (e) { return false; }
}
function startFlightSync(){
  if (SYNC.timer) return;
  pullSharedFlights(false);
  SYNC.timer = setInterval(() => {
    if (document.hidden) return;
    pullSharedFlights(true);
  }, 5000);
  document.addEventListener('visibilitychange', () => {
    if (!document.hidden) pullSharedFlights(true);
  });
}


async function sha256(text){const data=new TextEncoder().encode(String(text));const buf=await crypto.subtle.digest('SHA-256',data);return[...new Uint8Array(buf)].map(b=>b.toString(16).padStart(2,'0')).join('');}
async function matKhauKhop(plain,user){if(!user)return false;if(user.mkHash)return(await sha256(plain))===user.mkHash;if(user.mk!=null)return plain===user.mk;return false;}
function tokenPhien(){const a=new Uint8Array(16);crypto.getRandomValues(a);return[...a].map(b=>b.toString(16).padStart(2,'0')).join('');}
function logAct(hanhDong, chiTiet=''){
  const logs=store.get('activity_log',[]);
  logs.unshift({at:Date.now(), id:Date.now()+'-'+Math.random().toString(36).slice(2,6), user:(S.user&&S.user.ma)||'', vai:(S.user&&S.user.vai)||'', hanhDong, chiTiet});
  store.set('activity_log', logs.slice(0,200));
  // Đồng bộ cloud (gộp user/vé/lịch sử) — debounce tránh spam Blob
  clearTimeout(SYNC._pushTimer);
  SYNC._pushTimer = setTimeout(() => { pushSharedFlights(hanhDong + (chiTiet?(' · '+chiTiet):'')); }, 800);
}
function hangTen(h){return h==='TG'?'Thương gia':h==='TK'?'Tiết kiệm':'Phổ thông';}
function vaiTen(v){return v==='Dai ly'?'Đại lý':v==='Hanh khach'?'Hành khách':v==='Admin'?'Quản trị viên':v||'';}
function isAdmin(){return S.user&&S.user.vai==='Admin';}
function isDL(){return S.user&&S.user.vai==='Dai ly';}
function isHK(){return S.user&&S.user.vai==='Hanh khach';}

function hm(slot,add=0,ngayIso){
  const s=SLOT[slot%SLOT.length];
  let base;
  if(ngayIso){
    const [y,mo,da]=ngayIso.split('-').map(Number);
    base=new Date(y,mo-1,da,0,0,0,0);
  } else {
    base=new Date(); base.setHours(0,0,0,0);
  }
  base.setMinutes(s[0]*60+s[1]+add);
  if(!ngayIso){
    const now=new Date();
    if(base<now) base.setDate(base.getDate()+1);
  }
  const p=n=>String(n).padStart(2,'0');
  return `${p(base.getHours())}:${p(base.getMinutes())} ${p(base.getDate())}/${p(base.getMonth()+1)}`;
}
function ganGioTheoNgay(list, ngayIso){
  return list.map(f=>({...f, gioDi:hm(f.slot||0,0,ngayIso), gioDen:hm(f.slot||0,f.dur||75,ngayIso)}));
}


function parseGioVN(s){
  // "12:30 25/10" or "12:30 25/10/2026"
  if(!s) return null;
  const m = String(s).trim().match(/(\d{1,2}):(\d{2})\s+(\d{1,2})\/(\d{1,2})(?:\/(\d{4}))?/);
  if(!m) return null;
  const y = m[5] ? +m[5] : new Date().getFullYear();
  return new Date(y, +m[4]-1, +m[3], +m[1], +m[2], 0, 0);
}
function phutCho(den1, di2){
  const a = parseGioVN(den1), b = parseGioVN(di2);
  if(!a || !b) return -1;
  return (b - a) / 60000;
}
function shiftGioSangNgaySau(gioStr){
  const m = String(gioStr).match(/(\d{1,2}):(\d{2})\s+(\d{1,2})\/(\d{1,2})/);
  if(!m) return gioStr;
  const d = new Date(new Date().getFullYear(), +m[4]-1, +m[3], +m[1], +m[2]);
  d.setDate(d.getDate()+1);
  const p=n=>String(n).padStart(2,'0');
  return `${p(d.getHours())}:${p(d.getMinutes())} ${p(d.getDate())}/${p(d.getMonth()+1)}`;
}

function ttLabel(st){
  const m={BINH_THUONG:'Đúng giờ',TRE:'Trễ (delay)',HOAN:'Hoãn khởi hành',HUY:'Hủy chuyến',DOI_GIO:'Đổi giờ bay'};
  return m[st]||st||'Đúng giờ';
}
/** Khóa trạng thái = mã chuyến + ngày khai thác (YYYY-MM-DD) */
function statusKey(ma, ngay){
  const n = (ngay || (typeof ngayISO==='function'?ngayISO(0):'') || '').slice(0,10);
  return (ma||'') + '|' + n;
}
/** Chuẩn hóa ngày về YYYY-MM-DD (giờ VN, không lệch UTC) */
function normNgay(v){
  if(!v) return ngayISO(0);
  if(typeof v==='string'){
    const s=v.trim();
    if(/^\d{4}-\d{2}-\d{2}/.test(s)) return s.slice(0,10);
    const m=s.match(/^(\d{1,2})[\/\-](\d{1,2})[\/\-](\d{4})$/);
    if(m) return m[3]+'-'+String(+m[2]).padStart(2,'0')+'-'+String(+m[1]).padStart(2,'0');
  }
  try{
    const d=new Date(v);
    if(!isNaN(d)) return d.getFullYear()+'-'+String(d.getMonth()+1).padStart(2,'0')+'-'+String(d.getDate()).padStart(2,'0');
  }catch(e){}
  return ngayISO(0);
}
/** Xóa mọi khóa trạng thái cũ chỉ theo mã (không có ngày) — chặn lan sang mọi ngày */
function purgeLegacyStatusKeys(){
  const map=store.get('flight_status',{});
  let changed=false;
  Object.keys(map).forEach(k=>{
    if(!String(k).includes('|')){ delete map[k]; changed=true; }
  });
  if(changed) store.set('flight_status', map);
}
function getFlightStatus(ma, ngay){
  purgeLegacyStatusKeys();
  const map=store.get('flight_status',{});
  if(!ngay) return 'BINH_THUONG'; // bắt buộc có ngày — không fallback mã chung
  const k=statusKey(ma, normNgay(ngay));
  return (map[k] && map[k].status) || 'BINH_THUONG';
}
function getFlightStatusMeta(ma, ngay){
  purgeLegacyStatusKeys();
  const map=store.get('flight_status',{});
  if(!ngay) return null;
  return map[statusKey(ma, normNgay(ngay))] || null;
}
function setFlightStatus(ma, status, note, ngay){
  const n = normNgay(ngay || S.adminNgay || ngayISO(0));
  const map=store.get('flight_status',{});
  // luôn xóa khóa legacy theo mã
  if(map[ma]) delete map[ma];
  const k=statusKey(ma, n);
  if(status==='BINH_THUONG'){
    delete map[k];
  } else {
    map[k]={status, note:note||'', at:Date.now(), ma, ngay:n};
  }
  store.set('flight_status', map);
  taoThongBaoChuyen(n);
  pushSharedFlights(ma + ' @ ' + n + ' → ' + status + (note ? (' · ' + note) : ''));
}
function taoThongBaoChuyen(focusNgay){
  purgeLegacyStatusKeys();
  const alerts=[];
  const map=store.get('flight_status',{});
  const ngay = normNgay(focusNgay || S.ngayChon || S.adminNgay || ngayISO(0));
  Object.keys(map).forEach(k=>{
    if(!String(k).includes('|')) return; // bỏ key cũ
    const rec=map[k];
    if(!rec || !rec.status || rec.status==='BINH_THUONG') return;
    const ma = rec.ma || k.split('|')[0];
    const day = normNgay(rec.ngay || k.split('|')[1] || '');
    if(day !== ngay) return; // chỉ ngày đang xem
    const f=S.flights.find(x=>x.ma===ma);
    const tuyen = f ? (f.di+' → '+f.den) : ma;
    const gio = f ? hm(f.slot||0,0,day) : '';
    alerts.push({ma, tuyen, tt:ttLabel(rec.status)+(rec.note?(' · '+rec.note):'')+' · '+day, gio, status:rec.status, ngay:day});
  });
  S.alerts=alerts.slice(0,12);
}
function slotToMin(slot){
  const s=SLOT[slot%SLOT.length]||[6,0];
  return s[0]*60+s[1];
}
function checkMayBayConflict(mb, slot, dur, excludeMa){
  const start=slotToMin(+slot||0);
  const end=start+(+dur||90);
  for(const f of S.flights){
    if(excludeMa && f.ma===excludeMa) continue;
    if((f.mb||'')!==mb) continue;
    if((f.status||getFlightStatus(f.ma))==='HUY') continue;
    const fs=slotToMin(f.slot||0);
    const fe=fs+(f.dur||90);
    if(start < fe && end > fs) return f;
  }
  return null;
}
function phiHoanTheoHang(hang, paid, gia){
  // Tiết kiệm: không hoàn / phí 100%; Phổ thông 30%; Thương gia 10%
  if(!paid) return {phi:0, hoan:0, msg:'Vé chưa thanh toán — hủy miễn phí, không hoàn tiền.'};
  const g=gia||0;
  if(hang==='TK') return {phi:g, hoan:0, msg:'Hạng Tiết kiệm: không hoàn tiền sau thanh toán (phí hủy 100%).'};
  if(hang==='TG') return {phi:Math.round(g*0.1), hoan:Math.round(g*0.9), msg:'Hạng Thương gia: phí hủy 10%, hoàn 90%.'};
  return {phi:Math.round(g*0.3), hoan:Math.round(g*0.7), msg:'Hạng Phổ thông: phí hủy 30%, hoàn 70%.'};
}
function xuLyVeKhiHuyChuyen(maCB){
  let all=store.get('ves',[]);
  let n=0;
  all=all.map(v=>{
    if(v.cb!==maCB && !(v.legs&&v.legs.includes(maCB))) return v;
    if(v.huyChuyen) return v;
    const r=phiHoanTheoHang(v.hang, v.paid, v.gia);
    n++;
    return {...v, huyChuyen:true, refund:r.hoan, refundNote:r.msg, statusVe:'HUY_CHUYEN'};
  });
  store.set('ves', all);
  return n;
}
function exportCSV(filename, rows){
  const bom='\uFEFF';
  const csv=rows.map(r=>r.map(c=>{
    const s=String(c??'');
    return /[",\n]/.test(s)?'"'+s.replace(/"/g,'""')+'"':s;
  }).join(',')).join('\n');
  const blob=new Blob([bom+csv],{type:'text/csv;charset=utf-8'});
  const a=document.createElement('a');
  a.href=URL.createObjectURL(blob);
  a.download=filename;
  a.click();
  setTimeout(()=>URL.revokeObjectURL(a.href),500);
}
function boardingPassHTML(v){
  const qrData=encodeURIComponent([v.ma,v.cb,v.ghe,v.tuyen,v.hang].join('|'));
  const qr='https://api.qrserver.com/v1/create-qr-code/?size=140x140&data='+qrData;
  return `<div class="card" id="bpCard" style="max-width:420px">
    <h2>Thẻ lên máy bay</h2>
    <p><b>Mã vé:</b> ${esc(v.ma)}<br>
    <b>Chuyến:</b> ${esc(v.cb)} · ${esc(v.tuyen)}<br>
    <b>Ghế:</b> ${esc(v.ghe)} · <b>Hạng:</b> ${esc(hangTen(v.hang))}<br>
    <b>Hành khách:</b> ${esc(v.tenHK||v.kh||S.user&&S.user.ten||'')}<br>
    <b>Loại:</b> ${esc(v.loaiHK==='TE'?'Trẻ em':'Người lớn')}</p>
    <p style="text-align:center"><img src="${qr}" alt="QR" width="140" height="140" style="border-radius:8px;background:#fff;padding:6px"></p>
    <p class="mut">Quét mã QR tại quầy / cổng lên máy bay (demo).</p>
    <button type="button" class="b s no-print" onclick="printBoardingPass()">In thẻ</button>
  </div>`;
}
/** Chỉ in khối thẻ lên máy bay (#bpCard), không in cả trang web */
function printBoardingPass(){
  const card = document.getElementById('bpCard');
  if(!card){ try{ toast('Không tìm thấy thẻ để in', false); }catch(e){} return; }
  const clone = card.cloneNode(true);
  clone.querySelectorAll('button, .no-print').forEach(b => b.remove());
  const w = window.open('', '_blank', 'noopener,noreferrer,width=520,height=720');
  if(!w){
    // popup bị chặn: fallback ẩn tạm phần còn lại rồi print
    document.body.classList.add('print-bp-only');
    window.print();
    document.body.classList.remove('print-bp-only');
    return;
  }
  w.document.open();
  w.document.write('<!DOCTYPE html><html lang="vi"><head><meta charset="utf-8"><title>Thẻ lên máy bay</title>');
  w.document.write('<style>');
  w.document.write('*{box-sizing:border-box}body{font-family:system-ui,-apple-system,Segoe UI,Roboto,sans-serif;margin:0;padding:24px;color:#0f172a;background:#fff}');
  w.document.write('.card{max-width:420px;margin:0 auto;border:1px solid #e2e8f0;border-radius:16px;padding:20px 22px;box-shadow:none}');
  w.document.write('h2{margin:0 0 12px;font-size:1.25rem}p{margin:0 0 10px;line-height:1.5}');
  w.document.write('img{display:block;margin:8px auto;border-radius:8px}');
  w.document.write('.mut{color:#64748b;font-size:.9rem}');
  w.document.write('@page{margin:12mm}');
  w.document.write('</style></head><body>');
  w.document.write(clone.outerHTML);
  w.document.write('</body></html>');
  w.document.close();
  const trigger = () => { try{ w.focus(); w.print(); }catch(e){} };
  // đợi ảnh QR load
  const img = w.document.querySelector('img');
  if(img && !img.complete) img.onload = trigger;
  else setTimeout(trigger, 250);
}


async function load(){
  const [sb,cb,mb,hg,us,me]=await Promise.all([
    fetch('data/sanbay.json').then(r=>r.json()),
    fetch('data/chuyenbay.json').then(r=>r.json()),
    fetch('data/maybay.json').then(r=>r.json()),
    fetch('data/hang.json').then(r=>r.json()),
    fetch('data/users.json').then(r=>r.json()),
    fetch('data/meals.json').then(r=>r.json())
  ]);
  S.airports=store.get('airports',sb);
  S.planes=store.get('planes',mb);
  S.airlines=hg; S.meals=me;
  const extraF=store.get('extra_flights',[]);
  S.flights=[...cb,...extraF].map(f=>({...f,gioDi:hm(f.slot||0,0),gioDen:hm(f.slot||0,f.dur||75)}));
  S.users=(function(){const extra=mergeByKey(store.get('hk_users',[]),store.get('users_extra',[]),'ma');const map=new Map();us.forEach(u=>map.set(u.ma,u));extra.forEach(u=>map.set(u.ma,{...map.get(u.ma),...u}));return[...map.values()];})();
  const hhMap=store.get('hh_map',{});
  S.users.forEach(u=>{if(hhMap[u.ma]!=null)u.hhPhatSinh=hhMap[u.ma];});
  S.user=store.get('session',null);
  const stMap=store.get('flight_status',{});
  purgeLegacyStatusKeys();
  taoThongBaoChuyen(S.ngayChon||ngayISO(0));
  startFlightSync();
}

function apOpts(v){return S.airports.map(a=>`<option value="${esc(a.maSB)}" ${v===a.maSB?'selected':''}>${esc(a.maSB)} — ${esc(a.thanhPho||a.ten)}</option>`).join('');}
function need(roles){if(!S.user){go('home');return false;}if(roles&&!roles.includes(S.user.vai)){toast('Không đủ quyền',false);return false;}return true;}

function bannerCanhBao(){
  if(!S.alerts||!S.alerts.length) return '';
  return `<div class="card" style="border-left:4px solid #f59e0b;background:linear-gradient(90deg,#fffbeb,#fff)">
    <h2 style="font-size:1rem">⚠ Thông báo chuyến bay (trễ / hoãn / đổi giờ / hết chỗ)</h2>
    <ul style="margin:8px 0 0;padding-left:18px">${S.alerts.map(a=>`<li><b>${esc(a.ma)}</b> ${esc(a.tuyen)} · ${esc(a.gio)} — <span style="color:#b45309">${esc(a.tt)}</span></li>`).join('')}</ul>
  </div>`;
}


function setWho(){
  const el = $('#who');
  if(!el) return;
  if(!S.user){
    el.innerHTML = `<div class="header-auth">
      <button type="button" class="b" id="hdrLogin">Đăng nhập</button>
      <button type="button" class="b g" id="hdrReg">Đăng ký</button>
    </div>`;
    const li=$('#hdrLogin'), re=$('#hdrReg');
    if(li) li.onclick = () => { S.authMode = 'login'; closeAuthModal(); go('home'); };
    if(re) re.onclick = () => { S.authMode = 'register'; closeAuthModal(); go('home'); };
    return;
  }
  const role = isAdmin() ? 'Quản trị viên' : (isDL() ? 'Đại lý' : 'Hành khách');
  el.innerHTML = `<div class="user-box">
    <span class="chip"><b>${esc(S.user.ten)}</b> · ${role}</span>
    <button type="button" id="btnOut" class="b">Đăng xuất</button>
  </div>`;
  const b = $('#btnOut');
  if(b) b.onclick = () => {
    logAct('Đăng xuất');
    S.user = null;
    store.set('session', null);
    setWho();
    setNav();
    go('home');
    toast('Đã đăng xuất');
  };
}

function setNav(){
  const u=S.user;
  const nav=$('#nav');
  let items=[['home','Trang chủ'],['km','Ưu đãi'],['lienhe','Liên hệ']];
  if(u){
    if(isAdmin()){
      items=[['tim','Chuyến bay'],['noi','Nối chuyến'],['tracu','Tra cứu mã'],['tk','Thống kê'],['qly','Quản lý hệ thống'],['daily','Quản lý đại lý'],['km','Ưu đãi'],['lienhe','Liên hệ'],['log','Lịch sử']];
    } else if(isDL()){
      items=[['tim','Chuyến bay'],['noi','Nối chuyến'],['tracu','Tra cứu mã'],['datdl','Đặt vé hộ khách'],['ve','Vé đã bán hộ'],['hh','Hoa hồng'],['tk','Thống kê'],['km','Ưu đãi'],['lienhe','Liên hệ'],['log','Lịch sử']];
    } else {
      items=[['tim','Chuyến bay'],['noi','Nối chuyến'],['tracu','Tra cứu mã'],['dat','Đặt vé'],['ve','Vé của tôi'],['ss','So sánh đại lý'],['tk','Thống kê'],['km','Ưu đãi'],['lienhe','Liên hệ'],['log','Lịch sử']];
    }
    nav.innerHTML=items.map(([k,l])=>`<button type="button" data-t="${k}">${l}</button>`).join('');
    $$('#nav [data-t]').forEach(b=>b.onclick=()=>go(b.dataset.t));
  } else {
    // Khách thường: chỉ Trang chủ | Ưu đãi | Liên hệ
    // Máy chủ (?host=DT04): thêm Quản trị viên + Đại lý
    const host = isHostMode();
    const dls = (S.users||[]).filter(x=>x.vai==='Dai ly');
    let hostNav = '';
    if (host) {
      hostNav = `
      <button type="button" id="navAdmin" class="nav-acc">Quản trị viên</button>
      <div class="nav-dd" id="navDL">
        <button type="button" class="nav-acc nav-dd-btn" id="navDLBtn">Tài khoản đại lý ▾</button>
        <div class="nav-dd-menu" id="navDLMenu" hidden>
          ${dls.map(d=>`<button type="button" class="nav-dd-item" data-ma="${esc(d.ma)}" data-mk="${esc(d.mkMau||'')}">${esc(d.ten)} <small>${esc(d.ma)}</small></button>`).join('')}
        </div>
      </div>`;
    }
    nav.innerHTML = `
      <button type="button" data-t="home">Trang chủ</button>
      ${hostNav}
      <button type="button" data-t="km">Ưu đãi</button>
      <button type="button" data-t="lienhe">Liên hệ</button>`;
    $$('#nav [data-t]').forEach(b=>b.onclick=()=>go(b.dataset.t));
    const btnAd = $('#navAdmin');
    if(btnAd) btnAd.onclick = () => quickLogin('AD01');
    const ddBtn = $('#navDLBtn');
    const ddMenu = $('#navDLMenu');
    if(ddBtn && ddMenu){
      ddBtn.onclick = (e) => {
        e.stopPropagation();
        ddMenu.hidden = !ddMenu.hidden;
      };
      $$('#navDLMenu .nav-dd-item').forEach(it => {
        it.onclick = (e) => {
          e.stopPropagation();
          ddMenu.hidden = true;
          quickLogin(it.dataset.ma);
        };
      });
      if(!window._navDlClose){
        window._navDlClose = true;
        document.addEventListener('click', (e) => {
          const box = document.getElementById('navDL');
          const menu = document.getElementById('navDLMenu');
          if(menu && box && !box.contains(e.target)) menu.hidden = true;
        });
      }
    }
  }
  setWho();
}
async function quickLogin(ma){
  if(!isHostMode()){ toast('Chỉ máy chủ mới dùng đăng nhập nhanh', false); return; }
  const u = (S.users||[]).find(x => x.ma === ma);
  if(!u){ toast('Không tìm thấy tài khoản', false); return; }
  const mk = u.mkMau || u.mk || '';
  if(u.mkHash && mk){
    const h = await sha256(mk);
    if(h !== u.mkHash){ toast('Mật khẩu mẫu không khớp', false); return; }
  }
  S.user = { ma:u.ma, ten:u.ten, vai:u.vai, email:u.email||'', tyle:u.tyle };
  store.set('session', S.user);
  logAct('Đăng nhập nhanh', u.ma);
  toast('Xin chào, ' + u.ten);
  setNav();
  go('tim');
}


function go(t){
  $$('#nav button[data-t]').forEach(b=>b.classList.toggle('on',b.dataset.t===t));
  if(t==='out'){logAct('Đăng xuất');S.user=null;store.set('session',null);setNav();return go('home');}
  const map={home:pHome,tim:pTim,noi:pNoi,dat:pDat,ve:pVe,tk:pTK,qly:pQly,daily:pDaily,hh:pHH,ss:pSS,datdl:pDatDL,log:pLog,lienhe:pLienHe,km:pKhuyenMai,tracu:pTraCuu};
  (map[t]||pHome)();
}

(function authModalDelegate(){
  document.addEventListener('click', function(e){
    const t = e.target;
    if(!t) return;
    if(t.id === 'authClose' || (t.closest && t.closest('#authClose'))){
      e.preventDefault(); e.stopPropagation();
      closeAuthModal();
    }
  }, true);
})();
function openAuthModal(tab){
  tab = tab || 'login';
  let bg = $('#authModalBg');
  if(!bg){
    bg = document.createElement('div');
    bg.id = 'authModalBg';
    bg.className = 'auth-modal-bg';
    document.body.appendChild(bg);
  }
  bg.hidden = false;
  bg.innerHTML = `<div class="auth-modal" role="dialog" aria-modal="true">
    <div class="auth-tabs">
      <button type="button" id="tabLogin" class="${tab==='login'?'on':''}">Đăng nhập</button>
      <button type="button" id="tabReg" class="${tab==='register'?'on':''}">Đăng ký</button>
    </div>
    <div id="authBody"></div>
    <p style="margin-top:12px;text-align:center"><button type="button" class="link-btn" id="authClose">Đóng</button></p>
  </div>`;
  bg.style.display = 'flex';
  bg.hidden = false;
  bg.onclick = (e) => { if(e.target===bg) closeAuthModal(); };
  const closeBtn = document.getElementById('authClose');
  if(closeBtn){
    closeBtn.onclick = (e) => { e.preventDefault(); e.stopPropagation(); closeAuthModal(); };
  }
  $('#tabLogin').onclick = () => renderAuthBody('login');
  $('#tabReg').onclick = () => renderAuthBody('register');
  renderAuthBody(tab);
}
function closeAuthModal(){
  const bg = document.getElementById('authModalBg');
  if(bg){ bg.hidden = true; bg.style.display = 'none'; }
}

function renderAuthBody(tab){
  const box = $('#authBody');
  if(!box) return;
  $$('.auth-tabs button').forEach(b => b.classList.toggle('on', (b.id==='tabLogin' && tab==='login') || (b.id==='tabReg' && tab==='register')));
  if(tab==='login'){
    box.innerHTML = `
      <h2>Đăng nhập hệ thống</h2>
      <form id="fl">
        <div class="field"><label>Mã người dùng</label><input name="ma" id="loginMa" required autocomplete="username"></div>
        <div class="field"><label>Mật khẩu</label><div class="pwd-wrap"><input name="mk" id="loginMk" type="password" required autocomplete="current-password"><button type="button" class="pwd-toggle" data-for="loginMk" aria-label="Hiện mật khẩu">👁</button></div></div>
        <label class="check"><input type="checkbox" id="luuTk"> Lưu tài khoản trên máy này</label>
        <button class="b" type="submit" style="width:100%;margin-top:10px">Đăng nhập</button>
      </form>
      <button type="button" class="link-btn" id="btnQuen" style="margin-top:8px">Quên mật khẩu?</button>
      <div id="forgotBox" hidden style="margin-top:12px;padding-top:12px;border-top:1px solid #e2e8f0">
        <h3>Đặt lại mật khẩu</h3>
        <div class="field"><label>Mã người dùng</label><input id="qMa"></div>
        <div class="field"><label>Email đã đăng ký</label><input id="qEmail" type="email"></div>
        <div class="field"><label>Mật khẩu mới</label><input id="qMkMoi" type="password"></div>
        <div class="field"><label>Nhập lại mật khẩu</label><input id="qMkLai" type="password"></div>
        <button type="button" class="b s" id="btnResetMk">Xác nhận đổi</button>
        <p id="qKetQua" class="mut"></p>
      </div>`;
    bindPwdToggles(box);
    const saved = store.get('saved_login', null);
    if(saved && $('#loginMa')){ $('#loginMa').value = saved.ma||''; if($('#luuTk')) $('#luuTk').checked = true; }
    $('#fl').onsubmit = async e => {
      e.preventDefault();
      
      try { await pullSharedFlights(false); const extra=mergeByKey(store.get('hk_users',[]),store.get('users_extra',[]),'ma'); const map=new Map((S.users||[]).map(u=>[u.ma,u])); extra.forEach(u=>map.set(u.ma,{...map.get(u.ma),...u})); S.users=[...map.values()]; } catch(err) {}
      const f = Object.fromEntries(new FormData(e.target));
      const u = S.users.find(x => x.ma === f.ma);
      if(!u) return toast('Tài khoản chưa được đăng ký', false);
      if(u.vai==='Dai ly' && !isHostMode())
        return toast('Tài khoản đại lý chỉ dùng trên máy chủ', false);
      if(!(await matKhauKhop(f.mk, u))) return toast('Sai mật khẩu', false);
      if($('#luuTk') && $('#luuTk').checked) store.set('saved_login', {ma:f.ma});
      else store.set('saved_login', null);
      S.user = { ma:u.ma, ten:u.ten, vai:u.vai, email:u.email||'', tyle:u.tyle };
      store.set('session', S.user);
      if (u.vai === 'Admin' || u.vai === 'Dai ly') localStorage.setItem('asm_host_mode', '1');
      logAct('Đăng nhập'); toast('Xin chào '+u.ten);
      closeAuthModal(); setNav(); setWho(); go('tim');
    };
    if($('#btnQuen')) $('#btnQuen').onclick = () => { const b=$('#forgotBox'); if(b) b.hidden = !b.hidden; };
    if($('#btnResetMk')) $('#btnResetMk').onclick = async () => {
      const ma=$('#qMa').value.trim(), email=$('#qEmail').value.trim(), mk1=$('#qMkMoi').value, mk2=$('#qMkLai').value;
      const u=S.users.find(x=>x.ma===ma);
      if(!u) return $('#qKetQua').textContent='Tài khoản chưa được đăng ký';
      if(u.vai!=='Hanh khach') return $('#qKetQua').textContent='Chỉ hành khách đổi mật khẩu tại đây';
      if((u.email||'').toLowerCase()!==email.toLowerCase()) return $('#qKetQua').textContent='Email không khớp';
      if(mk1.length<3||mk1!==mk2) return $('#qKetQua').textContent='Mật khẩu mới không hợp lệ';
      u.mkHash = await sha256(mk1); delete u.mk;
      store.set('users_extra', S.users.filter(x=>x.vai==='Hanh khach'));
      $('#qKetQua').textContent='Đổi mật khẩu thành công — hãy đăng nhập lại';
      logAct('Đổi mật khẩu', ma);
    };
  } else {
    box.innerHTML = `
      <h2>Đăng ký hành khách</h2>
      <form id="fr">
        <div class="field"><label>Mã (vd HK100)</label><input name="ma" required></div>
        <div class="field"><label>Họ tên</label><input name="ten" required></div>
        <div class="field"><label>CCCD</label><input name="cccd" required></div>
        <div class="field"><label>SĐT</label><input name="sdt" required></div>
        <div class="field"><label>Email</label><input name="email" type="email" required></div>
        <div class="field"><label>Mật khẩu</label><div class="pwd-wrap"><input name="mk" id="regMk" type="password" required><button type="button" class="pwd-toggle" data-for="regMk">👁</button></div></div>
        <button class="b" type="submit" style="width:100%;margin-top:10px">Đăng ký</button>
      </form>`;
    bindPwdToggles(box);
    $('#fr').onsubmit = async e => {
      e.preventDefault();
      const f=Object.fromEntries(new FormData(e.target));
      const loi=[];
      if(S.users.some(x=>x.ma===f.ma)) loi.push('Mã đã tồn tại');
      if(S.users.some(x=>x.email&&x.email.toLowerCase()===f.email.toLowerCase())) loi.push('Email đã được dùng');
      if(S.users.some(x=>x.cccd&&x.cccd===f.cccd)) loi.push('CCCD đã được dùng');
      if(!/^\d{9}$|^\d{12}$/.test(f.cccd||'')) loi.push('CCCD phải 9 hoặc 12 số');
      if(!/^0\d{9,10}$/.test(f.sdt||'')) loi.push('Số điện thoại không hợp lệ');
      if(!/^[^\s@]+@[^\s@]+\.[^\s@]+$/.test(f.email||'')) loi.push('Email không hợp lệ');
      if((f.mk||'').length<3) loi.push('Mật khẩu quá ngắn');
      if(loi.length) return toast(loi.join('. '), false);
      const mkHash = await sha256(f.mk);
      S.users.push({ma:f.ma,ten:f.ten,vai:'Hanh khach',email:f.email,cccd:f.cccd,sdt:f.sdt,mkHash});
      store.set('users_extra', S.users.filter(x=>x.vai==='Hanh khach'));
      logAct('Đăng ký hành khách', f.ma); pushSharedFlights('Đăng ký '+f.ma);
      toast('Đăng ký thành công — hãy đăng nhập');
      renderAuthBody('login');
    };
  }
}

function pHome(){
  if(S.user){
    $('#panel').innerHTML=`${bannerCanhBao()}
      <div class="hero"><h2>Xin chào, ${esc(S.user.ten)}</h2></div>
      <div class="stat">
        <div class="card"><span class="mut">Sân bay</span><b>${S.airports.length}</b></div>
        <div class="card"><span class="mut">Chuyến bay</span><b>${S.flights.length}</b></div>
        <div class="card"><span class="mut">Vai trò</span><b>${esc(isAdmin()?'Admin':isDL()?'Đại lý':'Hành khách')}</b></div>
      </div>`;
    return;
  }
  const mode = S.authMode || 'welcome';
  let body = '';
  if(mode === 'welcome'){
    body = `<div class="hero hero-home">
      <div class="vis-3d" aria-hidden="true">
        <span class="aurora"></span>
        <span class="ray r1"></span>
        <span class="ray r2"></span>
        <span class="orb orb1"></span>
        <span class="orb orb2"></span>
        <span class="orb orb3"></span>
        <span class="orb orb4"></span>
        <span class="ring ring1"></span>
        <span class="ring ring2"></span>
        <span class="ring ring3"></span>
        <span class="float-plane p1">✈</span>
        <span class="float-plane p2">✈</span>
        <span class="spark s1"></span>
        <span class="spark s2"></span>
        <span class="spark s3"></span>
        <span class="spark s4"></span>
        <span class="spark s5"></span>
        <span class="spark s6"></span>
      </div>
      <div class="hero-copy">
        <h2>Chào mừng bạn đến với Skyport</h2>
        <p class="welcome-sub">Hệ thống quản lý và điều hành hàng không thông minh</p>
        <p class="welcome-tags">Kết nối chuyến bay · Tối ưu vận hành · Nâng tầm trải nghiệm</p>
      </div>
    </div>`;
  } else if(mode === 'login'){
    body = `<div class="card" style="max-width:480px;margin:0 auto">
      <button type="button" class="link-btn" id="backAuth">← Quay lại</button>
      <h2>Đăng nhập hệ thống</h2>
      <form id="fl">
        <div class="field"><label>Mã người dùng</label><input name="ma" id="loginMa" required autocomplete="username"></div>
        <div class="field"><label>Mật khẩu</label><div class="pwd-wrap"><input name="mk" id="loginMk" type="password" required autocomplete="current-password"><button type="button" class="pwd-toggle" data-for="loginMk" aria-label="Hiện mật khẩu">👁</button></div></div>
        <label class="check"><input type="checkbox" id="luuTk"> Lưu tài khoản trên máy này</label>
        <button class="b" type="submit" style="width:100%;margin-top:12px">Đăng nhập hệ thống</button>
      </form>
      <button type="button" class="link-btn" id="btnQuen" style="margin-top:10px">Quên mật khẩu?</button>
      <div id="forgotBox" hidden style="margin-top:16px;padding-top:16px;border-top:1px solid #e2e8f0">
        <h3>Đặt lại mật khẩu</h3>
        <div class="field"><label>Mã người dùng</label><input id="qMa"></div>
        <div class="field"><label>Email đã đăng ký</label><input id="qEmail" type="email"></div>
        <div class="field"><label>Mật khẩu mới</label><input id="qMkMoi" type="password"></div>
        <div class="field"><label>Nhập lại mật khẩu</label><input id="qMkLai" type="password"></div>
        <button type="button" class="b s" id="btnResetMk">Xác nhận đổi</button>
        <p id="qKetQua" class="mut"></p>
      </div>
      <p class="mut" style="margin-top:14px">Chưa có tài khoản? <button type="button" class="link-btn" id="goRegFromLogin">Đăng ký hành khách</button></p>
    </div>`;
  } else if(mode === 'register'){
    body = `<div class="card" style="max-width:480px;margin:0 auto">
      <button type="button" class="link-btn" id="backAuth">← Quay lại</button>
      <h2>Đăng ký hành khách</h2>
      <form id="fr">
        <div class="field"><label>Mã (vd HK100)</label><input name="ma" required></div>
        <div class="field"><label>Họ tên</label><input name="ten" required></div>
        <div class="field"><label>CCCD</label><input name="cccd" required></div>
        <div class="field"><label>SĐT</label><input name="sdt" required></div>
        <div class="field"><label>Email</label><input name="email" type="email" required></div>
        <div class="field"><label>Mật khẩu</label><div class="pwd-wrap"><input name="mk" id="regMk" type="password" required><button type="button" class="pwd-toggle" data-for="regMk">👁</button></div></div>
        <button class="b" type="submit" style="width:100%;margin-top:12px">Đăng ký</button>
      </form>
      <p class="mut" style="margin-top:14px">Đã có tài khoản? <button type="button" class="link-btn" id="goLoginFromReg">Đăng nhập</button></p>
    </div>`;
  }
  $('#panel').innerHTML = body;
  bindPwdToggles($('#panel'));

  if($('#backAuth')) $('#backAuth').onclick = () => { S.authMode = 'welcome'; pHome(); };
  if($('#goRegFromLogin')) $('#goRegFromLogin').onclick = () => { S.authMode = 'register'; pHome(); };
  if($('#goLoginFromReg')) $('#goLoginFromReg').onclick = () => { S.authMode = 'login'; pHome(); };

  if($('#fl')){
    const saved = store.get('saved_login', null);
    if(saved && $('#loginMa')){ $('#loginMa').value = saved.ma||''; if($('#luuTk')) $('#luuTk').checked = true; }
    $('#fl').onsubmit = async e => {
      e.preventDefault();
      
      try { await pullSharedFlights(false); const extra=mergeByKey(store.get('hk_users',[]),store.get('users_extra',[]),'ma'); const map=new Map((S.users||[]).map(u=>[u.ma,u])); extra.forEach(u=>map.set(u.ma,{...map.get(u.ma),...u})); S.users=[...map.values()]; } catch(err) {}
      const f = Object.fromEntries(new FormData(e.target));
      const u = S.users.find(x => x.ma === f.ma);
      if(!u) return toast('Tài khoản chưa được đăng ký', false);
      if(u.vai==='Dai ly' && !isHostMode())
        return toast('Tài khoản đại lý chỉ dùng trên máy chủ', false);
      if(!(await matKhauKhop(f.mk, u))) return toast('Sai mật khẩu', false);
      if($('#luuTk') && $('#luuTk').checked) store.set('saved_login', {ma:f.ma});
      else store.set('saved_login', null);
      S.user = { ma:u.ma, ten:u.ten, vai:u.vai, email:u.email||'', tyle:u.tyle };
      store.set('session', S.user);
      if (u.vai === 'Admin' || u.vai === 'Dai ly') localStorage.setItem('asm_host_mode', '1');
      logAct('Đăng nhập'); toast('Xin chào '+u.ten);
      S.authMode = 'welcome';
      setNav(); setWho(); go('tim');
    };
    if($('#btnQuen')) $('#btnQuen').onclick = () => { const b=$('#forgotBox'); if(b) b.hidden = !b.hidden; };
    if($('#btnResetMk')) $('#btnResetMk').onclick = async () => {
      const ma=$('#qMa').value.trim(), email=$('#qEmail').value.trim(), mk1=$('#qMkMoi').value, mk2=$('#qMkLai').value;
      const u=S.users.find(x=>x.ma===ma);
      if(!u) return $('#qKetQua').textContent='Tài khoản chưa được đăng ký';
      if(u.vai!=='Hanh khach') return $('#qKetQua').textContent='Chỉ hành khách đổi mật khẩu tại đây';
      if((u.email||'').toLowerCase()!==email.toLowerCase()) return $('#qKetQua').textContent='Email không khớp';
      if(mk1.length<3||mk1!==mk2) return $('#qKetQua').textContent='Mật khẩu mới không hợp lệ';
      u.mkHash = await sha256(mk1); delete u.mk;
      store.set('users_extra', S.users.filter(x=>x.vai==='Hanh khach'));
      $('#qKetQua').textContent='Đổi mật khẩu thành công — hãy đăng nhập lại';
      logAct('Đổi mật khẩu', ma);
    };
  }
  if($('#fr')){
    $('#fr').onsubmit = async e => {
      e.preventDefault();
      const f=Object.fromEntries(new FormData(e.target));
      const loi=[];
      if(S.users.some(x=>x.ma===f.ma)) loi.push('Mã đã tồn tại');
      if(S.users.some(x=>x.email&&x.email.toLowerCase()===f.email.toLowerCase())) loi.push('Email đã được dùng');
      if(S.users.some(x=>x.cccd&&x.cccd===f.cccd)) loi.push('CCCD đã được dùng');
      if(!/^\d{9}$|^\d{12}$/.test(f.cccd||'')) loi.push('CCCD phải 9 hoặc 12 số');
      if(!/^0\d{9,10}$/.test(f.sdt||'')) loi.push('Số điện thoại không hợp lệ');
      if(!/^[^\s@]+@[^\s@]+\.[^\s@]+$/.test(f.email||'')) loi.push('Email không hợp lệ');
      if((f.mk||'').length<3) loi.push('Mật khẩu quá ngắn');
      if(loi.length) return toast(loi.join('. '), false);
      const mkHash = await sha256(f.mk);
      S.users.push({ma:f.ma,ten:f.ten,vai:'Hanh khach',email:f.email,cccd:f.cccd,sdt:f.sdt,mkHash});
      store.set('users_extra', S.users.filter(x=>x.vai==='Hanh khach'));
      logAct('Đăng ký hành khách', f.ma); pushSharedFlights('Đăng ký '+f.ma);
      toast('Đăng ký thành công — hãy đăng nhập');
      S.authMode = 'login';
      pHome();
    };
  }
}

function pTim(){
  if(!need())return;
  if(!S.ngayChon) S.ngayChon = ngayISO(0);
  const buildStrip = (di, den) => {
    let html = '<div class="date-strip" id="dateStrip">';
    for(let i=0;i<14;i++){
      const iso = ngayISO(i);
      const gmin = giaThapNhat(di, den, iso);
      const on = iso === S.ngayChon ? ' on' : '';
      html += `<button type="button" class="date-chip${on}" data-ngay="${iso}">
        <span class="muted">${formatNgayVN(iso)}</span>
        <b>${gmin ? (Math.round(gmin/1000)+'K') : '—'}</b>
      </button>`;
    }
    return html + '</div>';
  };
  const di0 = S.airports[0]?.maSB || 'SGN';
  const den0 = S.airports[1]?.maSB || 'HAN';
  $('#panel').innerHTML=`${bannerCanhBao()}
  <div class="card search-card"><h2>Tìm chuyến bay</h2>
    <form id="f" class="search-bar">
      <div class="field"><label>Điểm đi</label><select name="di"><option value="">— Chọn điểm đi —</option>${apOpts()}</select></div>
      <div class="field"><label>Điểm đến</label><select name="den"><option value="">— Chọn điểm đến —</option>${apOpts()}</select></div>
      <button class="b search-btn" type="submit">Tìm chuyến bay</button>
    </form>
    <div id="destWrap" class="dest-wrap" hidden></div>
    <p class="mut" style="margin:14px 0 4px">Chọn ngày bay</p>
    <div id="stripWrap">${buildStrip(di0, den0)}</div>
  </div>
  <div class="card" id="mapCard" style="margin-top:12px">
    <h3 style="margin:0 0 6px">Bản đồ tuyến bay</h3>
    <p class="mut" id="mapHint" style="margin:0 0 8px">Chọn điểm đi và điểm đến, bấm Tìm chuyến bay. Nền map: Esri GPS.</p>
    <div id="routeMap" class="route-map" role="img" aria-label="Bản đồ tuyến bay"></div>
  </div>
  <div id="res"></div>`;
  S._lastMapList = [];
  const refreshStrip = () => {
    const f = Object.fromEntries(new FormData($('#f')));
    if(f.di && f.den) $('#stripWrap').innerHTML = buildStrip(f.di, f.den);
    bindStrip();
    const hint=$('#mapHint');
    // Đổi điểm → chỉ giữ list tìm nếu vẫn cùng tuyến
    if(S._lastMapList && S._lastMapList.length && f.di && f.den){
      if(!(S._lastMapList[0].di===f.di && S._lastMapList[0].den===f.den)) S._lastMapList=[];
    } else if(!f.di || !f.den || f.di===f.den) S._lastMapList=[];
    if(hint){
      if(!f.di || !f.den) hint.textContent='Chọn điểm đi và điểm đến, bấm Tìm chuyến bay. Nền map: Esri GPS.';
      else if(f.di===f.den) hint.textContent='Điểm đi và điểm đến phải khác nhau.';
      else hint.textContent='Đã chọn '+f.di+' → '+f.den+' · bấm «Tìm chuyến bay» để hiện đường bay trên bản đồ';
    }
    // Đổi điểm: chỉ nền, chưa vẽ đường — đường bay sau khi bấm Tìm
    renderRouteMap(f.di||'', f.den||'', [], S.ngayChon||ngayISO(0), false);
  };
  const bindStrip = () => {
    $$('#dateStrip .date-chip').forEach(ch => {
      ch.onclick = () => {
        S.ngayChon = ch.dataset.ngay;
        $$('#dateStrip .date-chip').forEach(x => x.classList.toggle('on', x.dataset.ngay === S.ngayChon));
        taoThongBaoChuyen(S.ngayChon);
        // tìm lại theo ngày mới nếu đã có kết quả
        const form=$('#f');
        if(form && $('#res') && $('#res').innerHTML.trim()){
          const fd=Object.fromEntries(new FormData(form));
          const list=S.flights.filter(c=>c.di===fd.di&&c.den===fd.den);
          renderList(list, fd);
        }
      };
    });
  };
  bindStrip();
  $('#f [name=di]').onchange = $('#f [name=den]').onchange = refreshStrip;
  // Bản đồ nền ngay khi mở trang — chưa có đường bay
  renderRouteMap('', '', [], S.ngayChon||ngayISO(0), false);
  const renderList = (list, f) => {
    const canBook = isHK();
    const ngay = normNgay(S.ngayChon || ngayISO(0));
    taoThongBaoChuyen(ngay);
    list = ganGioTheoNgay(list, ngay);
    if(!list.length){
      S._lastMapList = [];
      $('#res').innerHTML=`<div class="card mut">Không có chuyến thẳng ngày ${formatNgayVN(ngay)}. Thử ngày khác hoặc Nối chuyến.</div>`;
      renderRouteMap(f.di, f.den, [], ngay, true);
      return;
    }
    S._lastMapList = list.slice();
    $('#res').innerHTML=`<div class="card"><h2>${list.length} chuyến thẳng · ${formatNgayVN(ngay)}</h2>
      <div class="table-wrap"><table>
      <tr><th>Mã chuyến bay</th><th>Tuyến bay</th><th>Giờ đi</th><th>Giờ đến</th><th>Giá cơ bản</th><th>Ghế trống (TG/PT/TK)</th>${canBook?'<th>Thao tác</th>':''}</tr>
      ${list.map(c=>{
        const st=getFlightStatus(c.ma, ngay);
        const meta=getFlightStatusMeta(c.ma, ngay);
        const tag = (st && st!=='BINH_THUONG')
          ? ` <span class="tag" style="background:#fef3c7;color:#b45309">${esc(ttLabel(st))}${meta&&meta.note?(' · '+esc(meta.note)):''}</span>`
          : '';
        return `<tr><td><b>${esc(c.ma)}</b>${tag}</td>
        <td>${esc(c.di)} → ${esc(c.den)}</td><td>${esc(c.gioDi)}</td><td>${esc(c.gioDen)}</td><td>${vnd(c.gia)}</td>
        <td>${c.tg||8} / ${c.pt||36} / ${c.tk||18}</td>
        ${canBook?(st==='HUY'?`<td><span class="mut">Đã hủy</span></td>`:`<td><button class="b s" data-m="${esc(c.ma)}">Chọn đặt vé</button></td>`):''}</tr>`;
      }).join('')}</table></div>
      ${isDL()?'<p class="mut">Đại lý đặt hộ khách tại mục «Đặt vé hộ khách».</p>':''}
    </div>`;
    $$('#res [data-m]').forEach(b=>b.onclick=()=>{S.book={legs:[b.dataset.m], ngay:S.ngayChon}; S.compareCtx={...(S.compareCtx||{}), legs:[b.dataset.m], ngay:S.ngayChon, ma:b.dataset.m}; go('dat');});
    renderRouteMap(f.di, f.den, list, ngay, true);
  };
  const showDest = () => {
    const f = Object.fromEntries(new FormData($('#f')));
    const w = $('#destWrap');
    if(!w) return;
    w.hidden = false;
    w.innerHTML = destGalleryHTML(f.den, f.di);
  };
  $('#f').onsubmit=e=>{
    e.preventDefault();
    const f=Object.fromEntries(new FormData(e.target));
    showDest();
    const list=S.flights.filter(c=>c.di===f.di&&c.den===f.den);
    S.compareCtx={di:f.di, den:f.den, ngay:S.ngayChon, list:(typeof list!=='undefined'?list:[]).map(c=>({ma:c.ma,di:c.di,den:c.den,gia:c.gia}))};
    logAct('Tìm chuyến', f.di+'→'+f.den+' · '+S.ngayChon);
    renderList(list, f);
  };
}

function pNoi(){
  if(!need())return;
  $('#panel').innerHTML=`${bannerCanhBao()}
  <div class="card"><h2>Tìm nối chuyến</h2>
    <p class="mut">${isHK()?'Chọn phương án rồi đặt cả hành trình.':isDL()?'Xem phương án để đặt hộ.':'Chỉ tra cứu — Admin không đặt vé.'}</p>
    <form id="f" class="search-bar">
      <div class="field"><label>Điểm đi</label><select name="di">${apOpts()}</select></div>
      <div class="field"><label>Điểm đến</label><select name="den">${apOpts()}</select></div>
      <button class="b search-btn" type="submit">Tìm nối chuyến</button>
    </form></div><div id="res"></div>`;
  const den=$('#f [name=den]'); if(den.options.length>1) den.selectedIndex=1;
  $('#f').onsubmit=e=>{
    e.preventDefault(); const f=Object.fromEntries(new FormData(e.target));
    const MIN_CHO = 60; // phút tối thiểu giữa hạ cánh chặng 1 và cất cánh chặng 2
    const opts=[];
    const leg1s = S.flights.filter(c=>c.di===f.di && c.den!==f.den);
    for(const a of leg1s){
      const leg2s = S.flights.filter(c=>c.di===a.den && c.den===f.den && c.ma!==a.ma);
      for(const b0 of leg2s){
        let b = {...b0};
        let wait = phutCho(a.gioDen, b.gioDi);
        // Nếu chặng 2 trước hoặc chờ < 60 phút → đẩy sang ngày sau (tối đa thử 2 lần)
        let tries = 0;
        while((wait < MIN_CHO) && tries < 2){
          b = {
            ...b,
            gioDi: shiftGioSangNgaySau(b.gioDi),
            gioDen: shiftGioSangNgaySau(b.gioDen),
            _shifted: true
          };
          wait = phutCho(a.gioDen, b.gioDi);
          tries++;
        }
        if(wait < MIN_CHO) continue; // vẫn không hợp lệ
        opts.push({
          chang:[a,b],
          tong:(a.gia||0)+(b.gia||0),
          cho: Math.round(wait)
        });
      }
    }
    // Ưu tiên chờ hợp lý (60–360 phút), rồi giá
    opts.sort((x,y)=>{
      const score = o => (o.cho>=60 && o.cho<=480 ? 0 : 1)*1000000 + o.tong + o.cho*100;
      return score(x)-score(y);
    });
    // bỏ trùng cặp mã
    const seen = new Set();
    const uniq = [];
    for(const o of opts){
      const k = o.chang.map(c=>c.ma).join('+');
      if(seen.has(k)) continue;
      seen.add(k); uniq.push(o);
    }
    logAct('Tìm nối chuyến', f.di+'→'+f.den);
    if(!uniq.length){$('#res').innerHTML='<div class="card mut">Không có phương án nối hợp lệ (chặng 2 phải sau chặng 1 ít nhất 60 phút).</div>';return;}
    const canBook=isHK();
    const canBookDL=isDL();
    $('#res').innerHTML=`<div class="mut">Có <b>${uniq.length}</b> phương án (đã lọc giờ nối hợp lệ):</div>`+
      uniq.slice(0,12).map((h,i)=>`<div class="card path">
        <div style="display:flex;flex-wrap:wrap;justify-content:space-between;gap:10px;align-items:center">
          <div><b>Phương án ${i+1}</b> — ${h.chang.length} chặng — Chờ nối ~${h.cho} phút — Tổng: <b>${vnd(h.tong)}</b></div>
          ${canBook?`<button class="b" data-i="${i}">Đặt cả hành trình</button>`:''}
          ${canBookDL?`<button class="b s" data-i="${i}">Chọn đặt hộ</button>`:''}
        </div>
        <div class="table-wrap"><table style="margin-top:8px"><tr><th>Chặng</th><th>Mã chuyến</th><th>Tuyến</th><th>Giờ đi</th><th>Giờ đến</th><th>Giá</th></tr>
        ${h.chang.map((c,j)=>`<tr><td>Chặng ${j+1}</td><td><b>${esc(c.ma)}</b></td><td>${esc(c.di)} → ${esc(c.den)}</td>
          <td>${esc(c.gioDi)}</td><td>${esc(c.gioDen)}</td><td>${vnd(c.gia)}</td></tr>`).join('')}
        </table></div></div>`).join('');
    $$('#res [data-i]').forEach(btn=>btn.onclick=()=>{
      S.book={legs:uniq[+btn.dataset.i].chang.map(c=>c.ma), ho:isDL(), ngay:S.ngayChon}; S.compareCtx={di:f.di, den:f.den, ngay:S.ngayChon, legs:S.book.legs, list:S.book.legs.map(m=>{const c=S.flights.find(x=>x.ma===m); return c?{ma:c.ma,di:c.di,den:c.den,gia:c.gia}:null;}).filter(Boolean)};
      go(isDL()?'datdl':'dat');
    });
  };
}

function seatsFor(hang){
  const n=hang==='TG'?8:hang==='TK'?12:20; const arr=[];
  for(let r=1;r<=Math.ceil(n/4);r++) for(const col of ['A','B','C','D']){
    if(arr.length>=n) break;
    const so=r+col; arr.push({so,busy:store.get('busy_'+hang,[]).includes(so)});
  }
  return arr;
}

function pDat(){
  if(!need(['Hanh khach'])) return toast('Chỉ hành khách đặt vé tại đây',false), go('tim');
  _formDat(false);
}
function pDatDL(){
  if(!need(['Dai ly'])) return;
  _formDat(true);
}
function _formDat(isHo){
  const legs=(S.book&&S.book.legs)||[];
  if(!legs.length){
    if(isHo){
      $('#panel').innerHTML=`${bannerCanhBao()}
      <div class="card"><h2>Đặt vé hộ khách</h2>
        <p class="mut">Chọn điểm đi và điểm đến, tìm chuyến rồi đặt hộ.</p>
        <form id="f" class="search-bar">
          <div class="field"><label>Điểm đi</label><select name="di">${apOpts()}</select></div>
          <div class="field"><label>Điểm đến</label><select name="den">${apOpts()}</select></div>
          <button class="b search-btn" type="submit">Tìm chuyến</button>
        </form>
      </div><div id="res"></div>`;
      const den=$('#f [name=den]'); if(den&&den.options.length>1) den.selectedIndex=1;
      $('#f').onsubmit=e=>{
        e.preventDefault();
        const f=Object.fromEntries(new FormData(e.target));
        const list=S.flights.filter(c=>c.di===f.di&&c.den===f.den);
        if(!list.length){ $('#res').innerHTML='<div class="card mut">Không có chuyến thẳng. Thử cặp sân bay khác hoặc Nối chuyến.</div>'; return; }
        $('#res').innerHTML=`<div class="card"><h2>${list.length} chuyến thẳng</h2>
          <div class="table-wrap"><table>
          <tr><th>Mã chuyến</th><th>Tuyến</th><th>Giờ đi</th><th>Giờ đến</th><th>Giá</th><th></th></tr>
          ${list.map(c=>`<tr>
            <td><b>${esc(c.ma)}</b></td><td>${esc(c.di)} → ${esc(c.den)}</td>
            <td>${esc(c.gioDi||'')}</td><td>${esc(c.gioDen||'')}</td><td>${vnd(c.gia)}</td>
            <td><button type="button" class="b s" data-m="${esc(c.ma)}">Chọn đặt hộ</button></td>
          </tr>`).join('')}
          </table></div></div>`;
        $$('#res [data-m]').forEach(b=>b.onclick=()=>{ S.book={legs:[b.dataset.m],ho:true}; _formDat(true); });
      };
      return;
    }
    $('#panel').innerHTML=`<div class="card"><h2>Đặt vé</h2>
      <p class="mut">Chọn chuyến ở tab Chuyến bay / Nối chuyến trước.</p>
      <button type="button" class="b" id="toTim">Tìm chuyến bay</button>
    </div>`;
    if($('#toTim')) $('#toTim').onclick=()=>go('tim');
    return;
  }
  const detail=legs.map(m=>S.flights.find(c=>c.ma===m)).filter(Boolean);
  const base=detail.reduce((s,c)=>s+(c.gia||0),0);
  $('#panel').innerHTML=`<div class="card"><h2>${isHo?'Đặt vé hộ khách':'Đặt vé / hành trình'}</h2>
    <p>Chặng: <b>${legs.map(esc).join(' + ')}</b> · Giá cơ bản: <b>${vnd(base)}</b></p>
    <div class="form-grid">
      <div class="field"><label>Hạng ghế</label><select id="hg">
        <option value="TG">Thương gia (hệ số 2.5)</option>
        <option value="PT" selected>Phổ thông (hệ số 1.0)</option>
        <option value="TK">Tiết kiệm (hệ số 0.8)</option></select></div>
      <div class="field"><label>Số người lớn</label><input id="soNL" type="number" min="1" max="6" value="1"></div>
      <div class="field"><label>Số trẻ em</label><input id="soTE" type="number" min="0" max="4" value="0"></div>
      <div class="field"><label>Hành lý thêm (kg)</label><input id="kg" type="number" min="0" value="0"></div>
      <div class="field"><label>Suất ăn</label><select id="mon"><option value="">— Không —</option>
        ${S.meals.map(m=>`<option value="${esc(m.ten)}" data-g="${m.gia}">${esc(m.ten)} (${vnd(m.gia)})</option>`).join('')}</select></div>
      <div class="field"><label>Voucher áp dụng cho chuyến này</label><select id="voucher"><option value="">— Không dùng mã —</option></select></div>
      ${isHo?`<div class="field"><label>Mã / tên khách hàng</label><input id="mkh" placeholder="Nguyễn Văn A / HK01" required></div>`:''}
      ${isHK()?`<div class="field"><label>Mua qua đại lý (tuỳ chọn)</label><select id="mdl"><option value="">— Quầy / trực tiếp —</option>
        ${S.users.filter(u=>u.vai==='Dai ly').map(d=>`<option value="${esc(d.ma)}">${esc(d.ten)}</option>`).join('')}</select></div>`:''}
    </div>
    <p class="mut" id="voucherHint"></p>
    <h3>Chọn ghế (chặng đầu)</h3><div id="map" class="seatmap"></div>
    <p class="mut">Tạm tính: <b id="tt">${vnd(base)}</b></p>
    <button class="b" id="pay">Xác nhận đặt vé</button>
  </div><div class="card"><div id="out" class="mut">Chưa đặt</div></div>`;
    if(isHK() && S.book && S.book.preferDl && $('#mdl')){
    $('#mdl').value = S.book.preferDl;
  }
  const fillVoucher = () => {
    const hang = $('#hg').value;
    const list = voucherChoChuyen(legs, hang);
    const sel = $('#voucher');
    const cur = sel.value;
    sel.innerHTML = '<option value="">— Không dùng mã —</option>' +
      list.map(v => `<option value="${esc(v.ma)}" data-g="${v.giam}">${esc(v.ma)} −${v.giam}% · ${esc(v.ten)}</option>`).join('');
    if ([...sel.options].some(o => o.value === cur)) sel.value = cur;
    const hint = $('#voucherHint');
    if (hint) hint.textContent = list.length
      ? `Có ${list.length} voucher phù hợp chuyến/hạng ghế này.`
      : 'Không có voucher phù hợp cho chuyến hoặc hạng ghế đang chọn.';
  };
  const draw=()=>{
    const hang=$('#hg').value; const seats=seatsFor(hang);
    $('#map').innerHTML=`<div class="row">${seats.map(s=>`<button type="button" class="seat ${s.busy?'busy':'free'}" data-s="${s.so}" ${s.busy?'disabled':''}>${s.so}</button>`).join('')}</div>`;
    $$('#map .seat.free').forEach(b=>b.onclick=()=>{$$('#map .seat').forEach(x=>x.classList.remove('sel'));b.classList.add('sel');S.book.ghe=b.dataset.s;});
    fillVoucher();
    recalc();
  };
  const recalc=()=>{
    const hang=$('#hg').value; let t=base*HS[hang];
    const kg=+$('#kg').value||0; if(kg>HL[hang]) t+=(kg-HL[hang])*50000;
    const mon=$('#mon'); const opt=mon.options[mon.selectedIndex]; if(mon.value) t+=+(opt.dataset.g||0);
    const vo=$('#voucher'); const voOpt=vo.options[vo.selectedIndex];
    const giam = vo.value ? +(voOpt.dataset.g||0) : 0;
    if(giam) t = Math.max(0, Math.round(t * (1 - giam/100)));
    $('#tt').textContent=vnd(t); S.book.tong=t; S.book.hang=hang; S.book.voucher=vo.value||''; S.book.giam=giam;
  };
  $('#hg').onchange=$('#kg').oninput=$('#mon').onchange=$('#voucher').onchange=()=>{ fillVoucher(); draw(); };
  draw();
  $('#pay').onclick=()=>{
    if(!S.book.ghe) return toast('Chọn ghế',false);
    if(isHo&&!($('#mkh')?.value||'').trim()) return toast('Nhập thông tin khách',false);
    // chặn đặt chuyến đã hủy
    for(const c of detail){
      const st=getFlightStatus(c.ma, S.ngayChon||ngayISO(0));
      if(st==='HUY') return toast('Chuyến '+c.ma+' đã bị hủy, không thể đặt', false);
    }
    recalc();
    const soNL=Math.max(1, Math.min(6, +($('#soNL')?.value||1)));
    const soTE=Math.max(0, Math.min(4, +($('#soTE')?.value||0)));
    const pax=[];
    for(let i=0;i<soNL;i++) pax.push({loai:'NL', ten: (isHo?($('#mkh')?.value||'Khách'): (S.user.ten||'Hành khách')) + (soNL+soTE>1?(' (NL'+(i+1)+')'):'')});
    for(let i=0;i<soTE;i++) pax.push({loai:'TE', ten:'Trẻ em '+(i+1)});
    const ves=store.get('ves',[]);
    const dl=isHo?S.user.ma:($('#mdl')?.value||'');
    let tong1 = S.book.tong||0; // giá 1 người các chặng
    const giam = S.book.giam||0;
    const maKM = S.book.voucher||'';
    // trẻ em 75% giá người lớn
    const heSoPax = pax.map(p=>p.loai==='TE'?0.75:1);
    const tongAll = Math.round(tong1 * heSoPax.reduce((a,b)=>a+b,0));
    const ngay = new Date().toISOString();
    const nhomId='G'+Date.now().toString(36).toUpperCase();
    let cnt=0;
    pax.forEach((p,pi)=>{
      detail.forEach((c,i)=>{
        const gia1=Math.round((tong1/detail.length)*heSoPax[pi]);
        ves.push({
          ma:'V'+Date.now().toString(36).toUpperCase()+pi+i,
          nhom: nhomId,
          cb:c.ma, legs:detail.map(x=>x.ma), tuyen:c.di+'→'+c.den,
          gia:gia1, hang:S.book.hang,
          ghe: pi===0 ? S.book.ghe : (S.book.ghe.replace(/\\d+/, n=>String((+n||1)+pi))),
          user:S.user.ma, kh:p.ten, tenHK:p.ten, loaiHK:p.loai, dl,
          ci:false, paid:false, maKM, ngay
        });
        cnt++;
      });
    });
    let giamTxt = maKM ? ` · mã ${maKM} −${giam}%` : '';
    store.set('ves',ves);
    if(dl){const map=store.get('hh_map',{});const ty=(S.users.find(u=>u.ma===dl)||{}).tyle||0.05;map[dl]=(map[dl]||0)+Math.round(tongAll*ty);store.set('hh_map',map);}
    logAct(isHo?'Đặt vé hộ':'Đặt vé', legs.join('+')+' · '+soNL+' NL + '+soTE+' TE · '+vnd(tongAll)+giamTxt);
    pushSharedFlights('Đặt vé · '+cnt+' vé');
    $('#out').innerHTML=`<p><b>Đặt thành công</b> — ${cnt} vé (${soNL} người lớn, ${soTE} trẻ em) · Tổng <b>${vnd(tongAll)}</b>${giamTxt}. Vào «Vé» để check-in, thanh toán, in thẻ lên máy bay.</p>`;
    toast('Đặt vé thành công' + (giamTxt ? ' (đã áp voucher)' : '')); S.book=null;
  };
}

function pVe(){
  if(!need())return;
  if(!S._pullingVe){
    S._pullingVe = true;
    pullSharedFlights(false).then(changed=>{
      S._pullingVe = false;
      if(changed && S.page==='ve'){ try{ pVe(); }catch(e){} }
    }).catch(()=>{ S._pullingVe = false; });
  }
  let all=store.get('ves',[]);
  if(isHK()) all=all.filter(v=>v.user===S.user.ma||v.kh===S.user.ma);
  if(isDL()) all=all.filter(v=>v.dl===S.user.ma||v.user===S.user.ma);
  if(isAdmin()) all=store.get('ves',[]);
  // nhóm theo nhom (hoặc fallback theo ngay+cb+user)
  const groups={};
  all.forEach(v=>{
    const k=v.nhom||('LEGACY_'+((v.ngay||'')+(v.cb||'')+(v.user||'')+(v.hang||'')));
    if(!groups[k]) groups[k]=[];
    groups[k].push(v);
  });
  const keys=Object.keys(groups);
  let html=`${bannerCanhBao()}<div class="card"><h2>${isHK()?'Vé của tôi':isDL()?'Vé đã bán hộ':'Danh sách vé hệ thống'}</h2>`;
  if(!keys.length) html+=`<p class="mut">Chưa có vé.</p></div><div id="payBox"></div>`;
  else {
    keys.forEach((k,gi)=>{
      const list=groups[k];
      const tong=list.reduce((s,v)=>s+(v.gia||0),0);
      const unpaid=list.filter(v=>v.ci&&!v.paid&&!v.huyChuyen);
      const tongUnpaid=unpaid.reduce((s,v)=>s+(v.gia||0),0);
      html+=`<div style="margin:${gi? '22px':'8px'} 0 0;padding-top:${gi?'14px':'0'};border-top:${gi?'2px dashed #cbd5e1':'none'}">
        <p style="margin:0 0 8px"><b>Nhóm đặt ${gi+1}</b> · ${list.length} vé · Tổng ${vnd(tong)}
        ${unpaid.length&&isHK()?` · <button type="button" class="b s" data-paygroup="${esc(k)}">Thanh toán cả nhóm (${vnd(tongUnpaid)})</button>`:''}
        </p>
        <div class="table-wrap" style="overflow-x:auto;-webkit-overflow-scrolling:touch"><table>
        <tr><th>Mã vé</th><th>Chuyến</th><th>Tuyến</th><th>Hạng</th><th>Ghế</th><th>HK</th><th>Giá</th><th>Check-in</th><th>Thanh toán</th><th>Thao tác</th></tr>
        ${list.map(v=>`<tr>
          <td>${esc(v.ma)}</td><td>${esc(v.cb)}</td><td>${esc(v.tuyen)}</td><td>${esc(hangTen(v.hang))}</td>
          <td>${esc(v.ghe)}</td><td>${esc(v.tenHK||v.kh||'')}${v.loaiHK==='TE'?' (TE)':''}</td><td>${vnd(v.gia)}</td>
          <td>${v.ci?'Đã check-in':'Chưa check-in'}</td>
          <td>${v.paid?`Đã thanh toán (${esc(v.payMethod||'')})`:'Chưa thanh toán'}</td>
          <td style="white-space:nowrap">
            ${isHK()&&!v.ci&&!v.huyChuyen?`<button class="b s" data-ci="${esc(v.ma)}">Check-in</button> `:''}
            ${isHK()&&v.ci&&!v.paid&&!v.huyChuyen?`<button class="b s" data-pay="${esc(v.ma)}">Thanh toán</button> `:''}
            ${isHK()&&v.ci?`<button class="b s g" data-bp="${esc(v.ma)}">Thẻ lên máy bay</button> `:''}
            ${isHK()&&!v.huyChuyen?`<button class="b s g" data-doi="${esc(v.ma)}">Đổi vé</button> <button class="b s" style="background:#b91c1c" data-huy="${esc(v.ma)}">Hủy vé</button>`:''}
            ${v.huyChuyen?`<span class="mut">Chuyến hủy · hoàn ${vnd(v.refund||0)}</span>`:''}
          </td></tr>`).join('')}
        </table></div></div>`;
    });
    html+=`</div><div id="payBox"></div>`;
  }
  $('#panel').innerHTML=html;

  $$('[data-ci]').forEach(b=>b.onclick=()=>{
    const all=store.get('ves',[]); const v=all.find(x=>x.ma===b.dataset.ci); if(!v)return;
    v.ci=true; store.set('ves',all); logAct('Check-in', v.ma); pushSharedFlights('Check-in '+v.ma); toast('Check-in thành công'); pVe();
  });
  $$('[data-pay]').forEach(b=>b.onclick=()=>moThanhToan(b.dataset.pay));
  $$('[data-paygroup]').forEach(b=>b.onclick=()=>moThanhToanNhom(b.dataset.paygroup));
  $$('[data-huy]').forEach(b=>b.onclick=()=>{
    const all0=store.get('ves',[]);
    const ve=all0.find(x=>x.ma===b.dataset.huy);
    if(!ve) return;
    if(ve.hang==='TK' && ve.paid){
      if(!confirm('Hạng Tiết kiệm đã thanh toán: không hoàn tiền. Vẫn hủy vé '+ve.ma+'?')) return;
    }
    const r=phiHoanTheoHang(ve.hang, ve.paid, ve.gia);
    if(!confirm((r.msg||'')+'\nXác nhận hủy vé '+ve.ma+'?')) return;
    let all=all0.filter(x=>x.ma!==ve.ma); store.set('ves',all);
    logAct('Hủy vé', ve.ma+' · hoàn '+r.hoan); pushSharedFlights('Hủy vé '+ve.ma);
    toast(r.hoan>0?('Đã hủy. Ước hoàn '+vnd(r.hoan)):('Đã hủy vé. '+r.msg));
    pVe();
  });
  $$('[data-bp]').forEach(b=>b.onclick=()=>{
    const v=store.get('ves',[]).find(x=>x.ma===b.dataset.bp); if(!v)return;
    if(!v.ci) return toast('Cần check-in trước khi lấy thẻ lên máy bay', false);
    const box=$('#payBox'); if(box) box.innerHTML=boardingPassHTML(v);
  });
  $$('[data-doi]').forEach(b=>b.onclick=()=>{
    const all=store.get('ves',[]); const v=all.find(x=>x.ma===b.dataset.doi); if(!v)return;
    S.book={legs:[v.maCB||v.cb], doiTu:v.ma};
    store.set('ves', all.filter(x=>x.ma!==v.ma));
    logAct('Đổi vé (hủy vé cũ)', v.ma); toast('Vé cũ đã hủy — chọn chuyến mới để đặt lại'); go('tim');
  });
}

function moThanhToan(maVe){
  const all=store.get('ves',[]);
  const v=all.find(x=>x.ma===maVe); if(!v)return;
  // cùng nhóm: thanh toán tất cả vé đã check-in chưa trả
  const nhom=v.nhom;
  const ds=nhom? all.filter(x=>x.nhom===nhom&&x.ci&&!x.paid&&!x.huyChuyen) : [v];
  const tong=ds.reduce((s,x)=>s+(x.gia||0),0);
  const box=$('#payBox'); if(!box)return;
  box.innerHTML=`<div class="card"><h2>Thanh toán ${ds.length>1?('cả nhóm ('+ds.length+' vé)'):('vé '+esc(maVe))}</h2>
    <p>Tổng thanh toán: <b>${vnd(tong)}</b></p>
    <p class="mut">${ds.map(x=>x.ma+' · '+vnd(x.gia)).join('<br>')}</p>
    <div class="form-grid">
      <div class="field"><label>Phương thức</label>
        <select id="pttt">
          <option value="Vietcombank">Ngân hàng Vietcombank</option>
          <option value="Techcombank">Ngân hàng Techcombank</option>
          <option value="MB Bank">Ngân hàng MB Bank</option>
          <option value="VietinBank">Ngân hàng VietinBank</option>
          <option value="MoMo">Ví điện tử MoMo</option>
          <option value="ZaloPay">Ví điện tử ZaloPay</option>
          <option value="VNPay">Cổng VNPay</option>
          <option value="ShopeePay">Ví ShopeePay</option>
        </select></div>
    </div>
    <button class="b" id="xacNhanTT" style="margin-top:12px">Xác nhận thanh toán ${vnd(tong)}</button>
  </div>`;
  box.scrollIntoView({behavior:'smooth', block:'nearest'});
  $('#xacNhanTT').onclick=()=>{
    const method=$('#pttt').value;
    const all2=store.get('ves',[]);
    ds.forEach(d=>{
      const x=all2.find(t=>t.ma===d.ma); if(x){ x.paid=true; x.payMethod=method; }
    });
    store.set('ves',all2);
    logAct('Thanh toán nhóm', ds.map(x=>x.ma).join(',')+' · '+method+' · '+vnd(tong)); pushSharedFlights('Thanh toán');
    toast('Thanh toán thành công '+vnd(tong)+' qua '+method);
    pVe();
  };
}
function moThanhToanNhom(nhom){
  const all=store.get('ves',[]);
  const first=all.find(x=>x.nhom===nhom&&x.ci&&!x.paid);
  if(!first) return toast('Không còn vé cần thanh toán trong nhóm', false);
  moThanhToan(first.ma);
}

function pTK(){
  if(!need())return;
  let all=store.get('ves',[]);
  if(isHK()) all=all.filter(v=>v.user===S.user.ma||v.kh===S.user.ma);
  if(isDL()) all=all.filter(v=>v.dl===S.user.ma||v.user===S.user.ma);
  const tong=all.reduce((s,v)=>s+(v.gia||0),0);
  const by={TG:0,PT:0,TK:0}; all.forEach(v=>{by[v.hang||'PT']=(by[v.hang||'PT']||0)+1;});
  const labelTien=isHK()?'Tổng chi trả':'Doanh thu';
  const labelVe=isHK()?'Số vé đã mua':'Số vé';
  // theo tuyến / ngày
  const byRoute={};
  const byDay={};
  all.forEach(v=>{
    const r=v.tuyen||v.cb||'—';
    byRoute[r]=(byRoute[r]||0)+(v.gia||0);
    const d=(v.ngay||'').slice(0,10)||'không rõ';
    byDay[d]=(byDay[d]||0)+(v.gia||0);
  });
  const routeRows=Object.entries(byRoute).sort((a,b)=>b[1]-a[1]).slice(0,15);
  const dayRows=Object.entries(byDay).sort((a,b)=>a[0]<b[0]?1:-1).slice(0,15);
  $('#panel').innerHTML=`${bannerCanhBao()}<div class="stat">
    <div class="card"><span class="mut">${labelTien}</span><b>${vnd(tong)}</b></div>
    <div class="card"><span class="mut">${labelVe}</span><b>${all.length}</b></div>
    <div class="card"><span class="mut">Chuyến bay</span><b>${S.flights.length}</b></div>
    <div class="card"><span class="mut">Sân bay</span><b>${S.airports.length}</b></div>
  </div>
  <div class="card"><h2>Vé theo hạng ghế</h2>
    <p>Thương gia: <b>${by.TG||0}</b> · Phổ thông: <b>${by.PT||0}</b> · Tiết kiệm: <b>${by.TK||0}</b></p></div>
  <div class="card"><h2>Doanh thu / chi trả theo tuyến</h2>
    <div class="table-wrap"><table><tr><th>Tuyến / mã</th><th>Số tiền</th></tr>
    ${routeRows.map(([k,v])=>`<tr><td>${esc(k)}</td><td>${vnd(v)}</td></tr>`).join('')||'<tr><td colspan="2" class="mut">Chưa có dữ liệu</td></tr>'}
    </table></div></div>
  <div class="card"><h2>Theo ngày đặt</h2>
    <div class="table-wrap"><table><tr><th>Ngày</th><th>Số tiền</th></tr>
    ${dayRows.map(([k,v])=>`<tr><td>${esc(k)}</td><td>${vnd(v)}</td></tr>`).join('')||'<tr><td colspan="2" class="mut">Chưa có dữ liệu</td></tr>'}
    </table></div>
    <p style="margin-top:12px"><button type="button" class="b" id="btnCsv">Xuất báo cáo CSV</button></p>
  </div>`;
  const btn=$('#btnCsv');
  if(btn) btn.onclick=()=>{
    const rows=[['Ma ve','Chuyen','Tuyen','Hang','Ghe','Gia','Ngay','Trang thai','User']];
    all.forEach(v=>rows.push([v.ma,v.cb,v.tuyen,v.hang,v.ghe,v.gia,v.ngay||'',v.statusVe|| (v.paid?'PAID':'NEW'),v.user||'']));
    rows.push([]);
    rows.push(['Tong', '', '', '', '', tong, '', '', '']);
    exportCSV('bao_cao_ve_'+new Date().toISOString().slice(0,10)+'.csv', rows);
    toast('Đã tải file CSV');
  };
}

function pQly(){
  if(!need(['Admin']))return;
  const mbOpts=(S.planes||[]).map(p=>`<option value="${esc(p.soHieu||p.ma||p.ten)}">${esc(p.soHieu||p.ma||p.ten)}</option>`).join('')
    || '<option value="VN-A321">VN-A321</option>';
  const dayQ = normNgay(S.adminNgay || ngayISO(0));
  const rows=ganGioTheoNgay(S.flights.slice().sort((a,b)=>(a.ma||'').localeCompare(b.ma||'')), dayQ);
  $('#panel').innerHTML=`${bannerCanhBao()}
  <div class="card"><h2>Thêm sân bay</h2>
    <form id="fsb" class="form-grid">
      <div class="field"><label>Mã sân bay</label><input name="ma" required></div>
      <div class="field"><label>Tên sân bay</label><input name="ten" required></div>
      <div class="field"><label>Thành phố</label><input name="tp" required></div>
      <div class="form-actions"><button class="b" type="submit">Thêm sân bay</button></div>
    </form></div>
  <div class="card"><h2>Thêm chuyến bay</h2>
    <form id="fcb" class="form-grid">
      <div class="field"><label>Mã chuyến bay</label><input name="ma" required></div>
      <div class="field"><label>Điểm đi</label><select name="di">${apOpts()}</select></div>
      <div class="field"><label>Điểm đến</label><select name="den">${apOpts()}</select></div>
      <div class="field"><label>Máy bay</label><select name="mb">${mbOpts}</select></div>
      <div class="field"><label>Khung giờ (slot 0–17)</label><input name="slot" type="number" min="0" max="17" value="10"></div>
      <div class="field"><label>Thời gian bay (phút)</label><input name="dur" type="number" min="30" value="90"></div>
      <div class="field"><label>Giá cơ bản</label><input name="gia" type="number" value="900000"></div>
      <div class="form-actions"><button class="b" type="submit">Thêm chuyến bay</button></div>
    </form>
    <p class="mut">Hệ thống chặn nếu cùng máy bay bị chồng giờ với chuyến khác.</p></div>
  <div class="card"><h2>Quản lý vòng đời chuyến bay — ngày ${formatNgayVN(dayQ)} (${rows.length} lịch)</h2>
    <p class="mut">Đang quản lý lịch bay ngày <b>${esc(dayQ)}</b>. Trễ/hoãn/hủy/xóa chỉ áp dụng đúng ngày này.</p>
    <div style="display:flex;flex-wrap:wrap;gap:10px;align-items:center;margin-bottom:8px">
      <label class="mut">Chọn ngày:&nbsp;<input type="date" id="adminDateInput" value="${esc(dayQ)}" style="padding:6px 10px;border-radius:8px;border:1px solid #cbd5e1"></label>
    </div>
    <div class="date-strip" id="adminDateStrip">${(()=>{
      if(!S.adminNgay) S.adminNgay = ngayISO(0);
      let h='';
      for(let i=0;i<21;i++){
        const iso=ngayISO(i);
        h+=`<button type="button" class="date-chip${iso===S.adminNgay?' on':''}" data-admin-ngay="${iso}"><span class="muted">${formatNgayVN(iso)}</span></button>`;
      }
      return h;
    })()}</div>
    <div id="editBox"></div>
    <div class="table-wrap" style="overflow-x:auto;-webkit-overflow-scrolling:touch"><table>
      <tr><th>Mã</th><th>Tuyến</th><th>Máy bay</th><th>Giờ</th><th>Trạng thái</th><th>Thao tác</th></tr>
      ${rows.map(f=>{
        const st=getFlightStatus(f.ma, S.adminNgay||ngayISO(0));
        return `<tr>
          <td><b>${esc(f.ma)}</b></td>
          <td>${esc(f.di)} → ${esc(f.den)}</td>
          <td>${esc(f.mb||'')}</td>
          <td>${esc(f.gioDi||'')}</td>
          <td>${esc(ttLabel(st))}</td>
          <td style="white-space:nowrap">
            <button type="button" class="b s" data-edit="${esc(f.ma)}">Sửa</button>
            <button type="button" class="b s" data-st="${esc(f.ma)}" data-v="TRE">Trễ</button>
            <button type="button" class="b s" data-st="${esc(f.ma)}" data-v="HOAN">Hoãn</button>
            <button type="button" class="b s" data-st="${esc(f.ma)}" data-v="DOI_GIO">Đổi giờ</button>
            <button type="button" class="b s" style="background:#b91c1c" data-st="${esc(f.ma)}" data-v="HUY">Hủy chuyến</button>
            <button type="button" class="b s g" data-del="${esc(f.ma)}">Xóa</button>
          </td></tr>`;
      }).join('')}
    </table></div>
  </div>
  <div class="card"><h2>Sân bay (${S.airports.length})</h2>
    <table><tr><th>Mã sân bay</th><th>Tên sân bay</th><th>Thành phố</th></tr>
    ${S.airports.map(a=>`<tr><td><b>${esc(a.maSB)}</b></td><td>${esc(a.ten)}</td><td>${esc(a.thanhPho)}</td></tr>`).join('')}
    </table></div>`;
  $('#fsb').onsubmit=e=>{e.preventDefault();const f=Object.fromEntries(new FormData(e.target));
    if(S.airports.some(a=>a.maSB===f.ma))return toast('Trùng mã',false);
    S.airports.push({maSB:f.ma,ten:f.ten,thanhPho:f.tp}); store.set('airports',S.airports);
    logAct('Thêm sân bay', f.ma); toast('Đã thêm sân bay'); pQly();};
  $('#fcb').onsubmit=e=>{e.preventDefault();const f=Object.fromEntries(new FormData(e.target));
    if(S.flights.some(c=>c.ma===f.ma))return toast('Trùng mã chuyến',false);
    if(f.di===f.den)return toast('Điểm đi và đến phải khác nhau',false);
    const slot=+f.slot||10, dur=+f.dur||90, mb=f.mb||'VN-A321';
    const conf=checkMayBayConflict(mb, slot, dur, null);
    if(conf) return toast('Máy bay '+mb+' trùng lịch với chuyến '+conf.ma+' ('+conf.di+'→'+conf.den+')', false);
    const nf={ma:f.ma,di:f.di,den:f.den,gia:+f.gia||0,slot,dur,mb,tg:8,pt:36,tk:18,status:'BINH_THUONG'};
    const ex=store.get('extra_flights',[]); ex.push(nf); store.set('extra_flights',ex);
    S.flights.push({...nf,gioDi:hm(slot,0),gioDen:hm(slot,dur)});
    logAct('Thêm chuyến bay', f.ma); pushSharedFlights('Thêm chuyến '+f.ma); toast('Đã thêm chuyến'); pQly();};
  $$('#adminDateStrip [data-admin-ngay]').forEach(b=>b.onclick=()=>{ S.adminNgay=normNgay(b.dataset.adminNgay); pQly(); });
  const adi=$('#adminDateInput');
  if(adi) adi.onchange=()=>{ if(adi.value){ S.adminNgay=normNgay(adi.value); pQly(); } };
  $$('[data-st]').forEach(b=>b.onclick=()=>{
    const ma=b.dataset.st, v=b.dataset.v;
    let note='';
    if(v==='TRE'){
      const p=prompt('Ghi chú trễ (phút / lý do):','Delay 25 phút');
      if(p===null) return; // bấm Hủy → không đổi trạng thái
      note=p.trim()||'Delay';
    } else if(v==='DOI_GIO'){
      const p=prompt('Giờ mới / ghi chú đổi giờ:','Đổi giờ +30 phút');
      if(p===null) return;
      note=p.trim()||'Đổi giờ';
    } else if(v==='HOAN'){
      const p=prompt('Lý do hoãn:','Hoãn do thời tiết');
      if(p===null) return;
      note=p.trim()||'Hoãn';
    } else if(v==='HUY'){
      if(!confirm('Hủy chuyến '+ma+'? Vé liên quan sẽ được đánh dấu và tính hoàn theo hạng.')) return;
      const p=prompt('Lý do hủy:','Hủy khai thác');
      if(p===null) return;
      note=p.trim()||'Hủy';
      const n=xuLyVeKhiHuyChuyen(ma);
      toast('Đã hủy chuyến. Đã xử lý '+n+' vé liên quan.');
    } else return;
    setFlightStatus(ma, v, note, S.adminNgay||ngayISO(0));
    logAct('Cập nhật trạng thái chuyến', ma+' → '+v);
    if(v!=='HUY') toast('Đã cập nhật: '+ttLabel(v));
    pQly();
  });
  $$('[data-del]').forEach(b=>b.onclick=()=>{
    const ma=b.dataset.del;
    if(!confirm('Xóa chuyến '+ma+' khỏi danh sách?')) return;
    S.flights=S.flights.filter(f=>f.ma!==ma);
    let ex=store.get('extra_flights',[]); ex=ex.filter(f=>f.ma!==ma); store.set('extra_flights',ex);
    const map=store.get('flight_status',{}); Object.keys(map).forEach(k=>{ if(k===ma || k.startsWith(ma+'|')) delete map[k]; }); store.set('flight_status',map);
    logAct('Xóa chuyến bay', ma); pushSharedFlights('Xóa chuyến '+ma); toast('Đã xóa chuyến'); pQly();
  });
  $$('[data-edit]').forEach(b=>b.onclick=()=>{
    const f=S.flights.find(x=>x.ma===b.dataset.edit); if(!f)return;
    let box=$('#editBox');
    if(!box){ toast('Không mở được form sửa', false); return; }
    box.innerHTML=`<div class="card"><h2>Sửa chuyến ${esc(f.ma)}</h2>
      <form id="fedit" class="form-grid">
        <div class="field"><label>Giá</label><input name="gia" type="number" value="${f.gia||0}"></div>
        <div class="field"><label>Slot (0–17)</label><input name="slot" type="number" min="0" max="17" value="${f.slot||0}"></div>
        <div class="field"><label>Thời gian (phút)</label><input name="dur" type="number" value="${f.dur||90}"></div>
        <div class="field"><label>Máy bay</label><input name="mb" value="${esc(f.mb||'')}"></div>
        <div class="form-actions"><button class="b" type="submit">Lưu</button>
          <button type="button" class="b g" id="btnHuySua">Đóng</button></div>
      </form></div>`;
    box.scrollIntoView({behavior:'smooth', block:'nearest'});
    const bh=$('#btnHuySua'); if(bh) bh.onclick=()=>{ box.innerHTML=''; };
    $('#fedit').onsubmit=e=>{
      e.preventDefault();
      const fd=Object.fromEntries(new FormData(e.target));
      const conf=checkMayBayConflict(fd.mb||f.mb, +fd.slot, +fd.dur, f.ma);
      if(conf) return toast('Trùng lịch máy bay với '+conf.ma, false);
      f.gia=+fd.gia||f.gia; f.slot=+fd.slot; f.dur=+fd.dur; f.mb=fd.mb||f.mb;
      f.gioDi=hm(f.slot,0); f.gioDen=hm(f.slot,f.dur);
      let ex=store.get('extra_flights',[]);
      const i=ex.findIndex(x=>x.ma===f.ma);
      if(i>=0){ ex[i]={...ex[i],gia:f.gia,slot:f.slot,dur:f.dur,mb:f.mb}; store.set('extra_flights',ex); }
      else { ex.push({ma:f.ma,di:f.di,den:f.den,gia:f.gia,slot:f.slot,dur:f.dur,mb:f.mb,tg:f.tg,pt:f.pt,tk:f.tk}); store.set('extra_flights',ex); }
      logAct('Sửa chuyến bay', f.ma); pushSharedFlights('Sửa chuyến '+f.ma); toast('Đã lưu chuyến'); pQly();
    };
  });
}

function chenhHH(d){
  const now=(d.hh||0)+(d.hhPhatSinh||0);
  const hq=d.hhHomQua!=null?d.hhHomQua:now;
  const diff=now-hq;
  if(diff>0) return `<span style="color:#15803d">▲ Tăng ${vnd(diff)} so với hôm qua</span>`;
  if(diff<0) return `<span style="color:#b91c1c">▼ Giảm ${vnd(-diff)} so với hôm qua</span>`;
  return `<span class="mut">═ Không đổi so với hôm qua</span>`;
}

function pDaily(){
  if(!need(['Admin']))return;
  const dls=S.users.filter(u=>u.vai==='Dai ly');
  $('#panel').innerHTML=`<div class="card"><h2>Quản lý đại lý & hoa hồng</h2>
    <table><tr><th>Mã đại lý</th><th>Tên đại lý</th><th>Tỷ lệ hoa hồng</th><th>Hoa hồng</th><th>So với hôm qua</th></tr>
    ${dls.map(d=>`<tr>
      <td>${esc(d.ma)}</td><td>${esc(d.ten)}</td><td>${((d.tyle||0)*100).toFixed(1)}%</td>
      <td><b>${vnd((d.hh||0)+(d.hhPhatSinh||0))}</b></td><td>${chenhHH(d)}</td></tr>`).join('')}
    </table></div>`;
}

function pHH(){
  if(!need(['Dai ly']))return;
  const d=S.users.find(u=>u.ma===S.user.ma)||{};
  d.hhPhatSinh=store.get('hh_map',{})[S.user.ma]||0;
  $('#panel').innerHTML=`<div class="card"><h2>Hoa hồng — ${esc(S.user.ten)}</h2>
    <p>Hoa hồng hiện tại: <b>${vnd((d.hh||0)+(d.hhPhatSinh||0))}</b></p>
    <p>${chenhHH(d)}</p>
    <p class="mut">Tỷ lệ: ${((d.tyle||0)*100).toFixed(1)}% · Phát sinh khi đặt hộ trên demo.</p></div>`;
}

function pSS(){
  if(!need())return;
  if(isDL()){toast('Đại lý không dùng so sánh đại lý',false);return go('tim');}
  // Giá & hành trình lấy từ chuyến khách đã tìm / đã chọn — không bịa chuyến mới
  let legs = (S.book && Array.isArray(S.book.legs) && S.book.legs.length) ? S.book.legs.slice()
    : (S.compareCtx && Array.isArray(S.compareCtx.legs) && S.compareCtx.legs.length) ? S.compareCtx.legs.slice()
    : [];
  let detail = legs.map(m => S.flights.find(c => c.ma === m)).filter(Boolean);
  if(!detail.length && S.compareCtx && Array.isArray(S.compareCtx.list) && S.compareCtx.list.length){
    // Chưa bấm chọn chuyến: lấy chuyến rẻ nhất trong kết quả tìm gần nhất
    const sorted = S.compareCtx.list.slice().sort((a,b)=>(a.gia||0)-(b.gia||0));
    const pick = sorted[0];
    const full = S.flights.find(c => c.ma === pick.ma) || pick;
    legs = [full.ma];
    detail = [full];
  }
  if(!detail.length){
    $('#panel').innerHTML=`<div class="card"><h2>So sánh giá vé đại lý</h2>
      <p class="mut">Chưa có hành trình để so sánh giá. Hãy <b>Tìm chuyến bay</b> (và nên chọn một chuyến) trước, rồi quay lại mục này.</p>
      <button type="button" class="b" id="ssToTim">Tìm chuyến bay</button>
    </div>`;
    if($('#ssToTim')) $('#ssToTim').onclick=()=>go('tim');
    return;
  }
  const gia = detail.reduce((s,c)=>s+(Number(c.gia)||0),0);
  const tuyen = detail.map(c => (c.di||'?')+'→'+(c.den||'?')).join(' + ');
  const maStr = legs.join(' + ');
  const ngay = (S.book && S.book.ngay) || (S.compareCtx && S.compareCtx.ngay) || S.ngayChon || ngayISO(0);
  const dls = S.users.filter(u => u.vai==='Dai ly');
  $('#panel').innerHTML=`<div class="card"><h2>So sánh giá vé đại lý</h2>
    <p>Hành trình đã chọn: <b>${esc(maStr)}</b> · ${esc(tuyen)} · Ngày <b>${esc(normNgay(ngay))}</b></p>
    <p class="mut">Giá quầy (cơ bản) theo chuyến đã tìm: <b>${vnd(gia)}</b>. Chỉ so sánh giá vé — chọn kênh rồi vào form đặt đúng hành trình này.</p>
    <div class="table-wrap"><table><tr><th>Kênh bán</th><th>Giá vé ước tính</th><th>Thao tác</th></tr>
    <tr><td><b>Quầy / trực tiếp</b></td><td>${vnd(gia)}</td>
      <td><button type="button" class="b s" data-d="">Đặt trực tiếp</button></td></tr>
    ${dls.map(d=>{
      const g=Math.round(gia*(1-(d.tyle||0)*0.5));
      return `<tr><td>${esc(d.ten||d.ma)}</td><td>${vnd(g)}</td>
        <td><button type="button" class="b s" data-d="${esc(d.ma)}">Đặt qua đại lý</button></td></tr>`;
    }).join('')}
    </table></div></div>`;
  $$('#panel [data-d]').forEach(b=>b.onclick=()=>{
    S.book={
      legs: legs.slice(),
      ngay: ngay,
      preferDl: b.dataset.d || ''
    };
    S.compareCtx={...(S.compareCtx||{}), legs: legs.slice(), ngay};
    go('dat');
  });
}

function pLog(){
  if(!need())return;
  let logs=store.get('activity_log',[]);
  if(isHK()||isDL()) logs=logs.filter(l=>l.user===S.user.ma);
  $('#panel').innerHTML=`<div class="card"><h2>Lịch sử hoạt động</h2>
    ${logs.length?`<div class="table-wrap"><table><tr><th>Thời gian</th><th>Người dùng</th><th>Vai trò</th><th>Hành động</th><th>Chi tiết</th></tr>
    ${logs.slice(0,80).map(l=>`<tr><td>${esc(l.at)}</td><td>${esc(l.user)}</td><td>${esc(vaiTen(l.vai))}</td>
      <td>${esc(l.hanhDong)}</td><td class="mut">${esc(l.chiTiet)}</td></tr>`).join('')}</table></div>`
    :'<p class="mut">Chưa có hoạt động.</p>'}</div>`;
}



/* ===== Bản đồ tuyến bay (Leaflet) — minh họa theo lịch, KHÔNG phải GPS ADS-B ===== */
const SB_COORDS = {
  SGN:[10.8188,106.6519], HAN:[21.2212,105.8072], DAD:[16.0439,108.199],
  CXR:[12.2275,109.192], PQC:[10.1698,103.9931], DLI:[11.7500,108.367],
  HUI:[16.4015,107.7026], HPH:[20.8194,106.725], VCA:[10.0851,105.7114],
  VII:[18.7376,105.6708], THD:[19.9017,105.4678], VDH:[17.515,106.5906],
  UIH:[13.955,109.042], VCL:[15.4033,108.706], PXU:[14.0045,108.017],
  TBB:[13.0496,109.3337], BMV:[12.6683,108.1203], CAH:[9.1777,105.1628],
  VDO:[21.1178,107.414], DIN:[21.397,103.008], SQH:[21.217,104.033],
  PHA:[11.633,108.95], VCS:[8.7318,106.6325]
};
function loadLeaflet(){
  return new Promise((resolve,reject)=>{
    if(window.L) return resolve(window.L);
    if(!document.getElementById('leafletCss')){
      const l=document.createElement('link'); l.id='leafletCss'; l.rel='stylesheet';
      l.href='https://unpkg.com/leaflet@1.9.4/dist/leaflet.css'; document.head.appendChild(l);
    }
    const s=document.createElement('script');
    s.src='https://unpkg.com/leaflet@1.9.4/dist/leaflet.js';
    s.onload=()=>resolve(window.L);
    s.onerror=()=>reject(new Error('Leaflet load fail'));
    document.head.appendChild(s);
  });
}
/** Điểm trên cung Bézier giữa 2 sân bay; bend khác nhau cho mỗi chuyến */
function arcPoint(a,b,t,bend){
  const mx=(a[0]+b[0])/2, my=(a[1]+b[1])/2;
  const dx=b[1]-a[1], dy=-(b[0]-a[0]);
  const len=Math.sqrt(dx*dx+dy*dy)||1;
  const cx=mx+(dx/len)*bend, cy=my+(dy/len)*bend;
  const u=1-t;
  return [u*u*a[0]+2*u*t*cx+t*t*b[0], u*u*a[1]+2*u*t*cy+t*t*b[1]];
}
function bearingDeg(from,to){
  const dLon=(to[1]-from[1])*Math.PI/180;
  const lat1=from[0]*Math.PI/180, lat2=to[0]*Math.PI/180;
  const y=Math.sin(dLon)*Math.cos(lat2);
  const x=Math.cos(lat1)*Math.sin(lat2)-Math.sin(lat1)*Math.cos(lat2)*Math.cos(dLon);
  return (Math.atan2(y,x)*180/Math.PI+360)%360;
}
/** Vị trí minh họa theo giờ lịch (0=chưa bay, 1=đã đến) — không phải GPS thật */
function simProgress(f, ngay){
  const st=getFlightStatus(f.ma, ngay);
  if(st==='HUY'||st==='HOAN') return 0;
  try{
    const [y,mo,da]=normNgay(ngay).split('-').map(Number);
    const slot=SLOT[(f.slot||0)%SLOT.length];
    const dep=new Date(y,mo-1,da,slot[0],slot[1],0,0);
    const arr=new Date(dep.getTime()+((f.dur||90)*60000));
    const now=new Date();
    if(now<dep) return 0.08; // gần điểm đi
    if(now>arr) return 0.92; // gần điểm đến
    return 0.08+0.84*((now-dep)/(arr-dep));
  }catch(e){ return 0.5; }
}
async function renderRouteMap(di, den, flights, ngay, showRoute){
  const el=document.getElementById('routeMap');
  if(!el) return;
  const day=normNgay(ngay||S.ngayChon||ngayISO(0));
  const wantRoute = !!(showRoute && di && den && di!==den && SB_COORDS[di] && SB_COORDS[den]);
  const gen = (S._mapGen = (S._mapGen||0) + 1);

  const ensureMap = async () => {
    const L = await loadLeaflet();
    if(gen !== S._mapGen) return null;
    // Luôn hủy map cũ để chắc chắn đổi nền (không giữ tile cache phiên trước)
    if(el._leaflet_map){
      try{ el._leaflet_map.remove(); }catch(e){}
      el._leaflet_map = null; el._leaflet_L = null; el._routeLayer = null;
    }
    el.innerHTML = '';
    el.classList.add('route-map');
    el.style.minHeight = '280px';
    el.style.width = '100%';
    el.style.height = '420px';
    const map = L.map(el, { scrollWheelZoom: true, worldCopyJump: false });
    el._leaflet_map = map;
    el._leaflet_L = L;

    /* === NỀN MỚI RÕ RỆT: Ảnh vệ tinh Esri (khác hẳn bản OSM xanh cũ) === */
    const sat = L.tileLayer(
      'https://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/tile/{z}/{y}/{x}',
      { maxZoom: 19, attribution: '© Esri World Imagery' }
    );
    sat.addTo(map);
    map._skyportTileVer = 'esri-satellite-2026-10';

    // Nhãn đường/chữ tham chiếu nhẹ (Esri Reference Overlay) — tùy chọn, lỗi thì bỏ
    try{
      L.tileLayer(
        'https://server.arcgisonline.com/ArcGIS/rest/services/Reference/World_Boundaries_and_Places/MapServer/tile/{z}/{y}/{x}',
        { maxZoom: 19, opacity: 0.85, attribution: '© Esri Labels' }
      ).addTo(map);
    }catch(e){}

    // Badge góc map — nhìn thấy ngay đã load bản mới
    const badge = document.createElement('div');
    badge.id = 'mapTileBadge';
    badge.style.cssText = 'position:absolute;z-index:1000;left:10px;bottom:28px;background:rgba(15,23,42,0.88);color:#f8fafc;padding:6px 10px;border-radius:8px;font-size:12px;font-weight:600;pointer-events:none;box-shadow:0 2px 8px rgba(0,0,0,.35)';
    badge.textContent = 'Nền: Ảnh vệ tinh Esri (bản mới)';
    el.style.position = 'relative';
    el.appendChild(badge);

    // Ranh giới 34 tỉnh/TP 2025 — chỉ viền, không phủ xanh
    try{
      const gj = await fetch('data/vn_tinh_34_2025.geojson?v=20261028', { cache: 'no-store' }).then(r => r.json());
      if(gen !== S._mapGen) return null;
      const seen = new Set();
      const feats = (gj.features||[]).filter(f => {
        const n = (f.properties && f.properties.name) || '';
        if(!n || seen.has(n)) return false;
        if(!f.geometry || !f.geometry.coordinates) return false;
        seen.add(n); return true;
      });
      L.geoJSON({ type: 'FeatureCollection', features: feats }, {
        style: () => ({ color: '#fbbf24', weight: 1.4, opacity: 0.9, fillOpacity: 0 }),
        onEachFeature: (feature, lyr) => {
          try{
            const n = (feature.properties && feature.properties.name) || '';
            if(n) lyr.bindTooltip(n, { sticky: true, direction: 'center', className: 'prov-tip' });
            const b = lyr.getBounds && lyr.getBounds();
            if(b && b.isValid && b.isValid()){
              const c = b.getCenter();
              if(c && isFinite(c.lat) && isFinite(c.lng)){
                L.marker(c, {
                  icon: L.divIcon({
                    className: 'city-label-marker',
                    html: '<span class="city-label">'+n+'</span>',
                    iconSize: [120,16], iconAnchor: [60,8]
                  }),
                  interactive: false, keyboard: false
                }).addTo(map);
              }
            }
          }catch(e){}
        }
      }).addTo(map);
    }catch(e){ console.warn('vn_tinh', e); }

    map.setView([16.0, 107.0], 5);
    setTimeout(() => { try{ map.invalidateSize(true); }catch(e){} }, 200);
    const h = document.getElementById('mapHint');
    if(h) h.textContent = (h.textContent||'').split(' · Nền:')[0] + ' · Nền: ảnh vệ tinh Esri (bản mới)';
    return { L, map };
  };

  try{
    const ctx = await ensureMap();
    if(!ctx || gen !== S._mapGen) return;
    const { L, map } = ctx;

    // Xóa lớp tuyến cũ
    if(el._routeLayer){
      try{ map.removeLayer(el._routeLayer); }catch(e){}
      el._routeLayer = null;
    }

    if(!wantRoute){
      const hint = document.getElementById('mapHint');
      if(hint && (!di || !den)) hint.textContent = 'Chọn điểm đi, điểm đến rồi bấm «Tìm chuyến bay» để hiện đường bay.';
      return;
    }

    const c1 = SB_COORDS[di], c2 = SB_COORDS[den];
    const group = L.layerGroup();
    el._routeLayer = group;

    const apIcon = (label) => L.divIcon({
      className: 'ap-marker',
      html: '<div class="ap-pin">'+label+'</div>',
      iconSize: [40,40], iconAnchor: [20,20]
    });
    const a1 = (S.airports||[]).find(a => a.maSB === di) || {};
    const a2 = (S.airports||[]).find(a => a.maSB === den) || {};
    L.marker(c1, { icon: apIcon(di) }).bindPopup('<b>'+esc(di)+'</b> — '+esc(a1.ten||'')+'<br>'+esc(a1.thanhPho||'')).addTo(group);
    L.marker(c2, { icon: apIcon(den) }).bindPopup('<b>'+esc(den)+'</b> — '+esc(a2.ten||'')+'<br>'+esc(a2.thanhPho||'')).addTo(group);

    // Một đường cong chính
    const bend = 0.55;
    const curve = [];
    for(let s = 0; s <= 48; s++) curve.push(arcPoint(c1, c2, s/48, bend));
    L.polyline(curve, { color: '#1d4ed8', weight: 4, opacity: 0.95 }).addTo(group);

    // Máy bay minh họa nếu có list kết quả cùng tuyến
    const list = (flights||[]).filter(f => f && f.di === di && f.den === den);
    const colors = ['#1d4ed8','#7c3aed','#db2777','#059669','#d97706','#0891b2'];
    list.forEach((f, i) => {
      const t = 0.15 + 0.7 * (list.length === 1 ? 0.5 : (i / (list.length - 1 || 1)));
      const pos = arcPoint(c1, c2, t, bend);
      const nxt = arcPoint(c1, c2, Math.min(0.99, t + 0.04), bend);
      const rot = bearingDeg(pos, nxt);
      const st = getFlightStatus(f.ma, day);
      const meta = getFlightStatusMeta(f.ma, day);
      const col = colors[i % colors.length];
      const hang = (f.ma||'').replace(/\d.*/,'') || '—';
      const html = '<div class="map-pop"><b>'+esc(f.ma)+'</b> · '+esc(hang)+'<br>'+
        esc(f.mb||'Máy bay')+'<br>'+esc(f.di)+' → '+esc(f.den)+'<br>'+
        'Ngày <b>'+esc(day)+'</b> · '+esc(f.gioDi||'')+' → '+esc(f.gioDen||'')+'<br>'+
        '<span style="color:'+(st==='BINH_THUONG'?'#15803d':'#b45309')+'">'+esc(ttLabel(st))+(meta&&meta.note?(' · '+esc(meta.note)):'')+'</span>'+
        (st!=='HUY'&&S.user&&S.user.vai==='Hanh khach'?'<br><button type="button" class="b s" onclick="window.__bookFromMap&&window.__bookFromMap(\''+esc(f.ma)+'\')">Chọn chuyến</button>':'')+
        '</div>';
      if(pos && isFinite(pos[0]) && isFinite(pos[1])){
        L.marker(pos, {
          icon: L.divIcon({
            className: 'pl-marker',
            html: '<div class="pl-ico" style="transform:rotate('+((rot||0)-45)+'deg);color:'+col+'">✈</div>',
            iconSize: [28,28], iconAnchor: [14,14]
          })
        }).bindPopup(html).addTo(group);
      }
    });

    group.addTo(map);
    try{
      const bb = L.latLngBounds([c1, c2]);
      if(bb && bb.isValid && bb.isValid()) map.fitBounds(bb.pad(0.4));
      else map.setView([(c1[0]+c2[0])/2, (c1[1]+c2[1])/2], 6);
    }catch(e){ map.setView([16.0, 107.0], 5); }

    const hint = document.getElementById('mapHint');
    if(hint) hint.textContent = 'Tuyến '+di+' → '+den+(list.length?(' · '+list.length+' chuyến'):'');
    try{ el.scrollIntoView({ behavior: 'smooth', block: 'nearest' }); }catch(e){}
    setTimeout(() => { try{ map.invalidateSize(true); }catch(e){} }, 150);

    window.__bookFromMap = (ma) => { S.book = { legs: [ma], ngay: S.ngayChon }; go('dat'); };
  }catch(e){
    el.innerHTML = '<p class="mut">Không tải được bản đồ: '+esc(String(e && e.message || e))+'</p>';
  }
}




/* Chat AI box */

function dsKhuyenMai(){
  /* Chỉ mã đang/sắp diễn ra từ ~25/10/2026 — bỏ lễ đã qua */
  return [
    {ma:'HALLOWEEN', ten:'Halloween bay đêm', ngay:'31/10/2026', giam:12, moTa:'Giảm 12% chuyến bay đêm (sau 18:00).', mau:'#f97316', icon:'🎃', hang:null, routes:null, diem:null},
    {ma:'DOUBLE11', ten:'Siêu sale 11/11', ngay:'11/11/2026', giam:20, moTa:'Giảm 20% mọi chặng nội địa (tối đa 200.000đ).', mau:'#8b5cf6', icon:'🎯', hang:null, routes:null, diem:null},
    {ma:'NHAGIAO', ten:'Ngày Nhà giáo 20/11', ngay:'20/11/2026', giam:15, moTa:'Giảm 15% mọi hạng ghế.', mau:'#ef4444', icon:'📚', hang:null, routes:null, diem:null},
    {ma:'BLACKFRIDAY', ten:'Black Friday', ngay:'28/11/2026', giam:18, moTa:'Giảm 18% hạng Phổ thông & Tiết kiệm.', mau:'#111827', icon:'🖤', hang:['PT','TK'], routes:null, diem:null},
    {ma:'DALAT', ten:'Săn vé Đà Lạt', ngay:'Tháng 11–12/2026', giam:18, moTa:'Chặng có Đà Lạt (DLI).', mau:'#ec4899', icon:'🌸', hang:null, routes:null, diem:['DLI']},
    {ma:'NOEL', ten:'Giáng sinh', ngay:'24–25/12/2026', giam:15, moTa:'Giảm 15% + ưu tiên chọn ghế.', mau:'#16a34a', icon:'🎄', hang:null, routes:null, diem:null},
    {ma:'NEWYEAR27', ten:'Chào năm mới 2027', ngay:'01/01/2027', giam:12, moTa:'Giảm 12% mọi chặng nội địa.', mau:'#0ea5e9', icon:'🎆', hang:null, routes:null, diem:null},
    {ma:'TET2027', ten:'Tết Nguyên đán 2027', ngay:'Tết Ất Tỵ (2/2027)', giam:10, moTa:'Giảm 10% + đổi lịch 1 lần miễn phí.', mau:'#f59e0b', icon:'🧧', hang:null, routes:null, diem:null},
    {ma:'CUOITUAN', ten:'Bay cuối tuần', ngay:'T7–CN', giam:8, moTa:'Áp dụng thứ 7 & Chủ nhật, mọi chặng.', mau:'#10b981', icon:'✈️', hang:null, routes:null, diem:null},
    {ma:'MEMBER', ten:'Thành viên mới', ngay:'Ongoing', giam:10, moTa:'Vé đầu tiên của hành khách mới đăng ký.', mau:'#3b82f6', icon:'🎁', hang:null, routes:null, diem:null, member:true},
    {ma:'SGN_HAN', ten:'Bay trục SGN–HAN', ngay:'Ongoing', giam:12, moTa:'Chỉ áp dụng chiều SGN↔HAN.', mau:'#6366f1', icon:'📍', hang:null, routes:[['SGN','HAN'],['HAN','SGN']], diem:null},
  ];
}
function voucherChoChuyen(legs, hang){
  const fl = S.flights||[];
  const cbs = (legs||[]).map(m => fl.find(c=>c.ma===m)).filter(Boolean);
  if(!cbs.length) return [];
  const diem = new Set();
  cbs.forEach(c=>{ diem.add(c.di); diem.add(c.den); });
  return dsKhuyenMai().filter(v=>{
    if(v.member && store.get('ves',[]).some(x=>x.user===(S.user&&S.user.ma))) return false;
    if(v.hang && hang && !v.hang.includes(hang)) return false;
    if(v.diem && !v.diem.some(d=>diem.has(d))) return false;
    if(v.routes){
      const ok = cbs.some(c => v.routes.some(([a,b])=>c.di===a&&c.den===b));
      if(!ok) return false;
    }
    return true;
  });
}



function pTraCuu(){
  if(!need()) return;
  $('#panel').innerHTML = `${bannerCanhBao()}
  <div class="card"><h2>Tra cứu mã chuyến / mã vé</h2>
    <p class="mut">Nhập mã chuyến bay (ví dụ VN100) hoặc mã vé để kiểm tra thông tin và xem bạn đã đặt chưa.</p>
    <form id="fTra" class="search-bar">
      <div class="field" style="grid-column:span 2"><label>Mã chuyến bay hoặc mã vé</label>
        <input name="ma" id="maTra" placeholder="VN100 hoặc VXXXX" required style="text-transform:uppercase"></div>
      <button class="b search-btn" type="submit">Tra cứu</button>
    </form>
  </div><div id="resTra"></div>`;
  $('#fTra').onsubmit = e => {
    e.preventDefault();
    const ma = ($('#maTra').value||'').trim().toUpperCase();
    if(!ma) return toast('Nhập mã', false);
    logAct('Tra cứu mã', ma);
    const flight = S.flights.find(c => (c.ma||'').toUpperCase() === ma);
    let ves = store.get('ves', []);
    // filter by role
    if(isHK()) ves = ves.filter(v => v.user===S.user.ma || v.kh===S.user.ma);
    else if(isDL()) ves = ves.filter(v => v.dl===S.user.ma || v.user===S.user.ma);
    // admin: all
    const byVe = ves.filter(v => (v.ma||'').toUpperCase() === ma);
    const byCb = ves.filter(v => (v.cb||'').toUpperCase() === ma);
    let html = '';
    if(flight){
      html += `<div class="card"><h3>Chuyến bay ${esc(flight.ma)}</h3>
        <div class="table-wrap"><table>
          <tr><th>Tuyến</th><th>Giờ đi</th><th>Giờ đến</th><th>Giá cơ bản</th><th>Ghế (TG/PT/TK)</th></tr>
          <tr><td>${esc(flight.di)} → ${esc(flight.den)}</td><td>${esc(flight.gioDi||'')}</td>
            <td>${esc(flight.gioDen||'')}</td><td>${vnd(flight.gia)}</td>
            <td>${flight.tg||8}/${flight.pt||36}/${flight.tk||18}</td></tr>
        </table></div></div>`;
    }
    const booked = byVe.length ? byVe : byCb;
    if(booked.length){
      html += `<div class="card"><h3>Vé liên quan (${booked.length})</h3>
        <div class="table-wrap"><table>
          <tr><th>Mã vé</th><th>Chuyến</th><th>Tuyến</th><th>Hạng</th><th>Ghế</th><th>Giá</th><th>Check-in</th><th>Thanh toán</th></tr>
          ${booked.map(v=>`<tr>
            <td><b>${esc(v.ma)}</b></td><td>${esc(v.cb)}</td><td>${esc(v.tuyen)}</td>
            <td>${esc(hangTen(v.hang))}</td><td>${esc(v.ghe)}</td><td>${vnd(v.gia)}</td>
            <td>${v.ci?'Đã check-in':'Chưa'}</td>
            <td>${v.paid?'Đã thanh toán':'Chưa thanh toán'}</td>
          </tr>`).join('')}
        </table></div>
        <p class="mut" style="color:#15803d;margin-top:8px">✓ Có vé liên quan đến mã này trên tài khoản / phạm vi của bạn.</p>
      </div>`;
    } else if(flight){
      html += `<div class="card mut">Bạn chưa đặt vé cho chuyến <b>${esc(ma)}</b>${isAdmin()?' trong hệ thống (hoặc chưa có vé nào).':'.'}</div>`;
    }
    if(!flight && !booked.length){
      html = `<div class="card mut">Không tìm thấy mã <b>${esc(ma)}</b> trong danh sách chuyến bay hoặc vé đã đặt.</div>`;
    }
    $('#resTra').innerHTML = html;
  };
}

function pKhuyenMai(){
  const list = dsKhuyenMai();
  $('#panel').innerHTML = `
  <div class="card"><h2>Ưu đãi & mã giảm giá</h2>
    <p class="mut">Danh sách ưu đãi. Khi đặt vé hệ thống chỉ hiện mã phù hợp chuyến của bạn.</p></div>
  <div class="promo-grid">
    ${list.map(p => `
      <div class="promo-card" style="--pc:${p.mau}">
        <div class="promo-badge">${p.icon} −${p.giam}%</div>
        <h3>${esc(p.ten)}</h3>
        <div class="promo-date">Áp dụng: <b>${esc(p.ngay)}</b></div>
        <p>${esc(p.moTa)}</p>
        <div class="promo-code">Mã: <b>${esc(p.ma)}</b></div>
        <button type="button" class="b s" data-km="${esc(p.ma)}" data-g="${p.giam}">Dùng mã này</button>
      </div>`).join('')}
  </div>`;
  $$('[data-km]').forEach(b => b.onclick = () => {
    store.set('ma_km', { ma: b.dataset.km, giam: +b.dataset.g });
    toast('Đã chọn mã ' + b.dataset.km + ' (−' + b.dataset.g + '%)');
    logAct('Chọn mã KM', b.dataset.km);
  });
}

function pLienHe(){
  $('#panel').innerHTML = `
  <div class="card"><h2>Liên hệ chúng tôi</h2>
    <p class="mut">Tổng đài và kênh hỗ trợ Airline System Management — phản hồi trong giờ hành chính.</p>
  </div>
  <div class="contact-grid">
    <div class="card contact-item">
      <div class="ci">📞</div>
      <h3>Hotline</h3>
      <p><b>1900 6868</b></p>
      <p class="mut">08:00 – 22:00 hàng ngày</p>
    </div>
    <div class="card contact-item">
      <div class="ci">✉️</div>
      <h3>Email</h3>
      <p><b>support@aerobook.vn</b></p>
      <p class="mut">Phản hồi trong 24 giờ</p>
    </div>
    <div class="card contact-item">
      <div class="ci">💬</div>
      <h3>Chat trên web</h3>
      <p><b>Nút AI góc phải</b></p>
      <p class="mut">Hỗ trợ nhanh đặt vé & lịch bay</p>
    </div>
    <div class="card contact-item">
      <div class="ci">📍</div>
      <h3>Văn phòng</h3>
      <p><b>TP. Hồ Chí Minh</b></p>
      <p class="mut">Airline System Management</p>
    </div>
  </div>
  <div class="card">
    <h3>Gửi góp ý</h3>
    <form id="fContact" class="form-grid">
      <div class="field"><label>Họ tên</label><input name="ten" required></div>
      <div class="field"><label>Email hoặc SĐT</label><input name="lienhe" required></div>
      <div class="field" style="grid-column:1/-1"><label>Nội dung</label><input name="nd" required placeholder="Mô tả vấn đề hoặc góp ý..."></div>
      <div class="form-actions"><button class="b" type="submit">Gửi liên hệ</button></div>
    </form>
  </div>`;
  $('#fContact').onsubmit = e => {
    e.preventDefault();
    const f = Object.fromEntries(new FormData(e.target));
    const list = store.get('lien_he', []);
    list.unshift({ ...f, at: new Date().toLocaleString('vi-VN') });
    store.set('lien_he', list.slice(0, 50));
    logAct('Gửi liên hệ', f.ten);
    toast('Đã gửi liên hệ. Cảm ơn bạn!');
    e.target.reset();
  };
}


function ensureChat(){
  if($('#aiChat')) return;
  const box=document.createElement('div'); box.id='aiChat';
  box.innerHTML=`
  <button type="button" id="aiToggle" title="Trợ lý AI" aria-label="Mở chat AI">
    <span class="ai-fab-ring"></span>
    <svg viewBox="0 0 48 48" width="32" height="32" aria-hidden="true">
      <defs>
        <linearGradient id="aiG" x1="0" y1="0" x2="1" y2="1">
          <stop offset="0%" stop-color="#3b82f6"/><stop offset="100%" stop-color="#1d4ed8"/>
        </linearGradient>
      </defs>
      <circle cx="24" cy="24" r="22" fill="url(#aiG)" stroke="#fff" stroke-width="3"/>
      <ellipse cx="24" cy="26" rx="11" ry="8.5" fill="#fff"/>
      <circle cx="19.5" cy="25" r="2.4" fill="#1e3a8a"/>
      <circle cx="28.5" cy="25" r="2.4" fill="#1e3a8a"/>
      <path d="M17.5 30.5c2.2 1.8 10.8 1.8 13 0" stroke="#1e3a8a" stroke-width="2" fill="none" stroke-linecap="round"/>
      <rect x="14.5" y="12" width="4" height="5.5" rx="2" fill="#93c5fd"/>
      <rect x="29.5" y="12" width="4" height="5.5" rx="2" fill="#93c5fd"/>
    </svg>
  </button>
  <div id="aiPanel" hidden>
    <div class="ai-head" id="aiDrag">
      <span>✦ Trợ lý bay AI</span>
      <button type="button" id="aiClose" title="Đóng">×</button>
    </div>
    <div id="aiMsgs"></div>
    <form id="aiForm"><input id="aiIn" placeholder="Hỏi về đặt vé, chuyến bay..." autocomplete="off"/><button class="b s" type="submit">Gửi</button></form>
  </div>`;
  document.body.appendChild(box);

  if(!$('#aiStyle')){
    const css=document.createElement('style'); css.id='aiStyle';
    css.textContent=`
#aiChat{position:fixed;right:18px;bottom:18px;z-index:1300;width:0;height:0;pointer-events:none}
#aiToggle{
  position:fixed;right:18px;bottom:18px;z-index:1301;pointer-events:auto;
  width:60px;height:60px;border-radius:50%;border:3px solid #fff;padding:0;
  background:linear-gradient(145deg,#60a5fa,#2563eb 55%,#1d4ed8);
  cursor:grab;
  box-shadow:0 0 0 4px rgba(37,99,235,.35), 0 12px 32px rgba(0,0,0,.45);
  display:flex;align-items:center;justify-content:center;
  transition:transform .15s ease, box-shadow .15s ease;
}
#aiToggle .ai-fab-ring{
  position:absolute;inset:-6px;border-radius:50%;
  border:2px solid rgba(255,255,255,.55);
  pointer-events:none;
}
#aiToggle:hover{transform:scale(1.08);box-shadow:0 0 0 5px rgba(37,99,235,.45),0 14px 36px rgba(0,0,0,.5)}
#aiToggle:active{cursor:grabbing}
#aiToggle svg{position:relative;z-index:1;filter:drop-shadow(0 1px 2px rgba(0,0,0,.25))}
#aiPanel{
  position:fixed;right:18px;bottom:86px;z-index:1301;pointer-events:auto;
  width:min(380px,calc(100vw - 20px));height:min(460px,70vh);
  background:#fff;border-radius:18px;
  box-shadow:0 20px 50px rgba(0,0,0,.28);
  display:flex;flex-direction:column;overflow:hidden;
  border:1px solid rgba(148,163,184,.35);
}
#aiPanel[hidden]{display:none !important}
.ai-head{
  background:linear-gradient(135deg,#1e3a8a 0%,#2563eb 50%,#0ea5e9 100%);
  color:#fff;padding:12px 14px;font-weight:700;font-size:15px;
  display:flex;justify-content:space-between;align-items:center;
  cursor:move;user-select:none;touch-action:none;
  text-shadow:0 1px 2px rgba(0,0,0,.25);
}
#aiClose{background:rgba(255,255,255,.2);border:0;color:#fff;font-size:22px;line-height:1;width:34px;height:34px;border-radius:10px;cursor:pointer}
#aiClose:hover{background:rgba(255,255,255,.35)}
#aiMsgs{flex:1;overflow:auto;padding:12px;font-size:13.5px;background:#f1f5f9}
.ai-b{max-width:88%;margin:8px 0;padding:10px 12px;border-radius:12px;white-space:pre-wrap;line-height:1.45}
.ai-u{background:#dbeafe;margin-left:auto;color:#0f172a}
.ai-a{background:#fff;border:1px solid #e2e8f0;color:#0f172a}
#aiForm{display:flex;gap:8px;padding:10px;border-top:1px solid #e2e8f0;background:#fff}
#aiIn{flex:1;height:38px;border:1px solid #cbd5e1;border-radius:10px;padding:0 12px;font:inherit}
`;
    document.head.appendChild(css);
  }

  const panel = () => $('#aiPanel');
  const openChat = () => { const p=panel(); if(p){ p.hidden=false; p.style.display='flex'; } };
  const closeChat = () => { const p=panel(); if(p){ p.hidden=true; p.style.display='none'; } };
  const toggleChat = () => {
    const p=panel(); if(!p) return;
    if (p.hidden || p.style.display==='none') openChat(); else closeChat();
  };
  // Bấm nút AI: mở/đóng (không bị drag chặn)
  const tog = $('#aiToggle');
  if (tog) {
    tog.style.pointerEvents = 'auto';
    tog.style.zIndex = '1300';
    tog.addEventListener('click', (ev) => {
      if (window._aiDragMoved) { window._aiDragMoved = false; return; }
      ev.preventDefault();
      toggleChat();
    });
  }
  const closeBtn = $('#aiClose');
  if (closeBtn) {
    closeBtn.onclick = (ev) => { ev.preventDefault(); ev.stopPropagation(); closeChat(); };
  }
  const form = $('#aiForm');
  if (form) {
    form.onsubmit = e => {
      e.preventDefault();
      const q = ($('#aiIn').value||'').trim();
      if (!q) return;
      $('#aiIn').value = '';
      addMsg(q, true);
      addMsg(traLoiAI(q), false);
    };
  }
  addMsg('Xin chào! Mình hỗ trợ đặt vé và hàng không trên web này.', false);

  // Kéo nút AI: chỉ khi di chuyển > 10px mới coi là kéo; không preventDefault lúc chạm
  function makeDrag(el, isToggle){
    if(!el) return;
    let ox=0, oy=0, sx=0, sy=0, dragging=false, moved=false;
    const down = (e) => {
      if (e.target && e.target.id === 'aiClose') return;
      const pt = e.touches ? e.touches[0] : e;
      dragging = true; moved = false; window._aiDragMoved = false;
      sx = pt.clientX; sy = pt.clientY;
      const r = el.getBoundingClientRect();
      ox = sx - r.left; oy = sy - r.top;
    };
    const move = (e) => {
      if(!dragging) return;
      const pt = e.touches ? e.touches[0] : e;
      if (Math.abs(pt.clientX-sx)+Math.abs(pt.clientY-sy) > 10) {
        moved = true;
        window._aiDragMoved = true;
        if (e.cancelable) e.preventDefault();
      } else {
        return;
      }
      let x = pt.clientX - ox, y = pt.clientY - oy;
      const target = isToggle ? el : panel();
      if (!target) return;
      const w = target.offsetWidth || 60, h = target.offsetHeight || 60;
      x = Math.max(8, Math.min(window.innerWidth - w - 8, x));
      y = Math.max(8, Math.min(window.innerHeight - h - 8, y));
      if (isToggle) {
        el.style.left = x + 'px';
        el.style.top = y + 'px';
        el.style.right = 'auto';
        el.style.bottom = 'auto';
        const p = panel();
        if (p && !p.hidden) {
          let px = x - (p.offsetWidth - el.offsetWidth);
          let py = y - p.offsetHeight - 12;
          px = Math.max(8, Math.min(window.innerWidth - p.offsetWidth - 8, px));
          py = Math.max(8, Math.min(window.innerHeight - p.offsetHeight - 8, py));
          p.style.left = px + 'px'; p.style.top = py + 'px';
          p.style.right = 'auto'; p.style.bottom = 'auto';
        }
      } else {
        target.style.left = x + 'px';
        target.style.top = y + 'px';
        target.style.right = 'auto';
        target.style.bottom = 'auto';
      }
    };
    const up = () => { dragging = false; };
    el.addEventListener('mousedown', down);
    el.addEventListener('touchstart', down, {passive:true});
    window.addEventListener('mousemove', move);
    window.addEventListener('touchmove', move, {passive:false});
    window.addEventListener('mouseup', up);
    window.addEventListener('touchend', up);
  }
  makeDrag($('#aiToggle'), true);
  makeDrag($('#aiDrag'), false);
  // khi kéo head thì kéo cả panel
  (function(){
    const head = $('#aiDrag'), p = panel();
    if(!head||!p) return;
    let dragging=false, ox=0, oy=0, sx=0, sy=0;
    head.addEventListener('mousedown', e => {
      if (e.target.id==='aiClose') return;
      dragging=true; window._aiDragMoved=true;
      const r=p.getBoundingClientRect();
      ox=e.clientX-r.left; oy=e.clientY-r.top;
    });
    head.addEventListener('touchstart', e => {
      if (e.target.id==='aiClose') return;
      const pt=e.touches[0]; dragging=true;
      const r=p.getBoundingClientRect();
      ox=pt.clientX-r.left; oy=pt.clientY-r.top;
    }, {passive:true});
    window.addEventListener('mousemove', e => {
      if(!dragging) return;
      let x=e.clientX-ox, y=e.clientY-oy;
      x=Math.max(8,Math.min(innerWidth-p.offsetWidth-8,x));
      y=Math.max(8,Math.min(innerHeight-p.offsetHeight-8,y));
      p.style.left=x+'px'; p.style.top=y+'px'; p.style.right='auto'; p.style.bottom='auto';
    });
    window.addEventListener('touchmove', e => {
      if(!dragging||!e.touches[0]) return;
      const pt=e.touches[0];
      let x=pt.clientX-ox, y=pt.clientY-oy;
      x=Math.max(8,Math.min(innerWidth-p.offsetWidth-8,x));
      y=Math.max(8,Math.min(innerHeight-p.offsetHeight-8,y));
      p.style.left=x+'px'; p.style.top=y+'px'; p.style.right='auto'; p.style.bottom='auto';
    }, {passive:true});
    window.addEventListener('mouseup', ()=>dragging=false);
    window.addEventListener('touchend', ()=>dragging=false);
  })();
}
function addMsg(t,user){const m=$('#aiMsgs'); if(!m)return; const d=document.createElement('div'); d.className='ai-b '+(user?'ai-u':'ai-a'); d.textContent=t; m.appendChild(d); m.scrollTop=m.scrollHeight;}
function traLoiAI(q){
  const raw = String(q||'').trim();
  if (!raw) return 'Bạn nhắn gì mình cũng được, gửi thử đi.';
  const s = raw.toLowerCase().normalize('NFD').replace(/[\u0300-\u036f]/g,'').replace(/đ/g,'d');
  const has = (...ks) => ks.some(k => s.includes(k));
  const hasAll = (...ks) => ks.every(k => s.includes(k));

  /* --- ngoài lĩnh vực --- */
  if (has(
    'giai toan','dao ham','tich phan','vat ly hat nhan','hoa huu co','sinh hoc phan tu',
    'triet hoc mac','luat hinh su','chuan doan benh','ke don thuoc',
    'viet giup code java','viet giup code python','thuat toan sap xep bai tap',
    'bai tap c++ cau truc','thi dai hoc mon toan','cong thuc hoa hoc'
  ))
    return 'Phần này mình không hỗ trợ (môn/chuyên ngành khác). Hỏi chuyện khác hoặc về web/máy bay mình giúp được.';

  /* --- chào / tán gẫu --- */
  if (/^(hi+|hello|hey|hola)([\s,!.]*|$)/.test(s) || /^(hello ban|hi ban|xin chao|chao ban|chao)\b/.test(s) || has('xin chao'))
    return 'Chào bạn 👋 Bạn cần mình giúp gì không?';
  if (has('ban khoe','khoe khong','khoe ko','how are you'))
    return 'Mình vẫn ổn. Còn bạn thì sao?';
  if (has('cam on','thanks','thank you','tks'))
    return 'Có gì đâu, cần gì cứ hỏi tiếp nhé.';
  if (has('tam biet','bye','ngu ngon'))
    return 'Bye bạn, lúc nào cần thì nhắn lại nha.';
  if (has('haha','hihi','lol') || /\bkk\b/.test(s))
    return 'Haha 😄';
  if (/^(ok|oke|okie|uhm|vang|da|duoc|roi|good|nice)[\s!.]*$/.test(s))
    return 'Ừ, mình đang nghe đây.';
  if (has('ban la ai','may la ai','who are you'))
    return 'Mình là trợ lý trên web đặt vé này. Chat chơi cũng được, hỏi máy bay hay đặt vé cũng được.';
  if (has('biet code','code khong','code k'))
    return 'Biết sơ thôi, không làm hộ bài code. Còn web đặt vé hay máy bay thì hỏi mình được.';

  /* --- WEB CÓ GÌ / GIỚI THIỆU (bắt rộng ý người dùng) --- */
  if (
    has('chuc nang','tinh nang','gioi thieu web','airline system','skyport')
    || (has('web') && has('gom','co gi','nhung gi','nhung chuc','tinh nang','chuc nang','gioi thieu','la gi','hoat dong'))
    || (has('he thong') && has('gom','co gi','nhung gi','gioi thieu','la gi'))
    || (has('web') && has('hoi so','cho toi biet','ke xem','noi ve'))
    || hasAll('web','minh') && has('gi','gom','co')
  )
    return 'Web của mình gồm các phần chính sau:\n\n1) Hành khách: tìm chuyến bay, nối chuyến, đặt vé (chọn hạng ghế, ghế, hành lý, suất ăn, mã giảm giá), xem «Vé của tôi» để check-in / thanh toán / hủy hoặc đổi vé, so sánh đại lý, xem ưu đãi, tra cứu mã chuyến.\n2) Đại lý: tìm chuyến, đặt vé hộ khách, xem hoa hồng.\n3) Admin: quản lý hệ thống và quản lý đại lý (trên máy chủ), xem thống kê — không đặt vé cá nhân.\n\nNgoài ra còn cảnh báo chuyến trễ/hoãn và chat hỗ trợ như mình đang nói chuyện với bạn.\n\nBạn muốn mình nói kỹ phần nào (đặt vé, nối chuyến, đại lý…)?';

  /* --- nghiệp vụ --- */
  if (has('lich bay','gio bay','tim chuyen','tim ve'))
    return 'Vào menu «Chuyến bay» → chọn điểm đi và điểm đến → bấm Tìm chuyến → chọn ngày. Danh sách sẽ hiện mã chuyến, giờ bay, giá và ghế trống.';
  if (has('noi chuyen','qua canh','transit'))
    return 'Dùng menu «Nối chuyến»: chọn nơi đi–nơi đến, hệ thống gợi ý các chặng trung gian. Có thể đặt cả hành trình một lần.';
  if (has('dat ve','mua ve','cach dat'))
    return 'Cách đặt: tìm/chọn chuyến → chọn hạng ghế → chọn ghế → thêm hành lý/suất ăn/voucher nếu cần → xác nhận. Xong vào «Vé của tôi» để check-in và thanh toán.';
  if (has('hang ghe','thuong gia','pho thong','tiet kiem'))
    return 'Có 3 hạng: Thương gia, Phổ thông và Tiết kiệm. Giá và quyền lợi khác nhau; số ghế trống hiện khi bạn xem chuyến.';
  if (has('hanh ly','ky gui','xach tay'))
    return 'Xách tay mang lên cabin, ký gửi để dưới hầm. Lúc đặt vé trên web có chỗ chọn hành lý; quá mức sẽ phụ thu.';
  if (has('voucher','khuyen mai','uu dai','ma giam'))
    return 'Vào «Ưu đãi» xem mã đang có. Khi đặt vé, chỉ những mã phù hợp chuyến/hạng mới hiện để chọn.';
  if (has('check in','checkin','thanh toan','huy ve','doi ve'))
    return 'Trong «Vé của tôi» có đủ nút Check-in, Thanh toán, Hủy vé và Đổi vé. Bạn mở vé cần xử lý rồi bấm nút tương ứng.';
  if (has('dai ly','hoa hong','so sanh'))
    return 'Hành khách vào «So sánh đại lý» để xem giá. Đại lý xem hoa hồng trên menu của mình. Admin xem quản lý đại lý trên máy chủ.';
  if (has('dang nhap','dang ky','mat khau'))
    return 'Góc phải có Đăng nhập / Đăng ký. Hành khách tự đăng ký; đăng nhập bằng mã và mật khẩu. Quên mật khẩu thì cần đúng mã + email đã đăng ký.';
  if (has('san bay','delay','tre chuyen','hoan','may bay','hang bay','vietnam airlines','vietjet'))
    return 'Liên quan hàng không thì mình trả lời được. Trên web có danh sách sân bay/chuyến và khung cảnh báo trễ–hoãn. Bạn hỏi cụ thể hơn mình nói rõ hơn.';
  if (has('bay','ve may bay','hang khong','chuyen bay'))
    return 'Ok, mình hỗ trợ các câu về máy bay và đặt vé. Bạn đang cần tìm chuyến, đặt vé, hay hỏi kiến thức chung?';

  if (has('buon','vui','met','hom nay') && !has('bay','ve','web','chuyen'))
    return 'Mình nghe bạn kể được. Cứ nhắn tiếp nhé — hoặc nếu cần đặt vé gì mình chỉ luôn.';

  if (raw.length <= 2)
    return 'Bạn gửi thêm chút nội dung với, mình chưa nắm ý.';

  /* mặc định: đoán ý, không nhại lại nguyên câu kiểu “mình hiểu bạn đang nói…” */
  if (has('web','he thong','app','menu'))
    return 'Có vẻ bạn đang hỏi về web. Web có tìm/đặt vé, nối chuyến, quản lý vé, đại lý và admin. Bạn muốn nghe tóm tắt đầy đủ chức năng không?';
  if (has('giup','ho tro','chi','huong dan'))
    return 'Mình giúp được. Bạn nói rõ việc cần làm giúp mình (ví dụ: đặt vé SGN–HAN, hủy vé, xem ưu đãi…).';

  return 'Mình chưa chắc ý bạn lắm. Bạn hỏi ngắn lại giúp mình được không? Ví dụ: «web có gì», «cách đặt vé», «nối chuyến».';
}


(function logoHostBind(){
  const bind = () => {
    const targets = [
      document.querySelector('.brand'),
      document.querySelector('.logo-mark'),
      document.querySelector('.brand img'),
      document.querySelector('.logo-text')
    ].filter(Boolean);
    if (!targets.length) return;
    let n = 0, t = 0;
    const onClick = (e) => {
      e.preventDefault();
      e.stopPropagation();
      const now = Date.now();
      if (now - t > 2500) n = 0;
      t = now;
      n++;
      if (n < 5) {
        toast('Nhấp logo thêm ' + (5 - n) + ' lần để bật/tắt máy chủ');
        return;
      }
      n = 0;
      if (isHostMode()) {
        if (confirm('Tắt chế độ máy chủ trên trình duyệt này?')) tatMayChu();
      } else {
        batMayChu();
      }
    };
    targets.forEach(el => {
      if (el.dataset.hostBind) return;
      el.dataset.hostBind = '1';
      el.style.cursor = 'pointer';
      el.title = 'Nhấp 5 lần để bật/tắt máy chủ';
      el.addEventListener('click', onClick);
    });
  };
  if (document.readyState === 'loading') document.addEventListener('DOMContentLoaded', bind);
  else bind();
  setTimeout(bind, 500);
})();

(async()=>{await load();setNav();setWho();go(S.user?'tim':'home');ensureChat();})();
