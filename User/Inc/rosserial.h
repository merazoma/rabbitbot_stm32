/*
 * rosserial.h
 *
 *  Created on: May 29, 2021
 *      Author: masaya
 */

#ifndef ROSSERIAL_H_
#define ROSSERIAL_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

void init_rosserial(void);

#ifdef __cplusplus
}
#endif


#endif /* ROSSERIAL_H_ */
