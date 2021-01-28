#!/bin/sh

#  comment :csynth_design ; cosim_design ; export_design -format ip_catalog in script_tcl
#  example script.tcl is shown below
#
#  ###########################################################
#  # This file is generated automatically by Vivado HLS.
#  # Please DO NOT edit it.
#  # Copyright (C) 1986-2019 Xilinx, Inc. All Rights Reserved.
#  ###########################################################
#  open_project new_define_hls
#  set_top myrun
#  add_files define_instructions.h
#  add_files new_define_datapath2.cpp
#  add_files new_define_datapath2.h
#  add_files new_define_parallel.h
#  add_files new_define_riscv_core.cpp
#  add_files riscvISA.h
#  add_files riscv_cache_l1.cpp
#  add_files riscv_cache_l1.hpp
#  add_files riscv_memory_read_wrt_opr.cpp
#  add_files -tb new_define_tb.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
#  open_solution "solution1"
#  set_part {xc7z020-clg484-1}
#  create_clock -period 15 -name default
#  config_compile -no_signed_zeros=0 -unsafe_math_optimizations=0
#  config_sdx -target none
#  config_export -format ip_catalog -rtl verilog -vivado_optimization_level 2 -vivado_phys_opt place -vivado_report_level 0
#  config_interface -clock_enable=0 -expose_global=0 -m_axi_addr64=0 -m_axi_offset off -register_io off -trim_dangling_port=0
#  config_schedule -effort medium -enable_dsp_full_reg=0 -relax_ii_for_timing=0 -verbose=0
#  config_rtl -encoding onehot -kernel_profile=0 -module_auto_prefix -mult_keep_attribute -reset control -reset_async -reset_level high -verbose
#  config_bind -effort medium
#  set_clock_uncertainty 1
#  #source "./new_define_hls/solution1/directives.tcl"
#  csim_design -clean
#comment :csynth_design ; cosim_design ; export_design -format ip_catalog in script_tcl to generate call.tcl

#VIVADO__ver=2016.4
#VIVADO_path=/home/xilinx/vivado/Vivado/2019.2/bin/vivado_hls 
VIVADO_path=/opt/Xilinx/Vivado/2019.2/bin/vivado_hls 

# for old versions like  2016.4
#exec /opt/Xilinx/Vivado_HLS/$VIVADO__ver/bin/vivado_hls ./new_define_hls/solution1/call.tcl
exec $VIVADO_path ./new_define_hls/solution1/call.tcl 



