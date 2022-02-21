-- set brush size to zero
tpt.brushx = 0
tpt.brushy = 0

-- change DRAY/DTEC colors to distinguish them from ARAY
elem.property(elem.DEFAULT_PT_DTEC, "Colour", 0xFE720F)
elem.property(elem.DEFAULT_PT_DRAY, "Colour", 0xFFFF00)
-- make low-life BRAY brighter
tpt.set_bray_life_brightness_threshold(100)

-- enable subframe debugging
tpt.setdebug(0x8)

-- enable automatic particle order reloading
tpt.autoreload_enable(1)

-- pause the simulation
tpt.set_pause(1)
