#optimization level can be 0 1 2 3 (-O0 O1 O2 O3 )
MABI="ilp32" #ilp32 or lp64
data_mem_start_adress="0x4000" #data memory start adress, default is 16K = 16384 = 0x4000 ,1 adress holds 8 bits of data
#CFILE name of the .c file
-----------example_genann---------------
CFILE="example_genann"
OPTIMIZATION="3"
INST_SET="rv32im"

-----------kmeans_clstering_global---------------
CFILE="kmeans_clstering_global"
OPTIMIZATION="3"
INST_SET="rv32im"

-----------kmeans_clstering_normal---------------
CFILE="kmeans_clstering_normal"
OPTIMIZATION="3"
INST_SET="rv32im"

-----------knn_normal---------------
CFILE="knn_normal"
OPTIMIZATION="0"
INST_SET="rv32im"

-----------knn_normal---------------
CFILE="knn_normal"
OPTIMIZATION="3"
INST_SET="rv32im"

-----------knn_sel---------------
CFILE="knn_sel"
OPTIMIZATION="3"
INST_SET="rv32im"

-----------knn_mini---------------
CFILE="knn_mini"
OPTIMIZATION="3"
INST_SET="rv32im"

-----------matrix_mul---------------
CFILE="matrixmultiply"
OPTIMIZATION="3"
INST_SET="rv32im"

------------------------ CODE ----------------CTRL+C/ CTRL+SHIFT+V-------------------
rm _$OPTIMIZATION.exe
riscv32-unknown-elf-gcc -g0 -O$OPTIMIZATION -march=$INST_SET -Wl,--no-relax -nostartfiles start.S $CFILE.c -T link.ld -o _$OPTIMIZATION.exe -lm
riscv32-unknown-elf-objcopy -O binary -j .text* -j  .text.startup _$OPTIMIZATION.exe $CFILE$OPTIMIZATION.hex 
riscv32-unknown-elf-objdump -d _$OPTIMIZATION.exe > $CFILE$OPTIMIZATION.idump
riscv32-unknown-elf-objdump --disassembler-options=no-aliases,numeric -D -g  _$OPTIMIZATION.exe > $CFILE$OPTIMIZATION.all
riscv32-unknown-elf-objcopy -O binary  --remove-section  .text* --remove-section .text.startup --strip-debug  _$OPTIMIZATION.exe $CFILE$OPTIMIZATION.mem
riscv32-unknown-elf-objdump  -s  -b binary --adjust-vma=$data_mem_start_adress  $CFILE$OPTIMIZATION.mem  > $CFILE$OPTIMIZATION.mdump

------------------------ CODE ----------------CTRL+C/ CTRL+SHIFT+V-------------------
rm _$OPTIMIZATION.exe
riscv64-unknown-elf-gcc -mabi=$MABI -g0 -O$OPTIMIZATION -march=$INST_SET  -Wl,--no-relax -nostartfiles start.S $CFILE.c -T link.ld -o _$OPTIMIZATION.exe -lm
riscv64-unknown-elf-objcopy -O binary -j .text* -j  .text.startup _$OPTIMIZATION.exe $CFILE$OPTIMIZATION.hex 
riscv64-unknown-elf-objdump -d _$OPTIMIZATION.exe > $CFILE$OPTIMIZATION.idump
riscv64-unknown-elf-objdump --disassembler-options=no-aliases,numeric -D -g  _$OPTIMIZATION.exe > $CFILE$OPTIMIZATION.all
riscv64-unknown-elf-objcopy -O binary  --remove-section  .text* --remove-section .text.startup --strip-debug  _$OPTIMIZATION.exe $CFILE$OPTIMIZATION.mem
riscv64-unknown-elf-objdump  -s  -b binary --adjust-vma=$data_mem_start_adress  $CFILE$OPTIMIZATION.mem  > $CFILE$OPTIMIZATION.mdump

------------------------ CODE ----------------CTRL+C/ CTRL+SHIFT+V-------------------
rm _$OPTIMIZATION.exe
riscv-none-embed-gcc -mabi=$MABI -g0 -O$OPTIMIZATION -march=$INST_SET  -Wl,--no-relax -nostartfiles start.S $CFILE.c -T link.ld -o _$OPTIMIZATION.exe -lm
riscv-none-embed-objcopy -O binary -j .text* -j  .text.startup _$OPTIMIZATION.exe $CFILE$OPTIMIZATION.hex 
riscv-none-embed-objdump -d _$OPTIMIZATION.exe > $CFILE$OPTIMIZATION.idump
riscv-none-embed-objdump --disassembler-options=no-aliases,numeric -D -g  _$OPTIMIZATION.exe > $CFILE$OPTIMIZATION.all
riscv-none-embed-objcopy -O binary  --remove-section  .text* --remove-section .text.startup --strip-debug  _$OPTIMIZATION.exe $CFILE$OPTIMIZATION.mem
riscv-none-embed-objdump  -s  -b binary --adjust-vma=$data_mem_start_adress  $CFILE$OPTIMIZATION.mem  > $CFILE$OPTIMIZATION.mdump


-------------WINDOWS------------- 
SET  CFILE="knn_normal"
SET OPTIMIZATION="3"
SET INST_SET="rv32im"
SET MABI="ilp32" 
SET data_mem_start_adress="0x4000"

del _%OPTIMIZATION%.exe
riscv64-unknown-elf-gcc -mabi=%MABI% -g0 -O%OPTIMIZATION% -march=%INST_SET%  -Wl,--no-relax -nostartfiles start.S %CFILE%.c -T link.ld -o _%OPTIMIZATION%.exe -lm
riscv64-unknown-elf-objcopy -O binary -j .text* -j  .text.startup _%OPTIMIZATION%.exe "%CFILE%%OPTIMIZATION%.hex" 
riscv64-unknown-elf-objdump -d _%OPTIMIZATION%.exe > "%CFILE%%OPTIMIZATION%.idump"
riscv64-unknown-elf-objdump --disassembler-options=no-aliases,numeric -D -g  _%OPTIMIZATION%.exe > "%CFILE%%OPTIMIZATION%.all"
riscv64-unknown-elf-objcopy -O binary  --remove-section  .text* --remove-section .text.startup --strip-debug  _%OPTIMIZATION%.exe "%CFILE%%OPTIMIZATION%.mem"
riscv64-unknown-elf-objdump  -s  -b binary --adjust-vma=%data_mem_start_adress%  "%CFILE%%OPTIMIZATION%.mem"  > "%CFILE%%OPTIMIZATION%.mdump"
