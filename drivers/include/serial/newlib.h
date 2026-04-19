/*
 * SPDX-License-Identifier: MIT
 *
 * Header for Serial.
 *
 * Copyright (c) 2025 Embedded and Real-Time Systems Laboratory,
 *            Graduate School of Information Science, Nagoya Univ., JAPAN
 */

/**
 * \file    serial/newlib.h
 * \brief   newlib interface to Serial.
 * \author  Makoto Shimojima
 */

/**
 * \~English
 * \defgroup  Serial newlib
 * \brief     Serial.
 * @{
 *
 * \~Japanese
 * \defgroup  Serial newlib
 * \brief     シリアル．
 * @{
 */


#ifndef _SERIAL_NEWLIB_H_
#define _SERIAL_NEWLIB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <t_stddef.h>
#include <serial/serial.h>

extern FILE *serial_open_newlib_file(ID sio_portid);

#ifdef __cplusplus
}
#endif

#endif // _SERIAL_NEWLIB_H_

/**
 * @} // End of group Serial
 */
