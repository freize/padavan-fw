#ifndef __RALINK_FLASH_MAP_H__
#define __RALINK_FLASH_MAP_H__

#define MTD_BOOT_PART_SIZE		0x00030000	/* 192K */
#define MTD_CONFIG_PART_SIZE		0x00010000	/*  64K */
#if !defined(CONFIG_RT2880_FLASH_4M)
#define MTD_ROMFILE_PART_SIZE		0x00010000	/*  64K */
#define MTD_ROM_PART_SIZE		0x00010000	/*  64K */
#else
#define MTD_ROMFILE_PART_SIZE		0x0
#define MTD_ROM_PART_SIZE		0x0
#endif
#define MTD_FACTORY_PART_SIZE		0x00010000	/*  64K */
#if defined (CONFIG_MTD_STORE_PART_SIZ) && (CONFIG_MTD_STORE_PART_SIZ >= 0x10000)
#define MTD_STORE_PART_SIZE		CONFIG_MTD_STORE_PART_SIZ
#else
#define MTD_STORE_PART_SIZE		0x00010000	/*  64K */
#endif

#if defined (CONFIG_RT2880_FLASH_4M)
#define IMAGE1_SIZE			0x00400000
#elif defined (CONFIG_RT2880_FLASH_8M)
#define IMAGE1_SIZE			0x00800000
#elif defined (CONFIG_RT2880_FLASH_16M)
#define IMAGE1_SIZE			0x01000000
#elif defined (CONFIG_RT2880_FLASH_32M)
#define IMAGE1_SIZE			0x02000000
#else
#define IMAGE1_SIZE			CONFIG_RT2880_MTD_PHYSMAP_LEN
#endif

#define MTD_KERNEL_PART_OFFSET		(MTD_BOOT_PART_SIZE + MTD_CONFIG_PART_SIZE)

#if defined (CONFIG_RT2880_ROOTFS_IN_FLASH)
#define MTD_STORE_PART_IDX		4
#if !defined(CONFIG_RT2880_FLASH_4M)
#define MTD_FWSTUB_PART_IDX		8
#else
#define MTD_FWSTUB_PART_IDX		6
#endif
#else
#define MTD_STORE_PART_IDX		3
#if !defined(CONFIG_RT2880_FLASH_4M)
#define MTD_FWSTUB_PART_IDX		7
#else
#define MTD_FWSTUB_PART_IDX		5
#endif
#endif

/* NOR, SPI partitions (Ralink/MTK reference + mod) */
static struct mtd_partition rt2880_partitions[] = {
	{
		name:   "Bootloader",		/* mtdblock0 */
		size:   MTD_BOOT_PART_SIZE,	/* 192K */
		offset: 0,
	}, {
		name:   "Config",		/* mtdblock1 */
		size:   MTD_CONFIG_PART_SIZE,	/* 64K */
		offset: MTDPART_OFS_APPEND,
	}, {
		name:   "Kernel",		/* mtdblock2 */
		size:   0,			/* calc */
		offset: MTD_KERNEL_PART_OFFSET,
#if defined (CONFIG_RT2880_ROOTFS_IN_FLASH)
	}, {
		name:   "RootFS",		/* mtdblock3 */
		size:   0,			/* calc */
		offset: MTDPART_OFS_APPEND,
#endif
	}, {
		name:   "Storage",		/* mtdblock4 */
		size:   MTD_STORE_PART_SIZE,	/* 64K */
		offset: MTDPART_OFS_APPEND,
#if !defined (CONFIG_RT2880_FLASH_4M)
	}, {
		name:	"Romfile",		/* mtdblock5 */
		size:	MTD_ROMFILE_PART_SIZE,	/* 64K */
		offset:	MTDPART_OFS_APPEND,
	}, {
		name:	"Rom",			/* mtdblock6 */
		size:	MTD_ROM_PART_SIZE,	/* 64K */
		offset:	MTDPART_OFS_APPEND,
#endif
	}, {
		name:	"Factory",		/* mtdblock7 */
		size:	MTD_FACTORY_PART_SIZE,	/* 64K */
		offset:	MTDPART_OFS_APPEND,
	}, {
		name:   "Firmware_Stub",	/* mtdblock8 */
		size:   0,			/* calc */
		offset: MTD_KERNEL_PART_OFFSET,
	}
};

inline void recalc_partitions(uint64_t flash_size, uint32_t kernel_size)
{
	/* first, calc "Storage" size (use 256K for >= 16MB flash) */
#if !defined (CONFIG_MTD_STORE_PART_SIZ)
	if (flash_size > 0x00800000)
		rt2880_partitions[MTD_STORE_PART_IDX].size = 0x00040000;
#endif

	/* calc "Kernel" size */
#if defined (CONFIG_RT2880_ROOTFS_IN_FLASH)
#if defined (CONFIG_ROOTFS_IN_FLASH_NO_PADDING)
	rt2880_partitions[2].size = kernel_size;
#else
	rt2880_partitions[2].size = CONFIG_MTD_KERNEL_PART_SIZ;
#endif
	/* calc "RootFS" size */
	rt2880_partitions[3].size = flash_size - (MTD_KERNEL_PART_OFFSET + MTD_ROMFILE_PART_SIZE + MTD_ROM_PART_SIZE + MTD_FACTORY_PART_SIZE + rt2880_partitions[MTD_STORE_PART_IDX].size + rt2880_partitions[2].size);
#else
	/* ROOTFS_IN_RAM */
	rt2880_partitions[2].size = flash_size - (MTD_KERNEL_PART_OFFSET + MTD_ROMFILE_PART_SIZE + MTD_ROM_PART_SIZE + MTD_FACTORY_PART_SIZE + rt2880_partitions[MTD_STORE_PART_IDX].size);
#endif
	/* calc "Fimrware_Stub" size */
	rt2880_partitions[MTD_FWSTUB_PART_IDX].size = flash_size - (MTD_KERNEL_PART_OFFSET + MTD_ROMFILE_PART_SIZE + MTD_ROM_PART_SIZE + MTD_FACTORY_PART_SIZE + rt2880_partitions[MTD_STORE_PART_IDX].size);
}

#endif
