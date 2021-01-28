############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2018 Xilinx, Inc. All Rights Reserved.
############################################################
open_project new_define_hls
set_top myrun
add_files riscv_memory_read_wrt_opr.cpp
add_files riscv_cache_l1.hpp
add_files riscv_cache_l1.cpp
add_files riscvISA.h
add_files new_define_riscv_core.cpp
add_files new_define_parallel.h
add_files new_define_datapath2.h
add_files new_define_datapath2.cpp
add_files define_instructions.h
add_files -tb new_define_tb.cpp -cflags "-Wno-unknown-pragmas"
open_solution "solution1"
set_part {xc7z020-clg484-1}
create_clock -period 15 -name default
config_compile  
config_interface    -m_axi_offset off -register_io off 
#config_schedule -effort medium -enable_dsp_full_reg 0  
#config_rtl -encoding onehot -kernel_profile 0 -module_auto_prefix 1 -mult_keep_attribute 1 -reset control -reset_async -reset_level high -verbose
#config_bind -effort medium
set_clock_uncertainty 12.5%
#source "./new_define_hls/solution1/directives.tcl"
csim_design -clean
#csynth_design
#cosim_design
#export_design -rtl verilog -format ip_catalog
