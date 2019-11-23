#include "mb_config.h"

modbus_master master_ap;
ac_param ac_ctrl;

static bool master_ac_send_receive(uint16_t timeout)
{
    master_ac.rx_len = 0;
    master_ac.buff_len = master_ac.tx_len;
    master_crc16(&master_ac, master_ac.tx_buf);
    master_ac.tx_buf[master_ac.tx_len++] = master_ac.crc_hi;
    master_ac.tx_buf[master_ac.tx_len++] = master_ac.crc_lo;
    uart_write_bytes(master_ac.uart_num, (const char *)master_ac.tx_buf, master_ac.tx_len);
    if (xSemaphoreTake(master_ac.reply_sem, timeout / portTICK_RATE_MS) == pdTRUE)
    {
        master_ac.status = true;
    }
    else
    {
        master_ac.status = false;
    }
    return master_ac.status;
}

static void master_ac_read_write_task(void *param)
{
    while (true)
    {
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

void csro_master_ac_init(uint8_t uart_number)
{
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};
    uart_param_config(uart_number, &uart_config);
    uart_set_pin(uart_number, TXD0_PIN, RXD0_PIN, RTS0_PIN, UART_PIN_NO_CHANGE);
    uart_driver_install(uart_number, 2048, 2048, 0, NULL, 0);
    uart_set_mode(uart_number, UART_MODE_RS485_HALF_DUPLEX);

    master_ac.uart_num = uart_number;
    master_ac.slave_id = 1;
    master_ac.master_send_receive = master_ac_send_receive;
    master_ac.reply_sem = xSemaphoreCreateBinary();
    master_ac.uart_mutex = xSemaphoreCreateMutex();

    ac_ctrl.ac_power = 0;
    ac_ctrl.ac_mode = 2;
    ac_ctrl.ac_temp = 220;

    xTaskCreate(master_ac_read_write_task, "master_ac_read_write_task", 2048, NULL, configMAX_PRIORITIES - 7, NULL);
}
