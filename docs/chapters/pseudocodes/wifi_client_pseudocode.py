def start_transmission_request_handling():
    # Klient WiFi powinien zaczekac na polaczenie z siecia
    wait_event(WIFI_CONNECTED)

    while (TRUE):
        status = wifi_socket_connect()
        if (status != SUCCESS):
            break
        # Glowna petla obslugi zadan
        while (TRUE):
            # Czekaj na zadanie transmisji przychodzacej lub wychodzacej
            wait_event(SEND_PENDING or RECEIVE_PENDING)
            if (event_pending(SEND_PENDING)):
                # Przyjeto zadanie obslugi transmisji wychodzacej
                status = wifi_socket_send(data_to_send)
                if (status != SUCCESS):
                    # Zasygnalizuj blad transmisji
                    notify_event(TRANSMISSION_FAIL)
                    break
            else:
                # Przyjeto zadanie obslugi transmisji przychodzacej
                status, received_data = wifi_socket_receive_data()
                if (status != SUCCESS):
                    # Zasygnalizuj blad transmisji
                    notify_event(TRANSMISSION_FAIL)
                    break
            # Zasygnalizuj sukces transmisji
            notify_event(TRANSMISSION_SUCCESS)
        wifi_socket_shutdown()
