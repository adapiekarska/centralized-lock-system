def wifi_client_start():
    # WiFi Client should wait for connection to AP
    wait_event(WIFI_CONNECTED)

    while (TRUE):
        status = wifi_socket_connect()
        if (status != SUCCESS)
            break
        # Communication loop
        while (TRUE):
            # Wait for pending operation
            wait_event(WIFI_CLIENT_SEND_PENDING or WIFI_CLIENT_RECEIVE_PENDING)
            if (event_pending(WIFI_CLIENT_SEND_PENDING)):
                # Requested operation is SEND
                status = wifi_socket_send(data_to_send)
                if (status != SUCCESS):
                    # Notify transmission fail
                    notify_event(WIFI_CLIENT_TRANSMISSION_FAIL)
                    break
            else:
                # Requested operation is RECEIVE
                status, received_data = wifi_socket_receive_data()
                if (status != SUCCESS):
                    # Notify transmission fail
                    notify_event(WIFI_CLIENT_TRANSMISSION_FAIL)
                    break
            # Notify transmission success
            notify_event(WIFI_CLIENT_TRANSMISSION_SUCCESS)
        wifi_socket_shutdown()
