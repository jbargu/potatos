#include <irq.h>
#include <stdlib.h>
#include <string.h>

#define CURRENT_YEAR        2016                            // Change this each year!

int century_register = 0x00;                                // Set by ACPI table parsing code if possible

unsigned char second;
unsigned char minute;
unsigned char hour;
unsigned char day;
unsigned char month;
unsigned int year;

enum {
      cmos_address = 0x70,
      cmos_data    = 0x71
};

int get_update_in_progress_flag() {
      outportb(cmos_address, 0x0A);
      return (inportb(cmos_data) & 0x80);
}

unsigned char get_RTC_register(int reg) {
      outportb(cmos_address, reg);
      return inportb(cmos_data);
}

void read_rtc() {
      unsigned char century;
      unsigned char last_second;
      unsigned char last_minute;
      unsigned char last_hour;
      unsigned char last_day;
      unsigned char last_month;
      unsigned char last_year;
      unsigned char last_century;
      unsigned char registerB;

      while (get_update_in_progress_flag());
      second = get_RTC_register(0x00);
      minute = get_RTC_register(0x02);
      hour = get_RTC_register(0x04);
      day = get_RTC_register(0x07);
      month = get_RTC_register(0x08);
      year = get_RTC_register(0x09);
      if(century_register != 0) {
            century = get_RTC_register(century_register);
      }

      do {
            last_second = second;
            last_minute = minute;
            last_hour = hour;
            last_day = day;
            last_month = month;
            last_year = year;
            last_century = century;

            while (get_update_in_progress_flag());           // Make sure an update isn't in progress
            second = get_RTC_register(0x00);
            minute = get_RTC_register(0x02);
            hour = get_RTC_register(0x04);
            day = get_RTC_register(0x07);
            month = get_RTC_register(0x08);
            year = get_RTC_register(0x09);
            if(century_register != 0) {
                  century = get_RTC_register(century_register);
            }
      } while( (last_second != second) || (last_minute != minute) || (last_hour != hour) ||
               (last_day != day) || (last_month != month) || (last_year != year) ||
               (last_century != century) );

      registerB = get_RTC_register(0x0B);

      // Convert BCD to binary values if necessary
      if (!(registerB & 0x04)) {
            second = (second & 0x0F) + ((second / 16) * 10);
            minute = (minute & 0x0F) + ((minute / 16) * 10);
            hour = ( (hour & 0x0F) + (((hour & 0x70) / 16) * 10) ) | (hour & 0x80);
            day = (day & 0x0F) + ((day / 16) * 10);
            month = (month & 0x0F) + ((month / 16) * 10);
            year = (year & 0x0F) + ((year / 16) * 10);
            if(century_register != 0) {
                  century = (century & 0x0F) + ((century / 16) * 10);
            }
      }

      // Convert 12 hour clock to 24 hour clock if necessary
      if (!(registerB & 0x02) && (hour & 0x80)) {
            hour = ((hour & 0x7F) + 12) % 24;
      }

      // Calculate the full (4-digit) year
      if(century_register != 0) {
            year += century * 100;
      }
      else {
            year += (CURRENT_YEAR / 100) * 100;
            if(year < CURRENT_YEAR) year += 100;
      }
}

// Convert time to string in UTC format. No flexible format right now.
char *get_time() {
      read_rtc();
      char *sec = itoa(second);
      char *min = itoa(minute);
      char *h = itoa(hour);
      char *d = itoa(day);
      char *m = itoa(month);
      char *y = itoa(year);

      int sum = strlen(y) + strlen(m) + strlen(d) + strlen(h) + strlen(min) + strlen(sec);

      // Allocate space + delimiters.
      char *buf = (char *)malloc(sum + 6);

      int k = 0;
      // YEAR
      for (int i = 0; i < strlen(y); i++, k++)
            buf[k] = y[i];

      // MONTH
      buf[k++] = '-';
      for (int i = 0; i < strlen(m); i++, k++)
            buf[k] = m[i];
      buf[k++] = '-';

      // DAY
      for (int i = 0; i < strlen(d); i++, k++)
            buf[k] = d[i];
      buf[k++] = ' ';

      for (int i = 0; i < strlen(h); i++, k++)
            buf[k] = h[i];
      buf[k++] = ':';
      for (int i = 0; i < strlen(min); i++, k++)
            buf[k] = min[i];
      buf[k++] = ':';
      for (int i = 0; i < strlen(sec); i++, k++)
            buf[k] = sec[i];
      buf[k] = '\0';

      return buf;
}

