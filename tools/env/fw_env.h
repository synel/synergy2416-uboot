/*
 * (C) Copyright 2002-2008
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * To build the utility with the run-time configuration
 * uncomment the next line.
 * See included "fw_env.config" sample file (TRAB board)
 * for notes on configuration.
 */
#define CONFIG_FILE     "/etc/fw_env.config"

#define HAVE_REDUND /* For systems with 2 env sectors */
#define DEVICE1_NAME      "/dev/mtd1"
#define DEVICE2_NAME      "/dev/mtd2"
#define DEVICE1_OFFSET    0x0000
#define ENV1_SIZE         0x4000
#define DEVICE1_ESIZE     0x4000
#define DEVICE2_OFFSET    0x0000
#define ENV2_SIZE         0x4000
#define DEVICE2_ESIZE     0x4000

#define CONFIG_BAUDRATE		115200
#define CONFIG_BOOTDELAY	5	/* autoboot after 5 seconds	*/
#define CONFIG_ETHADDR          00:12:E5:00:00:01
#define CONFIG_NETMASK          255.255.255.0
#define CONFIG_IPADDR           192.168.1.110
#define CONFIG_SERVERIP         192.168.1.100
#define CONFIG_GATEWAYIP        192.168.1.1
#define CONFIG_BOOTARGS         "console=ttySAC2,38400 root=/dev/mmcblk0p2 rw rootwait init=/linuxrc panic=2"

#define CONFIG_EXTRA_ENV_SETTINGS                                                                       \
        "mmcloaduboot=fatload mmc 0:1 c0008000 u-boot\0"                                                \
        "updateuboot=nand erase 0  40000 ; nand write.e c0008000 0 40000\0"                             \
        "mmcloadkernel=fatload mmc 0:1 c0008000 zImage\0"                                               \
        "updatekernel=nand erase 100000  280000 ; nand write.e c0008000 100000 280000\0"                \
        "mmcloadrootfs=fatload mmc 0:1 c0008000 rootfs\0"                                               \
        "updaterootfs=nand erase 400000 200000 ; nand write.e c0008000 400000 200000\0"                 \
        "bootkernel=nand read.e c0008000 100000 280000; bootm c0008000\0"                               \
        "mmcstart=mmcinit\0"                                                                            \
        "silent=1\0"                                                                            \
        ""


#define CONFIG_MMCLOADUBOOT "fatload mmc 0:1 c0008000 u-boot"
#define CONFIG_UPDATEUBOOT  "nand erase 0  40000 ; nand write.e c0008000 0 40000"
#define CONFIG_MMCLOADKERNEL "fatload mmc 0:1 c0008000 zImage"
#define CONFIG_UPDATEKERNEL  "nand erase 100000  280000 ; nand write.e c0008000 100000 280000"
#define CONFIG_MMCLOADROOTFS "fatload mmc 0:1 c0008000 rootfs"
#define CONFIG_UPDATEROOTFS  "nand erase 400000 200000 ; nand write.e c0008000 400000 200000"
#define CONFIG_BOOTKERNEL    "nand read.e c0008000 100000 280000; bootm c0008000"
#define CONFIG_MMCSTART      "mmcinit"
#define CONFIG_BOOTCOMMAND      "mmcinit; run mmcloadkernel; bootm c0008000"

extern int   fw_printenv(int argc, char *argv[]);
extern char *fw_getenv  (char *name);
extern int fw_setenv  (int argc, char *argv[]);

extern unsigned	long  crc32	 (unsigned long, const unsigned char *, unsigned);
