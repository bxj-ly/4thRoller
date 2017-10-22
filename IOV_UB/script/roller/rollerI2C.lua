require"common"
module(...,package.seeall)

local i2cid = 2

local function print(...)
	_G.print("rollerI2C",...)
end

local function init()
	local i2cslaveaddr = 0x0E
	--i2cslaveaddr is 7bit address
	if i2c.setup(i2cid,i2c.SLOW,i2cslaveaddr) ~= i2c.SLOW then
		print("init fail")
		return
	end
	local id=i2c.read(i2cid,0xD0,1)
	--print("init",string.format("%02X"),common.binstohexs(id))
end

init()
--Close I2C channel after 5 s
sys.timer_start(i2c.close,5000,i2cid)