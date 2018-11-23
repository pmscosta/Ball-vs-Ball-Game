/**
 * @file driver_receive.h
 * @author Pedro Costa, João Fidalgo
 * @data 1 Jan 2018
 * @brief Fila that contains the information about the function driver_receive()
 */
#ifndef __DRIVER_RECEIVE_H
#define __DRIVER_RECEIVE_H

#include <minix/drivers.h>
#include <minix/com.h>

/** defgroup driver_receive driver_receive
 * @{
 *
 * Information regarding driver_receive()
 */

/**
 * @brief Used by device drivers to receive messages from the kernel or other processes.
 *
 * @param origin - specifies the sender of the messages
 * @param msg - address of a message variable that will be initialized by the function with the message received
 * @param ipc_status - address of a int variable that will be initialized by the function with the IPC related status
 *
 * @return 0 upon success, non-zero otherwise
 */

int driver_receive( (endpoint_t) int origin, message * msg, int * ipc_status);

/** @} end of driver_receive */

#endif
