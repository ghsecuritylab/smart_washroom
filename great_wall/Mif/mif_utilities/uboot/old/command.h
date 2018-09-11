/**
  ******************************************************************************
  * @file    cmd.c
  * @author  Grugbai
  * @version V0.0.1
  * @date    2018-08-28
  * @brief   This file provides main program functions
  ******************************************************************************
  * @attention
    *
    * (C) Copyright 2000-2009
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
    * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    * GNU General Public License for more details.
    *
    * You should have received a copy of the GNU General Public License
    * along with this program; if not, write to the Free Software
    * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
    * MA 02111-1307 USA
    *
  *@attention
        Porting from uboot
  ******************************************************************************
  */
#ifndef __COMMAND_H_
#define __COMMAND_H_

/*包含系统平台的定义，主要是变量类型*/
#include "stm32f4xx.h"


#ifndef NULL
#define NULL    0
#endif

/**
 * Config Prompt string
 */
#define printf cmd_uart_printf
/**
 * Config Prompt string
 */
#define CONFIG_CMDLINE_EDITING  1
/**
 * Config Prompt string
 */
#define CONFIG_SYS_CBSIZE       256           /* console I/O buffer  */

/**
 * Config Prompt string
 */
#define CMD_DATA_SIZE   1
/**
 * Config Prompt string
 */
#define CONFIG_CMD_BOOTD    1
/**
 * Config Prompt string
 */
#define CONFIG_CMD_RUN      1
/**
 * Config Prompt string
 */
#define CONFIG_BOOTARGS     "This is Test String"
/**
 * Config Prompt string
 */
#define CONFIG_BOOTCOMMAND  "Run your owen scripts"
/**
 * Config Prompt string
 */
#ifndef CONFIG_SYS_HELP_CMD_WIDTH
#define CONFIG_SYS_HELP_CMD_WIDTH   8
#endif
/**
 * Config show help information--》显示帮忙信息
 */
#define CONFIG_SYS_LONGHELP         1
/**
 * Config Prompt string
 */
#define CONFIG_SYS_MAXARGS          12

/**
 * Config Prompt string
 */
#define CONFIG_SYS_PROMPT           "cmd> "

/**
 * Config auto complete command
 */
#define CONFIG_AUTO_COMPLETE        1

/*
 * Monitor Command Table
 */
struct cmd_tbl_s {
    char    *name;                                              /* Command Name         */
    int     maxargs;                                            /* maximum number of arguments  */
    int     repeatable;                                         /* autorepeat allowed?      */
    int     (*cmd)(struct cmd_tbl_s *, int, int, char *[]);     /* Implementation function  */
    char    *usage;                                             /* Usage message    (short) */

    #ifdef  CONFIG_SYS_LONGHELP
    char        *help;                                          /* Help  message    (long)  */
    #endif

    #ifdef CONFIG_AUTO_COMPLETE
    /* do auto completion on the arguments */
    int     (*complete)(int argc, char *argv[], char last_char, int maxv, char *cmdv[]);
    #endif
};

typedef struct cmd_tbl_s    cmd_tbl_t;

extern cmd_tbl_t  *__u_boot_cmd_start;
extern cmd_tbl_t  *__u_boot_cmd_end;

/*
 * Command Flags:
 */
#define CMD_FLAG_REPEAT     0x0001  /* repeat last command      */
#define CMD_FLAG_BOOTD      0x0002  /* command is from bootd    */

#define Struct_Section  __attribute__ ((unused,section (".u_boot_cmd")))

#ifdef CONFIG_AUTO_COMPLETE
# define _CMD_COMPLETE(x) x,
#else
# define _CMD_COMPLETE(x)
#endif
#ifdef CONFIG_SYS_LONGHELP
# define _CMD_HELP(x) x,
#else
# define _CMD_HELP(x)
#endif

/*
    将命令结构体定义到指定FLASH位置，并且连续放置，
    不同的编译器定义不一样
*/
//#define U_BOOT_CMD(name,maxargs,rep,cmd,usage,help) \
//            const cmd_tbl_t strcmd_##name __attribute__ ((section ("cmd"))) =  {#name, maxargs, rep, cmd, usage,help}


#define U_BOOT_CMD_MKENT_COMPLETE(name,maxargs,rep,cmd,usage,help,comp) \
      {#name, maxargs, rep, cmd, usage, _CMD_HELP(help) _CMD_COMPLETE(comp)}

#define U_BOOT_CMD_MKENT(name,maxargs,rep,cmd,usage,help) \
      U_BOOT_CMD_MKENT_COMPLETE(name,maxargs,rep,cmd,usage,help,NULL)

#define U_BOOT_CMD_COMPLETE(name,maxargs,rep,cmd,usage,help,comp) \
      cmd_tbl_t __u_boot_cmd_##name Struct_Section = \
          U_BOOT_CMD_MKENT_COMPLETE(name,maxargs,rep,cmd,usage,help,comp)

/**
 * name-> 命令名，非字符串，但在U_BOOT_CMD中用
 * maxargs--> 命令最大参数个数
 * rep--> 是否自动重复，按下ENTER 会自动重复
 * cmd--> 该命令对应的实现函数
 * usage--> 简短的使用说明
 * help--> 较详细的使用说明
 */
#define U_BOOT_CMD(name,maxargs,rep,cmd,usage,help) \
      U_BOOT_CMD_COMPLETE(name,maxargs,rep,cmd,usage,help,NULL)



int _do_help (cmd_tbl_t *cmd_start, int cmd_items, cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[]);
cmd_tbl_t *find_cmd(const char *cmd);
cmd_tbl_t *find_cmd_tbl (const char *cmd, cmd_tbl_t *table, int table_len);

extern int cmd_usage(cmd_tbl_t *cmdtp);

#ifdef CONFIG_AUTO_COMPLETE //defined in command.h
extern int var_complete(int argc, char * const argv[], char last_char, int maxv, char *cmdv[]);
extern int cmd_auto_complete(const char *const prompt, char *buf, int *np, int *colp);
#endif

void install_auto_complete(void);

#endif
