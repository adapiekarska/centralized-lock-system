#ifndef FLOW_CONTROLLER_H
#define FLOW_CONTROLLER_H

#define TASK_WIFI_PRIORITY 8 | portPRIVILEGE_BIT
#define TASK_RFID_PRIORITY 4

void deep_sleep_wakeup_flow();

#endif // FLOW_CONTROLLER_H