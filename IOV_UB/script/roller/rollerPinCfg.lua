require"pins"
module(...,package.seeall)

DEBUG_LED = {pin=pio.P0_11}

pmd.ldoset(5,pmd.LDO_VMMC)
pins.reg(DEBUG_LED)