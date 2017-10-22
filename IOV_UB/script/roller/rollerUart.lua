module(...,package.seeall)

local UART_ID = 1
local CMD_SCANNER,CMD_GPIO,CMD_PORT,FRM_TAIL = 1,2,3,string.char(0xC0)
local rdbuf = ""

local function print(...)
	_G.print("rollerUart",...)
end

local function parse(data)
	if not data then return end	
	
	local tail = string.find(data,string.char(0xC0))
	if not tail then return false,data end	
	local cmdtyp = string.byte(data,1)
	local body,result = string.sub(data,2,tail-1)
	
	print("parse",common.binstohexs(data),cmdtyp,common.binstohexs(body))
	
	if cmdtyp == CMD_SCANNER then
		write("CMD_SCANNER")
	elseif cmdtyp == CMD_GPIO then
		write("CMD_GPIO")
	elseif cmdtyp == CMD_PORT then
		write("CMD_PORT")
	else
		write("CMD_ERROR")
	end
	
	return true,string.sub(data,tail+1,-1)	
end


local function proc(data)
	if not data or string.len(data) == 0 then return end
	-- append data to buffer
	rdbuf = rdbuf..data	
	
	local result,unproc
	unproc = rdbuf
	--Parser buffer data
	while true do
		result,unproc = parse(unproc)
		if not unproc or unproc == "" or not result then
			break
		end
	end

	rdbuf = unproc or ""
end

local function read()
	local data = ""
	while true do		
		data = uart.read(UART_ID,"*l",0)
		if not data or string.len(data) == 0 then break end
		--print("read",data,common.binstohexs(data))
		proc(data)
	end
end

function write(s)
	print("write",s)
	uart.write(UART_ID,s.."\r\n")
end

--pm.wake("test"):Enter active mode，pm.sleep("test"):Enter low power mode
pm.wake("test")
--register serail receiver. The callback function will be invoked after receiving int
sys.reguart(UART_ID,read)
--uart configuration
uart.setup(UART_ID,115200,8,uart.PAR_NONE,uart.STOP_1)

--[[
local function txdone()
	print("txdone")
end
uart.setup(UART_ID,115200,8,uart.PAR_NONE,uart.STOP_1,nil,1)
sys.reguartx(UART_ID,txdone)
]]
