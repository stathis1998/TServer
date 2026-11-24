# Database changes

One folder per change, named by date. Run `apply.sql` against the game database in date order, and `undo.sql` to reverse a change.
Some folders also hold `.tcd` files that go in the server's data folder, plus the original next to them.

`2026-11-13-soul-lottery-log` must be applied before starting a TMapSvr built from `c9df439` or later.
Without it, draws are not logged and the server log shows `SoulLotteryLog:` errors.

`2026-11-15-rebirth` must be applied before starting a TMapSvr, TBoWSvr and TBRSvr built from `9952504` or later, and before running the portal's `main` from `6753209` or later.
Without it the servers log `TCHARREBIRTHTABLE read failed` at every character load and give bonus 0, and the portal's stats pages and character list fail.

The portal's games need no server rebuild, only their scripts, in this order: `2026-11-14-portal-wheel`, `2026-11-14-portal-roulette`,
`2026-11-14-portal-slots`, `2026-11-15-portal-slots-jackpot`, `2026-11-15-portal-slots-modern`, `2026-11-15-portal-slots-cascades`.
Each slots script changes the tables of the ones before it. The portal refuses those games on a database without them.

`2026-11-16-rebirth-extras` (rebirth EXP and gold bonuses and a reward letter per rebirth) is applied after `2026-11-15-rebirth`,
before the portal's `feat/rebirth-extras` and before a TMapSvr built from `feature/rebirth-extras`. Out of order, that TMapSvr fails its rebirth read on a database without this script, so every reborn character also loses its stat bonus (it reads 0) until the script is applied.

`2026-11-23-steadfast-saddle` (renames item 31307 to Steadfast Saddle) goes with a TMapSvr built from `feature/steadfast-saddle`. Either order works: the server keys on the item ID, the name is only for people.

`2026-11-24-growth-potion-500` (item 30309 becomes a 500% Growth Potion (1 hour), +400% kill EXP) needs only a TMapSvr restart, no rebuild. Players also need the client charts from `TClient Saddle and 500 Potion.zip` for the right name and tooltip.
