/*
 * User defined configuration file.
*/

// Wi-fi SSID and password for the connection this ESP32 will use. Change
// this to suit your needs
#define MY_WIFI_SSID "SSID1234"
#define MY_WIFI_PASSWORD "WIFI1234"

// strings that contain the URI of the REST API we would like to fetch, as
// well as the API key for said API, in this case, openweathermap. The locale
// here is set to Chicago and units are set to imperial. Change this to suit 
// your needs.
#define API_URI  "http://api.openweathermap.org/data/2.5/weather?q=Chicago&units=imperial&APPID="
#define API_KEY "my_api_key_123456789ABCDEF"

// I2C data and clock lines. as we are using a bit-banged i2c driver, you may 
// use any GPIO pins you wish to use.
#define I2C_SDA 18
#define I2C_SCL 19
