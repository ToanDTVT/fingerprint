idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES uart_lib gpio_lib user_data handle_password handle_fingerprint wireless mqtt json)
