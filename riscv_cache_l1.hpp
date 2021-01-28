#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "ap_int.h"


#define	RISCV_L1_CACHE_SIZE 5000


/* START OF OPCODE DEFINES */
#define	RISCV_OPCODE_ALU_OPR					0b0110011
#define	RISCV_OPCODE_IMM_ALU_OPR				0b0010011
#define	RISCV_OPCODE_AUIPC_OPR					0b0010111
#define	RISCV_OPCODE_LUI_OPR					0b0110111
#define	RISCV_OPCODE_LOAD_OPR					0b0000011
#define	RISCV_OPCODE_STORE_OPR					0b0100011
#define	RISCV_OPCODE_BRANCH_OPR					0b1100011
#define	RISCV_OPCODE_JAL_OPR					0b1101111
#define	RISCV_OPCODE_JALR_OPR					0b1100111
#define	RISCV_OPCODE_FLOAT_OPR					0b1010011
/* END   OF OPCODE DEFINES */

/* START OF FUNCT7 DEFINES */
#define	RISCV_FUNCT7_ARITHMATIC_LOGIC_OPR		0b0000000
#define	RISCV_FUNCT7_SUB_SRA_OPR				0b0100000
#define	RISCV_FUNCT7_MULDIV_OPR					0b0000001
#define	RISCV_FUNCT7_FLOAT_ADD_OPR				0b0000000
#define	RISCV_FUNCT7_FLOAT_SUB_OPR				0b0000100
#define	RISCV_FUNCT7_FLOAT_MUL_OPR				0b0001000
#define	RISCV_FUNCT7_FLOAT_DIV_OPR				0b0001100
/* END   OF FUNCT7 DEFINES */

/* START OF FUNCT3 DEFINES */
#define	RISCV_FUNCT3_LOAD_BYTE					0b000
#define	RISCV_FUNCT3_LOAD_HALFWORD				0b001
#define	RISCV_FUNCT3_LOAD_WORD					0b010
#define	RISCV_FUNCT3_LOAD_DOUBLE_WORD			0b011
#define	RISCV_FUNCT3_LOAD_BYTE_UNSIGNED			0b100
#define	RISCV_FUNCT3_LOAD_HALFWORD_UNSIGNED		0b101
#define	RISCV_FUNCT3_LOAD_WORD_UNSIGNED			0b110

#define	RISCV_FUNCT3_STORE_BYTE					0b000
#define	RISCV_FUNCT3_STORE_HALFWORD				0b001
#define	RISCV_FUNCT3_STORE_WORD					0b010
#define	RISCV_FUNCT3_STORE_DOUBLE_WORD			0b011
/* END   OF FUNCT3 DEFINES */

enum {
	RISCV_CPU_L1_CACHE_READ_REQ,
	RISCV_CPU_L1_CACHE_WRITE_REQ
};


#define ADDRESS_SIZE 			32
#define TAG_SIZE 				19
#define INDEX_SIZE 				8
#define BYTE_SIZE 				2
#define CACHE_WAY_SIZE			4
#define OFFSET_SIZE 			(ADDRESS_SIZE - INDEX_SIZE - TAG_SIZE - BYTE_SIZE)
#define CACHE_DEPTH 			(1<<INDEX_SIZE)
#define CACHE_WIDTH 			(1<<OFFSET_SIZE)
#define MEMORY_4_BIT_ALIGNER	0xFFFFFFFC

typedef ap_uint<ADDRESS_SIZE> 	address_type;
typedef ap_uint<TAG_SIZE> 		tag_type;
typedef ap_uint<INDEX_SIZE> 	index_type;
typedef ap_uint<OFFSET_SIZE> 	offset_type;
typedef ap_uint<BYTE_SIZE> 		byte_type;
typedef ap_uint<ADDRESS_SIZE> 	data_type;

enum{NO,YES};
enum{L1_CACHE_DISABLE, L1_CACHE_ENABLE};

enum{
	L1_CACHE_WR_POLCY_NO,
	L1_CACHE_WR_POLCY_WR_THROUGH,
	L1_CACHE_WR_POLCY_WR_BACK,
};

enum{
	L1_CACHE_OPERATION_MEMORY_ACCESS,
	L1_CACHE_OPERATION_FLUSH,
	L1_CACHE_OPERATION_INVALIDATE
};

struct cache_address_s{
    tag_type 			ca_tag;
    index_type 			ca_index;
    offset_type 		ca_offset;
    byte_type			ca_byte;

    data_type 			ca_ret_value;
    address_type		ca_fetch_addr;
    address_type		ca_tagindex;
    address_type		ca_offsetbyte;
    address_type 		ca_writeback_address;
};


void riscv_cache_access(address_type address, data_type value, ap_uint<2> OpType, ap_uint<3> access_type,  ap_uint<32>& dataOut,bool &cache_accessfailed,volatile data_type *main_memory ,cache_address_s		&cache_addr_1);
ap_uint<32> riscv_cache_access_failed(address_type address, data_type value, ap_uint<2> OpType,volatile data_type *main_memory, ap_uint<3> access_type,bool cache_accessfailed,cache_address_s		cache_addr_1 );
//void riscv_cache_access_failed(address_type address, data_type value, ap_uint<2> OpType,volatile data_type *main_memory, ap_uint<3> access_type,bool cache_accessfailed,cache_address_s		cache_addr_1 ,  ap_uint<32>& dataOut);

data_type riscv_uncached_access(address_type address, data_type value, ap_uint<1> action, volatile data_type *main_memory, ap_uint<3> access_type);
ap_uint<32> mem_read_op (ap_uint<3> access_type, byte_type last_bits, data_type ret_value_1);
void mem_write_op (	 ap_uint<3> access_type, byte_type last_bits, data_type write_data, volatile data_type *mem_ptr, address_type inner_addr);
void cache_write_op (ap_uint<3> access_type, byte_type last_byte,					 data_type value,					 cache_address_s cache_addr_1, unsigned int cache_way);
void riscv_cache_level_1(ap_uint<68> 			*cache_access_request,
						 ap_uint<32> 			*cache_access_read_data_result,
						 volatile ap_uint<32> 	main_memory[8192]);


extern bool 		cache_valid[CACHE_WAY_SIZE][CACHE_DEPTH];
extern bool 		cache_dirty[CACHE_WAY_SIZE][CACHE_DEPTH];
extern tag_type 	cache_tag[CACHE_WAY_SIZE][CACHE_DEPTH];
extern data_type 	cache[CACHE_WAY_SIZE][CACHE_DEPTH][CACHE_WIDTH];
