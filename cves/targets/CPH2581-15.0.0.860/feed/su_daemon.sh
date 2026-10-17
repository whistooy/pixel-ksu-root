#!/system/bin/sh
# su_daemon.sh: holds the landed session's cred and serves one-shot root
# command files. Regular-file protocol (.su_req + su.PID.cmd/.out) because
# FIFO creation is denied for the shell domain. Per-boot only.
cd /data/local/tmp
: > .su_req
echo "daemon up pid=$$ $(date)" >> su_daemon.log
while true; do
  pid=$(head -1 .su_req 2>/dev/null)
  if [ -n "$pid" ] && [ -f "su.$pid.cmd" ] && [ -f "su.$pid.out" ]; then
    : > .su_req
    echo "=== $(date) serving $pid" >> "su.$pid.out"
    sh "su.$pid.cmd" >> "su.$pid.out" 2>&1
    echo "=== SU-EOF" >> "su.$pid.out"
    rm -f "su.$pid.cmd"
    echo "served $pid" >> su_daemon.log
  fi
  sleep 1
done
