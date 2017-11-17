require"common"
module(...,package.seeall)

local function print(...)
    _G.print("rollerBMP280",...)
end

local function init()
    i2c.mpu6050(1)
    local tab6050=i2c.mpu6050(2)
    -- show key and value  
    for k, v in pairs(tab6050) do  
        print(string.format("%s : %s",tostring(k), tostring(v)))  
    end
    i2c.lps25hb(1)
    print("LSP25HB Pressure:", string.format("%d",i2c.lps25hb(2)))
    print("LSP25HB Temperature:", string.format("%d",i2c.lps25hb(3)))

    i2c.bmp280(1)
    local pressure, temperature=i2c.bmp280(2)
    print(string.format("BMP280 Pressure:%d",pressure))
    print(string.format("BMP280 Temperature:%d",temperature))
    sys.timer_start(init,2000)
end

init()
