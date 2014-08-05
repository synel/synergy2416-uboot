/*
 * (C) Copyright 2014
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <regs.h>

/* ------------------------------------------------------------------------- */
static inline void delay(unsigned long loops)
{
	__asm__ volatile ("1:\n" "subs %0, %1, #1\n" "bne 1b":"=r" (loops):"0"(loops));
}

/*static void smsc9220_pre_init(void)
{
        SMBIDCYR5_REG = 0;                      //Bank5 Idle cycle ctrl.
        SMBWSTWRR5_REG = 14;                    //Bank5 Write Wait State ctrl.
        SMBWSTOENR5_REG = 2;                    //Bank5 Output Enable Assertion Delay ctrl.     Tcho?
        SMBWSTWENR5_REG = 2;                    //Bank5 Write Enable Assertion Delay ctrl.
        SMBWSTRDR5_REG = 14;                    //Bank5 Read Wait State cont. = 14 clk      Tacc?

        SMBCR5_REG |=  ((1<<15)|(1<<7));                //dsf
        SMBCR5_REG |=  ((1<<2)|(1<<0));         //SMWAIT active High, Read Byte Lane Enabl          WS1?
        SMBCR5_REG &= ~((3<<20)|(3<<12));       //SMADDRVALID = always High when Read/Write
        SMBCR5_REG &= ~(3<<4);                  //Clear Memory Width
        SMBCR5_REG |=  (1<<4);                  //Memory Width = 16bit
}*/

/*
 * Miscellaneous platform dependent initialisations
 */

static void cs8900_pre_init(void)
{
#if 0
       SMBIDCYR1_REG = 0;                      //Bank1 Idle cycle ctrl.
       SMBWSTWRR1_REG = 14;                    //Bank1 Write Wait State ctrl.
       SMBWSTOENR1_REG = 2;                    //Bank1 Output Enable Assertion Delay ctrl.     Tcho?
       SMBWSTWENR1_REG = 2;                    //Bank1 Write Enable Assertion Delay ctrl.
       SMBWSTRDR1_REG = 14;                    //Bank1 Read Wait State cont. = 14 clk      Tacc?
#endif

#if 0
       SMBCR1_REG |=  ((1<<15)|(1<<7));                //dsf
       SMBCR1_REG |=  ((1<<2)|(1<<0));         //SMWAIT active High, Read Byte Lane Enabl          WS1?
       SMBCR1_REG &= ~((3<<20)|(3<<12));       //SMADDRVALID = always High when Read/Write
       SMBCR1_REG &= ~(3<<4);                  //Clear Memory Width
       SMBCR1_REG |=  (1<<4);                  //Memory Width = 16bit
#endif
       SMBCR1_REG =  0x303010;                 //Memory Width = 16bit
#ifdef CONFIG_PRINTK   
       printf("SMBCR1_REG = %x\n", SMBCR1_REG);
#endif
}

static void usb_pre_init (void)
{
	CLKDIV1CON_REG |= 1<<4;

	USB_RSTCON_REG = 0x1;
	delay(500);
	USB_RSTCON_REG = 0x2;
	delay(500);
	USB_RSTCON_REG = 0x0;
	delay(500);

//	USB_PHYCTRL_REG &= ~0x2;
	USB_CLKCON_REG |= 0x2;
}

int board_init(void)
{
	DECLARE_GLOBAL_DATA_PTR;

	cs8900_pre_init();
	usb_pre_init();
	GPACON_REG = GPACON_REG & (~(0x1<<13));   //GPA13 OUT   0 or 1 ? 
	GPACON_REG = GPACON_REG & (~(0x1<<14));   //GPA14 OUT   0 or 1 ? 
	GPACON_REG = GPACON_REG & (~(0x1<<15));   //GPA15 OUT   0 or 1 ? 
	GPACON_REG = GPACON_REG & (~(0x1<<16));   //GPA16 OUT   0 or 1 ? 
	GPACON_REG = GPACON_REG & (~(0x1<<23));   //GPA23 OUT   0 or 1 ? 
	GPACON_REG = GPACON_REG & (~(0x1<<24));   //GPA24 OUT   0 or 1 ? 

	GPBCON_REG = (GPBCON_REG & (~(0xff<<0))) + 0x55;  //GPB0.1.2.3 out  0 or 1?
	GPBCON_REG = (GPBCON_REG & (~(0x3<<10))) + (0x01<<10); //GPB5 out 0 or 1?
	GPBCON_REG = (GPBCON_REG & (~(0x3<<12))) ; //GPB6
	GPBCON_REG = (GPBCON_REG & (~(0x3<<18))) + (0x01<<18); //GPB9 out 0 or 1?
	GPBCON_REG = (GPBCON_REG & (~(0x3<<20))) + (0x01<<20); //GPB10 out


	GPCCON_REG = (GPCCON_REG & (~(0x3<<0))) + 0x01;   //GPC0 out  0 or 1?

	GPFCON_REG = (GPFCON_REG & (~(0x3<<0)));   //GPF0 in
      
	GPKCON_REG = (GPKCON_REG & (~(0xff<<24))) + (0x55<<24); //GPK12 14 out 13.15 in
	GPKCON_REG = (GPKCON_REG & (~(0x03<<18))) + (0x01<<18); //GPK9  out 

	GPKDAT_REG = (GPKDAT_REG | (0xf<<12)); 

	gd->bd->bi_arch_number = MACH_TYPE;
	gd->bd->bi_boot_params = (PHYS_SDRAM_1+0x100);

	return 0;
}

int dram_init(void)
{
	DECLARE_GLOBAL_DATA_PTR;

	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;
	gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
	gd->bd->bi_dram[1].size = PHYS_SDRAM_2_SIZE;

   /*	 if (CONFIG_NR_DRAM_BANKS == 2) {
	    gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
	    gd->bd->bi_dram[1].size = PHYS_SDRAM_2_SIZE;
    }*/

	return 0;
}

#ifdef BOARD_LATE_INIT
int board_late_init (void)
{
	uint *magic = (uint*)(PHYS_SDRAM_1);
	char boot_cmd[100];

	if ((0x24564236 == magic[0]) && (0x20764316 == magic[1])) {
		sprintf(boot_cmd, "nand erase 0 40000;nand write %08x 0 40000", PHYS_SDRAM_1 + 0x8000);
		magic[0] = 0;
		magic[1] = 0;
		printf("\nready for self-burning U-Boot image\n\n");
		setenv("bootdelay", "0");
		setenv("bootcmd", boot_cmd);
	}

	return 0;
}
#endif

#ifdef CONFIG_DISPLAY_BOARDINFO
int checkboard(void)
{
	vu_long *mem_reg = (vu_long*) 0x48000000;

	printf("Board: SYNERGY2416 ");
	switch ((*mem_reg>>1) & 0x7) {
	case 0:
		puts("SDRAM\n");
		break;
	case 1:
		puts("DDR2\n");
		break;
	case 2:
		puts("Mobile SDRAM\n");
		break;
	case 4:
		puts("DDR\n");
		break;
	case 6:
		puts("Mobile DDR\n");
		break;
	default:
		puts("unknown Memory Type\n");
	}
	return (0);
}
#endif

#ifdef CONFIG_ENABLE_MMU
ulong virt_to_phy_smdk2416(ulong addr)
{
	if ((0xC0000000 <= addr) && (addr < 0xC4000000))
		return (addr - 0xC0000000 + 0x30000000);
//	else if ((0xC4000000 <= addr) && (addr < 0xC8000000))
//        return (addr - 0xC4000000 +0x38000000);
//	else if ((0x30000000 <= addr) && (addr < 0x34000000))
//        return addr;
//   	else if ((0x38000000 <= addr) && (addr < 0x3c000000))
//		return addr;
	else
		printf("do not support this address : %08lx\n", addr);

	return addr;
}
#endif

#if defined(CONFIG_CMD_NAND) && defined(CFG_NAND_LEGACY)
#include <linux/mtd/nand.h>
extern struct nand_chip nand_dev_desc[CFG_MAX_NAND_DEVICE];
void nand_init(void)
{
	nand_probe(CFG_NAND_BASE);
	if (nand_dev_desc[0].ChipID != NAND_ChipID_UNKNOWN) {
		print_size(nand_dev_desc[0].totlen, "\n");
	}
}
#endif
