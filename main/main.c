#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"

#include <cJSON.h>
#include "app_config.h"
#include "app_wifi.h"
#include "app_http.h"
#include "app_rest.h"
#include "ssd1306.h"
#include "fonts.h"

// Helpful time related macros
#define SECONDS(X) ((X) * (1000 / portTICK_RATE_MS))
#define MINUTES(Y) SECONDS(60 * (Y) )

/*
 *  Main FreeRTOS task that updates the weather display. 
 *  Refreshes every 5 minutes.
*/
static void weather_task(void *pvParameters)
{
    while(1){

        // wait for a Wi-FI connection
        app_wifi_wait_connected();

        // fetch the REST resource once connected. REST data is
        // stored in a JSON string called 'http_json_message' contained
        // within 'app_http.c' inside 'http/' directory
        app_rest_fetch();

        // Using cJSON, parse retrieved JSON string into human readable weather data
        cJSON *root = cJSON_Parse(http_json_message);

        cJSON *_main = cJSON_GetObjectItem(root,"main");
        cJSON *_weather = cJSON_GetObjectItem(root,"weather");
        cJSON *_zero = cJSON_GetArrayItem(_weather, 0);

        char *city = cJSON_GetObjectItem(root,"name")->valuestring;
        double temp = cJSON_GetObjectItem(_main, "temp")->valuedouble;
        int humidity = cJSON_GetObjectItem(_main, "humidity")->valueint;
        char *description = cJSON_GetObjectItem(_zero, "description")->valuestring;

        char city_buf[32];
        char temp_buf[32];
        char hum_buf[32];

        // display parsed data on terminal for debugging purposes. OPTIONAL
        printf("\nCity: %s", city);
        printf("\nTemperature: %.0lf F", temp);
        printf("\nHumidity: %d %%", humidity);
        printf("\nDescription: %s\n", description);

        // format parsed data into 'pretty' strings to display on LCD
        sprintf(city_buf, "City: %s", city);
        sprintf(temp_buf, "Temperature: %.0lf F", temp);
        sprintf(hum_buf, "Humidity: %d %%", humidity);

        // clear LCD, and select font 1
        ssd1306_clear(0);
        ssd1306_select_font(0, 1);

        // print weather data on LCD
        ssd1306_draw_string(0, 0, 0, city_buf, 1, 0);
        ssd1306_draw_string(0, 0, 10, temp_buf, 1, 0);
        ssd1306_draw_string(0, 0, 20, hum_buf, 1, 0);
        ssd1306_draw_string(0, 0, 40, "Description:", 1, 0);
        ssd1306_draw_string(0, 0, 50, description, 1, 0);
        ssd1306_refresh(0, true);

        // wait 5 minutes before refreshing weather data
        vTaskDelay(MINUTES(5));

    }   

}


void app_main()
{

    // initialize NVS flash, needed for WI-FI usage
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // initialize WI-FI
    app_wifi_initialise();

    // initialize LCD display
    if (ssd1306_init(0, I2C_SCL, I2C_SDA)) {
        ESP_LOGI("OLED", "oled initialized");
    }
    else {
        ESP_LOGE("OLED", "oled init failed");
    }

    // Create main weather task
    xTaskCreate(&weather_task, "weather_task", 8192, NULL, 5, NULL);   

}
