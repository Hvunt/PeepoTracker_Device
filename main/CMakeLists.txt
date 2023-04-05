idf_component_register(SRCS "main.c" "wifi_connection.c"
                       INCLUDE_DIRS "inc"
                       REQUIRES "esp_netif" "esp_wifi" "mqtt_x" 
                                "screen" "nvs_flash" "lwip")
