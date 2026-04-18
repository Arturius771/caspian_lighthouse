/**
 * @file Networking.h
 * @brief Network communication module for Wi-Fi and MQTT connectivity.
 *
 * This module provides functionality for establishing and managing network
 * communication, including secure Wi-Fi connections and MQTT messaging.
 */

#include <Modes.h>
#include <PubSubClient.h> // MQTT client library

// Encryption using R13 from https://letsencrypt.org/certificates/
const char tlsPublicCertificateR13[] = ("\
-----BEGIN CERTIFICATE-----\n\
MIIFBTCCAu2gAwIBAgIQWgDyEtjUtIDzkkFX6imDBTANBgkqhkiG9w0BAQsFADBP\n\
MQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJuZXQgU2VjdXJpdHkgUmVzZWFy\n\
Y2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBYMTAeFw0yNDAzMTMwMDAwMDBa\n\
Fw0yNzAzMTIyMzU5NTlaMDMxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBF\n\
bmNyeXB0MQwwCgYDVQQDEwNSMTMwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEK\n\
AoIBAQClZ3CN0FaBZBUXYc25BtStGZCMJlA3mBZjklTb2cyEBZPs0+wIG6BgUUNI\n\
fSvHSJaetC3ancgnO1ehn6vw1g7UDjDKb5ux0daknTI+WE41b0VYaHEX/D7YXYKg\n\
L7JRbLAaXbhZzjVlyIuhrxA3/+OcXcJJFzT/jCuLjfC8cSyTDB0FxLrHzarJXnzR\n\
yQH3nAP2/Apd9Np75tt2QnDr9E0i2gB3b9bJXxf92nUupVcM9upctuBzpWjPoXTi\n\
dYJ+EJ/B9aLrAek4sQpEzNPCifVJNYIKNLMc6YjCR06CDgo28EdPivEpBHXazeGa\n\
XP9enZiVuppD0EqiFwUBBDDTMrOPAgMBAAGjgfgwgfUwDgYDVR0PAQH/BAQDAgGG\n\
MB0GA1UdJQQWMBQGCCsGAQUFBwMCBggrBgEFBQcDATASBgNVHRMBAf8ECDAGAQH/\n\
AgEAMB0GA1UdDgQWBBTnq58PLDOgU9NeT3jIsoQOO9aSMzAfBgNVHSMEGDAWgBR5\n\
tFnme7bl5AFzgAiIyBpY9umbbjAyBggrBgEFBQcBAQQmMCQwIgYIKwYBBQUHMAKG\n\
Fmh0dHA6Ly94MS5pLmxlbmNyLm9yZy8wEwYDVR0gBAwwCjAIBgZngQwBAgEwJwYD\n\
VR0fBCAwHjAcoBqgGIYWaHR0cDovL3gxLmMubGVuY3Iub3JnLzANBgkqhkiG9w0B\n\
AQsFAAOCAgEAUTdYUqEimzW7TbrOypLqCfL7VOwYf/Q79OH5cHLCZeggfQhDconl\n\
k7Kgh8b0vi+/XuWu7CN8n/UPeg1vo3G+taXirrytthQinAHGwc/UdbOygJa9zuBc\n\
VyqoH3CXTXDInT+8a+c3aEVMJ2St+pSn4ed+WkDp8ijsijvEyFwE47hulW0Ltzjg\n\
9fOV5Pmrg/zxWbRuL+k0DBDHEJennCsAen7c35Pmx7jpmJ/HtgRhcnz0yjSBvyIw\n\
6L1QIupkCv2SBODT/xDD3gfQQyKv6roV4G2EhfEyAsWpmojxjCUCGiyg97FvDtm/\n\
NK2LSc9lybKxB73I2+P2G3CaWpvvpAiHCVu30jW8GCxKdfhsXtnIy2imskQqVZ2m\n\
0Pmxobb28Tucr7xBK7CtwvPrb79os7u2XP3O5f9b/H66GNyRrglRXlrYjI1oGYL/\n\
f4I1n/Sgusda6WvA6C190kxjU15Y12mHU4+BxyR9cx2hhGS9fAjMZKJss28qxvz6\n\
Axu4CaDmRNZpK/pQrXF17yXCXkmEWgvSOEZy6Z9pcbLIVEGckV/iVeq0AOo2pkg9\n\
p4QRIy0tK2diRENLSF2KysFwbY6B26BFeFs3v1sYVRhFW9nLkOrQVporCS0KyZmf\n\
wVD89qSTlnctLcZnIavjKsKUu1nA1iU0yYMdYepKR7lWbnwhdx3ewok=\n\
-----END CERTIFICATE-----\n");

/**
 * @brief Hostname or IP address of the MQTT broker.
 */
const char MQTT_BROKER[] = SECRET_MQTT_BROKER;
/**
 * @brief Port number used for MQTT communication.
 */
const int MQTT_PORT = SECRET_MQTT_PORT;
/**
 * @brief Username used for MQTT authentication.
 */
const char MQTT_USER[] = SECRET_MQTT_USER;
/**
 * @brief Password used for MQTT authentication.
 */
const char MQTT_PASSWORD[] = SECRET_MQTT_PASSWORD;
/**
 * @brief Prefix used to build MQTT topic paths.
 */
const String MQTT_PREFIX = SECRET_MQTT_PREFIX;
/**
 * @brief Year component used in MQTT topic paths.
 */
const int MQTT_YEAR = SECRET_MQTT_YEAR;
/**
 * @brief Board identifier used in MQTT topic paths.
 */
const String MQTT_BOARD_ID = SECRET_MQTT_BOARDID;

/**
 * @brief Manages the Wi-Fi client used for network communication.
 */
class WiFiManager {
private:
  /**
   * @brief Underlying Wi-Fi client instance.
   */
  WiFiClientSecure wifiClient;

public:
  /**
   * @brief Returns the Wi-Fi client instance.
   *
   * @return Reference to the Wi-Fi client.
   */
  WiFiClientSecure &getWifiClient() { return wifiClient; }
};

/**
 * @brief Global MQTT manager instance.
 */
extern WiFiManager wifiManager;

/**
 * @brief Manages MQTT topics and the MQTT client instance.
 */
class MQTTManager {
private:
  /**
   * @brief Base MQTT topic path for this board.
   */
  String mqttTopic;
  /**
   * @brief Base MQTT publish topic path.
   */
  String mqttPublish;
  /**
   * @brief Base MQTT subscribe topic path.
   */
  String mqttSubscribe;
  /**
   * @brief MQTT client instance.
   */
  PubSubClient mqttClient;

public:
  /**
   * @brief Constructs the MQTT manager and initializes topic paths and client.
   */
  MQTTManager()
      : mqttTopic(MQTT_PREFIX + "/" + String(MQTT_YEAR) + "/" + MQTT_BOARD_ID +
                  "/"),
        mqttPublish(mqttTopic), mqttSubscribe(mqttTopic),
        mqttClient(wifiManager.getWifiClient()) {}
  /**
   * @brief Returns the MQTT publish topic prefix.
   *
   * @return MQTT publish topic prefix.
   */
  String getMqttPublish() { return mqttPublish; }
  /**
   * @brief Returns the MQTT subscribe topic prefix.
   *
   * @return MQTT subscribe topic prefix.
   */
  String getMqttSubscribe() { return mqttSubscribe; }
  /**
   * @brief Returns the MQTT client instance.
   *
   * @return PubSubClient& Reference to the MQTT client.
   */
  PubSubClient &getMqttClient() { return mqttClient; }
};

/**
 * @brief Global MQTT manager instance.
 */
extern MQTTManager mqttManager;

/**
 * @brief Checks whether the MQTT client is connected to the broker.
 *
 * @return Status indicating connection state.
 */
ComponentStatus mqttConnectedToBroker();

/**
 * @brief Configures and starts the MQTT client.
 *
 * @param mqttCallbackFunction Callback function for handling incoming MQTT
 * messages.
 */
void startMQTT(void (*mqttCallbackFunction)(char *topic, byte *payload,
                                            unsigned int length));

/**
 * @brief Processes incoming MQTT messages.
 */
void checkForMessages();