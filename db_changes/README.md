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
