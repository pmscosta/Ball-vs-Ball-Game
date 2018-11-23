/**
 * @file keyboard.h
 * @author Pedro Costa, João Fidalgo
 * @date 27 Dec 2017
 * @brief File containing all the functions for manipulating and using the i8042 keyboard
 */

#ifndef __KEYBOARD_H
#define __KEYBOARD_H

/** @defgroup keyboard keyboard
 * @{
 *
 * Module that handles everything related to the i8042 keyboard, allowing the user to interact with the game.
 */

/**
 * MACROS FOR KEYBOARD
 */
#define DUMMY_KEY 0x0

/** @name KBD Key Structure */
/**@{
 *
 *@brief KBD Key Structure containing all of its attributes
 *
 * This structure will contain the information regarding the key that was pressed or released in each interrupt.
 * Given that each key generates different codes when it is pressed or released (i.e., when it is a breakcode, the MSB is set to 1)
 * we decided to keep in each key it's make code only and then, in the member pressed, keep the information about being pressed or not,
 * thus only needing to store the information of half the scancodes.
 */
typedef struct {
	unsigned long makecode; /**< The key's makecode */
	int pressed; /**< Set to 1 when the key was pressed, 0 otherwise*/
} kbd_key;

/** @} end of KBD Key Structure */

extern kbd_key * g_key;

/** @name Keys Makecodes */
/**@{
 *
 * @brief Enumeration of the makecodes regarding the keys used in our project.
 *
 * We also kept the information regarding the Key ESC breakcode since it is a special key
 * in our project that allows the user to exit from the game/menu.
 *
 */
typedef enum {
	KEY_ESC = 0x0001,
	KEY_W = 0x0011,
	KEY_A = 0x001E,
	KEY_S = 0x001F,
	KEY_D = 0x0020,
	KEY_SPACEBAR = 0x0039,
	KEY_NUMENTER = 0xE01C,
	KEY_ARROWUP = 0xE048,
	KEY_ARROWLEFT = 0xE04B,
	KEY_ARROWRIGHT = 0xE04D,
	KEY_ARROWDOWN = 0xE050,
	KEY_ESC_RELEASE = 0x0081
} KEY;

/** @} end of Keys Makecodes */

/**
 * @brief Subscribes and enables keyboard interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int keyboard_unsubscribe(void);

/**
 * @brief Unsubscribes keyboard 0 interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int keyboard_subscribe();

/**
 * @brief Keyboard interrupt handler
 *
 * Reads the contents of the KBC output buffer and the status register. If no errors occur
 * it will create a new key, using the function createKey which will be stored in a global variable, g_key.
 * If some error occurs, e.g., parity error, or if the key pressed has a two byte scancode (thus we need to wait another cycle
 * to get the full scancode) the handler will create a DUMMY_KEY (prevents the game from crashing).
 * If the scancode is equal to a KEY Esc release it will delete the key, given that it could mean a game exit status.
 */
void kbd_int_handler();

/**
 * @brief Creates a new key, allocating the needed memory
 *
 * Creates the key as described in the KBD Key Structure description.
 *
 * @param scancode - contains the information about which key was pressed or released, obtained by kbd_int_handler();
 * @return * kbd_key - a pointer to the key created
 */
kbd_key * createKey(unsigned long scancode);

/**
 * @brief Frees the memory allocated for the kbd_key
 */
void deleteKey(kbd_key * key);

/** @} end of keyboard */

#endif
