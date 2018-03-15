#ifndef __RALINK_FLASH_H__
#define __RALINK_FLASH_H__

#define BOOT_FROM_NOR	0
#define BOOT_FROM_NAND	2
#define BOOT_FROM_SPI	3

#if defined (CONFIG_RT2880_ROOTFS_IN_FLASH)

#define MD5SUM_LEN  16
typedef struct __tplink_image_header {
        uint32_t        version;                        /* 0x00: header version */
        char            fw_version[48];                 /* 0x04: fw version string */
        uint32_t        hw_id;                          /* 0x34: hardware id */
        uint32_t        hw_rev;                         /* 0x38: FIXME: hardware revision? */
        uint32_t        hw_ver_add;                     /* 0x3c: additional hardware version */
        uint8_t         md5sum1[MD5SUM_LEN];            /* 0x40 */
        uint32_t        unk2;                           /* 0x50: 0x00000000 */
        uint8_t         md5sum2[MD5SUM_LEN];            /* 0x54 */
        uint32_t        unk3;                           /* 0x64: 0xffffffff */

        uint32_t        kernel_la;                      /* 0x68: kernel load address */
        uint32_t        kernel_ep;                      /* 0x6c: kernel entry point */
        uint32_t        fw_length;                      /* 0x70: total length of the image */
        uint32_t        kernel_ofs;                     /* 0x74: kernel data offset */
        uint32_t        kernel_len;                     /* 0x78: kernel data length */
        uint32_t        rootfs_ofs;                     /* 0x7c: rootfs data offset */
        uint32_t        rootfs_len;                     /* 0x80: rootfs data length */
} _tplink_ihdr2_t;

#if defined (CONFIG_ROOTFS_IN_FLASH_NO_PADDING)
typedef struct __image_header {
	uint8_t unused[60];
	uint32_t ih_ksz;
} _ihdr_t;
#endif
#endif

extern int ra_check_flash_type(void);
extern int ra_mtd_write_nm(char *name, loff_t to, size_t len, const unsigned char *buf);
extern int ra_mtd_read_nm(char *name, loff_t from, size_t len, unsigned char *buf);

#endif
