// Module for interfacing with the I2C interface

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "platform.h"
#include "platform_i2c.h"
#include "auxmods.h"
#include "lrotable.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "bmp280.h"
#include "MPU6050.h"
#include "LPS25HB_Driver.h"

// Lua: speed = i2c.setup( id, speed, slaveaddr )
static int i2c_setup( lua_State *L )
{
    unsigned id = luaL_checkinteger( L, 1 );
    PlatformI2CParam i2cParam;

    i2cParam.speed = ( u32 )luaL_checkinteger( L, 2 );
    i2cParam.slaveAddr = (u8) luaL_checkinteger(L, 3);

    MOD_CHECK_ID( i2c, id );
    lua_pushinteger( L, platform_i2c_setup( id, &i2cParam ) );

    return 1;
}

static int i2c_close( lua_State *L )
{
    unsigned id = luaL_checkinteger( L, 1 );
    MOD_CHECK_ID( i2c, id );
    lua_pushinteger( L, platform_i2c_close( id ) );
    return 1;
}

// Lua: wrote = i2c.write( id, [slave,] reg, data )
// data can be either a string, a table or an 8-bit number
static int i2c_write( lua_State *L )
{
    u8 arg_index = 1;
    unsigned id = luaL_checkinteger( L, arg_index++ );
    u16 slave_addr = I2C_NULL_SLAVE_ADDR;
    int n; //参数个数
    u8 regAddr;
    u32 wrote = 0;

    n = lua_gettop(L);
    
    if (n != 2)
    {
        regAddr = (u8)luaL_checkinteger(L, arg_index++);
    }
    
    MOD_CHECK_ID( i2c, id );

    switch(lua_type(L, arg_index))
    {
    case LUA_TNUMBER:
        {
            u8 numdata = (u8) luaL_checkinteger(L, arg_index);

            if (n == 2)
            {
                wrote = platform_i2c_send_data(id, slave_addr, NULL, &numdata, 1);
            }
            else 
            {
                wrote = platform_i2c_send_data(id, slave_addr, &regAddr, &numdata, 1);
            }
        }
        break;

    case LUA_TSTRING:
        {
            const char *pdata;
            u32 slen;
            
            pdata = luaL_checklstring(L, arg_index, &slen);
            if (n == 2)
            {
                wrote = platform_i2c_send_data(id, slave_addr, NULL, pdata, slen);
            }
            else
            {
                wrote = platform_i2c_send_data(id, slave_addr, &regAddr, pdata, slen);
            }
        }
        break;

    case LUA_TTABLE:
        {
            size_t datalen = lua_objlen(L, arg_index);
            size_t i;
            u8 *pBuff;

            pBuff = malloc(datalen);

            for(i = 0; i < datalen; i++)
            {
                lua_rawgeti(L, arg_index, i+1);
                pBuff[i] = (u8)luaL_checkinteger(L, -1);
            }

            if (n == 2)
            {
                wrote = platform_i2c_send_data(id, slave_addr,  NULL, pBuff, datalen);
            }
            else 
            {
                wrote = platform_i2c_send_data(id, slave_addr, &regAddr, pBuff, datalen);
            }
            free(pBuff);
        }
        break;

    default:
        return luaL_error(L, "i2c.write: data must be number,string,table");
        break;
    }

    lua_pushinteger( L, wrote );
    return 1;
}

// Lua: read = i2c.read( id, [slave,] reg, size )
static int i2c_read( lua_State *L )
{
    u8 arg_index = 1;
    unsigned id = luaL_checkinteger( L, arg_index++ );
    u16 slave_addr = I2C_NULL_SLAVE_ADDR;
    int n;
    u8 regAddr;
    u32 size;
/*+\NEW\RUFEI\2015.5.20\完善I2C处理*/
    luaL_Buffer b={0};
/*-\NEW\RUFEI\2015.5.20\完善I2C处理*/

    n = lua_gettop(L);

    if (n != 2)
    {
        regAddr = (u8)luaL_checkinteger(L, arg_index++);
    }
    size = (u32)luaL_checkinteger( L, arg_index++ );

    MOD_CHECK_ID( i2c, id );
    if( size == 0 )
        return 0;
    
    if(size >= LUAL_BUFFERSIZE)
        return luaL_error(L, "i2c.read: size must < %d", LUAL_BUFFERSIZE);

    luaL_buffinit( L, &b );

/*+\NEW\RUFEI\2015.5.20\完善I2C处理*/
    if (n == 2)
    {
        b.p +=platform_i2c_recv_data(id, slave_addr, NULL, b.p, size);
    }
    else 
    {
        b.p +=platform_i2c_recv_data(id, slave_addr, &regAddr, b.p, size);
    }
/*-\NEW\RUFEI\2015.5.20\完善I2C处理*/

    luaL_pushresult( &b );

    return 1;
}

static int bmp280_handle( lua_State *L )
{
  /* The variable used to read real temperature*/
  s32 v_actual_temp_combined_s32 = BMP280_INIT_VALUE;
  /* The variable used to read real pressure*/
  u32 v_actual_press_combined_u32 = BMP280_INIT_VALUE;
  
  unsigned handleType = luaL_checkinteger( L, 1 );
  switch(handleType)
  {
  case 1:
    bmp280_setting(); 
    lua_pushinteger( L, 0); 
    break;
  case 2:
    /* API is used to read the true temperature and pressure*/
    bmp280_read_pressure_temperature(&v_actual_press_combined_u32,&v_actual_temp_combined_s32);
    lua_pushinteger( L, v_actual_press_combined_u32); 
    break;
  case 3:
    /* API is used to read the true temperature and pressure*/
    bmp280_read_pressure_temperature(&v_actual_press_combined_u32,&v_actual_temp_combined_s32);
    lua_pushinteger( L, v_actual_temp_combined_s32); 
    break;
  default:
    lua_pushinteger( L, 0); 
    break;
  }
  
  return 1;

}

static int mpu6050_handle( lua_State *L )
{
  s16 AccelGyro[6];

  unsigned handleType = luaL_checkinteger( L, 1 );
  switch(handleType)
  {
  case 1:
    MPU6050_I2C_Init(); 
    MPU6050_Initialize();
    lua_pushinteger( L, 0); 
    break;
  case 2:
    MPU6050_GetRawAccelGyro(AccelGyro);
    printf("Y_Y %d %d %d, %d %d %d\n",AccelGyro[0],AccelGyro[1],AccelGyro[2],AccelGyro[3],AccelGyro[4],AccelGyro[5]);
    lua_pushinteger( L, AccelGyro[0]); 
    lua_pushinteger( L, AccelGyro[1]); 
    lua_pushinteger( L, AccelGyro[2]); 
    lua_pushinteger( L, AccelGyro[3]); 
    lua_pushinteger( L, AccelGyro[4]); 
    lua_pushinteger( L, AccelGyro[5]); 
    break;
  default:
    lua_pushinteger( L, 0); 
    break;
  }
  
  return 1;

}

static int lps25hb_handle( lua_State *L )
{
  int32_t Pressure;
  int16_t Temperature;

  unsigned handleType = luaL_checkinteger( L, 1 );
  switch(handleType)
  {
  case 1:
    LPS25HB_HalInit(); 
    LPS25HB_Activate();
    LPS25HB_Set_AvgP(LPS25HB_AVGP_32);
    LPS25HB_Set_AvgT(LPS25HB_AVGT_16);
    lua_pushinteger( L, 0); 
    break;
  case 2:
    LPS25HB_Get_Pressure(&Pressure);
    printf("Pressure: %d\n",Pressure);
    lua_pushinteger( L, Pressure); 
    break;
  case 3:
    LPS25HB_Get_Temperature(&Temperature);
    printf("Temperature: %d\n",Temperature);
    lua_pushinteger( L, Temperature); 
    break;   
  default:
    lua_pushinteger( L, 0); 
    break;
  }
  
  return 1;

}

// Module function map
#define MIN_OPT_LEVEL   2
#include "lrodefs.h"
const LUA_REG_TYPE i2c_map[] = 
{
  { LSTRKEY( "setup" ),  LFUNCVAL( i2c_setup ) },
  { LSTRKEY( "close" ),  LFUNCVAL( i2c_close ) },
  { LSTRKEY( "write" ), LFUNCVAL( i2c_write ) },
  { LSTRKEY( "read" ), LFUNCVAL( i2c_read ) },
  { LSTRKEY( "bmp280" ), LFUNCVAL( bmp280_handle ) },
  { LSTRKEY( "mpu6050" ), LFUNCVAL( mpu6050_handle ) },
  { LSTRKEY( "lps25hb" ), LFUNCVAL( lps25hb_handle ) },
#if LUA_OPTIMIZE_MEMORY > 0
  { LSTRKEY( "FAST" ), LNUMVAL( PLATFORM_I2C_SPEED_FAST ) },
  { LSTRKEY( "SLOW" ), LNUMVAL( PLATFORM_I2C_SPEED_SLOW ) },
#endif
  { LNILKEY, LNILVAL }
};

LUALIB_API int luaopen_i2c( lua_State *L )
{
#if LUA_OPTIMIZE_MEMORY > 0
  return 0;
#else // #if LUA_OPTIMIZE_MEMORY > 0
  luaL_register( L, AUXLIB_I2C, i2c_map );
  
  MOD_REG_NUMBER( L, "FAST", PLATFORM_I2C_SPEED_FAST );
  MOD_REG_NUMBER( L, "SLOW", PLATFORM_I2C_SPEED_SLOW ); 
  
  return 1;
#endif // #if LUA_OPTIMIZE_MEMORY > 0
}

