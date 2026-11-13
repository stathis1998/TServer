// usage: validate.mjs BEFOREDIR AFTERDIR TCD : proves every menu lands in the same place and labels now match.
import fs from 'node:fs';import path from 'node:path';
const [,,BEF,AFT,TCD]=process.argv;
class R{constructor(b){this.b=b;this.o=0}u8(){return this.b.readUInt8(this.o++)}u16(){const v=this.b.readUInt16LE(this.o);this.o+=2;return v}u32(){const v=this.b.readUInt32LE(this.o);this.o+=4;return v}f32(){const v=this.b.readFloatLE(this.o);this.o+=4;return v}s(){let l=this.u8();if(l===0xff){l=this.u16();if(l===0xffff)l=this.u32()}const v=this.b.toString('latin1',this.o,this.o+l);this.o+=l;return v}}
const rd=f=>new R(fs.readFileSync(path.join(TCD,f)));
const pr=rd('TPortal.tcd');const client=new Map();for(let c=pr.u16();c>0;c--){const id=pr.u16(),name=pr.s(),region=pr.u16(),mapId=pr.u16(),x=pr.f32(),z=pr.f32();pr.u8();pr.u8();client.set(id,{id,name,region,mapId,x,z})}
const tdr=rd('TDest.tcd');const tdNpc=new Map();const tdPairs=new Set();for(let c=tdr.u16();c>0;c--){const from=tdr.u16(),to=tdr.u16();tdr.u32();const npc=tdr.u16();tdr.o+=7;tdr.u16();tdr.o+=2;if(!tdNpc.has(npc))tdNpc.set(npc,from);tdPairs.add(from+'>'+to)}
const load=dir=>{const d=l=>fs.readFileSync(path.join(dir,l+'.txt'),'utf8').split(/\r?\n/).filter(Boolean).map(x=>x.split('|'));const num=x=>x==='NULL'?null:Number(x);
 const P=new Map(d('portals').map(a=>[+a[0],{id:+a[0],country:+a[1],local:+a[2],spawn:+a[3],cond:+a[4],mapId:num(a[5]),x:num(a[6]),z:num(a[8])}]));
 const D=d('dests').map(a=>({from:+a[0],to:+a[1],rest:a.slice(2).join(','),en:+a[3],c:[+a[4],+a[6],+a[8]]}));
 const N=new Map(d('npcs').map(a=>[+a[0],{id:+a[0],type:+a[1],mapId:+a[4]}]));const I=d('npcitems').map(a=>[+a[0],+a[1]]);return{P,D,N,I}};
const B=load(BEF),A=load(AFT);
const PCT_COUNTRY=1;const check=(l,bad)=>{console.log((bad.length?'FAIL ':'PASS ')+l+(bad.length?` (${bad.length})`:''));if(bad.length)console.log(bad.slice(0,20).join('\n'))};
const menu=(W,npc)=>{const own=W.I.filter(([n])=>n===npc).map(([,i])=>i).filter(i=>W.P.has(i));const out=[];for(const o of own)for(const d of W.D.filter(d=>d.from===o)){const t=W.P.get(d.to);if(!t)continue;out.push([t.spawn,d.rest,d.c.includes(PCT_COUNTRY)?t.country:'-'].join('/'))}return out.sort().join(' ; ')};
const ownOf=(W,npc)=>W.I.filter(([n])=>n===npc).map(([,i])=>W.P.get(i)).filter(Boolean).map(p=>[p.country,p.local,p.cond].join('/')).sort().join(';');
const pn=[...B.N.values()].filter(n=>n.type===8);
check('V1 every portal NPC offers the same landing spots, prices and conditions',pn.filter(n=>menu(B,n.id)!==menu(A,n.id)).map(n=>`npc ${n.id}\n  before ${menu(B,n.id)}\n  after  ${menu(A,n.id)}`));
check('V2 every portal NPC keeps its own country/local/condition',pn.filter(n=>ownOf(B,n.id)!==ownOf(A,n.id)).map(n=>`npc ${n.id}: ${ownOf(B,n.id)} → ${ownOf(A,n.id)}`));
const keyI=x=>x.join('>');const nonPortal=(W)=>W.I.filter(([n])=>W.N.get(n)?.type!==8).map(keyI).sort().join();
check('V3 shop rows (non-portal NPCs) untouched',nonPortal(B)===nonPortal(A)?[]:['differs']);
const inS=i=>(i>=9500&&i<=9520)||(i>=9700&&i<=9712)||(i>=9900&&i<=9999);
const dump=W=>[...W.P.values()].filter(p=>!inS(p.id)).map(p=>JSON.stringify(p)).join();
check('V4 portal rows outside the mission range untouched',dump(B)===dump(A)?[]:['differs']);
const labels=W=>{const wrong=[],hidden=[];for(const d of W.D){if(!d.en||!inS(d.to))continue;const t=W.P.get(d.to),c=client.get(d.to);if(!t||t.mapId==null)continue;if(!c){hidden.push(d.to);continue}if(c.mapId!==t.mapId||Math.hypot(c.x-t.x,c.z-t.z)>=100)wrong.push(`${d.from}>${d.to} "${c.name}" client map ${c.mapId} real map ${t.mapId}`)}return{wrong,hidden:[...new Set(hidden)]}};
const lb=labels(B),la=labels(A);console.log(`labels before: ${lb.wrong.length} wrong, ${lb.hidden.length} hidden | after: ${la.wrong.length} wrong, hidden ids ${la.hidden.join(',')}`);
check('V5 no wrong labels left in the mission range',la.wrong);
const links=W=>pn.map(n=>{const c=tdNpc.get(n.id);if(!c||!inS(c))return null;const it=W.I.filter(([x])=>x===n.id).map(([,i])=>i);return it.includes(c)?null:`npc ${n.id} TDest ${c} DB ${it}`}).filter(Boolean);
console.log(`NPC links disagreeing with TDest in mission range: before ${links(B).length}, after ${links(A).length}`);check('V6 NPC links match client TDest in the mission range',links(A));
const pairs=W=>W.D.filter(d=>d.en&&inS(d.from)&&inS(d.to));const pct=W=>{const p=pairs(W);return `${p.filter(d=>tdPairs.has(d.from+'>'+d.to)).length}/${p.length}`};console.log('mission menu rows also present in client TDest: before',pct(B),'after',pct(A));
