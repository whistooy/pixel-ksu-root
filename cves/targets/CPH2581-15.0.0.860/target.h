/* targets/CPH2581-15.0.0.860/target.h, committed, not resolver-composed:
 * the release string does not match effective_target_header()'s "-ab(\d+)"
 * pattern, so composition would emit no kernel-image include.
 *
 * Describes the India CPH2573 15.0.0.860 boot Image (boot SHA-256
 * c07125dc7048af293a24be05ff037a31a597fdd67c43042b6811d6ba05c5ccfb, Image
 * SHA-256 bacaae948fcb328352e84bb589626756c37dba358e789f451924402c3990a91b);
 * its banner and embedded config match the running kernel.
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

/* The image this payload is valid for (payloads are per image, not per
 * device). */
#ifndef TARGET_KERNEL_ID
#define TARGET_KERNEL_ID "6.1.118-android14-11-o-g818f2506c9e7"
#endif

#include "../kernel/CPH2581-15.0.0.860.h"
#include "../kmi/android14-6.1.h"

/* Launch defaults from the 2026-10-13 A/B census; the environment
 * overrides each. */
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
