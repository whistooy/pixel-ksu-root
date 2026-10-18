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

## Capture feed

feed/*.sh runs after a landed session (root-feed-1: enforcing snapshot,
root-feed-2: landed session + su_daemon). The runner pushes it and the
stage setup pre-creates the cap.* files it appends to. Daemon client:
sh /data/local/tmp/su.sh "id".

The ladder loads KernelSU with the ksud the runner derived from the
installed manager (libksud.so -> /data/local/tmp/ksud-manager); late-load
carries its own module. A staged kernelsu.ko selects the explicit-insmod
form; do not stage the official tiann module, the manager rejects its cert.

The policy load runs by default: the runner builds policy.permissive from
the device's own policy and pushes it. FFROOT_MINIMAL=1 (or
FFROOT_POLICY_LOAD=0) keeps the grants-only session instead.
