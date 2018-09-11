/**
  ******************************************************************************
  * @file    cli_simple.c
  * @author  Grugbai
  * @version V0.0.1
  * @date    2018-09-01
  * @brief   This file provides main program functions
  ******************************************************************************
  * @attention
  *     V0.01   Init    baixiaodong
  ******************************************************************************
  */

#include <console.h>
#include <serial.h>


/** U-Boot INITIAL CONSOLE-COMPATIBLE FUNCTION *****************************/

int getc(void)
{
    /* Send directly to the handler */
    return serial_getc();
}

int tstc(void)
{
    /* Send directly to the handler */
    return serial_tstc();
}

void putc(const char c)
{
    /* Send directly to the handler */
    serial_putc(c);
}

void puts(const char *s)
{
    serial_puts(s);
}

/* test if ctrl-c was pressed */
static int ctrlc_disabled = 0;  /* see disable_ctrl() */
static int ctrlc_was_pressed = 0;
int ctrlc(void)
{
#ifndef CONFIG_SANDBOX
    if (!ctrlc_disabled && gd->have_console) {
        if (tstc()) {
            switch (getc()) {
            case 0x03:      /* ^C - Control C */
                ctrlc_was_pressed = 1;
                return 1;
            default:
                break;
            }
        }
    }
#endif

    return 0;
}
/* Reads user's confirmation.
   Returns 1 if user's input is "y", "Y", "yes" or "YES"
*/
int confirm_yesno(void)
{
    int i;
    char str_input[5];

    /* Flush input */
    while (tstc())
        getc();
    i = 0;
    while (i < sizeof(str_input)) {
        str_input[i] = getc();
        putc(str_input[i]);
        if (str_input[i] == '\r')
            break;
        i++;
    }
    putc('\n');
    if (strncmp(str_input, "y\r", 2) == 0 ||
        strncmp(str_input, "Y\r", 2) == 0 ||
        strncmp(str_input, "yes\r", 4) == 0 ||
        strncmp(str_input, "YES\r", 4) == 0)
        return 1;
    return 0;
}
/* pass 1 to disable ctrlc() checking, 0 to enable.
 * returns previous state
 */
int disable_ctrlc(int disable)
{
    int prev = ctrlc_disabled;  /* save previous state */

    ctrlc_disabled = disable;
    return prev;
}

int had_ctrlc (void)
{
    return ctrlc_was_pressed;
}

void clear_ctrlc(void)
{
    ctrlc_was_pressed = 0;
}



