idf_component_register(SRCS "rfid.c" "rc522.c" "tasks/task_rfid.c"
                       INCLUDE_DIRS "include" "../../common/" "../flow_controller/include/"
                       REQUIRES flow_controller)
