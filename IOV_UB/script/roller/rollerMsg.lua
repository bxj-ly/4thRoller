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

local qos0cnt,qos1cnt = 1,1

local function pubqos0testsndcb(usertag,result)
	print("pubqos0testsndcb",usertag,result)
	sys.timer_start(pubqos0test,10000)
	qos0cnt = qos0cnt+1
end

function pubqos0test()
	mqttclient:publish("/qos0topic","qos0data",0,pubqos0testsndcb,"publish0test_"..qos0cnt)
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
	pubqos0test()
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