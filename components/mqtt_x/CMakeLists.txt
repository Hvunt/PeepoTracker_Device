idf_component_register(SRCS "mqtt_x.c"
                        INCLUDE_DIRS "."
                        REQUIRES "freertos" "esp_event" "mqtt" "screen" "GPS")
