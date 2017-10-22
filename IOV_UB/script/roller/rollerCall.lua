--[[
Call in/out control
]]

module(...,package.seeall)
require"cc"
require"audio"
require"common"

--[[
Add prefix "call" in each print
]]
local function print(...)
	_G.print("call",...)
end

--[[
Call connected
]]
local function connected()
	print("connected")
	--sys.timer_start(audio.play,5000,0,"TTSCC",common.binstohexs(common.gb2312toucs2("通话中播放TTS测试")),audiocore.VOL7)
	--sys.timer_start(cc.hangup,50000,"AUTO_DISCONNECT")
end

--[[
Call disconnected
]]
local function disconnected(para)
	print("disconnected:"..(para or "nil"))
	sys.timer_stop(cc.hangup,"AUTO_DISCONNECT")
end

--[[
Receive a call
]]
local function incoming(num)
	print("Received a call. Num is"..num)
	cc.accept()
end

--[[
call ready
]]
local function ready()
	print("call ready")
	--cc.dial("13641903068")
end



--[[
]]
local function alerting()
	print("alerting")
end

cc.regcb("READY",ready,"INCOMING",incoming,"CONNECTED",connected,"DISCONNECTED",disconnected,"ALERTING",alerting)
