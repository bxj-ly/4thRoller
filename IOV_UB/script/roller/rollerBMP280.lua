require"common"
module(...,package.seeall)

local function print(...)
	_G.print("rollerBMP280",...)
end

local function init()
	i2c.mpu6050(1)
	print("-------------------")
	print("-------------------")
	print("TestConnection:")
	Accelx,Accely,Accelz,Gyrox,Gyroy,Gyroz=i2c.mpu6050(2)
	print(Accelx,Accely,Accelz,Gyrox,Gyroy,Gyroz)

	i2c.lps25hb(1)
	print("Pressure:")
	print(string.format("%d",i2c.lps25hb(2)))
	print("Temp.:")
	print(string.format("%d",i2c.lps25hb(3)))


	i2c.bmp280(1)
	--bmp280.setting(1)
	print("-------------------")
	--print(bmp280.pressure(1))
	print("-------------------")
	print("Pressure:")
	--local pressure = bmp280.bmp280_read_pressure
	print(string.format("%d",i2c.bmp280(2)))
	print("Temp.:")
	--local temp = bmp280.bmp280_read_temperature
	print(string.format("%d",i2c.bmp280(3)))
	--print(string.format("%d",bmp280.bmp280_read_temperature))
	sys.timer_start(init,10000)
end

init()
