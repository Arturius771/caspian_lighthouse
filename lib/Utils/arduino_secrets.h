/*
 * Copy this template into arduino_secrets.h and change values accordingly.
 *
 * This file holds settings following the Arduino convention, see
 * https://docs.arduino.cc/arduino-cloud/tutorials/store-your-sensitive-data-safely-when-sharing
 *
 */

// // Settings: Eduroam Wi-Fi credentials
// #define SECRET_WIFI_SSID        "eduroam"               // Replace "eduroam"
// for other Wi-Fi #define SECRET_WIFI_PASSWORD    "********"              //
// Wi-Fi or university account password #define SECRET_WIFI_ANONYMOUSID
// "wlan@tu-berlin.de"     // Don't change, only used for Eduroam #define
// SECRET_WIFI_EDUROAMID   "********@tu-berlin.de" // TU account name, only used
// for Eduroam

// Settings: Eduroam Wi-Fi credentials
// #define SECRET_WIFI_SSID "IT Hz when IP"
// #define SECRET_WIFI_PASSWORD "HvBiBB6Z4JnWo7FD8LJ4"

#define SECRET_WIFI_SSID "IT Hz when IP 2.4"
#define SECRET_WIFI_PASSWORD "Popevisit2018"

// Settings: MQTT TUB
#define SECRET_MQTT_BROKER "heide.bastla.net" // Server hostname (FQDN)
#define SECRET_MQTT_PORT 8883                 // Server TLS port
#define SECRET_MQTT_USER "mse25"              // Server credentials
#define SECRET_MQTT_PASSWORD "dasCrazy"
#define SECRET_MQTT_PREFIX "cadse"  // Prefix for topic and client ID
#define SECRET_MQTT_YEAR 2025       // Current year (beginning of course)
#define SECRET_MQTT_BOARDID "25_16" // [ID]: Assigned PCB number

// // Settings: MQTT Localhost
// #define SECRET_MQTT_BROKER   "192.168.2.41"        // Server hostname (FQDN)
// #define SECRET_MQTT_PORT     1883               // Server TLS port
// #define SECRET_MQTT_USER     ""                 // Server credentials
// #define SECRET_MQTT_PASSWORD ""
// #define SECRET_MQTT_PREFIX   "cadse"            // Prefix for topic and
// client ID #define SECRET_MQTT_YEAR     2025               // Current year
// (beginning of course)
