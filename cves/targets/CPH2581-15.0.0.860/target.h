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

#endif /* OFFSET_H */
