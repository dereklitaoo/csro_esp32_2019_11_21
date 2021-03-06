#include "csro_devices.h"

#ifdef NLIGHT_TOUCH_3K3R

#define THRESHOLD 15

#define TOUCH_01_NUM 0
#define TOUCH_02_NUM 2
#define TOUCH_03_NUM 3

#define RELAY_01_NUM GPIO_NUM_27
#define RELAY_02_NUM GPIO_NUM_14
#define RELAY_03_NUM GPIO_NUM_12

#define LED_01_NUM GPIO_NUM_19
#define LED_02_NUM GPIO_NUM_18
#define LED_03_NUM GPIO_NUM_5
#define GPIO_OUTPUT_PIN_SEL ((1ULL << RELAY_01_NUM) | (1ULL << RELAY_02_NUM) | (1ULL << RELAY_03_NUM) | (1ULL << LED_01_NUM) | (1ULL << LED_02_NUM) | (1ULL << LED_03_NUM))

int light_state[3] = {0, 0, 0};

static void csro_update_nlight_touch_3k3r_state(void)
{
}

static void nlight_touch_3k3r_relay_led_task(void *args)
{
    while (true)
    {
        printf("%d %d %d\r\n", light_state[0], light_state[1], light_state[2]);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

static void nlight_touch_3k3r_key_task(void *args)
{

    static uint16_t first_touch_value[4];
    static uint16_t holdtime[4];
    touch_pad_init();
    touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    for (int i = 0; i < 4; i++)
    {
        touch_pad_config(i, 0);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        touch_pad_read(i, &first_touch_value[i]);
    }

    while (true)
    {
        uint16_t touchvalue;
        for (uint8_t i = 0; i < 4; i++)
        {
            touch_pad_read(i, &touchvalue);
            if (first_touch_value[i] > touchvalue && (first_touch_value[i] - touchvalue >= THRESHOLD))
            {
                holdtime[i]++;
                if (holdtime[i] == 4)
                {
                    if (i == 0)
                    {
                        light_state[0] = !light_state[0];
                    }
                    else if (i == 2)
                    {
                        light_state[1] = !light_state[1];
                    }
                    else if (i == 3)
                    {
                        light_state[2] = !light_state[2];
                    }
                }
            }
            else
            {
                holdtime[i] = 0;
            }
        }
        if (holdtime[0] > 6000 && holdtime[2] > 6000 && holdtime[3] > 6000)
        {
            esp_restart();
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void csro_nlight_touch_3k3r_init(void)
{
    xTaskCreate(nlight_touch_3k3r_relay_led_task, "nlight_touch_3k3r_relay_led_task", 2048, NULL, configMAX_PRIORITIES - 8, NULL);
    xTaskCreate(nlight_touch_3k3r_key_task, "nlight_touch_3k3r_key_task", 2048, NULL, configMAX_PRIORITIES - 6, NULL);
}

void csro_nlight_touch_3k3r_on_connect(esp_mqtt_event_handle_t event)
{
}

void csro_nlight_touch_3k3r_on_message(esp_mqtt_event_handle_t event)
{
}

#endif
