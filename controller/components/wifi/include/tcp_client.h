#ifndef TASK_TCP_CLIENT_H
#define TASK_TCP_CLIENT_H

/**
 * @brief Communicate via TCP socket
 *
 */
void tcp_client_start();

/**
 * @brief Transfer given data to attestation server
 * 
 * @param data      pointer to buffer containing data to be sent
 * @param data_size data buffer size
 */
void send_data_to_server(
    void*   data,
    int     data_size
);

#endif // TASK_TCP_CLIENT_H