module(...,package.seeall)

require"gps"
require"agps"

local function print(...)
	_G.print("rollerGPS",...)
end

local function test1cb(cause)
  print("test1cb",cause,gps.isfix(),gps.getgpslocation(),gps.getgpsspd(),gps.getgpscog(),gps.getaltitude())
end

local function test2cb(cause)
  print("test2cb",cause,gps.isfix(),gps.getgpslocation(),gps.getgpsspd(),gps.getgpscog(),gps.getaltitude())
end

local function test3cb(cause)
  print("test3cb",cause,gps.isfix(),gps.getgpslocation(),gps.getgpsspd(),gps.getgpscog(),gps.getaltitude())
end

local testidx = 1

local function gps_open(typ)
  if typ==1 then
    gps.open(gps.DEFAULT,{cause="TEST1",cb=test1cb})
    gps.open(gps.TIMERORSUC,{cause="TEST2",val=10,cb=test2cb})
    gps.open(gps.TIMER,{cause="TEST3",val=300,cb=test3cb})

  elseif typ==2 then
    gps.open(gps.DEFAULT,{cause="TEST1",cb=test1cb})
    sys.timer_start(gps.close,30000,gps.DEFAULT,{cause="TEST1"})
    gps.open(gps.TIMERORSUC,{cause="TEST2",val=10,cb=test2cb})
    gps.open(gps.TIMER,{cause="TEST3",val=60,cb=test3cb}) 
  end
end

local function nemacb(data)
	print("nemacb",data)
end

gps.init()
-- 0：GPS+BD
-- 1：GPS
-- 2：BD
gps.setfixmode(0)
-- 0: only gps.lua handles NEMA data
-- 1,nemacb: nemacb handles NEMA data
-- 2,nemacb: both gps.lua and nemacb handle NEMA data
gps.setnemamode(0)
--gps.settimezone(gps.GPS_BEIJING_TIME)
gps_open(testidx)
