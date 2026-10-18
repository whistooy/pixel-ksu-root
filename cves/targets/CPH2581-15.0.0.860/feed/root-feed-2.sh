#!/system/bin/sh
# Feed section 2: the landed session (uid0 + caps, shell domain). Captures
# append to pre-created cap.* files; the ladder loads KernelSU first (the
# module comes from ksud itself unless a kernelsu.ko was staged).
# FFROOT_MINIMAL=1 skips the ladder (grants-only session).
# FFROOT_MINIMAL=1 skips the ladder (grants-only session).
date
getenforce
id
grep -E '^(Uid|Gid|CapEff|CapPrm|CapBnd)' /proc/self/status
{ date; getenforce; id; grep -E '^(Uid|Gid|CapEff|CapPrm|CapBnd)' /proc/self/status; } >> /data/local/tmp/cap.env 2>&1
sync
grep -E ' tracing_mark_write$| init_task$| selinux_state$| input_dev_list$| anon_pipe_buf_ops$' /proc/kallsyms >> /data/local/tmp/cap.kallsyms 2>&1
sync
head -100 /proc/kallsyms >> /data/local/tmp/cap.kallsyms 2>&1
sync
echo "kptr_restrict write attempt: $(echo 0 > /proc/sys/kernel/kptr_restrict 2>&1)"
sync
dmesg >> /data/local/tmp/cap.dmesg 2>&1
sync
cat /proc/iomem >> /data/local/tmp/cap.iomem 2>&1
sync
cat /proc/buddyinfo >> /data/local/tmp/cap.buddy 2>&1
sync
cat /sys/kernel/btf/vmlinux >> /data/local/tmp/cap.btf 2>&1
sync
{ date;
  echo "kallsyms: $(head -1 /proc/kallsyms 2>&1 | cut -c1-60)"
  echo "dmesg: $(dmesg 2>&1 | head -1 | cut -c1-60)"
  echo "iomem: $(head -1 /proc/iomem 2>&1 | cut -c1-60)"
  echo "slabinfo: $(head -1 /proc/slabinfo 2>&1 | cut -c1-60)"
  echo "proc/modules ksu-check: $(grep -ci kernelsu /proc/modules 2>&1)"
  echo "pagemap: $(dd if=/proc/self/pagemap bs=8 count=1 2>/dev/null | od -An -tx8 | tr -d ' ')"
} >> /data/local/tmp/cap.env 2>&1
sync
if [ "${FFROOT_MINIMAL:-0}" = "1" ]; then
  {
    echo "=== MINIMAL LADDER $(date) (grants-only; no module, no late-load) ==="
    echo "getenforce: $(getenforce 2>&1)"
    echo "minimal probe kptr: write=$( [ -w /proc/sys/kernel/kptr_restrict ] && echo 1 || echo 0) now=$(cat /proc/sys/kernel/kptr_restrict 2>&1)"
    echo "minimal probe dmesg: $(dmesg 2>&1 | head -1 | cut -c1-40)"
    ls -d /data/adb 2>/dev/null && echo "WARNING: /data/adb exists in minimal mode (leftover?)" || echo "no /data/adb (as expected)"
    echo "=== MINIMAL LADDER DONE ==="
  } >> /data/local/tmp/cap.minimal 2>&1
  sync
else
  {
    echo "=== LADDER $(date) ==="
  echo "getenforce: $(getenforce 2>&1)"
  echo "kptr write: $(echo 1 > /proc/sys/kernel/kptr_restrict 2>&1)"
  echo "kptr now: $(cat /proc/sys/kernel/kptr_restrict 2>&1)"
  echo "kallsyms head: $(head -1 /proc/kallsyms 2>&1 | cut -c1-48)"
  echo "--- ksuinit insmod (kallsyms pre-resolution; decisive + captured) ---"
  # Zeroed kallsyms (OPLUS clamps kptr_restrict) would make ksuinit resolve
  # every import to 0 and load a NULL-import module; only load with real
  # addresses visible.
  if [ "$(head -1 /proc/kallsyms 2>/dev/null | cut -d' ' -f1)" = "0000000000000000" ]; then
    echo "SKIPPED: kallsyms addresses zeroed (kptr_restrict pinned) -- a ksuinit load here would build a NULL-import module; use the generated-kallsyms + bind-mount route instead"
  else
    # ksud carries its own per-KMI module. Use a pushed kernelsu.ko when one
    # is there (the launcher form); otherwise late-load loads the embedded one.
    KSUD=/data/local/tmp/ksud-manager
    [ -x "$KSUD" ] || KSUD=/data/local/tmp/bakasu-ksud.so
    LL_LOADED=0
    if [ -f /data/local/tmp/kernelsu.ko ] && [ -x "$KSUD" ]; then
      echo "--- ksud insmod (explicit module) ---"
      "$KSUD" insmod /data/local/tmp/kernelsu.ko allow_shell=1 2>&1
      echo "insmod rc=$?"
    elif [ -x "$KSUD" ]; then
      echo "--- ksud late-load (no module file; ksud carries its own) ---"
      "$KSUD" late-load --allow-shell --kmi android14-6.1 2>&1
      echo "late-load rc=$?"
      LL_LOADED=1
    else
      echo "SKIPPED: no ksud staged (ksud-manager / bakasu-ksud.so)"
    fi
    grep -i kernelsu /proc/modules 2>&1
    if grep -qi kernelsu /proc/modules 2>/dev/null && [ -x "$KSUD" ] && [ "$LL_LOADED" = 0 ]; then
      echo "--- module LIVE: ksud late-load (full install; runs late-load stage scripts, system.prop, overlay mount, post-mount, service) ---"
      "$KSUD" late-load --allow-shell --kmi android14-6.1 2>&1
    fi
    # Second dmesg capture AFTER the load: the pre-load capture above cannot
    # show module/crown logs.
    sleep 3
    dmesg > /data/local/tmp/cap.dmesg.post 2>&1
    dmesg | grep -aE "KernelSU.*(is_manager|Crowning|manager|sepolicy|Loaded|init)" > /data/local/tmp/cap.manager 2>&1
    echo "--- cap.dmesg.post + cap.manager written (post-load window) ---"
  fi
  echo "=== LADDER DONE ==="
  } >> /data/local/tmp/cap.insmod 2>&1
fi
sync
# Persistent root: replace any stale daemon with one holding THIS session's
# cred (setsid survives the draw). Client: sh /data/local/tmp/su.sh "id"
pkill -f "[s]u_daemon.sh" 2>/dev/null
sleep 0.3
rm -f /data/local/tmp/su.*.out /data/local/tmp/su.*.cmd /data/local/tmp/.su_req
(setsid sh /data/local/tmp/su_daemon.sh >/dev/null 2>&1 &)
sleep 0.5
echo "su_daemon pid: $(pgrep -f '[s]u_daemon' | head -1) (holds this boot's root)"
echo "late-load at $(date) by feed pid $$" >> /data/local/tmp/ksu-late-load.log
echo "boot $(date)" >> /data/local/tmp/boot-marker.log
echo "=== SECTION 2 DONE (INSID v4 captures attempted) ==="
