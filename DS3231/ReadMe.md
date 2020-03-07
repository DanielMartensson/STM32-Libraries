# DS3231 RTC with larm
This is an easy RTC with very high accuracy in time. You can use this if you want to connect a larm of week or date with it.
Perhaps both. This is much better than the low budget RTC DS1307. The output pin SQW will become HIGH when no alarms exist. SQW pin will become LOW when an alarm appear or all alarms are activated. Then you can make sure that if SQW is HIGH, then the RTC is still alive.

Program example:

```
  //Include
  #include "DS3231/DS3231.h"

  // Structures
  I2C_HandleTypeDef hi2c1;
  DS3231_I2C DS_i2c;

  // Init RTC - Set larm for: Week = false, Date = true and also the address 0x00
  DS3231_init(&DS_i2c, &hi2c1, 0x0, false, true);
  // Set the clock - Second 55. Hour 12. Day 5(Friday). Day of month 6. Month of year 3 (Mars). Year 2020.
  DS3231_setClock(&DS_i2c, 55, 59, 12, 5, 6, 3, 20);
  // Set alarm for the week - Minute 0. Hour 13. Day 5
  DS3231_setWeekAlarm(&DS_i2c, 0, 13, 5);
  // Sett alarm for the month
  DS3231_setDateAlarm(&DS_i2c, 15, 0, 13, 6);
  
  while(1){
    // Get the time and the status of booth alarms
    DS3231_getClock(&DS_i2c);
    uint8_t status = DS3231_readALarmStatus(&DS_i2c); 
    char text2[50];
    sprintf(text2, "HH:%d,MM:%d,SS:%d,S:%d", DS_i2c.hour, DS_i2c.minute, DS_i2c.second, status);
    
    // Shut off the alarm with a trigg - It will start again when the time has come again
    DS3231_offTriggAlarm(&DS_i2c);
  }
  
```
