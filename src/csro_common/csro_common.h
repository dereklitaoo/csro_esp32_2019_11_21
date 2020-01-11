#ifndef CSRO_COMMON_H_
#define CSRO_COMMON_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_smartconfig.h"
#include "esp_event.h"
#include "esp_event_loop.h"

#include "driver/gpio.h"
#include "driver/uart.h"
#include "lwip/apps/sntp.h"
#include "lwip/sockets.h"
#include "nvs_flash.h"
#include "mqtt_client.h"

#include "time.h"
#include "cJSON.h"

#define CLOUD_SERV

#define NLIGHT_TOUCH_3K3R
/* 
#define AIR_SYS
#define NLIGHT_TOUCH_3K3R
*/

typedef struct
{
    uint8_t restore_flag;
    uint8_t router_flag;
    char router_ssid[50];
    char router_pass[50];

    uint8_t mac[6];
    char mac_str[20];
    uint8_t ip[4];
    char ip_str[20];

    char host_name[20];
    char dev_type[20];

    uint32_t power_cnt;
    uint32_t router_cnt;
    uint32_t server_cnt;

    uint8_t time_sync;
    time_t time_start;
    time_t time_now;
    int time_run;
    struct tm time_info;
    char time_str[64];
} csro_system;

typedef struct
{
    char id[50];
    char name[50];
    char pass[50];

    char sub_topic[100];
    char pub_topic[100];
    char lwt_topic[100];

    char content[1000];

    char broker[50];
    char uri[50];
    char prefix[50];

    uint16_t interval;
} csro_mqtt;

extern csro_system sysinfo;
extern csro_mqtt mqttinfo;
extern esp_mqtt_client_handle_t mqttclient;

//common.c
void csro_mqtt_client_info(void);

//csro_start_config.c
void csro_start_smart_config(void);

//csro_mqtt.c
void csro_start_mqtt(void);

#endif