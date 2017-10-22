#ifndef __LSM330D_H__
#define __LSM330D_H__

/*!
* @brief The following definition uses for define the data types
*
* @note While porting the API please consider the following
* @note Please check the version of C standard
* @note Are you using Linux platform
*/

/*!
* @brief For the Linux platform support
* Please use the types.h for your data types definitions
*/
#ifdef	__KERNEL__

#include <linux/types.h>
#include <linux/math64.h>
/* singed integer type*/
typedef	int8_t s8;/**< used for signed 8bit */
typedef	int16_t s16;/**< used for signed 16bit */
typedef	int32_t s32;/**< used for signed 32bit */
typedef	int64_t s64;/**< used for signed 64bit */

typedef	u_int8_t u8;/**< used for unsigned 8bit */
typedef	u_int16_t u16;/**< used for unsigned 16bit */
typedef	u_int32_t u32;/**< used for unsigned 32bit */
typedef	u_int64_t u64;/**< used for unsigned 64bit */



#else /* ! __KERNEL__ */
/**********************************************************
* These definition uses for define the C
* standard version data types
***********************************************************/
#if defined(__STDC_VERSION__)

/************************************************
 * compiler is C11 C standard
************************************************/
#if (__STDC_VERSION__ == 201112L)

/************************************************/
#include <stdint.h>
/************************************************/

/*unsigned integer types*/
typedef	uint8_t u8;/**< used for unsigned 8bit */
typedef	uint16_t u16;/**< used for unsigned 16bit */
typedef	uint32_t u32;/**< used for unsigned 32bit */
typedef	uint64_t u64;/**< used for unsigned 64bit */

/*signed integer types*/
typedef	int8_t s8;/**< used for signed 8bit */
typedef	int16_t s16;/**< used for signed 16bit */
typedef	int32_t s32;/**< used for signed 32bit */
typedef	int64_t s64;/**< used for signed 64bit */
/************************************************
 * compiler is C99 C standard
************************************************/

#elif (__STDC_VERSION__ == 199901L)

/* stdint.h is a C99 supported c library.
which is used to fixed the integer size*/
/************************************************/
#include <stdint.h>
/************************************************/

/*unsigned integer types*/
typedef	uint8_t u8;/**< used for unsigned 8bit */
typedef	uint16_t u16;/**< used for unsigned 16bit */
typedef	uint32_t u32;/**< used for unsigned 32bit */
typedef	uint64_t u64;/**< used for unsigned 64bit */

/*signed integer types*/
typedef int8_t s8;/**< used for signed 8bit */
typedef	int16_t s16;/**< used for signed 16bit */
typedef	int32_t s32;/**< used for signed 32bit */
typedef	int64_t s64;/**< used for signed 64bit */
/************************************************
 * compiler is C89 or other C standard
************************************************/

#else /*  !defined(__STDC_VERSION__) */
/*!
* @brief By default it is defined as 32 bit machine configuration
*	define your data types based on your
*	machine/compiler/controller configuration
*/
#define  MACHINE_32_BIT

/*! @brief
 *	If your machine support 16 bit
 *	define the MACHINE_16_BIT
 */
#ifdef MACHINE_16_BIT
#include <limits.h>
/*signed integer types*/
typedef	signed char  s8;/**< used for signed 8bit */
typedef	signed short int s16;/**< used for signed 16bit */
typedef	signed long int s32;/**< used for signed 32bit */

#if defined(LONG_MAX) && LONG_MAX == 0x7fffffffffffffffL
typedef long int s64;/**< used for signed 64bit */
typedef unsigned long int u64;/**< used for unsigned 64bit */
#elif defined(LLONG_MAX) && (LLONG_MAX == 0x7fffffffffffffffLL)
typedef long long int s64;/**< used for signed 64bit */
typedef unsigned long long int u64;/**< used for unsigned 64bit */
#else
#warning Either the correct data type for signed 64 bit integer \
could not be found, or 64 bit integers are not supported in your environment.
#warning The API will only offer 32 bit pressure calculation.This will \
slightly impede accuracy(noise of ~1 pascal RMS will be added to output).
#warning If 64 bit integers are supported on your platform, \
please set s64 manually.
#endif

/*unsigned integer types*/
typedef	unsigned char u8;/**< used for unsigned 8bit */
typedef	unsigned short int u16;/**< used for unsigned 16bit */
typedef	unsigned long int u32;/**< used for unsigned 32bit */

/* If your machine support 32 bit
define the MACHINE_32_BIT*/
#elif defined MACHINE_32_BIT
/*signed integer types*/
typedef	signed char  s8;/**< used for signed 8bit */
typedef	signed short int s16;/**< used for signed 16bit */
typedef	signed int s32;/**< used for signed 32bit */
typedef	signed long long int s64;/**< used for signed 64bit */

/*unsigned integer types*/
typedef	unsigned char u8;/**< used for unsigned 8bit */
typedef	unsigned short int u16;/**< used for unsigned 16bit */
typedef	unsigned int u32;/**< used for unsigned 32bit */
typedef	unsigned long long int u64;/**< used for unsigned 64bit */

/* If your machine support 64 bit
define the MACHINE_64_BIT*/
#elif defined MACHINE_64_BIT
/*signed integer types*/
typedef	signed char  s8;/**< used for signed 8bit */
typedef	signed short int s16;/**< used for signed 16bit */
typedef	signed int s32;/**< used for signed 32bit */
typedef	signed long int s64;/**< used for signed 64bit */

/*unsigned integer types*/
typedef	unsigned char u8;/**< used for unsigned 8bit */
typedef	unsigned short int u16;/**< used for unsigned 16bit */
typedef	unsigned int u32;/**< used for unsigned 32bit */
typedef	unsigned long int u64;/**< used for unsigned 64bit */

#else
#warning The data types defined above which not supported \
define the data types manually
#endif
#endif

/*** This else will execute for the compilers
 *	which are not supported the C standards
 *	Like C89/C99/C11***/
#else
/*!
* @brief By default it is defined as 32 bit machine configuration
*	define your data types based on your
*	machine/compiler/controller configuration
*/
#define  MACHINE_32_BIT

/* If your machine support 16 bit
define the MACHINE_16_BIT*/
#ifdef MACHINE_16_BIT
#include <limits.h>
/*signed integer types*/
typedef	signed char  s8;/**< used for signed 8bit */
typedef	signed short int s16;/**< used for signed 16bit */
typedef	signed long int s32;/**< used for signed 32bit */

#if defined(LONG_MAX) && LONG_MAX == 0x7fffffffffffffffL
typedef long int s64;/**< used for signed 64bit */
typedef unsigned long int u64;/**< used for unsigned 64bit */
#elif defined(LLONG_MAX) && (LLONG_MAX == 0x7fffffffffffffffLL)
typedef long long int s64;/**< used for signed 64bit */
typedef unsigned long long int u64;/**< used for unsigned 64bit */
#else
#warning Either the correct data type for signed 64 bit integer \
could not be found, or 64 bit integers are not supported in your environment.
#warning The API will only offer 32 bit pressure calculation.This will \
slightly impede accuracy(noise of ~1 pascal RMS will be added to output).
#warning If 64 bit integers are supported on your platform, \
please set s64 manually.
#endif

/*unsigned integer types*/
typedef	unsigned char u8;/**< used for unsigned 8bit */
typedef	unsigned short int u16;/**< used for unsigned 16bit */
typedef	unsigned long int u32;/**< used for unsigned 32bit */

/*! @brief If your machine support 32 bit
define the MACHINE_32_BIT*/
#elif defined MACHINE_32_BIT
/*signed integer types*/
typedef	signed char  s8;/**< used for signed 8bit */
typedef	signed short int s16;/**< used for signed 16bit */
typedef	signed int s32;/**< used for signed 32bit */
typedef	signed long long int s64;/**< used for signed 64bit */

/*unsigned integer types*/
typedef	unsigned char u8;/**< used for unsigned 8bit */
typedef	unsigned short int u16;/**< used for unsigned 16bit */
typedef	unsigned int u32;/**< used for unsigned 32bit */
typedef	unsigned long long int u64;/**< used for unsigned 64bit */

/* If your machine support 64 bit
define the MACHINE_64_BIT*/
#elif defined MACHINE_64_BIT
/*signed integer types*/
typedef	signed char  s8;/**< used for signed 8bit */
typedef	signed short int s16;/**< used for signed 16bit */
typedef	signed int s32;/**< used for signed 32bit */
typedef	signed long int s64;/**< used for signed 64bit */

/*unsigned integer types*/
typedef	unsigned char u8;/**< used for unsigned 8bit */
typedef	unsigned short int u16;/**< used for unsigned 16bit */
typedef	unsigned int u32;/**< used for unsigned 32bit */
typedef	unsigned long int u64;/**< used for unsigned 64bit */

#else
#warning The data types defined above which not supported \
define the data types manually
#endif
#endif
#endif

// LSM330D I2C address
// Accelerometer = 001100xb
#define LSM330D_A_ADDR0     0b00011000  // SA0=0(GND)
#define LSM330D_A_ADDR1     0b00011001  // SA0=1(VDD)

// gyro = 110101xb
#define LSM330D_G_ADDR0     0b01101010  // SA0=0(GND)
#define LSM330D_G_ADDR1     0b01101011  // SA0=1(VDD)

// LSM330D Register Map
// Accelerometer registers
#define CTRL_REG1_A     0x20
#define CTRL_REG2_A     0x21
#define CTRL_REG3_A     0x22
#define CTRL_REG4_A     0x23
#define CTRL_REG5_A     0x24

#define OUT_X_L_A       0x28
#define OUT_X_H_A       0x29
#define OUT_Y_L_A       0x2A
#define OUT_Y_H_A       0x2B
#define OUT_Z_L_A       0x2C
#define OUT_Z_H_A       0x2D


#define CTRL_REG6_A 	0x25
#define REFERENCE_A 	0x26
#define STATUS_REG_A	0x27

#define FIFO_CTRL_REG 	0x2E
#define FIFO_SRC_REG 	0x2F
#define INT1_CFG_A 		0x30
#define INT1_SOURCE_A 	0x31
#define INT1_THS_A 		0x32
#define INT1_DURATION_A 0x33
#define INT2_CFG_A 		0x34
#define INT2_SOURCE_A 	0x35
#define INT2_THS_A 		0x36
#define INT2_DURATION_A 0x37
#define CLICK_CFG_A 	0x38
#define CLICK_SRC_A 	0x39
#define CLICK_THS_A 	0x3A
#define TIME_LIMIT_A 	0x3B


#define TIME_LATENCY_A 	0x3C
#define TIME_WINDOW_A 	0x3D
#define Act_THS 		0x3E
#define Act_DUR 		0x3F



// Gyroscope registers
#define WHO_AM_I_G      0x0F
#define WHOAMI_G_VAL    0xD4

#define CTRL_REG1_G     0x20
#define CTRL_REG2_G     0x21
#define CTRL_REG3_G     0x22
#define CTRL_REG4_G     0x23
#define CTRL_REG5_G     0x24

#define REFERENCE_G		0x25
#define OUT_TEMP_G		0x26
#define STATUS_REG_G 	0x27

#define OUT_X_L_G       0x28
#define OUT_X_H_G       0x29
#define OUT_Y_L_G       0x2A
#define OUT_Y_H_G       0x2B
#define OUT_Z_L_G       0x2C
#define OUT_Z_H_G       0x2D

#define FIFO_CTRL_REG_G 0x2E
#define FIFO_SRC_REG_G	0x2F
#define INT1_CFG_G		0x30
#define INT1_SRC_G 		0x31
#define INT1_TSH_XH_G 	0x32
#define INT1_TSH_XL_G 	0x33
#define INT1_TSH_YH_G 	0x34
#define INT1_TSH_YL_G 	0x35
#define INT1_TSH_ZH_G 	0x36
#define INT1_TSH_ZL_G 	0x37
#define INT1_DURATION_G 0x38

// The LSM330D functions over both I2C or SPI. This library supports both.
// But the interface mode used must be sent to the LSM330D constructor. Use
// one of these two as the first parameter of the constructor.
//#define MODE_I2C 1
//#define interface_mode MODE_I2C

typedef enum 
{
	MODE_SPI,
	MODE_I2C,
}interface_mode;


typedef enum 
{
	G_SCALE_250DPS,		// 00:  250 degrees per second
	G_SCALE_500DPS,		// 01:  500 dps
	G_SCALE_2000DPS	// 10:  2000 dps
}gyro_scale;
// accel_scale defines all possible FSR's of the accelerometer:
typedef enum 
{
	A_SCALE_2G,	// 00:  2g
	A_SCALE_4G,	// 01:  4g
	A_SCALE_8G,	// 10:  8g
	A_SCALE_16G	// 11:  16g
}accel_scale;
// gyro_odr defines all possible data rate/bandwidth combos of the gyro:
typedef enum 
{							// ODR (Hz) --- Cutoff
	G_ODR_95_BW_125  = 0x0, //   95         12.5
	G_ODR_95_BW_25   = 0x1, //   95          25
	// 0x2 and 0x3 define the same data rate and bandwidth
	G_ODR_190_BW_125 = 0x4, //   190        12.5
	G_ODR_190_BW_25  = 0x5, //   190         25
	G_ODR_190_BW_50  = 0x6, //   190         50
	G_ODR_190_BW_70  = 0x7, //   190         70
	G_ODR_380_BW_20  = 0x8, //   380         20
	G_ODR_380_BW_25  = 0x9, //   380         25
	G_ODR_380_BW_50  = 0xA, //   380         50
	G_ODR_380_BW_100 = 0xB, //   380         100
	G_ODR_760_BW_30  = 0xC, //   760         30
	G_ODR_760_BW_35  = 0xD, //   760         35
	G_ODR_760_BW_50  = 0xE, //   760         50
	G_ODR_760_BW_100 = 0xF //   760         100
}gyro_odr;
// accel_oder defines all possible output data rates of the accelerometer:
typedef enum 
{
	A_POWER_DOWN, 	// Power-down mode (0x0)
	A_ODR_1,		// 1 Hz	(0x1)
	A_ODR_10,		// 10 Hz (0x2)
	A_ODR_25,		// 25 Hz (0x3)
	A_ODR_50,		// 50 Hz (0x4)
	A_ODR_100,		// 100 Hz (0x5)
	A_ODR_200,		// 200 Hz (0x6)
	A_ODR_400,		// 400 Hz (0x7)
	A_ODR_1620,		// 1620 Hz (0x9)
	A_ODR_5376		// 5376 Hz (0x9)
}accel_odr;

// LSM9DS0 -- LSM9DS0 class constructor
// The constructor will set up a handful of private variables, and set the
// communication mode as well.
// Input:
//	- interface = Either MODE_SPI or MODE_I2C, whichever you're using
//				to talk to the IC.
//	- gAddr = If MODE_I2C, this is the I2C address of the gyroscope.
// 				If MODE_SPI, this is the chip select pin of the gyro (CSG)
//	- xmAddr = If MODE_I2C, this is the I2C address of the accel.
//				If MODE_SPI, this is the cs pin of the accel (CSXM)
void LSM330D(interface_mode interface, uint8_t gAddr, uint8_t xmAddr);

// begin() -- Initialize the gyro, accelerometer, and magnetometer.
// This will set up the scale and output rate of each sensor. It'll also
// "turn on" every sensor and every axis of every sensor.
// Input:
//	- gScl = The scale of the gyroscope. This should be a gyro_scale value.
//	- aScl = The scale of the accelerometer. Should be a accel_scale value.
//	- gODR = Output data rate of the gyroscope. gyro_odr value.
//	- aODR = Output data rate of the accelerometer. accel_odr value.

// Output: The function will return an unsigned 16-bit value. The most-sig
//		bytes of the output are the WHO_AM_I reading of the accel. The
//		least significant two bytes are the WHO_AM_I reading of the gyro.
// All parameters have a defaulted value, so you can call just "begin()".
// Default values are FSR's of:  245DPS, 2g, 2Gs; ODRs of 95 Hz for 
// gyro, 100 Hz for accelerometer.
// Use the return value of this function to verify communication.
uint16_t begin(gyro_scale gScl, accel_scale aScl,  
						gyro_odr gODR, accel_odr aODR);

// readGyro() -- Read the gyroscope output registers.
// This function will read all six gyroscope output registers.
// The readings are stored in the class' gx, gy, and gz variables. Read
// those _after_ calling readGyro().
void readGyro(void);

// readAccel() -- Read the accelerometer output registers.
// This function will read all six accelerometer output registers.
// The readings are stored in the class' ax, ay, and az variables. Read
// those _after_ calling readAccel().
void readAccel(void);

// readTemp() -- Read the temperature output register.
// This function will read two temperature output registers.
// The combined readings are stored in the class' temperature variables. Read
// those _after_ calling readTemp().
void readTemp(void);

// calcGyro() -- Convert from RAW signed 16-bit value to degrees per second
// This function reads in a signed 16-bit value and returns the scaled
// DPS. This function relies on gScale and gRes being correct.
// Input:
//	- gyro = A signed 16-bit raw reading from the gyroscope.
float calcGyro(int16_t gyro);

// calcAccel() -- Convert from RAW signed 16-bit value to gravity (g's).
// This function reads in a signed 16-bit value and returns the scaled
// g's. This function relies on aScale and aRes being correct.
// Input:
//	- accel = A signed 16-bit raw reading from the accelerometer.
float calcAccel(int16_t accel);

// setGyroScale() -- Set the full-scale range of the gyroscope.
// This function can be called to set the scale of the gyroscope to 
// 245, 500, or 200 degrees per second.
// Input:
// 	- gScl = The desired gyroscope scale. Must be one of three possible
//		values from the gyro_scale enum.
void setGyroScale(gyro_scale gScl);

// setAccelScale() -- Set the full-scale range of the accelerometer.
// This function can be called to set the scale of the accelerometer to
// 2, 4, 6, 8, or 16 g's.
// Input:
// 	- aScl = The desired accelerometer scale. Must be one of five possible
//		values from the accel_scale enum.
void setAccelScale(accel_scale aScl);

// setGyroODR() -- Set the output data rate and bandwidth of the gyroscope
// Input:
//	- gRate = The desired output rate and cutoff frequency of the gyro.
//		Must be a value from the gyro_odr enum (check above, there're 14).
void setGyroODR(gyro_odr gRate);

// setAccelODR() -- Set the output data rate of the accelerometer
// Input:
//	- aRate = The desired output rate of the accel.
//		Must be a value from the accel_odr enum (check above, there're 11).
void setAccelODR(accel_odr aRate); 	


// configGyroInt() -- Configure the gyro interrupt output.
// Triggers can be set to either rising above or falling below a specified
// threshold. This function helps setup the interrupt configuration and 
// threshold values for all axes.
// Input:
//	- int1Cfg = A 8-bit value that is sent directly to the INT1_CFG_G
//		register. This sets AND/OR and high/low interrupt gen for each axis
//	- int1ThsX = 16-bit interrupt threshold value for x-axis
//	- int1ThsY = 16-bit interrupt threshold value for y-axis
//	- int1ThsZ = 16-bit interrupt threshold value for z-axis
//	- duration = Duration an interrupt holds after triggered. This value
// 		is copied directly into the INT1_DURATION_G register.
// Before using this function, read about the INT1_CFG_G register and
// the related INT1* registers in the LMS9DS0 datasheet.
void configGyroInt(uint8_t int1Cfg, uint16_t int1ThsX, uint16_t int1ThsY, uint16_t int1ThsZ, uint8_t duration);


void calLSM330D(float gbias[3], float abias[3]);


#endif
