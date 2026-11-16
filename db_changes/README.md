# Database changes

One folder per change, named by date. Run `apply.sql` against the game database in date order, and `undo.sql` to reverse a change.
Some folders also hold `.tcd` files that go in the server's data folder, plus the original next to them.

`2026-11-13-soul-lottery-log` must be applied before starting a TMapSvr built from `c9df439` or later.
Without it, draws are not logged and the server log shows `SoulLotteryLog:` errors.

`2026-11-15-rebirth` must be applied before starting a TMapSvr, TBoWSvr and TBRSvr built from `feature/rebirth-bonus`, and before the portal's `feat/rebirth` is merged.
Without it the servers log `TCHARREBIRTHTABLE read failed` at every character load and give bonus 0, and the portal's stats pages and character list fail.
