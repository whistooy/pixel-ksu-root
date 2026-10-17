#!/system/bin/sh
# Feed section 1: the landed chain's first child shell (shell domain, the
# session's grants). Append-only; the runner pre-creates every cap.* file.
# The `exit` hands the pipe to the section-2 shell the chain execs next.
id
grep -E '^(Uid|Gid|CapEff|CapPrm|CapBnd)' /proc/self/status
touch /data/local/tmp/_probe_create 2>&1
{ date; uname -a; uptime; getenforce; id; grep -E '^(Uid|Gid|CapEff|CapPrm|CapBnd)' /proc/self/status; } >> /data/local/tmp/cap.env 2>&1
sync
cat /proc/slabinfo >> /data/local/tmp/cap.slabinfo 2>&1
sync
cat /proc/zoneinfo >> /data/local/tmp/cap.zoneinfo 2>&1
sync
cat /proc/config.gz >> /data/local/tmp/cap.config 2>&1
sync
sync
echo "=== SECTION 1 DONE (pre-T2 captures synced) ==="
exit
