// Generates the portal renumbering SQL and mapping from the client charts + DB export.
import fs from 'node:fs';import path from 'node:path';
const OUT=process.argv[2],TCD=process.argv[3];
const {portals,dbP,dbD,dbN,dbI}=JSON.parse(fs.readFileSync(OUT+'/data.json','utf8'));
const cp=new Map(portals.map(p=>[p.id,p]));const sp=new Map(dbP.map(p=>[p.id,p]));const I=new Map(dbI);const N=new Map(dbN.map(n=>[n.id,n]));
const b=fs.readFileSync(path.join(TCD,'TDest.tcd'));let o=0;const u8=()=>b[o++],u16=()=>{const v=b.readUInt16LE(o);o+=2;return v},u32=()=>{const v=b.readUInt32LE(o);o+=4;return v};const td=[];for(let c=u16();c>0;c--)td.push({from:u16(),to:u16(),price:u32(),npc:u16(),base:u8(),local:u8(),hub:u8(),d:u8(),c:u8(),b:u8(),p:u8(),map:u16(),ux:u8(),uz:u8()});
const tdNpc=new Map();for(const r of td)if(!tdNpc.has(r.npc))tdNpc.set(r.npc,r.from);
const inS=i=>(i>=9500&&i<=9520)||(i>=9700&&i<=9712)||(i>=9900&&i<=9969);
const is97=i=>i>=9700&&i<=9712;
const near=(s,m)=>{let best=null;for(const p of portals){if(p.mapId!==s.mapId)continue;const d=Math.hypot(p.x-s.x,p.z-s.z);if(!best||d<best.d)best={p,d}}return best&&best.d<=m?best:null};
const portalNpcs=[...I].filter(([n,v])=>N.get(n)?.type===8&&v.some(inS)).map(([n,v])=>{const inScope=v.filter(inS);if(inScope.length!==1)throw new Error('npc '+n+' has '+inScope);return {npc:n,own:inScope[0],td:tdNpc.get(n)}});
// T: an id as a destination (it owns the spawn). 97xx were already renumbered, so they stay.
const T=new Map();const extras=[];
for(const s of dbP.filter(s=>inS(s.id)).sort((a,c)=>a.id-c.id)){
 if(is97(s.id)||s.mapId==null){T.set(s.id,s.id);continue}
 const pos=near(s,30)?.p.id;const npcs=portalNpcs.filter(n=>n.own===s.id&&n.td).map(n=>n.td);
 const npc=npcs[0];if(pos&&npc&&pos!==npc)throw new Error('conflict '+s.id);
 const c=pos??npc;if(c)T.set(s.id,c);else extras.push(s.id)
}
extras.forEach((id,i)=>T.set(id,9980+i));if(extras.length>20)throw new Error('too many extras');
// O: an id as an NPC's own portal (it owns the outgoing rows).
const O=new Map();for(const n of portalNpcs){const v=n.td&&inS(n.td)?n.td:T.get(n.own);if(O.has(n.own)&&O.get(n.own)!==v)throw new Error('O conflict '+n.own);O.set(n.own,v)}
for(const d of dbD)if(inS(d.from)&&!O.has(d.from))O.set(d.from,T.get(d.from));
const inj=(m,l)=>{const r=new Map();for(const[k,v]of m){if(r.has(v))throw new Error(l+' collision '+k+' & '+r.get(v)+' → '+v);r.set(v,k)}return r};
const Ti=inj(T,'T'),Oi=inj(O,'O');
// New TPORTALCHART rows for the scope.
const ids=new Set([...T.values(),...O.values()]);const newP=[];
for(const x of [...ids].sort((a,c)=>a-c)){const tgt=Ti.has(x)?sp.get(Ti.get(x)):null;const own=Oi.has(x)?sp.get(Oi.get(x)):tgt;newP.push({id:x,country:own.country,local:own.local,spawn:tgt?tgt.spawn:0,cond:own.cond})}
// Outside scope stays. New destination rows and NPC rows.
const newD=dbD.map(d=>({...d,from:inS(d.from)?O.get(d.from):d.from,to:inS(d.to)?(T.get(d.to)??d.to):d.to}));
const seen=new Set();for(const d of newD){const k=d.from+'>'+d.to;if(seen.has(k))throw new Error('dest PK '+k);seen.add(k)}
const newOwn=portalNpcs.map(n=>({npc:n.npc,from:n.own,to:O.get(n.own)})).filter(n=>n.from!==n.to);
const scopeRows=dbP.filter(s=>inS(s.id));
const v=(a)=>a.join(',');
const sql=`-- Generated from the client charts. Renumbers mission portals so TPORTALCHART IDs mean what the client's TPortal.tcd says.
-- Run on a stopped or running server; TMapSvr only reads these tables at startup. Needs a TMapSvr restart to take effect.
SET NOCOUNT ON; SET XACT_ABORT ON;
IF OBJECT_ID('TPORTALCHART_bak_20261101') IS NOT NULL THROW 50000,'Already applied: backup tables exist',1;
IF NOT EXISTS(SELECT 1 FROM TPORTALCHART p JOIN TSPAWNPOSCHART s ON s.wID=p.wSpawnID WHERE p.wPortalID=9959 AND s.wMapID=703) THROW 50000,'Unexpected state: 9959 is not on map 703',1;
BEGIN TRAN;
SELECT * INTO TPORTALCHART_bak_20261101 FROM TPORTALCHART;
SELECT * INTO TDESTINATIONCHART_bak_20261101 FROM TDESTINATIONCHART;
SELECT * INTO TNPCITEMCHART_bak_20261101 FROM TNPCITEMCHART;
CREATE TABLE #p (id smallint PRIMARY KEY, c tinyint, l smallint, s smallint, b tinyint);
${newP.map(p=>`INSERT #p VALUES (${v([p.id,p.country,p.local,p.spawn,p.cond])});`).join('\n')}
CREATE TABLE #own (o smallint PRIMARY KEY, n smallint);
${[...O].filter(([a,c])=>a!==c).map(([a,c])=>`INSERT #own VALUES (${a},${c});`).join('\n')}
CREATE TABLE #tgt (o smallint PRIMARY KEY, n smallint);
${[...T].filter(([a,c])=>a!==c).map(([a,c])=>`INSERT #tgt VALUES (${a},${c});`).join('\n')}
DELETE TPORTALCHART WHERE wPortalID IN (${scopeRows.map(s=>s.id).join(',')});
INSERT TPORTALCHART (wPortalID,bCountry,wLocalID,wSpawnID,bCondition) SELECT id,c,l,s,b FROM #p;
SELECT COALESCE(ow.n,d.wPortalID) AS wPortalID, COALESCE(tg.n,d.wDestID) AS wDestID, d.dwPrice,d.bEnable,d.bCondition1,d.dwConditionID1,d.bCondition2,d.dwConditionID2,d.bCondition3,d.dwConditionID3
 INTO #d FROM TDESTINATIONCHART d LEFT JOIN #own ow ON ow.o=d.wPortalID LEFT JOIN #tgt tg ON tg.o=d.wDestID;
DELETE TDESTINATIONCHART; INSERT TDESTINATIONCHART SELECT * FROM #d;
UPDATE i SET dwItemID=ow.n FROM TNPCITEMCHART i JOIN TNPCCHART n ON n.wID=i.wNpcID AND n.bType=8 JOIN #own ow ON ow.o=i.dwItemID;
IF @@ROWCOUNT<>${newOwn.length} THROW 50000,'Unexpected number of NPC rows updated',1;
COMMIT;
PRINT 'Applied to '+DB_NAME();
`;
fs.writeFileSync(OUT+'/apply.sql',sql);
fs.writeFileSync(OUT+'/plan.json',JSON.stringify({T:[...T],O:[...O],extras,newP,newOwn}));
const q=x=>`"${String(x??'').replace(/"/g,'""')}"`;
fs.writeFileSync(OUT+'/mapping.csv',['old_id,new_id_as_destination,new_id_as_npc_portal,server_map,client_label_after,note',...[...new Set([...T.keys(),...O.keys()])].sort((a,c)=>a-c).filter(x=>T.get(x)!==x||(O.has(x)&&O.get(x)!==x)).map(x=>{const nt=T.get(x);const ex=extras.includes(x);return [x,nt,O.get(x)??'',sp.get(x)?.mapId??'',q(ex?'':cp.get(nt)?.name),ex?'no client name: hidden from menus until added':''].join(',')})].join('\n')+'\n');
console.log({T:T.size,Tchanged:[...T].filter(([a,c])=>a!==c).length,O:O.size,Ochanged:[...O].filter(([a,c])=>a!==c).length,extras,newOwn:newOwn.length,destRowsChanged:newD.filter((d,i)=>d.from!==dbD[i].from||d.to!==dbD[i].to).length});
console.log('sample',[9959,9960,9963,9501,9512,9958,9707,9702].map(x=>`${x}: T→${T.get(x)} O→${O.get(x)}`).join(' | '));
