############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2019 Xilinx, Inc. All Rights Reserved.
############################################################
open_project data_path
set_top myrun
add_files riscv_memory_read_wrt_opr.cpp
add_files riscv_core.cpp
add_files riscv_cache_l1.hpp
add_files riscv_cache_l1.cpp
add_files riscvISA.h
add_files parallel.h
add_files new_define.h
add_files define_instructions.h
add_files datapath2.h
add_files datapath2.cpp
add_files -tb tb.cpp -cflags "-Wno-unknown-pragmas -Wno-unknown-pragmas -Wno-unknown-pragmas -Wno-unknown-pragmas -Wno-unknown-pragmas -Wno-unknown-pragmas -Wno-unknown-pragmas -Wno-unknown-pragmas -Wno-unknown-pragmas -Wno-unknown-pragmas -Wno-unknown-pragmas -Wno-unknown-pragmas -Wno-unknown-pragmas -Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
open_solution "solution1"
set_part {xc7z020-clg484-1}
create_clock -period 15 -name default
config_compile -no_signed_zeros=0 -unsafe_math_optimizations=0
config_sdx -target none
config_export -format ip_catalog -rtl verilog -vivado_optimization_level 2 -vivado_phys_opt place -vivado_report_level 0
config_schedule -effort medium -enable_dsp_full_reg=0 -relax_ii_for_timing=0 -verbose=0
config_rtl -encoding onehot -kernel_profile=0 -module_auto_prefix -mult_keep_attribute -reset control -reset_async -reset_level high -verbose
config_bind -effort medium
config_interface -clock_enable=0 -expose_global=0 -m_axi_addr64=0 -m_axi_offset off -register_io off -trim_dangling_port
set_clock_uncertainty 12.5%
#source "./data_path/solution1/directives.tcl"
csynth_design
cosim_design