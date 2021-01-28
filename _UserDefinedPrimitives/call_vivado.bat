Rem  comment :csynth_design ; cosim_design ; export_design -format ip_catalog in script_tcl
Rem  example script.tcl is shown below

Rem  ###########################################################
Rem  # This file is generated automatically by Vivado HLS.
Rem  # Please DO NOT edit it.
Rem  # Copyright (C) 1986-2019 Xilinx, Inc. All Rights Reserved.
Rem  ###########################################################
Rem  open_project new_define_hls
Rem  set_top myrun
Rem  add_files define_instructions.h
Rem  add_files new_define_datapath2.cpp
Rem  add_files new_define_datapath2.h
Rem  add_files new_define_parallel.h
Rem  add_files new_define_riscv_core.cpp
Rem  add_files riscvISA.h
Rem  add_files riscv_cache_l1.cpp
Rem  add_files riscv_cache_l1.hpp
Rem  add_files riscv_memory_read_wrt_opr.cpp
Rem  add_files -tb new_define_tb.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
Rem  open_solution "solution1"
Rem  set_part {xc7z020-clg484-1}
Rem  create_clock -period 15 -name default
Rem  config_compile -no_signed_zeros=0 -unsafe_math_optimizations=0
Rem  config_sdx -target none
Rem  config_export -format ip_catalog -rtl verilog -vivado_optimization_level 2 -vivado_phys_opt place -vivado_report_level 0
Rem  config_interface -clock_enable=0 -expose_global=0 -m_axi_addr64=0 -m_axi_offset off -register_io off -trim_dangling_port=0
Rem  config_schedule -effort medium -enable_dsp_full_reg=0 -relax_ii_for_timing=0 -verbose=0
Rem  config_rtl -encoding onehot -kernel_profile=0 -module_auto_prefix -mult_keep_attribute -reset control -reset_async -reset_level high -verbose
Rem  config_bind -effort medium
Rem  set_clock_uncertainty 1
Rem  #source "./new_define_hls/solution1/directives.tcl"
Rem  csim_design -clean


SET VIVADO_path=F:/Xilin/1/Vivado/2019.2/bin/vivado_hls

 %@Try%
%VIVADO_path% ./new_define_hls/solution1/call.tcl
rem C:/Xilinx/Vivado/%VIVADO__ver%/bin/vivado_hls ./new_define_hls/solution1/call.tcl
%@EndTry%
:@Catch
echo Something is wrong!!!
pause
:@EndCatch


