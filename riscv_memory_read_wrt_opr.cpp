#include "riscv_cache_l1.hpp"


/* Function Name.: mem_read_op
 * Definition....:
 *
 */
ap_uint<32> mem_read_op (ap_uint<3> access_type, byte_type last_bits, data_type ret_value_1)
{
#pragma HLS inline
ap_uint<32> addr_val_1,  read_data;

addr_val_1 = ret_value_1;

switch(access_type)
{
	case RISCV_FUNCT3_LOAD_BYTE:
	case RISCV_FUNCT3_LOAD_BYTE_UNSIGNED:
		switch(last_bits)
		{
			case 0b00:
				read_data.range( 7, 0) = addr_val_1.range( 7, 0);
				read_data.range(31, 8) = 0;
				break;

			case 0b01:
				read_data.range( 7, 0) = addr_val_1.range(15, 8);
				read_data.range(31, 8) = 0;
				break;

			case 0b10:
				read_data.range( 7, 0) = addr_val_1.range(23,16);
				read_data.range(31, 8) = 0;
				break;

			case 0b11:
				read_data.range( 7, 0) = addr_val_1.range(31,24);
				read_data.range(31, 8) = 0;
				break;
		}
		break;


	case RISCV_FUNCT3_LOAD_HALFWORD:
	case RISCV_FUNCT3_LOAD_HALFWORD_UNSIGNED:
		switch(last_bits)
		{
			case 0b00:
				read_data.range(15, 0) = addr_val_1.range(15, 0);
				read_data.range(31,16) = 0;
				break;

			case 0b01:
				read_data.range(15, 0) = addr_val_1.range(23, 8);
				read_data.range(31,16) = 0;
				break;

			case 0b10:
				read_data.range(15, 0) = addr_val_1.range(31,16);
				read_data.range(31,16) = 0;
				break;

			case 0b11:
				read_data.range( 7, 0) = addr_val_1.range(31,24);
				read_data.range(15, 8) = addr_val_1.range( 7, 0);
				read_data.range(31,16) = 0;
				break;
		}
		break;

	case RISCV_FUNCT3_LOAD_WORD:
	case RISCV_FUNCT3_LOAD_WORD_UNSIGNED:
		switch(last_bits)
		{
			case 0b00:
				read_data			   = addr_val_1;
				break;

			case 0b01:
				read_data.range(23, 0) = addr_val_1.range(31, 8);
				read_data.range(31,24) = addr_val_1.range( 7, 0);
				break;

			case 0b10:
				read_data.range(15, 0) = addr_val_1.range(31,16);
				read_data.range(31,16) = addr_val_1.range(15, 0);
				break;

			case 0b11:
				read_data.range( 7, 0) = addr_val_1.range(31,24);
				read_data.range(31, 8) = addr_val_1.range(23, 0);
				break;
		}
		break;
}


return read_data;
}


/* Function Name.: mem_write_op
 * Definition....:
 *
 */
void mem_write_op (	 ap_uint<3> access_type, byte_type last_bits,
					 data_type write_data,
					 volatile data_type *mem_ptr, address_type inner_addr)
{
#pragma HLS inline
data_type addr_val_1, addr_val_2;

switch(access_type){
	case RISCV_FUNCT3_STORE_BYTE:

		addr_val_1 = mem_ptr[inner_addr];

		switch(last_bits)
		{
			case 0b00:
				addr_val_1.range( 7, 0) = write_data.range(7, 0);
				break;

			case 0b01:
				addr_val_1.range(15, 8) = write_data.range(7, 0);
				break;

			case 0b10:
				addr_val_1.range(23,16) = write_data.range(7, 0);
				break;

			case 0b11:
				addr_val_1.range(31,24) = write_data.range(7, 0);
				break;
		}
		mem_ptr[inner_addr] = addr_val_1;
		break;

	case RISCV_FUNCT3_STORE_HALFWORD:

		addr_val_1 = mem_ptr[inner_addr];

		switch(last_bits)
		{
			case 0b00:
				addr_val_1.range(15, 0) = write_data.range(15, 0);
				break;

			case 0b01:
				addr_val_1.range(23, 8) = write_data.range(15, 0);
				break;

			case 0b10:
				addr_val_1.range(31,16) = write_data.range(15, 0);
				break;

			case 0b11:
				addr_val_2 = mem_ptr[inner_addr + 1];

				addr_val_1.range(31,24) = write_data.range( 7, 0);
				addr_val_2.range( 7, 0) = write_data.range(15, 8);

				mem_ptr[inner_addr + 1] = addr_val_2;

				break;
		}
		mem_ptr[inner_addr] 	= addr_val_1;

		break;

	case RISCV_FUNCT3_STORE_WORD:
		switch(last_bits)
		{
			case 0b00:
				mem_ptr[inner_addr] 	= write_data;
				break;

			case 0b01:
				addr_val_2 = mem_ptr[inner_addr + 1];

				addr_val_1.range(31, 8)	= write_data.range(23, 0);
				addr_val_2.range( 7, 0)	= write_data.range(31,24);

				mem_ptr[inner_addr] 	= addr_val_1;
				mem_ptr[inner_addr + 1] = addr_val_2;
				break;

			case 0b10:
				addr_val_2 = mem_ptr[inner_addr + 1];

				addr_val_1.range(31,16)	= write_data.range(15, 0);
				addr_val_2.range(15, 0)	= write_data.range(31,16);

				mem_ptr[inner_addr] 	= addr_val_1;
				mem_ptr[inner_addr + 1] = addr_val_2;
				break;

			case 0b11:
				addr_val_2 = mem_ptr[inner_addr + 1];

				addr_val_1.range(31,24)	= write_data.range( 7, 0);
				addr_val_2.range(23, 0)	= write_data.range(31, 8);

				mem_ptr[inner_addr] 	= addr_val_1;
				mem_ptr[inner_addr + 1] = addr_val_2;
				break;
		}
		break;

	case RISCV_FUNCT3_STORE_DOUBLE_WORD:
		break;
}

}


/* Function Name.: cache_write_op
 * Definition....:
 *
 */
void cache_write_op (ap_uint<3> access_type, byte_type last_byte,
					 data_type value,
					 cache_address_s cache_addr_1, unsigned int cache_way)
{
#pragma HLS inline
	data_type *	cell_val;
	data_type	low_val, high_val;

	cell_val = &cache[cache_way][cache_addr_1.ca_index][cache_addr_1.ca_offset];

	switch(access_type)
	{
		case	RISCV_FUNCT3_LOAD_BYTE:
			switch(last_byte)
			{
				case 0b00:
					(*cell_val).range( 7, 0) = value.range(7,0);
					break;

				case 0b01:
					(*cell_val).range(15, 8) = value.range(7,0);

					break;

				case 0b10:
					(*cell_val).range(23,16) = value.range(7,0);
					break;

				case 0b11:
					(*cell_val).range(31,24) = value.range(7,0);
					break;
			}
			//cache[cache_way][cache_addr_1.ca_index][cache_addr_1.ca_offset] = cell_val;
			break;

		case	RISCV_FUNCT3_LOAD_HALFWORD:
			switch(last_byte)
			{
				case 0b00:
					(*cell_val).range(15, 0) = value.range(15,0);
					break;

				case 0b01:
					//(*cell_val).range(23, 8) = value.range(15,0);
					break;

				case 0b10:
					(*cell_val).range(31,16) = value.range(15,0);
					break;

				case 0b11:
					break;
			}
			//cache[cache_way][cache_addr_1.ca_index][cache_addr_1.ca_offset] = cell_val;
			break;

		case	RISCV_FUNCT3_LOAD_WORD:
		default:
			switch(last_byte)
			{
				case 0b00:
				case 0b01:
				case 0b10:
				case 0b11:

					(*cell_val)= value;
					break;
			}
			//cache[cache_way][cache_addr_1.ca_index][cache_addr_1.ca_offset] = cell_val;
			break;
	}

	cache_dirty[cache_way][cache_addr_1.ca_index]	 				= YES;

}
