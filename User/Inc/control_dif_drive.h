/*
 * control_dif_drive.h
 *
 *  Created on: May 24, 2021
 *      Author: masaya
 */

#ifndef INC_CONTROL_DIF_DRIVE_H_
#define INC_CONTROL_DIF_DRIVE_H_

#ifdef __cplusplus
 extern "C" {
#endif

void init_dif_drive();
void init_dif_drive_task();
void dif_drive_task(void * argument);

#ifdef __cplusplus
}
#endif
#endif /* INC_CONTROL_DIF_DRIVE_H_ */
