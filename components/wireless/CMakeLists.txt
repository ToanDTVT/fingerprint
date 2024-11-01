idf_component_register(SRCS "wireless.c"
                    INCLUDE_DIRS "include"
                    REQUIRES wireless esp_wifi mqtt esp_event)