/*
 * (C) Copyright 2003
 * Tait Electronics Limited, Christchurch, New Zealand
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
 * This file provides a shell like 'test' function to return
 * true/false from an integer or string compare of two memory
 * locations or a location and a scalar/literal.
 * A few parts were lifted from bash 'test' command
 */

#include <common.h>
#include <config.h>
#include <command.h>
#include <malloc.h>

int do_bootcnt (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]){
	char * bootcount;
	char * newBootCnt;
	bootcount = calloc(sizeof(char),5);
	newBootCnt = calloc(sizeof(char),5);
	ulong increBoot = 0;
	bootcount=getenv ("bootcount");
	printf("Current bootcount=%s\n",bootcount);
	increBoot=simple_strtoul(bootcount,NULL,10);
	printf("Int Incre Before:%lu\n",increBoot);
	++increBoot;
	printf("Int Incre After:%lu\n",increBoot);
	sprintf(newBootCnt,"%lu",increBoot);
	printf("Incremented bootcount=%s\n",newBootCnt);
	setenv ("bootcount",newBootCnt);
	if(increBoot > 5){
		setenv("bootcmd","run altbootcmd\0");
		setenv("bootcount","0");
	}
	else{
		setenv("bootcmd","run normboot\0");
	}
	saveenv();
	return 0;
}


U_BOOT_CMD(
	bootcnt, CFG_MAXARGS, 1, do_bootcnt,
	"bootcnt\t- count the numner of boots\n",
	"run it to count the boots\n"
);
