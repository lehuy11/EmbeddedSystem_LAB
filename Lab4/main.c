#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/queue.h"
#include <stdlib.h>

typedef struct {
  uint8_t id;
  int32_t data;
} data_t;

QueueHandle_t xQueue_Task_1, xQueue_Task_2;

void vTask_Reception() {
  BaseType_t xTask_Status = pdFAIL;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100UL);
  data_t *request_ptr = (data_t*) malloc(sizeof(data_t));;
  while (1) {
    request_ptr->id = 1 + (int)( rand()*(2 + 1.0)/(1.0 + RAND_MAX) );
    request_ptr->data = rand() % 10;
    printf(
						"RECEPTION ==> Nhan request: id: %d, data: %d\r\n", 
						request_ptr->id, 
						request_ptr->data
					);

		switch(request_ptr->id)
		{
			case 1:
				xTask_Status = xQueueSendToBack(xQueue_Task_1, request_ptr, xTicksToWait);
				break;
			case 2:
				xTask_Status = xQueueSendToBack(xQueue_Task_2, request_ptr, xTicksToWait);
				break;
			default:
				printf(
								"RECEPTION ==> Khong co Task xu ly request id: %d !\r\n",
								request_ptr->id
							);
		}

    if (xTask_Status == pdPASS) {
      printf(
							"RECEPTION ==> Chuyen request id: %d den queue tuong ung!\r\n",
							request_ptr->id
						);
			xTask_Status = pdFAIL;
    }
    vTaskDelay(pdMS_TO_TICKS(300));
  }
}

void vTask_1() {
  data_t request_infor;
  BaseType_t xTask_Status;
  while (1) {
    xTask_Status = xQueueReceive(xQueue_Task_1, &request_infor, 0);
    if (xTask_Status == pdPASS) {
      printf(
							"TASK 1 ==> Xu ly request: id = %d, data = %d\r\n",
							request_infor.id,
							request_infor.data
						);
    }
  }
}

void vTask_2() {
  data_t request_infor;
  BaseType_t xTask_Status;
  while (1) {
    xTask_Status = xQueueReceive(xQueue_Task_2, &request_infor, 0);
    if (xTask_Status == pdPASS) {
      printf(
							"TASK 2 ==> Xu ly request: id = %d, data = %d\r\n",
							request_infor.id, request_infor.data
						);
    }
  }
}

void app_main(void) {

  // Create queue
  xQueue_Task_1 = xQueueCreate(5, sizeof(data_t));
  xQueue_Task_2 = xQueueCreate(5, sizeof(data_t));

  if (xQueue_Task_1 != NULL && xQueue_Task_2 != NULL) {
    xTaskCreate(vTask_Reception, "Reception task", 1024 * 5, NULL, 2, NULL);
    xTaskCreate(vTask_1, "Task 1", 1024 * 5, NULL, 1, NULL);
    xTaskCreate(vTask_2, "Task 2", 1024 * 5, NULL, 1, NULL);
  }
}