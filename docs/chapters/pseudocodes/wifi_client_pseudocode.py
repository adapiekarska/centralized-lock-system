def wifi_client_start():
    # WiFi Client should wait for connection to AP
    wait_event(WIFI_CONNECTED, DONT_CLEAR)

    Status status

    while (TRUE):
        status = wifi_socket_connect()
        if (status != SUCCESS)
            break

        # Communication loop
        while (TRUE):
            LOG("wifi_client", "Ready to transmit")
            notify_event(WIFI_CLIENT_READY)

            # Wait for pending operation
            wait_event(
                WIFI_CLIENT_SEND_PENDING or WIFI_CLIENT_RECEIVE_PENDING,
                DONT_CLEAR
                )
            if (event_pending(WIFI_CLIENT_SEND_PENDING)):
                # Requested operation is SEND
                clear_event(WIFI_CLIENT_SEND_PENDING)
                status = wifi_socket_transfer_data(pending_data, pending_data_size);
                if (status != SUCCESS):
                    # Notify transmission fail
                    notify_event(WIFI_CLIENT_TRANSMISSION_FAIL)
                    break

            else:
                # Requested operation is RECEIVE
                clear_event(WIFI_CLIENT_RECEIVE_PENDING)
                status = wifi_socket_receive_data(pending_data, pending_data_size)
                if (status != SUCCESS):
                    # Notify transmission fail
                    notify_event(WIFI_CLIENT_TRANSMISSION_FAIL)
                    break
            # Notify transmission success
            notify_event(WIFI_CLIENT_TRANSMISSION_SUCCESS)
        wifi_socket_shutdown()
