#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/task.h"
#include "driver/gpio.h"


volatile uint32_t ulIdleCycleCount = 0UL;

volatile uint32_t ulIdleTask1Count = 1UL;       // PREEMPTION = 0

// ============= PREEMPTION = 1 ================
volatile uint32_t ulIdleTask2Count = 0UL;
volatile uint32_t ulIdleTask3Count = 0UL;
// =============================================

void func_2(void *pvParameter)
{
    int cnt = 0;
    while (1)
    {
        if (configUSE_PREEMPTION == 0)
        {
            cnt++;
            printf("====|| task 2cnt: %d\n",cnt);
            if(cnt == 10)    
            {
                printf("====|| task 2 end !!!\n");
                vTaskDelete(NULL);
            }
        }
        else if (configUSE_PREEMPTION == 1)
        {
            ulIdleTask2Count++;
        }
    }
}

void func_3(void *pvParameter)
{
    int cnt = 0;
    while (1)
    {
        if (configUSE_PREEMPTION == 0)
        {
            cnt++;
            printf("==============|| task 3 cnt: %d\n",cnt);
            if(cnt == 5)    
            {
                printf("==============|| task 3 end !!!\n");
                vTaskDelete(NULL);
            }
        }
        else if (configUSE_PREEMPTION == 1)   
        {
            ulIdleTask3Count++;
        } 
    }
}

void func_1(void *pvParameter)
{
    while (1)
    {
        if (configUSE_PREEMPTION == 0)
        {
            ulIdleTask1Count++;
            if (ulIdleTask1Count == 2)
            {
                xTaskCreatePinnedToCore(&func_2,"task2",1024*5,NULL,2,NULL,0);
            }
            else if (ulIdleTask1Count == 4)
            {
                xTaskCreatePinnedToCore(&func_3,"task3",1024*5,NULL,5,NULL,0);
            }
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        else 
        {
            printf("---------------|| %d \n", ulIdleTask2Count);
            printf("-----|| %d \n", ulIdleTask3Count);
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
    }
}

void app_main()
{
    printf("Preempt : %d \n",configUSE_PREEMPTION);
    printf("Time-slicing : %d \n\n",configUSE_TIME_SLICING);

    if (configUSE_PREEMPTION == 0)
    {
        xTaskCreate(&func_1,"task1",1024*5,NULL,0,NULL);
    }
    else
    {
        xTaskCreatePinnedToCore(&func_1,"task2",1024*5,NULL,2,NULL,1);
        xTaskCreatePinnedToCore(&func_2,"task2",1024*5,NULL,0,NULL,0);
        xTaskCreatePinnedToCore(&func_3,"task3",1024*5,NULL,0,NULL,0);
    }
}

