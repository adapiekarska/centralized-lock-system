#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

// In production these will need to be replaced by something more secure
#define WIFI_SSID CONFIG_WIFI_SSID
#define WIFI_PASS CONFIG_WIFI_PASS

#define WIFI_MAXIMUM_RETRY  10

#define SERVER_IP_ADDR CONFIG_SERVER_ADDR
#define SERVER_PORT CONFIG_SERVER_PORT
#define SERVER_HOSTNAME SERVER_IP_ADDR

#endif // WIFI_CONFIG_H