/*
 * RabbitBotConfig.h
 *
 *  Created on: Jul 18, 2021
 *      Author: masaya
 */

#ifndef INC_RABBITBOTCONFIG_H_
#define INC_RABBITBOTCONFIG_H_

const float WheelSeparation = 0.185; //0.183??
const float DiameterWheel = 0.066;
const uint16_t EncPPR = 448;
const uint16_t GearRatio = 10;
const int16_t MaxMotorNrpm = 1500;
const float MaxMotorNrps = MaxMotorNrpm/60;

const float PgainMotor = 2.9;
const float IgainMotor = 0.1;
const float DgainMotor = 0.1;

/* Period of Periodic Task [ms] */
const uint16_t DifDrivePeriod = 2;
const uint16_t IMUPeriod = 8;
const uint16_t DiagPeriod = 64;
const uint16_t TPauseRosSerial = 8;
const uint16_t DivisionPubJointStates = 4;
const uint16_t DivisionBatteryMsg = 8;

/* ROS Frame ID */
#define OdomHeaderFrameID "odom"
//#define OdomChildFrameID "base_footprint"
#define IMUFrameID "imu_link"
#define JointStateFrameID "base_link"
#define LeftWheelJointStateName "wheel_left_joint"
#define RightWheelJointStateName "wheel_right_joint"
#define WHEEL_NUM 2

/* Battery Info */
#define CapacityCurrent 1.8
#define CapacityVoltage 11.1
#define VoltageOffset 0.12

#endif /* INC_RABBITBOTCONFIG_H_ */
