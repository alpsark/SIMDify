# Version Differences
This is the fourth chekpoint of my thesis. It encomposes finest version of the imporovments of the first eight weeks. Large atapath c file is parsed and divided in mini c files for specific functions. 


# HLS-RISCV-32IM-datapath
Datapath in high level syntesis that takes machine code format in 32 IM RISCV architecture and processes them.   
It uses 1kb memory and 32 registers.   
## Usage   
1. Write a C code that does not contain       
```
printf()
malloc()
calloc()
```
2. Create hls.hex from .c file using command below.   
   - hls.hex creates RISCV ISA machine language of .c file using [RISCV-tools](https://github.com/riscv/riscv-tools/tree/cf052a0e005d537bba45312146449b7451609dbd).  
   - riscv tools must be installed
   - march=rv32i part can be changed to march=rv32im if multplication block is needed.
   - go to linker-files > readme.txt for details
   - copy linker-files > link.ld and start.S to the .c file location
   - memory size and data allocation can be changed in link.ld
   - open terminal:
```

#optimization level can be 0 1 2 3 (-O0 O1 O2 O3 )
MABI="ilp32" #ilp32 or lp64
data_mem_start_adress="0x4000" #data memory start adress, default is 16K = 16384 = 0x4000 ,1 adress holds 8 bits of data
#CFILE name of the .c file
-----------example_genann---------------
CFILE="example_genann"  #name of the C file
OPTIMIZATION="3"   #optimziation level(0,1,2,3) (for higher optimization than 0 variables must be volatile)
INST_SET="rv32im"  #instruction sets are in the https://riscv.org/wp-content/plugins/pdf-viewer/stable/web/viewer.html?file=https://content.riscv.org/wp-content/uploads/2017/05/riscv-spec-v2.2.pdf#chapter.19 OR if link is busted go to riscv-spec-v2.2.pdf#chapter.19


MABI="ilp32" #ilp32 or lp64
rm _$OPTIMIZATION.exe
riscv64-unknown-elf-gcc -mabi=$MABI -g0 -O$OPTIMIZATION -march=$INST_SET  -Wl,--no-relax -nostartfiles start.S $CFILE.c -T link.ld -o _$OPTIMIZATION.exe -lm
riscv64-unknown-elf-objcopy -O binary -j .text -j  .text.startup _$OPTIMIZATION.exe $CFILE$OPTIMIZATION.hex 
riscv64-unknown-elf-objdump -d _$OPTIMIZATION.exe > $CFILE$OPTIMIZATION.idump
riscv64-unknown-elf-objdump --disassembler-options=no-aliases,numeric -D -g  _$OPTIMIZATION.exe > $CFILE$OPTIMIZATION.all
riscv64-unknown-elf-objcopy -O binary  --remove-section  .text --remove-section .text.startup --strip-debug  _$OPTIMIZATION.exe $CFILE$OPTIMIZATION.mem
riscv64-unknown-elf-objdump  -s  -b binary --adjust-vma=0xa000  $CFILE$OPTIMIZATION.mem  > $CFILE$OPTIMIZATION.mdump

#riscv32-unknown-elf
rm _$OPTIMIZATION.exe
riscv32-unknown-elf-gcc -g0 -O$OPTIMIZATION -march=$INST_SET -Wl,--no-relax -nostartfiles start.S $CFILE.c -T link.ld -o _$OPTIMIZATION.exe -lm
riscv32-unknown-elf-objcopy -O binary -j .text* -j  .text.startup _$OPTIMIZATION.exe $CFILE$OPTIMIZATION.hex 
riscv32-unknown-elf-objdump -d _$OPTIMIZATION.exe > $CFILE$OPTIMIZATION.idump
riscv32-unknown-elf-objdump --disassembler-options=no-aliases,numeric -D -g  _$OPTIMIZATION.exe > $CFILE$OPTIMIZATION.all
riscv32-unknown-elf-objcopy -O binary  --remove-section  .text* --remove-section .text.startup --strip-debug  _$OPTIMIZATION.exe $CFILE$OPTIMIZATION.mem
riscv32-unknown-elf-objdump  -s  -b binary --adjust-vma=$data_mem_start_adress  $CFILE$OPTIMIZATION.mem  > $CFILE$OPTIMIZATION.mdump

#riscv64-unknown-elf
rm _$OPTIMIZATION.exe
riscv64-unknown-elf-gcc -mabi=$MABI -g0 -O$OPTIMIZATION -march=$INST_SET  -Wl,--no-relax -nostartfiles start.S $CFILE.c -T link.ld -o _$OPTIMIZATION.exe -lm
riscv64-unknown-elf-objcopy -O binary -j .text* -j  .text.startup _$OPTIMIZATION.exe $CFILE$OPTIMIZATION.hex 
riscv64-unknown-elf-objdump -d _$OPTIMIZATION.exe > $CFILE$OPTIMIZATION.idump
riscv64-unknown-elf-objdump --disassembler-options=no-aliases,numeric -D -g  _$OPTIMIZATION.exe > $CFILE$OPTIMIZATION.all
riscv64-unknown-elf-objcopy -O binary  --remove-section  .text* --remove-section .text.startup --strip-debug  _$OPTIMIZATION.exe $CFILE$OPTIMIZATION.mem
riscv64-unknown-elf-objdump  -s  -b binary --adjust-vma=$data_mem_start_adress  $CFILE$OPTIMIZATION.mem  > $CFILE$OPTIMIZATION.mdump

#riscv-none-embed
rm _$OPTIMIZATION.exe
riscv-none-embed-gcc -mabi=$MABI -g0 -O$OPTIMIZATION -march=$INST_SET  -Wl,--no-relax -nostartfiles start.S $CFILE.c -T link.ld -o _$OPTIMIZATION.exe -lm
riscv-none-embed-objcopy -O binary -j .text* -j  .text.startup _$OPTIMIZATION.exe $CFILE$OPTIMIZATION.hex 
riscv-none-embed-objdump -d _$OPTIMIZATION.exe > $CFILE$OPTIMIZATION.idump
riscv-none-embed-objdump --disassembler-options=no-aliases,numeric -D -g  _$OPTIMIZATION.exe > $CFILE$OPTIMIZATION.all
riscv-none-embed-objcopy -O binary  --remove-section  .text* --remove-section .text.startup --strip-debug  _$OPTIMIZATION.exe $CFILE$OPTIMIZATION.mem
riscv-none-embed-objdump  -s  -b binary --adjust-vma=$data_mem_start_adress  $CFILE$OPTIMIZATION.mem  > $CFILE$OPTIMIZATION.mdump


```

This part generates 4 files:

**.all:**  Memory and instruction memory combined. Registers are numbered as 0x0 0x1 0x2 ........ 0x31  
**.idump:**   Instruction memory in human readable format. Registers are named as z ra(return address) sp(stack pointer)   ........... t6 , for all names check linker files > riscv registers.JPG  
**.hex:**  Instruction memory in **non** human readable format. This is for program. Can also be checked with hex editor. 
**.mdump:**  Data memory in human readable format. Memory is in little endian and byte addressable (can be changed via link.ld). So 03feffff is 0xfffffe03  
**.mem:**  Data memory in **non** human readable format. This is for program. Can also be checked with hex editor. 

#### Common tips:   
1- use **static** for pointer variables and arrays.  
2- use **volatile** for end results.   
3- use **volatile int __attribute__((section(".mySection_par"))) startPar =  0x00;** for  putting variables to spesific addresses.   
4- Normally instruction memory starts with 0x0000 and data memory starts with 0xa000(40K), but you can change this in linker file(link.ld).   


