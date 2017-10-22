require"rollerPinCfg"
module(...,package.seeall)

local function print(...)
	_G.print("rollerLED",...)
end

local isLEDOn = true

local function ledFlash()
	pins.set(isLEDOn,rollerPinCfg.DEBUG_LED)
	isLEDOn = not isLEDOn
end
--
sys.timer_loop_start(ledFlash,1000)
