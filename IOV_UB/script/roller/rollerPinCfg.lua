require"pins"
module(...,package.seeall)

--pin define rules:
--pio.P0_XX:GPIO 0 ~ GPIO 31,Such as pio.P0_15 is GPIO15
--pio.P1_XX:>=GPIO 32,Such as pio.P1_2 is GPIO34
DEBUG_LED = {pin=pio.P0_11}

PIN13 = {pin=pio.P1_1,dir=pio.OUTPUT1,valid=0}

pmd.ldoset(5,pmd.LDO_VMMC)
pins.reg(DEBUG_LED)