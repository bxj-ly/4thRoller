require"misc"
require"mqtt"
require"common"
module(...,package.seeall)

local ssub,schar,smatch,sbyte,slen = string.sub,string.char,string.match,string.byte,string.len
--www.coolbug.cn
local PROT,ADDR,PORT = "TCP","139.129.97.106",61613
local mqttclient

local function print(...)
	_G.print("rollerMsg",...)
end

local qos1cnt = 1

local function pubPressureSendCallback(usertag,result)
	print("pubPressureSendCallback",usertag,result)
	sys.timer_start(pubPressure,10000)
end

function pubPressure()
	i2c.lps25hb(1)
	mqttclient:publish("/roller","LSP25HB Pressure: "..i2c.lps25hb(2),0,pubPressureSendCallback,"LSP25HB Pressure:")
	i2c.bmp280(1)
	local pressure, temperature=i2c.bmp280(2)
	mqttclient:publish("/roller","BMP280 Pressure: "..pressure,0,pubPressureSendCallback,"BMP280 Pressure:")
end

local function pubTemperatureSendCallback(usertag,result)
	print("pubTemperatureSendCallback",usertag,result)
	sys.timer_start(pubTemperature,10000)
end

function pubTemperature()
	i2c.lps25hb(1)
	mqttclient:publish("/roller","LSP25HB Temperature: "..i2c.lps25hb(3),0,pubTemperatureSendCallback,"LSP25HB Temperature:")
	i2c.bmp280(1)
	local pressure, temperature=i2c.bmp280(2)
	mqttclient:publish("/roller","BMP280 Temperature: "..temperature,0,pubTemperatureSendCallback,"BMP280 Temperature:")
end

local function pubAccelGyroSendCallback(usertag,result)
	print("pubAccelGyroSendCallback",usertag,result)
	sys.timer_start(pubAccelGyro,10000)
end

function pubAccelGyro()
	i2c.mpu6050(1)
    --local tab6050=i2c.mpu6050(2)
    -- show key and value  
    --for k, v in pairs(tab6050) do  
    --    print(string.format("%s : %s",tostring(k), tostring(v)))  
    --end
    local AccelX,AccelY,AccelZ,GyroX,GyroY,GyroZ=i2c.mpu6050(2)
	mqttclient:publish("/roller","MPU6050： AX("..AccelX..") AY("..AccelY..") AZ("..AccelZ..") GX("..GyroX..") GY("..GyroY..") GZ-"..GyroZ..")",0,pubAccelGyroSendCallback,"MPU6050:")
	i2c.bmp280(1)
end

local function pubPositionSendCallback(usertag,result)
	print("pubPositionSendCallback",usertag,result)
	sys.timer_start(pubPosition,10000)
end

function pubPosition()
	--gps.isfix(),gps.getgpslocation(),gps.getgpsspd(),gps.getgpscog(),gps.getaltitude()
	--mqttclient:publish("/roller","Position",0,pubPositionSendCallback,gps.getgpslocation())
end

local function pubqos1testackcb(usertag,result)
	print("pubqos1testackcb",usertag,result)
	sys.timer_start(pubqos1test,20000)
	qos1cnt = qos1cnt+1
end

function pubqos1test()
	mqttclient:publish("/qos1topic","qos1data",1,pubqos1testackcb,"publish1test_"..qos1cnt)
end

--[[
MQTT SUBSCRIBE callback
]]
local function subackcb(usertag,result)
	print("subackcb",usertag,result)
end

--[[
Topic is GB2312 code
]]
local function rcvmessagecb(topic,payload,qos)
	print("rcvmessagecb",topic,payload,qos)
end

local function discb()
	print("discb")
	--rebuild MQTT connection after 20s
	sys.timer_start(connect,20000)
end


local function disconnect()
	mqttclient:disconnect(discb)
end

--[[
MQTT CONNECT successfully
]]
local function connectedcb()
	print("connectedcb")
	--subscribe topic
	mqttclient:subscribe({{topic="/event0",qos=0}, {topic="/event1",qos=1}}, subackcb, "subscribetest")
	--Register publish receiver
	mqttclient:regevtcb({MESSAGE=rcvmessagecb})
	--publish a qos0 msg
	pubPressure()
	pubTemperature()
	pubAccelGyro()
	--publish a qos1 msg
	pubqos1test()
	--Disconnect after 20 s
	--sys.timer_start(disconnect,20000)
end

--[[
MQTT CONNECT failed
		r：
			1：Connection Refused: unacceptable protocol version
			2：Connection Refused: identifier rejected
			3：Connection Refused: server unavailable
			4：Connection Refused: bad user name or password
			5：Connection Refused: not authorized
]]
local function connecterrcb(r)
	print("connecterrcb",r)
end

--[[
SOCKET exception handling
]]
local function sckerrcb(r)
	print("sckerrcb",r)
	misc.setflymode(true)
	sys.timer_start(misc.setflymode,30000,false)
end

function connect()
	--Connect to MQTT server
	--There will be a reset if socket has an exception.
	--If you want to control socket exception by yourself, uncomment--[[,sckerrcb]]
	mqttclient:connect(misc.getimei(),240,"bxj","Asdf#1234",connectedcb,connecterrcb--[[,sckerrcb]])
end

local function statustest()
	print("statustest",mqttclient:getstatus())
end

--[[
Create MQTT client when IMEI is ready.
]]
local function imeirdy()
	--Creat a mqtt client，Default MQTT version is 3.1，Uncomment --[[,"3.1.1"]] if you want 3.1.1
	mqttclient = mqtt.create(PROT,ADDR,PORT--[[,"3.1.1"]])
	--Config will paramenters
	--mqttclient:configwill(1,0,0,"/willtopic","will payload")
	--Set clean session flag. Default value is 1
	--mqttclient:setcleansession(0)
	--check client test status
	--sys.timer_loop_start(statustest,1000)
	connect()
end

local procer =
{
	IMEI_READY = imeirdy,
}
--register msg handling function
sys.regapp(procer)