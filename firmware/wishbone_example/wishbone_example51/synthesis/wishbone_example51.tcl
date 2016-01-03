# TCL script automaticaly generated by POD
cd ../objs
project new wishbone_example51.ise
# configure platform params
project set family spartan6
project set device XC6SLX16
project set package CSG225
project set speed -2
project set {Preferred Language} VHDL
## add components sources file
# add top level sources file
xfile add ../synthesis/top_wishbone_example51.vhd
xfile add ../synthesis/imx51_wb16_wrapper/imx51_wb16_wrapper.vhd
xfile add ../synthesis/rstgen_syscon/rstgen_syscon.vhd
xfile add ../synthesis/led/led.vhd
xfile add ../synthesis/imx51_wb16_wrapper00_mwb16/imx51_wb16_wrapper00_mwb16_intercon.vhd
xfile add ../synthesis/irq_mngr/irq_mngr.vhd
xfile add ../synthesis/button/button.vhd
# add constraint file
xfile add ../synthesis/wishbone_example51.ucf 
process run "Generate Programming File"
process run "Generate Post-Synthesis Simulation Model"
process run "Generate Post-Place & Route Simulation Model"
