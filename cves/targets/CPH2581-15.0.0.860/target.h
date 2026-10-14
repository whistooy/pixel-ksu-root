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

#endif /* OFFSET_H */
