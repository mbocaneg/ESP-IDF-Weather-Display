# ESP32-IOT-Weather-Display
An IOT weather display based on the ESP32 microcontroller and an SSD1306 LDC. Programming was carried out in C, under the ESP-IDF environment. LCD logic is taken care of using imxieyie's [SSD1306 library](https://github.com/imxieyi/esp32-i2c-ssd1306-oled/tree/pure-c). A micropython version of this project is available [HERE.](https://github.com/mbocaneg/IOT-Weather-Display)

# Theory of Operation
Operation is fairly straightforward; A single RTOS task caries out a GET request to the openweather api and returns a JSON object. This JSON object is then parsed using the cJSON library, where relevant weather data is extracted. This data is then presented to the user using the SSD1306 LCD.

# Usage
User must edit the app_config.h file to suit their own needs. Within this file, users set the SSID and password for the wifi connection the ESP32 will use(MY_WIFI_SSID and MY_WIFI_PASSWORD defines). The user must also edit the API_KEY define with their own api key. Finally, users might want to change the I2C data/clock lines used for the SSD1306 by changing defines I2C_SDA and I2C_SCL. A bit-banged I2C driver is used to communicate with the SSD1306, so any GPIO pins may be used.
