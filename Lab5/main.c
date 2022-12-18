#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "FreeRTOSConfig.h"

TimerHandle_t xTimer_1;
TimerHandle_t xTimer_2;
TimerHandle_t xTimer_temp;

uint8_t counter_timer_1 = 0;
uint8_t counter_timer_2 = 0;
uint8_t counter_time = 1;
void timer_Callback(TimerHandle_t xTimer)
{
  const uint8_t* timer_id = pvTimerGetTimerID(xTimer);
  if(timer_id == 1)
  {
    printf("ahihi at second: %d \r\n", counter_time);
    counter_timer_1++;
    if(counter_timer_1 == 10) {
      printf("Timer 1 stop \r\n");
      printf("Exit program! \r\n");
      xTimerStop(xTimer, 0);
    } 
  }
  else if(timer_id == 2)
  {
    printf("ihaha at second: %d \r\n", counter_time);
    counter_timer_2++;
    if(counter_timer_2 == 5) {
      printf("Timer 2 stop \r\n");
      xTimerStop(xTimer, 0);
    } 
  }
}

void timer_CountTime()
{
  counter_time++;
}

void app_main()
{
  xTimer_1 = xTimerCreate("Timer 1", pdMS_TO_TICKS(2000), pdTRUE, (void*)1, timer_Callback);
  xTimer_2 = xTimerCreate("Timer 2", pdMS_TO_TICKS(3000), pdTRUE, (void*)2, timer_Callback);
  xTimer_temp = xTimerCreate("Timer temp", pdMS_TO_TICKS(1000), pdTRUE, (void*)3, timer_CountTime);

  if(xTimer_1 != NULL && xTimer_2 != NULL && xTimer_temp != NULL)
  {
    printf("Start timer !!!\r\n");
    xTimerStart(xTimer_temp, 0);
    xTimerStart(xTimer_1, 0);
    xTimerStart(xTimer_2, 0);
  }
  else printf("Can not start timer, exit!!!\r\n");
}