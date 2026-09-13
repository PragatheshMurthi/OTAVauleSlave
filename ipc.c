#include "ipc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#define IPC_MASTER_IP        "127.0.0.1"
#define IPC_MASTER_PORT      46001U

#define IPC_BROADCAST_IP     "255.255.255.255"
#define IPC_SLAVE_PORT          47001U

#define IPC_BUFFER_SIZE      256U

static int g_iIpcSocket = -1;


/*
 * Initialize Slave IPC
 *
 * The socket is bound to the Slave receive port.
 * Master will broadcast messages to:
 *
 *     255.255.255.255:46001
 */
ERROR_CODE initialize_ipc(void)
{
    struct sockaddr_in local_addr;
    int enable = 1;

    if (g_iIpcSocket >= 0)
    {
        print_info("initialize_ipc: already initialized\n");
        return ERR_OK;
    }

    /*
     * Create UDP socket
     */
    g_iIpcSocket = socket(AF_INET, SOCK_DGRAM, 0);

    if (g_iIpcSocket < 0)
    {
        perror("socket");
        print_err("initialize_ipc: socket creation failed\n");
        return ERR_NET_IF_FAIL;
    }

    /*
     * Allow multiple Slave processes to listen
     * on the same broadcast port.
     */
    if (setsockopt(g_iIpcSocket,
                   SOL_SOCKET,
                   SO_REUSEADDR,
                   &enable,
                   sizeof(enable)) < 0)
    {
        perror("setsockopt(SO_REUSEADDR)");

        close(g_iIpcSocket);
        g_iIpcSocket = -1;

        return ERR_NET_IF_FAIL;
    }

    /*
     * Enable broadcast capability.
     */
    if (setsockopt(g_iIpcSocket,
                   SOL_SOCKET,
                   SO_BROADCAST,
                   &enable,
                   sizeof(enable)) < 0)
    {
        perror("setsockopt(SO_BROADCAST)");

        close(g_iIpcSocket);
        g_iIpcSocket = -1;

        return ERR_NET_IF_FAIL;
    }

    /*
     * Bind Slave to port 46001.
     *
     * INADDR_ANY is intentional here.
     *
     * We receive packets whose destination is:
     *
     *     255.255.255.255:46001
     */
    memset(&local_addr, 0, sizeof(local_addr));

    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_addr.sin_port = htons(IPC_SLAVE_PORT);

    if (bind(g_iIpcSocket,
             (struct sockaddr *)&local_addr,
             sizeof(local_addr)) < 0)
    {
        perror("bind");
        print_err("initialize_ipc: bind failed\n");

        close(g_iIpcSocket);
        g_iIpcSocket = -1;

        return ERR_NET_IF_FAIL;
    }

    print_info("initialize_ipc: Slave IPC ready\n");
    print_info("initialize_ipc: listening on %s:%u\n",
               IPC_BROADCAST_IP,
               IPC_SLAVE_PORT);

    return ERR_OK;
}


/*
 * Slave -> Master
 *
 * This is UNICAST.
 *
 * Destination:
 *
 *     127.0.0.1:46000
 */
ERROR_CODE send_ipc(PVOID pvData, UINT16 u16Len)
{
    struct sockaddr_in master_addr;
    ssize_t sent_len;

    if (pvData == NULL || u16Len == 0U)
    {
        print_err("send_ipc: invalid data\n");
        return ERR_INVALID_PARAM;
    }

    if (u16Len > IPC_BUFFER_SIZE)
    {
        print_err("send_ipc: payload exceeds buffer size\n");
        return ERR_INVALID_PARAM;
    }

    if (g_iIpcSocket < 0)
    {
        print_err("send_ipc: IPC not initialized\n");
        return ERR_NET_IF_FAIL;
    }

    memset(&master_addr, 0, sizeof(master_addr));

    master_addr.sin_family = AF_INET;
    master_addr.sin_addr.s_addr = inet_addr(IPC_MASTER_IP);
    master_addr.sin_port = htons(IPC_MASTER_PORT);

    sent_len = sendto(g_iIpcSocket,
                      pvData,
                      u16Len,
                      0,
                      (struct sockaddr *)&master_addr,
                      sizeof(master_addr));

    if (sent_len < 0)
    {
        perror("sendto");
        print_err("send_ipc: send to Master failed\n");
        return ERR_NET_IF_FAIL;
    }

    if ((UINT16)sent_len != u16Len)
    {
        print_err("send_ipc: incomplete send (%u/%u bytes)\n",
                  (unsigned int)sent_len,
                  (unsigned int)u16Len);

        return ERR_NET_IF_FAIL;
    }

    print_info("send_ipc: Slave -> Master\n");
    print_info("send_ipc: sent %u bytes to %s:%u\n",
               (unsigned int)u16Len,
               IPC_MASTER_IP,
               IPC_MASTER_PORT);

    return ERR_OK;
}


/*
 * Master -> Slave
 *
 * Master sends a UDP broadcast to:
 *
 *     255.255.255.255:46001
 *
 * Slave receives it here.
 */
ERROR_CODE receive_ipc(PVOID *ppvData)
{
    static UINT8 au8ReceiveBuffer[IPC_BUFFER_SIZE];

    struct sockaddr_in sender_addr;
    socklen_t sender_addr_len = sizeof(sender_addr);

    ssize_t received_len;

    if (ppvData == NULL)
    {
        print_err("receive_ipc: invalid output pointer\n");
        return ERR_INVALID_PARAM;
    }

    *ppvData = NULL;

    if (g_iIpcSocket < 0)
    {
        print_err("receive_ipc: IPC not initialized\n");
        return ERR_NET_IF_FAIL;
    }

    memset(au8ReceiveBuffer, 0, sizeof(au8ReceiveBuffer));
    memset(&sender_addr, 0, sizeof(sender_addr));

    /*
     * Wait for Master broadcast.
     */
    received_len = recvfrom(g_iIpcSocket,
                            au8ReceiveBuffer,
                            sizeof(au8ReceiveBuffer),
                            0,
                            (struct sockaddr *)&sender_addr,
                            &sender_addr_len);

    if (received_len < 0)
    {
        perror("recvfrom");
        print_err("receive_ipc: receive failed\n");
        return ERR_NET_IF_FAIL;
    }

    if (received_len == 0)
    {
        print_err("receive_ipc: received empty packet\n");
        return ERR_NET_IF_FAIL;
    }

    /*
     * Return received buffer to caller.
     *
     * Static buffer remains valid after function returns.
     */
    *ppvData = au8ReceiveBuffer;

    print_info("receive_ipc: Master -> Slave\n");
    print_info("receive_ipc: received %u bytes\n",
               (unsigned int)received_len);

    print_info("receive_ipc: sender = %s:%u\n",
               inet_ntoa(sender_addr.sin_addr),
               ntohs(sender_addr.sin_port));

    return ERR_OK;
}