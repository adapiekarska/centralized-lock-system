idf_component_register(SRCS "src/rfid.c" "src/rc522.c" "src/tasks/task_rfid.c"
                       INCLUDE_DIRS "include" "../../common/" "../flow_controller/include/"
                       REQUIRES flow_controller)
