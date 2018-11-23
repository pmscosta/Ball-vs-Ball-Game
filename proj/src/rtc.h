/**
 * @file rtc.h
 * @author Pedro Costa, João Fidalgo
 * @date 29 Dec 2017
 * @brief File containing all the functions and informations regarding the rtc.
 *
 */


#ifndef _RTC_H
#define _RTC_H

/** @defgroup rtc rtc
 * @{
 *
 * Module that handles everything related to the rtc.
 */

/**
 * MACROS FOR RTC
 */
#define BIT(n) (0x01<<(n))
//=================ADDRESS BITS===============
#define RTC_SECS 		0
#define RTC_SECS_ALARM 	1
#define RTC_MINS		2
#define RTC_MINS_ALARM	3
#define RTC_HOURS 		4
#define RTC_HOURS_ALARM	5
#define RTC_WEEK_DAY	6
#define RTC_MONTH_DAY	7
#define RTC_MONTH		8
#define RTC_YEAR		9
//================REGISTERS==============
#define RTC_ADDR_REG 		0x70
#define RTC_DATA_REG 		0x71
#define RTC_REG_A 			10
#define RTC_REG_B 			11
#define RTC_REG_C 			12
//==============HELPFULL=================
#define RTC_UIP				BIT(7)
#define RTC_IRQ 			8

/*
 * @brief Gets the current date and time of the day
 *
 * @param *hour pointer to be changed to the current hour of the day
 * @param *min pointer to be changed to the current minutes of the day
 * @param *sec pointer to be changed to the current seconds of the day
 * @param *day pointer to be changed to the current day
 * @param *month pointer to be changed to the current month
 * @param *year pointer to be changed to the current year of the day
 *
 * @return 1 when fails,0 upon success
 */
int rtc_get_date(unsigned long *hour, unsigned long *min, unsigned long *sec,
		unsigned long *day, unsigned long *month, unsigned long *year);
/*
 * @brief Waits for BIT(7) of regA to be 0
 *
 * 	Wich means that there is no update in progress
 *
 */
void wait_valid_rtc();
/*
 * @brief Converts from binary to bcd
 *
 * @param n number to be converted
 *
 * @return the number after conversion
 */
int rtc_convert(int n);

/** @} end of rtc */

#endif
