#!/system/bin/sh
# su.sh "commands": one-shot root via su_daemon (per-boot). The client runs
# as the plain shell, so it can create the request/response files it needs;
# the daemon appends the output and the SU-EOF sentinel.
P=$$
cd /data/local/tmp
: > "su.$P.out"
{ echo "$*"; } > "su.$P.cmd"
echo "$P" > .su_req
n=0
while ! grep -q "=== SU-EOF" "su.$P.out" 2>/dev/null; do
  sleep 1; n=$((n+1)); [ $n -ge 30 ] && { echo "SU-TIMEOUT (daemon alive? was a session landed?)"; exit 42; }
done
grep -v "=== SU-EOF" "su.$P.out"
rm -f "su.$P.out" "su.$P.cmd"
