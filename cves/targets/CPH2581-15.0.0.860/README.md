# CPH2581-15.0.0.860 (OnePlus 12, EEA) launch parameters

Measured on this device by the 2026-10-13 A/B campaigns (8 sessions in 22
verdict draws, ~36%):

```sh
FFWHEEL_SPRAY_CAP_MB=4096
FFWHEEL_KEEP_FREE_MB=2048
FFROOT_TRIGGER_TRIES=12
FFREAD_GUESS_ORACLE=1
FFREAD_ORACLE_METHOD=direct
FFREAD_CROSS_NSPRAY=16
FFROOT_CROSS_NSPRAY=512
FFROOT_RESOLVED_PAGE=1
```

A CONFIRMED runs the full chain (avtab grants, uid0 + full caps in
u:r:shell:s0, enforcing untouched). Tune one knob at a time.
