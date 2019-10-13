idf_component_register(
    SRCS "tasks/task_connection_watcher.c" "tasks/task_wifi_client.c" "wifi_client.c" "wifi_status.c" "wifi.c"
    INCLUDE_DIRS "include" "../../common/"
    EMBED_TXTFILES ca.cer)