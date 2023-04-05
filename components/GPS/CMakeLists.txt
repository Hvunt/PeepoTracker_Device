idf_component_register(SRCS "gps_handler.c" "gps_driver.c"
                        INCLUDE_DIRS "."
                        REQUIRES "driver" "freertos" "mqtt_x")