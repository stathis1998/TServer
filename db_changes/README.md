# Database changes

One folder per change, named by date. Run `apply.sql` against the game database in date order, and `undo.sql` to reverse a change.
Some folders also hold `.tcd` files that go in the server's data folder, plus the original next to them.

`2026-11-13-soul-lottery-log` must be applied before starting a TMapSvr built from `c9df439` or later.
Without it, draws are not logged and the server log shows `SoulLotteryLog:` errors.
