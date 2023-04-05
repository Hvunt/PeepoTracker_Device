idf_component_register(SRCS "screen.c"
                        INCLUDE_DIRS "."
                        REQUIRES "OLED" "freertos" "mqtt_x")