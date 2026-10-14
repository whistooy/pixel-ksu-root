# CPH2581-15.0.0.860 (OnePlus 12, EEA) — validated launch parameters

The FFWheel knobs are runtime `getenv` reads (upstream design); the recipe
does not pin them. These are the values the 2026-10-13 A/B campaigns
measured on this device (two 12- and 14-draw cells, pooled 8 complete
sessions in 22 verdict draws ≈ 36% vs the pre-boost ~17%, p ≈ 0.02):

```sh
FFWHEEL_SPRAY_CAP_MB=4096   # 3072->4096: +coverage; cells within noise
FFWHEEL_KEEP_FREE_MB=2048   # the ranked-#1 lever: allocator headroom
                             # for the reclaim term (the census's measured
                             # bottleneck) — the knob that doubled the rate
FFROOT_TRIGGER_TRIES=12      # 8->12: more reclaim rolls after a CONFIRMED
FFREAD_GUESS_ORACLE=1        # oracle mode: every draw is a root attempt or
FFREAD_ORACLE_METHOD=direct  #   a clean measured miss (no blind fires)
FFREAD_CROSS_NSPRAY=16       # the groom placement for the ask
FFROOT_CROSS_NSPRAY=512      # matched reclaim spray budget
FFROOT_RESOLVED_PAGE=1        # resolved-entry flow (krw endgame)
```

Draw anatomy at these settings: ~1 complete session per 3 verdict draws,
each draw hard-bounded ≤6 minutes; a CONFIRMED runs the full chain
(avtab grants -> uid0+full caps in u:r:shell:s0, enforcing untouched).
Tune ONE knob at a time against the draw-log verdicts, per the FFWheel
README's guidance.
