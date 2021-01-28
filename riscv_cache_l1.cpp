#include "riscv_cache_l1.hpp"

bool 		cache_valid[CACHE_WAY_SIZE][CACHE_DEPTH];
bool 		cache_dirty[CACHE_WAY_SIZE][CACHE_DEPTH];
tag_type 	cache_tag[CACHE_WAY_SIZE][CACHE_DEPTH];
data_type 	cache[CACHE_WAY_SIZE][CACHE_DEPTH][CACHE_WIDTH];

char		cache_replacement_policies[CACHE_DEPTH];
//int			read_allocation_pol  = YES;
//int			write_allocation_pol = L1_CACHE_WR_POLCY_WR_BACK;
#define 	read_allocation_pol_YES
#define 	write_allocation_pol_L1_CACHE_WR_POLCY_WR_BACK

/* Function Name.: cache_replacement_policy
 *
 * Definition....:
 *
 */
int cache_replacement_policy(int index)
{
#pragma HLS inline
	int i;

	for(i=0; i<CACHE_WAY_SIZE; i++){
#pragma HLS UNROLL
		if(cache_valid[i][index] == NO){
			return i;
		}
	}

	i = (cache_replacement_policies[index]++) % CACHE_WAY_SIZE;
	return i;
}



/* Function Name.: scan_cache_way
 * Definition....:
 *
 */
int scan_cache_way(int index, int tag)
{
#pragma HLS inline
	int i;

	for(i=0; i<CACHE_WAY_SIZE; i++)
	{
#pragma HLS UNROLL
		if((cache_valid[i][index] == YES) && (cache_tag[i][index] == tag))
			break;
	}

	return i;
}

/* Function Name.: writeback_block
 * Definition....:
 *
 */
void writeback_block(data_type lowermem[CACHE_WIDTH],volatile data_type *highermem, address_type address)
{
#pragma HLS inline
    address_type i,real_addr;

    real_addr = address>>2;

    for (i=0;i<CACHE_WIDTH;i++)
    {
#pragma HLS UNROLL
    	highermem[real_addr+i]=lowermem[i];
    }
}

/* Function Name.: fetch_block
 * Definition....:
 *
 */
void fetch_block(data_type lowermem[CACHE_WIDTH],volatile data_type *highermem, address_type address)
{
#pragma HLS inline
    address_type i, real_addr;

    real_addr = address>>2;

    for (i=0;i<CACHE_WIDTH;i++)
    {
#pragma HLS UNROLL
    	lowermem[i]	= highermem[real_addr+i];
    }
}

/* Function Name.: fill_cache_address_struct
 * Definition....:
 *
 */
void 	fill_cache_address_struct(cache_address_s *cache_addr, address_type address)
{
#pragma HLS inline
	cache_addr->ca_tagindex 	= address	>> (OFFSET_SIZE + BYTE_SIZE);
	cache_addr->ca_tag 			= cache_addr->ca_tagindex	>> INDEX_SIZE;
	cache_addr->ca_index 		= cache_addr->ca_tagindex & ((1 << INDEX_SIZE) - 1);
	cache_addr->ca_offsetbyte	= (address & ((1 << (OFFSET_SIZE + BYTE_SIZE)) - 1));
	cache_addr->ca_offset		= cache_addr->ca_offsetbyte >> BYTE_SIZE;
	cache_addr->ca_byte			= cache_addr->ca_offsetbyte & ((1 << BYTE_SIZE) - 1);
	cache_addr->ca_fetch_addr 	= address - cache_addr->ca_offsetbyte;

}

/* Function Name.: riscv_cache_invalidate
 * Definition....:
 *
 */
void riscv_cache_invalidate(address_type address, volatile data_type *main_memory, uint32_t mem_size)
{
#pragma HLS inline
    cache_address_s		cache_addr_1;
    int 				i, curr_way;

    mem_size = (mem_size >> 5) + 1;

	for(i=0; i<mem_size; i++){
		fill_cache_address_struct(&cache_addr_1, address);
		curr_way = scan_cache_way(cache_addr_1.ca_index, cache_addr_1.ca_tag);

		if(curr_way < CACHE_WAY_SIZE){
		#pragma HLS UNROLL
			/* DATA IS PLACED IN CACHE */
			cache_valid[curr_way][cache_addr_1.ca_index] = NO;
			cache_dirty[curr_way][cache_addr_1.ca_index] = NO;
		}

		/* GO FOR NEXT LINE */
		address = address + 32;
	}

}

/* Function Name.: riscv_cache_flush
 * Definition....:
 *
 */
void riscv_cache_flush(address_type address, volatile data_type *main_memory, uint32_t mem_size)
{
#pragma HLS inline
    cache_address_s		cache_addr_1;
    int 				i, curr_way;

    mem_size = (mem_size >> 5) + 1;

	for(i=0; i<mem_size; i++){
		fill_cache_address_struct(&cache_addr_1, address);
		curr_way = scan_cache_way(cache_addr_1.ca_index, cache_addr_1.ca_tag);

		if(curr_way < CACHE_WAY_SIZE){
			/* DATA IS PLACED IN CACHE */

			if(cache_dirty[curr_way][cache_addr_1.ca_index]==YES){
				/* WRITE BACK IF THE LINE IS DIRTY */
				cache_addr_1.ca_writeback_address = (((address_type)(cache_tag[curr_way][cache_addr_1.ca_index])<<INDEX_SIZE)+ cache_addr_1.ca_index)<<(OFFSET_SIZE + BYTE_SIZE);
				writeback_block(cache[curr_way][cache_addr_1.ca_index],main_memory,cache_addr_1.ca_writeback_address);
				cache_dirty[curr_way][cache_addr_1.ca_index] = NO;
			}
		}

		/* GO FOR NEXT LINE */
		address = address + 32;
	}

}

/* Function Name.: riscv_cache_access
 * Definition....:
 *
 */

ap_uint<32> riscv_cache_access_failed(address_type address, data_type value, ap_uint<2> OpType,volatile data_type *main_memory, ap_uint<3> access_type,bool cache_accessfailed,cache_address_s		cache_addr_1 )
//void riscv_cache_access_failed(address_type address, data_type value, ap_uint<2> OpType,volatile data_type *main_memory, ap_uint<3> access_type,bool cache_accessfailed,cache_address_s		cache_addr_1 ,  ap_uint<32>& dataOut)
		{
#pragma HLS inline off // ,
//#pragma HLS pipeline II=4
#pragma HLS latency min=0

	if(cache_accessfailed){
	int curr_way = cache_replacement_policy(cache_addr_1.ca_index);

	data_type ret_value_1;

	/* DATA IS NOT IN CACHE */
    if(cache_dirty[curr_way][cache_addr_1.ca_index]==YES)
    {
    	cache_addr_1.ca_writeback_address = (((address_type)(cache_tag[curr_way][cache_addr_1.ca_index])<<INDEX_SIZE)+ cache_addr_1.ca_index)<<(OFFSET_SIZE + BYTE_SIZE);
    	writeback_block(cache[curr_way][cache_addr_1.ca_index],main_memory,cache_addr_1.ca_writeback_address);
    }
    fetch_block(cache[curr_way][cache_addr_1.ca_index],main_memory, cache_addr_1.ca_fetch_addr);
    cache_dirty[curr_way][cache_addr_1.ca_index]	= NO;
    cache_valid[curr_way][cache_addr_1.ca_index]	= YES;
    cache_tag[curr_way][cache_addr_1.ca_index]		= cache_addr_1.ca_tag;

    switch(OpType){
    case(0):
    case(4):
	return 0;
		break;
    case(1)://ST
				cache_write_op(access_type, cache_addr_1.ca_byte, value, cache_addr_1, curr_way);
		#ifdef  write_allocation_pol_L1_CACHE_WR_POLCY_WR_THROUGH
				if(write_allocation_pol == L1_CACHE_WR_POLCY_WR_THROUGH){
					main_memory[address>>2] 	= cache[curr_way][cache_addr_1.ca_index][cache_addr_1.ca_offset];
					main_memory[(address>>2)+1] = cache[curr_way][cache_addr_2.ca_index][cache_addr_2.ca_offset];
				}
		#endif
				return 0;
				break;
    case(2)://LD
				ret_value_1 = cache[curr_way][cache_addr_1.ca_index][cache_addr_1.ca_offset];
				//dataOut = mem_read_op(access_type, cache_addr_1.ca_byte, ret_value_1);
				return mem_read_op(access_type, cache_addr_1.ca_byte, ret_value_1);
				break;
    }


	}else{
		return 0;
	}
	return 0;
}
//		riscv_cache_access(address[par_num], valueToWrite[(par_num-1)] ,   (rf_addr == (memorybits)  (par_num))?OpType:NONE,executereg.funct3,result[par_num-1], cache_accessfailed,mem_extra);
void riscv_cache_access(address_type address, data_type value, ap_uint<2> OpType, ap_uint<3> access_type,  ap_uint<32>& dataOut,bool &cache_accessfailed,volatile data_type *main_memory ,cache_address_s		&cache_addr_1)
{
	/*
	 *
	 * typedef enum {
  NONE = 0,
  ST,
  LD
} memOpType;
	 *
	 *
	 */
#pragma HLS inline// off
#pragma HLS pipeline// off

    data_type 			ret_value_1;
   // cache_address_s		cache_addr_1;
    //cache_address_s		cache_addr_2;
    int 				i, curr_way;

#pragma HLS ARRAY_PARTITION variable=cache_valid complete  dim=1
#pragma HLS ARRAY_PARTITION variable=cache_dirty complete  dim=1
#pragma HLS ARRAY_PARTITION variable=cache complete  dim=1
#pragma HLS ARRAY_PARTITION variable=cache_tag complete  dim=1

#pragma HLS resource variable=cache_valid core=RAM_2P_1S
#pragma HLS resource variable=cache_dirty core=RAM_2P_1S
#pragma HLS resource variable=cache core=RAM_2P_1S
#pragma HLS resource variable=cache_tag core=RAM_2P_1S
//#pragma HLS resource variable=cache_replacement_policies core=RAM_2P_1S

/*
 * #define 	read_allocation_pol_YES
#define 	write_allocation_pol_L1_CACHE_WR_POLCY_WR_BACK
 */

    fill_cache_address_struct(&cache_addr_1, address);
   // fill_cache_address_struct(&cache_addr_2, address+0x04);

	curr_way = (OpType==0)? 0 : scan_cache_way(cache_addr_1.ca_index, cache_addr_1.ca_tag);


	/*if(cache_addr_1.ca_fetch_addr != cache_addr_2.ca_fetch_addr)
	{
		if(((cache_addr_1.ca_byte == 0x11) & (access_type == RISCV_FUNCT3_STORE_HALFWORD)) || ((cache_addr_1.ca_byte != 0x00) & (access_type == RISCV_FUNCT3_STORE_WORD)))
		{
			if(cache_valid[curr_way][cache_addr_2.ca_index] == NO || cache_tag[curr_way][cache_addr_2.ca_index] != cache_addr_2.ca_tag)
				{
					if(cache_dirty[curr_way][cache_addr_2.ca_index]==YES)
					{
						cache_addr_2.ca_writeback_address = (((address_type)(cache_tag[curr_way][cache_addr_2.ca_index])<<INDEX_SIZE)+ cache_addr_2.ca_index)<<(OFFSET_SIZE + BYTE_SIZE);
						writeback_block(cache[curr_way][cache_addr_2.ca_index],main_memory,cache_addr_2.ca_writeback_address);
					}
					fetch_block(cache[curr_way][cache_addr_2.ca_index],main_memory, cache_addr_2.ca_fetch_addr);
					cache_dirty[curr_way][cache_addr_2.ca_index]	= NO;
					cache_valid[curr_way][cache_addr_2.ca_index]	= YES;
					cache_tag[curr_way][cache_addr_2.ca_index]		= cache_addr_2.ca_tag;
				}
		}
	}*/

	 if(curr_way >= CACHE_WAY_SIZE)
	    {
	    	/* CHECK ALLOCATION POLICIES */
	#ifdef read_allocation_pol_NO
	    	if(((action == RISCV_CPU_L1_CACHE_READ_REQ) && (read_allocation_pol == NO)) ||  ((action == RISCV_CPU_L1_CACHE_WRITE_REQ) && (write_allocation_pol == L1_CACHE_WR_POLCY_NO))){
	    		dataOut= riscv_uncached_access(address, value, action, main_memory, access_type);
	    	}
	#endif
	    	 cache_accessfailed = true;
	    	 //riscv_cache_access_failed( address,  value, OpType, main_memory,  access_type,   dataOut, cache_accessfailed,		cache_addr_1 );
	    }else {
	    	 cache_accessfailed = false;

		        switch(OpType){
		        case(0):
		        case(4):
					break;
		        case(1)://ST
						cache_write_op(access_type, cache_addr_1.ca_byte, value, cache_addr_1, curr_way);
#ifdef  write_allocation_pol_L1_CACHE_WR_POLCY_WR_THROUGH
		if(write_allocation_pol == L1_CACHE_WR_POLCY_WR_THROUGH){
			main_memory[address>>2] 	= cache[curr_way][cache_addr_1.ca_index][cache_addr_1.ca_offset];
			main_memory[(address>>2)+1] = cache[curr_way][cache_addr_2.ca_index][cache_addr_2.ca_offset];
		}
#endif
		break;
		        case(2)://LD
				ret_value_1 = cache[curr_way][cache_addr_1.ca_index][cache_addr_1.ca_offset];
		        dataOut = mem_read_op(access_type, cache_addr_1.ca_byte, ret_value_1);
		        break;
		        }



	    }
}


/* Function Name.: uncached_access
 * Definition....:
 *
 */
#ifdef read_allocation_pol_NO
data_type riscv_uncached_access(address_type address, data_type value, ap_uint<1> action, volatile data_type *main_memory, ap_uint<3> access_type)
{
#pragma HLS inline off
	data_type 			ret_value_1, ret_value_2;
	data_type			ret_value, real_mem_addr;
	byte_type			last_byte;

	real_mem_addr = address >> 2;
	last_byte	  = address.range(1,0);

	switch(action)
	{	case RISCV_CPU_L1_CACHE_READ_REQ:
			ret_value_1 = main_memory[real_mem_addr];

			if(((last_byte == 0x11) & (access_type == RISCV_FUNCT3_STORE_HALFWORD)) || ((last_byte != 0x00) & (access_type == RISCV_FUNCT3_STORE_WORD)))
				ret_value_2 = main_memory[real_mem_addr + 1];

			ret_value = mem_read_op(access_type, last_byte, ret_value_1, ret_value_2);
			break;

		case RISCV_CPU_L1_CACHE_WRITE_REQ:

			mem_write_op(access_type, last_byte, value, main_memory, real_mem_addr);
			break;
	}

	return ret_value;
}
#endif



