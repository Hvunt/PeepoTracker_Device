idf_component_register(SRCS "OLED_0in91.c" "GUI_Paint.c" "font8.c" "font12.c" 
                            "font16.c" "font20.c" "font24.c" 
                            "DEV_Config.c" "ImageData.c"
                        INCLUDE_DIRS "inc"
                        REQUIRES "driver" "freertos")
