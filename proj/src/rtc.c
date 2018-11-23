#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/com.h>
#include "rtc.h"

int rtc_convert(int n) {
	n = (n & 0x0F) + ((n >> 4) * 10);
	return n;
}

int rtc_get_date(unsigned long *hour, unsigned long *min, unsigned long *sec,
		unsigned long *day, unsigned long *month, unsigned long *year) {
	asm("cli");
	wait_valid_rtc();
	if (sys_outb(RTC_ADDR_REG, RTC_SECS) != OK)
		return 1;
	if (sys_inb(RTC_DATA_REG, sec) != OK)
		return 1;
	if (sys_outb(RTC_ADDR_REG, RTC_MINS) != OK)
		return 1;
	if (sys_inb(RTC_DATA_REG, min) != OK)
		return 1;
	if (sys_outb(RTC_ADDR_REG, RTC_HOURS) != OK)
		return 1;
	if (sys_inb(RTC_DATA_REG, hour) != OK)
		return 1;
	if (sys_outb(RTC_ADDR_REG, RTC_MONTH_DAY) != OK)
		return 1;
	if (sys_inb(RTC_DATA_REG, day) != OK)
		return 1;
	if (sys_outb(RTC_ADDR_REG, RTC_MONTH) != OK)
		return 1;
	if (sys_inb(RTC_DATA_REG, month) != OK)
		return 1;
	if (sys_outb(RTC_ADDR_REG, RTC_YEAR) != OK)
		return 1;
	if (sys_inb(RTC_DATA_REG, year) != OK)
		return 1;
	*hour = rtc_convert(*hour);
	*min = rtc_convert(*min);
	*sec = rtc_convert(*sec);
	*day = rtc_convert(*day);
	*month = rtc_convert(*month);
	*year = rtc_convert(*year);
	asm("sti");
	return 0;
}

void wait_valid_rtc() {
	unsigned long regA = 0;
	do {
		sys_outb(RTC_ADDR_REG, RTC_REG_A);
		sys_inb(RTC_DATA_REG, &regA);
	} while (regA & RTC_UIP);
}
