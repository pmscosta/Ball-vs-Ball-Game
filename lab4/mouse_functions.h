#ifndef __MOUSE_FUNCTIONS_H
#define __MOUSE_FUNCTIONS_H


extern int g_hook_id;

/**
 * @brief Subscribes and enables mouse interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int mouse_subscribe(void);



/**
 * @brief Unsubscribes mouse  interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_unsubscribe();


/**
 * @brief Enables stream mode
 *
 */

/**
 * @brief Writes a byte to Mouse
 *
 *
 * Also reads and deals with the eventual ACK response
 *
 * @param cmd - the command argument passed to command 0xD4
 */
int write_to_mouse(unsigned long cmd);


/**
 * @brief Disables Minix's Default IH
 *
 * Useful to do poll tests
 * Disables the keyboard and disables the interrupts on OBF
 *
 */
int disableDefaultIH();

/**
 * @brief Enables Minix's Default IH
 *
 * Useful to do poll tests
 * Enables the keyboard and disables the interrupts on OBF
 *
 */
int enableDefaultIH();


/**
 * @brief Prints a mouse Packet
 *
 * @param packet - a pointer to the mouse packet
 * @param length - the size of the packet
 *
 */
void print_packet(unsigned long * packet, int length);

/**
 * @brief Flushes the output buffer until it's empty
 *
 * Useful for debugging code
 *
 */
void emptyOutBuffer();

/*
 * @briefs Gives the value of the x-axis displacement
 *
 * Can be either positive or negative
 *
 * @param packet - the mouse packet
 */
int get_X_Delta(unsigned long * packet);


/*
 * @briefs Gives the value of the y-axis displacement
 *
 * Can be either positive or negative
 *
 * @param packet - the mouse packet
 */
int get_Y_Delta(unsigned long * packet);



#endif
