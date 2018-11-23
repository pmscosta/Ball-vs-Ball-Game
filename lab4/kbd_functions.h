#ifndef __KBD_FUNCTIONS_H
#define __KBD_FUNCTIONS_H


extern int g_hook_id_kbd;

/**
 * @brief Subscribes and enables keyboard interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int kbc_subscribe(void);



/**
 * @brief Unsubscribes keyboard 0 interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int kbc_unsubscribe();
#endif
