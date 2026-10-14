/* CPH2581-15.0.0.860: symbol offsets into one kernel image. The banner
 * says Linux 6.1.118-android14-11-o-g818f2506c9e7; the Image comes from the
 * India CPH2573 15.0.0.860 OTA boot (boot SHA-256
 * c07125dc7048af293a24be05ff037a31a597fdd67c43042b6811d6ba05c5ccfb, Image
 * SHA-256 bacaae948fcb328352e84bb589626756c37dba358e789f451924402c3990a91b)
 * whose banner and config match the running kernel.
 *
 * Struct offsets belong to targets/kmi/android14-6.1.h; derive_offsets.py
 * re-derives these, and --compare re-checks them against the image.
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
/* kimage_voffset (runtime _text VA minus physical load). */
#define KIMAGE_VOFFSET_OFF                         0x015cdfe0ULL
/* selinux_state; .policy is at +0x50. */
#define SELINUX_STATE_OFF                          0x0228b8a8ULL
/* modprobe_path; unused under CONFIG_STATIC_USERMODEHELPER. */
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
/* tracing_mark_write: the executed copy is the local 0x4452fc duplicate;
 * the _THIS_IP_ site is fn+0x164. */
#define SLIDE_TRACE_MARK_IP_OFF                    0x00445460ULL

/* mm_struct.owner (Image BTF, not KMI-guaranteed). */
#define MM_OWNER_OFF                               0x338
#define SWAPPER_PG_DIR_OFF                         0x01bf5000ULL

/* Guess band [762,997] permille: this device's populated high bank. */
#define LIB_ZONEGUESS_BAND_LO                      762
#define LIB_ZONEGUESS_BAND_HI                      997

#endif /* CPH2581_15_0_0_860_H */
