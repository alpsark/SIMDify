
/*
* riscvISA.h
*
* Created on: 2 déc. 2016
* Author: simon
*/
#ifndef INCLUDES_ISA_RISCVISA_H_
#define INCLUDES_ISA_RISCVISA_H_
#include <string.h>
#include <iomanip>
#include <sstream>

#ifndef __HLS__
std::string printDecodedInstrRISCV(unsigned int oneInstruction);
#endif

// Major opcodes
#define RISCV_LUI               0x37    //0x0D
#define RISCV_AUIPC             0x17    //0x05
#define RISCV_JAL               0x6F    //0x1B
#define RISCV_JALR              0x67    //0x19
#define RISCV_BR                0x63    //0x18
#define RISCV_LD                0x03    //0x00
#define RISCV_ST                0x23    //0x08
#define RISCV_OPI               0x13    //0x04
#define RISCV_OP                0x33    //0x0C
#define RISCV_MISC_MEM          0x0F    //0x03 ///multi core
#define RISCV_OPIW              0x1B    //0x06
#define RISCV_OPW               0x3B    //0x0E

// funct3 or funct7
#define RISCV_BR_BEQ            0x0
#define RISCV_BR_BNE            0x1
#define RISCV_BR_BLT            0x4
#define RISCV_BR_BGE            0x5
#define RISCV_BR_BLTU           0x6
#define RISCV_BR_BGEU           0x7
#define RISCV_LD_LB             0x0
#define RISCV_LD_LH             0x1
#define RISCV_LD_LW             0x2
#define RISCV_LD_LD             0x3
#define RISCV_LD_LBU            0x4
#define RISCV_LD_LHU            0x5
#define RISCV_LD_LWU            0x6
#define RISCV_ST_STB            0x0
#define RISCV_ST_STH            0x1
#define RISCV_ST_STW            0x2
#define RISCV_ST_STD            0x3
#define RISCV_OPI_ADDI          0x0
#define RISCV_OPI_SLTI          0x2
#define RISCV_OPI_SLTIU         0x3
#define RISCV_OPI_XORI          0x4
#define RISCV_OPI_ORI           0x6
#define RISCV_OPI_ANDI          0x7
#define RISCV_OPI_SLLI          0x1
#define RISCV_OPI_SRI           0x5     // SRLI & SRAI
#define RISCV_OPI_SRI_SRAI      0x20
#define RISCV_OPI_SRI_SRLI      0x0
#define RISCV_OP_ADD            0x0
#define RISCV_OP_SLL            0x1
#define RISCV_OP_SLT            0x2
#define RISCV_OP_SLTU           0x3
#define RISCV_OP_XOR            0x4
#define RISCV_OP_SR             0x5
#define RISCV_OP_OR             0x6
#define RISCV_OP_AND            0x7
#define RISCV_OP_ADD_ADD        0x0
#define RISCV_OP_ADD_SUB        0x20
#define RISCV_OP_SR_SRL         0x0
#define RISCV_OP_SR_SRA         0x20
#define RISCV_OPIW_ADDIW        0x0
#define RISCV_OPIW_SLLIW        0x1
#define RISCV_OPIW_SRW          0x5
#define RISCV_OPIW_SRW_SRLIW    0x0
#define RISCV_OPIW_SRW_SRAIW    0x20
#define RISCV_OPW_ADDSUBW       0x0
#define RISCV_OPW_SLLW          0x1
#define RISCV_OPW_SRW           0x5
#define RISCV_OPW_ADDSUBW_ADDW  0x0
#define RISCV_OPW_ADDSUBW_SUBW  0x20
#define RISCV_OPW_SRW_SRLW      0x0
#define RISCV_OPW_SRW_SRAW      0x20



/******************************************************************************************************
 * Specification of the privileged architecture                                                       *
 ******************************************************************************************************
 * This offers system capability                                                                      *
 ******************************************************************************************************/
#define RISCV_SYSTEM            0x73    //0x1C
#define RISCV_SYSTEM_ENV        0x0
#define RISCV_SYSTEM_ENV_ECALL  0x0
#define RISCV_SYSTEM_ENV_EBREAK 0x1
#define RISCV_SYSTEM_CSRRW      0x1
#define RISCV_SYSTEM_CSRRS      0x2
#define RISCV_SYSTEM_CSRRC      0x3
#define RISCV_SYSTEM_CSRRWI     0x5
#define RISCV_SYSTEM_CSRRSI     0x6
#define RISCV_SYSTEM_CSRRCI     0x7



#define RISCV_CSR_MVENDORID     0xF11 // MRO mvendorid Vendor ID.
#define RISCV_CSR_MARCHID       0xF12 // MRO marchid Architecture ID.
#define RISCV_CSR_MIMPID        0xF13 // MRO mimpid Implementation ID.
#define RISCV_CSR_MHARTID       0xF14 // MRO mhartid Hardware thread ID.
//Machine Trap Setup
#define RISCV_CSR_MSTATUS       0x300 // MRW mstatus Machine status register.
#define RISCV_CSR_MSTATUS_WIRI  0x00000000 // ignore those bits in read & write
#define RISCV_CSR_MSTATUS_WPRI  0x7F100644 // preserve bits in write, ignore read
#define RISCV_CSR_MSTATUS_WLRL  0x00000000 // R/W legal value only
#define RISCV_CSR_MSTATUS_WARL  0x00001900 // write any, read legal

#define RISCV_CSR_MISA          0x301 // MRW misa ISA and extensions
#define RISCV_CSR_MISA_WIRI     0x3C000000 // ignore those bits in read & write
#define RISCV_CSR_MISA_WPRI     0x00000000 // preserve bits in write, ignore read
#define RISCV_CSR_MISA_WLRL     0x00000000 // R/W legal value only
#define RISCV_CSR_MISA_WARL     0xC3FFFFFF // write any, read legal

#define RISCV_CSR_MEDELEG       0x302 // MRW medeleg Machine exception delegation register.
#define RISCV_CSR_MEDELEG_WARL  0xFFFFFFFF // write any, read legal

#define RISCV_CSR_MIDELEG       0x303 // MRW mideleg Machine interrupt delegation register.
#define RISCV_CSR_MIDELEG_WARL  0xFFFFFFFF // write any, read legal

#define RISCV_CSR_MIE           0x304 // MRW mie Machine interrupt-enable register.
#define RISCV_CSR_MIE_WIRI      0x00000000 // ignore those bits in read & write
#define RISCV_CSR_MIE_WPRI      0xFFFFF444 // preserve bits in write, ignore read
#define RISCV_CSR_MIE_WLRL      0x00000000 // R/W legal value only
#define RISCV_CSR_MIE_WARL      0x00000BBB // write any, read legal

#define RISCV_CSR_MTVEC         0x305 // MRW mtvec Machine trap-handler base address.
#define RISCV_CSR_MTVEC_WARL    0xFFFFFFFF // write any, read legal

#define RISCV_CSR_MCOUNTEREN    0x306 // MRW mcounteren Machine counter enable.

//Machine Trap Handling
#define RISCV_CSR_MSCRATCH      0x340 // MRW mscratch Scratch register for machine trap handlers.
#define RISCV_CSR_MEPC          0x341 // MRW mepc Machine exception program counter.
#define RISCV_CSR_MEPC_WARL     0xFFFFFFFF

#define RISCV_CSR_MCAUSE        0x342 // MRW mcause Machine trap cause.
#define RISCV_CSR_MCAUSE_WLRL   0x7FFFFFFF

#define RISCV_CSR_MTVAL         0x343 // MRW mtval Machine bad address or instruction.

#define RISCV_CSR_MIP           0x344 // MRW mip Machine interrupt pending.
#define RISCV_CSR_MIP_WIRI      0xFFFFF444 // ignore those bits in read & write
#define RISCV_CSR_MIP_WARL      0x00000BBB // write any, read legal

//Machine Protection and Translation
// not implemented

//Machine Counter/Timers
#define RISCV_CSR_MCYCLE        0xB00 // MRW mcycle Machine cycle counter.
#define RISCV_CSR_MINSTRET      0xB02 // MRW minstret Machine instructions-retired counter.
#define RISCV_CSR_MCYCLEH       0xB80 // MRW mcycleh Upper 32 bits of mcycle, RV32I only.
#define RISCV_CSR_MINSTRETH     0xB82 // MRW minstreth Upper 32 bits of minstret, RV32I only.

/******************************************************************************************************
* Specification of the standard M extension
********************************************
* This extension brings the support for multiplication operation.
* It is composed of the RISCV_OP opcode then a dedicated value for funct7 which identify it.
* Then funct3 is used to determine which of the eight operation to use.
* Added operations are MUL, MULH, MULHSU, MLHU, DIV, DIVU, REM, REMU
*****************************************************************************************************/
#define RISCV_OP_M              0x1
#define RISCV_OP_M_MUL          0x0
#define RISCV_OP_M_MULH         0x1
#define RISCV_OP_M_MULHSU       0x2
#define RISCV_OP_M_MULHU        0x3
#define RISCV_OP_M_DIV          0x4
#define RISCV_OP_M_DIVU         0x5
#define RISCV_OP_M_REM          0x6
#define RISCV_OP_M_REMU         0x7
#define RISCV_OPW_M_MULW        0x0
#define RISCV_OPW_M_DIVW        0x4
#define RISCV_OPW_M_DIVUW       0x5
#define RISCV_OPW_M_REMW        0x6
#define RISCV_OPW_M_REMUW       0x7


/******************************************************************************************************
* Specification of the standard A extension
********************************************
* This extension brings the support for atomic operation.
* It is always of R-type instruction, and funct3 is always 0b010
* Then funct7[6:2] is used to determine which of the eleven operation to use.
* Added operations are LR, SC, AMOSWAP, AMOADD, AMOXOR, AMOAND, AMOOR, AMOMIN, AMOMAX, AMOMINU, AMOMAXU
*****************************************************************************************************/
#define RISCV_ATOMIC            0x2F    //0x0B
#define RISCV_ATOMIC_LR         0x2
#define RISCV_ATOMIC_SC         0x3
#define RISCV_ATOMIC_SWAP       0x1
#define RISCV_ATOMIC_ADD        0x0
#define RISCV_ATOMIC_XOR        0x4
#define RISCV_ATOMIC_AND        0xC
#define RISCV_ATOMIC_OR         0x8
#define RISCV_ATOMIC_MIN        0x10
#define RISCV_ATOMIC_MAX        0x14
#define RISCV_ATOMIC_MINU       0x18
#define RISCV_ATOMIC_MAXU       0x1C


/******************************************************************************************************
* Specification of the standard F extension
********************************************
*  Author : Lauric
*****************************************************************************************************/

// Opcode
#define RISCV_FLOAT_LW 		0x7
#define RISCV_FLOAT_SW		0x27
#define RISCV_FLOAT_MADD	0x43
#define RISCV_FLOAT_MSUB	0x47
#define RISCV_FLOAT_NMADD	0x4F
#define RISCV_FLOAT_NMSUB	0x4B
#define RISCV_FLOAT_OP 		0x53

// funct7
#define RISCV_FLOAT_OP_ADD	0x0
#define RISCV_FLOAT_OP_SUB	0x4
#define RISCV_FLOAT_OP_MUL	0x8
#define RISCV_FLOAT_OP_DIV	0xC
#define RISCV_FLOAT_OP_SQRT	0x2C
#define RISCV_FLOAT_OP_SGN	0x10
#define RISCV_FLOAT_OP_MINMAX	0x14
#define RISCV_FLOAT_OP_CVTWS	0x60
#define RISCV_FLOAT_OP_CMP	0x50
#define RISCV_FLOAT_OP_CVTSW	0x68
#define RISCV_FLOAT_OP_MVWX	0x78
#define RISCV_FLOAT_OP_CLASSMVXW 0x70




#ifndef __CATAPULT
#ifndef __NIOS
//std::string printDecodedInstrRISCV(uint32 instruction);
extern const char* riscvNamesOP[8];
extern const char* riscvNamesOPI[8];
extern const char* riscvNamesOPW[8];
extern const char* riscvNamesOPIW[8];
extern const char* riscvNamesLD[8];
extern const char* riscvNamesST[8];
extern const char* riscvNamesBR[8];
extern const char* riscvNames[8];
#endif
#endif
#define SYS_exit                93
#define SYS_exit_group          94
#define SYS_getpid              172
#define SYS_kill                129
#define SYS_read                63
#define SYS_write               64
#define SYS_open                1024
#define SYS_openat              56
#define SYS_close               57
#define SYS_lseek               62
#define SYS_brk                 214
#define SYS_link                1025
#define SYS_unlink              1026
#define SYS_mkdir               1030
#define SYS_chdir               49
#define SYS_getcwd              17
#define SYS_stat                1038
#define SYS_fstat               80
#define SYS_lstat               1039
#define SYS_fstatat             79
#define SYS_access              1033
#define SYS_faccessat           48
#define SYS_pread               67
#define SYS_pwrite              68
#define SYS_uname               160
#define SYS_getuid              174
#define SYS_geteuid             175
#define SYS_getgid              176
#define SYS_getegid             177
#define SYS_mmap                222
#define SYS_munmap              215
#define SYS_mremap              216
#define SYS_time                1062
#define SYS_getmainvars         2011
#define SYS_rt_sigaction        134
#define SYS_writev              66
#define SYS_gettimeofday        169
#define SYS_times               153
#define SYS_fcntl               25
#define SYS_getdents            61
#define SYS_dup                 23

// Custom syscall
#define SYS_threadstart         0x1234
#define SYS_nbcore              0x4321
// end of custom syscall


// Translation of flags from riscv to local machine
#define SYS_O_RDONLY            0x0
#define SYS_O_WRONLY            0x1
#define SYS_O_RDWR              0x2
#define SYS_O_APPEND            0x8
#define SYS_O_CREAT             0x200
#define SYS_O_TRUNC             0x400
#define SYS_O_EXCL              0x800
#define SYS_O_SYNC              0x2000
#define SYS_O_NONBLOCK          0x4000
#define SYS_O_NOCTTY            0x8000






#endif /* INCLUDES_ISA_RISCVISA_H_ */
