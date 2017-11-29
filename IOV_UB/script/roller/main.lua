MODULE_TYPE = "Air202"
PROJECT = "Roller"
VERSION = "1.0.0"
require"sys"
--[[
Trace control
]]
--sys.opntrace(true,1)

require"rollerPinCfg" 
require"rollerLED"
require"rollerGPS"
--require"rollerBMP280"
--require"rollerI2C"
require"rollerMsg"
--require"rollerUart"
require"rollerCall"

sys.init(0,0)
sys.run()
