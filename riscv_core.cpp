//Top block for RISCV design
#include <stdint.h>
#include <stdio.h>
#include "datapath2.h"

//ap_uint<32> PC;

FILE *myfile;


/*core list
 * Vivado_DDS Vivado_FFT Vivado_FIR AddSub AddSubnS Mul MulnS DivnS MuxnS FIFO FIFO_BRAM FIFO_LUTRAM FIFO_SRL RAM_T2P_BRAM
 * RAM_T2P_URAM RAM_2P RAM_2P_LUTRAM RAM_2P_BRAM RAM_2P_URAM RAM_1P RAM_1P_LUTRAM RAM_1P_BRAM
 * RAM_1P_URAM RAM_S2P_BRAM RAM_S2P_URAM RAM_S2P_LUTRAM ROM_nP_LUTRAM ROM_nP_BRAM ROM_2P ROM_2P_LUTRAM
 * ROM_2P_BRAM ROM_1P ROM_1P_LUTRAM ROM_1P_BRAM DSP_Macro AddSub_DSP QAddSub_DSP Mul_LUT XPM_MEMORY
 * HDiv HSqrt HAddSub_nodsp HAddSub_meddsp HAddSub_fulldsp HMul_nodsp HMul_fulldsp HMul_maxdsp FDiv
 * FSqrt DDiv DSqrt DRecip DRSqrt FAddSub_nodsp FAddSub_fulldsp FMul_nodsp FMul_meddsp FMul_fulldsp
 * FMul_maxdsp FRSqrt_nodsp FRSqrt_fulldsp FRecip_nodsp FRecip_fulldsp FLog_nodsp FLog_meddsp FLog_fulldsp
 * FExp_nodsp FExp_meddsp FExp_fulldsp DAddSub_nodsp DAddSub_fulldsp
 * DMul_nodsp DMul_meddsp DMul_fulldsp DMul_maxdsp DLog_nodsp DLog_meddsp DLog_fulldsp DExp_nodsp
 * DExp_meddsp DExp_fulldsp
 * */


/*
 *     for (i=0;i<NUM_COL;i=i+1) begin
        always @(posedge clk) begin
            if (ce0) begin
                if (we0[i]) begin
                    ram[addr0][i*COL_WIDTH +: COL_WIDTH] <= d0[i*COL_WIDTH +: COL_WIDTH];
                end
                q0[i*COL_WIDTH +: COL_WIDTH] <= ram[addr0][i*COL_WIDTH +: COL_WIDTH];
            end
        end
    end
 */

void getsetint(ap_uint<32> addr, memMask mask, memOpType opType, ap_int<32> dataIn ,ap_int<32>*memory, ap_uint<32>& dataOut  ){// , ap_uint<32>& dataOut  ap_int<32>*memory,  ap_uint<32>& dataOut
			#pragma  HLS inline //off
			#pragma HLS PIPELINE

			const int adress =  (ap_uint<32>)  addr.range(31,2);
			//ap_int<32>* dataptr = &mem_par[0][adress];;
			ap_int<32>* dataptr = &memory[adress];
			//ap_uint<32> dataOut;

			//dataOut  = (*dataptr).range( 31,0);
			ap_uint<32> datamid ;
			 ap_int<32> loaddata;



			switch(opType) {

			    case ST:

			    	//(* dataptr) =dataIn ;
			      switch(mask) {
			        case BYTE:
			      		switch(addr.range(1,0)){
			      			case 0:
			      				(*dataptr).range( 7,0) = (ap_int<8>)dataIn.range(7,0);
			      				break;
			      			case 1:
			      				(*dataptr).range(15,8) = (ap_int<8>)dataIn.range(7,0);
			      				break;
			      			case 2:
			      				(*dataptr).range( 23,16)  =(ap_int<8>) dataIn.range(7,0);
			      				break;
			      			case 3:
			      				(*dataptr).range( 31,24) =(ap_int<8>) dataIn.range(7,0);
			      				break;
			      		}
			          break;
			        case HALF:
			        	switch(addr.range(1,0)){
			        	case 2:
			        	case 3:
			        		(*dataptr).range( 31,16) =(ap_int<16>) dataIn.range(15,0);
			      			break;
			        	case 0:
			        	case 1:
			        		(*dataptr).range(15,0) = (ap_int<16>)dataIn.range(15,0);
			      			break;
			      		}
			          break;
			        case WORD:
			        	(*dataptr).range( 31,0)= dataIn.range( 31,0);
			          break;
			        default:
			        break;
			      }

			      break;
			    case LD:
					//dataOut.range( 31,0) = (*dataptr).range( 31,0);
			    //	dataOut  = (*dataptr).range( 31,0);
			    	//dataOut = *dataptr;
			    	//loaddata = *dataptr;
			      switch(mask) {
			      	  case BYTE: //|
			      		switch(addr.range(1,0)){
			      			case 0:
			      				dataOut = (ap_int<8>)((*dataptr).range( 7,0)) ;
			      				break;
			      			case 1:
			      				dataOut = (ap_int<8>)((*dataptr).range(15,8)) ;
			      				break;
			      			case 2:
			      				dataOut = (ap_int<8>)((*dataptr).range( 23,16)) ;
			      				break;
			      			case 3:
			      				dataOut = (ap_int<8>)((*dataptr).range( 31,24)) ;
			      				break;
			      		}
			          break;
			        case HALF:
			        	switch(addr.range(1,0)){
			        	case 2:
			        	case 3:
			      			dataOut = (ap_int<16>)((*dataptr).range(31,16)) ;
			      			break;
			        	case 0:
			        	case 1:
			      			dataOut = (ap_int<16>)((*dataptr).range( 15,0)) ;
			      			break;
			      		}
			          break;
			        case WORD:
			        	dataOut = (*dataptr).range( 31,0);
			          break;
			        case BYTE_U:
			      		switch(addr.range(1,0)){
			      			case 0:
			      				dataOut = (ap_uint<8>)((*dataptr).range( 7,0)) ;
			      				break;
			      			case 1:
			      				dataOut = (ap_uint<8>)((*dataptr).range(15,8)) ;
			      				break;
			      			case 2:
			      				dataOut = (ap_uint<8>)((*dataptr).range( 23,16)) ;
			      				break;
			      			case 3:
			      				dataOut = (ap_uint<8>)((*dataptr).range( 31,24)) ;
			      				break;
			      		}
			          break;
			        case HALF_U:
			        	switch(addr.range(1,0)){
			        	case 2:
			        	case 3:
			      			dataOut = (ap_uint<16>)((*dataptr).range(31,16)) ;
			      			break;
			        	case 0:
			        	case 1:
			      			dataOut = (ap_uint<16>)((*dataptr).range( 15,0)) ;
			      			break;
			      		}
			          break;
			        default:
			        	break;
			      }

			      break;
				    case NONE:
						//dataOut = *dataptr;

				    	//dataOut =dataIn;
			      		break;
			  }
//return dataOut;
		}

void getset_mem(ap_uint<32> addr, memMask mask, memOpType opType, ap_int<32> dataIn , ap_uint<32>& dataOut ){// , ap_uint<32>& dataOut  ap_int<32>*memory,  ap_uint<32>& dataOut
#pragma  HLS inline //off
#pragma HLS PIPELINE
#pragma HLS latency min=0 max=0
#pragma HLS resource variable=mem core=RAM_2P_1S

	const int adress =  (ap_uint<32>)  addr.range(31,2);

			ap_int<8>* dataptr0 = &(mem[adress][0]);
			ap_int<8>* dataptr1 = &(mem[adress][1]);
			ap_int<8>* dataptr2 = &(mem[adress][2]);
			ap_int<8>* dataptr3 = &(mem[adress][3]);
			ap_uint<32> datamid ; //= (*dataptr).range( 31,0);
			 ap_int<16> temp16;
			 ap_int<32> loaddata;


			switch(opType) {

			    case ST:

			    	//(* dataptr) =dataIn ;
			      switch(mask) {
			        case BYTE:
			      		switch(addr.range(1,0)){
			      			case 0:
			      				//(*dataptr).range( 7,0) = (ap_int<8>)dataIn.range(7,0);
			      				(*dataptr0) = (ap_int<8>)dataIn.range(7,0);
			      				break;
			      			case 1:
			      				//(*dataptr).range(15,8) = (ap_int<8>)dataIn.range(7,0);
			      				(*dataptr1) = (ap_int<8>)dataIn.range(7,0);
			      				break;
			      			case 2:
			      				//(*dataptr).range( 23,16)  =(ap_int<8>) dataIn.range(7,0);
			      				(*dataptr2)  =(ap_int<8>) dataIn.range(7,0);
			      				break;
			      			case 3:
			      				//(*dataptr).range( 31,24) =(ap_int<8>) dataIn.range(7,0);
			      				(*dataptr2) =(ap_int<8>) dataIn.range(7,0);
			      				break;
			      		}
			          break;
			        case HALF:
			        	switch(addr.range(1,0)){
			        	case 2:
			        	case 3:
			        		//(*dataptr).range( 31,16) =(ap_int<16>) dataIn.range(15,0);
			        		(*dataptr2) =(ap_int<8>) dataIn.range(7,0);
			        		(*dataptr3) =(ap_int<8>) dataIn.range(15,8);
			      			break;
			        	case 0:
			        	case 1:
			        		//(*dataptr).range(15,0) = (ap_int<16>)dataIn.range(15,0);
			        		(*dataptr0) =(ap_int<8>) dataIn.range(7,0);
			        		(*dataptr1) =(ap_int<8>) dataIn.range(15,8);
			      			break;
			      		}
			          break;
			        case WORD:
			        	//(*dataptr).range( 31,0)= dataIn.range( 31,0);
		        		(*dataptr0) =(ap_int<8>) dataIn.range(7,0);
		        		(*dataptr1) =(ap_int<8>) dataIn.range(15,8);
		        		(*dataptr2) =(ap_int<8>) dataIn.range(23,16);
		        		(*dataptr3) =(ap_int<8>) dataIn.range(31,24);
			          break;
			        default:
			        break;
			      }

			      break;
			    case LD:
					//dataOut.range( 31,0) = (*dataptr).range( 31,0);
			    //	dataOut  = (*dataptr).range( 31,0);
			    	//dataOut = *dataptr;
			    	//loaddata = *dataptr;
			      switch(mask) {
			      	  case BYTE: //|
			      		switch(addr.range(1,0)){
			      			case 0:
			      				//dataOut = (ap_int<8>)((*dataptr).range( 7,0)) ;
			      				dataOut = (ap_int<8>)(*dataptr0) ;
			      				break;
			      			case 1:
			      				//dataOut = (ap_int<8>)((*dataptr).range(15,8)) ;
			      				dataOut = (ap_int<8>)(*dataptr1) ;
			      				break;
			      			case 2:
			      				//dataOut = (ap_int<8>)((*dataptr).range( 23,16)) ;
			      				dataOut = (ap_int<8>)(*dataptr2) ;
			      				break;
			      			case 3:
			      				//dataOut = (ap_int<8>)((*dataptr).range( 31,24)) ;
			      				dataOut = (ap_int<8>)(*dataptr3) ;
			      				break;
			      		}
			          break;
			        case HALF:
			        	switch(addr.range(1,0)){
			        	case 2:
			        	case 3:
			      			//dataOut = (ap_int<16>)((*dataptr).range(31,16)) ;
			        		temp16 = ((*dataptr3) << 8) | (*dataptr2);
			      			dataOut = (ap_int<16>)(temp16) ;
			      			break;
			        	case 0:
			        	case 1:
			      			//dataOut = (ap_int<16>)((*dataptr).range( 15,0)) ;
			        		temp16 = ((*dataptr1) << 8) | (*dataptr0);
			      			dataOut = (ap_int<16>)(temp16) ;
			      			break;
			      		}
			          break;
			        case WORD:
			        	//dataOut = (*dataptr).range( 31,0);
		        		  dataOut.range(7,0)  =(*dataptr0) ;
		        		  dataOut.range(15,8) =(*dataptr1) ;
		        		  dataOut.range(23,16)=(*dataptr2) ;
		        		  dataOut.range(31,24)=(*dataptr3) ;
			          break;
			        case BYTE_U:
			      		switch(addr.range(1,0)){
			      			case 0:
			      				//dataOut = (ap_uint<8>)((*dataptr).range( 7,0)) ;
			      				dataOut = (ap_uint<8>)(*dataptr0) ;
			      				break;
			      			case 1:
			      				//dataOut = (ap_uint<8>)((*dataptr).range(15,8)) ;
			      				dataOut = (ap_uint<8>)(*dataptr1) ;
			      				break;
			      			case 2:
			      				//dataOut = (ap_uint<8>)((*dataptr).range( 23,16)) ;
			      				dataOut = (ap_uint<8>)(*dataptr2) ;
			      				break;
			      			case 3:
			      				//dataOut = (ap_uint<8>)((*dataptr).range( 31,24)) ;
			      				dataOut = (ap_uint<8>)(*dataptr3) ;
			      				break;
			      		}
			          break;
			        case HALF_U:
			        	switch(addr.range(1,0)){
			        	case 2:
			        	case 3:
			      			//dataOut = (ap_uint<16>)((*dataptr).range(31,16)) ;
			        		temp16 = ((*dataptr3) << 8) | (*dataptr2);
			      			dataOut = (ap_uint<16>)(temp16) ;
			      			break;
			        	case 0:
			        	case 1:
			      			//dataOut = (ap_uint<16>)((*dataptr).range( 15,0)) ;
			        		temp16 = ((*dataptr1) << 8) | (*dataptr0);
			      			dataOut = (ap_uint<16>)(temp16) ;
			      			break;
			      		}
			          break;
			        default:
			        	break;
			      }

			      break;
				    case NONE:
						//dataOut = *dataptr;

				    	//dataOut =dataIn;
			      		break;
			  }

//return dataOut;
		}
#ifdef parallel_mode
template<int i> //ap_uint<32> void
void getset0(ap_uint<32> addr, memMask mask, memOpType opType, ap_int<32> dataIn , ap_uint<32>& dataOut  ){// , ap_uint<32>& dataOut  ap_int<32>*memory,  ap_uint<32>& dataOut
			#pragma  HLS inline off
#pragma HLS PIPELINE
#pragma HLS latency max=0
#pragma HLS resource variable=mem_par core=RAM_2P_1S


	const int adress =  (ap_uint<32>)  addr.range(31,2);

		ap_int<8>* dataptr0 = &(mem_par[i][adress][0]);
		ap_int<8>* dataptr1 = &(mem_par[i][adress][1]);
		ap_int<8>* dataptr2 = &(mem_par[i][adress][2]);
		ap_int<8>* dataptr3 = &(mem_par[i][adress][3]);


			ap_uint<32> datamid ; //= (*dataptr).range( 31,0);
			 ap_int<16> temp16;
			 ap_int<32> loaddata;
			switch(opType) {

			    case ST:

			    	//(* dataptr) =dataIn ;
			      switch(mask) {
			        case BYTE:
			      		switch(addr.range(1,0)){
			      			case 0:
			      				//(*dataptr).range( 7,0) = (ap_int<8>)dataIn.range(7,0);
			      				(*dataptr0) = (ap_int<8>)dataIn.range(7,0);
			      				break;
			      			case 1:
			      				//(*dataptr).range(15,8) = (ap_int<8>)dataIn.range(7,0);
			      				(*dataptr1) = (ap_int<8>)dataIn.range(7,0);
			      				break;
			      			case 2:
			      				//(*dataptr).range( 23,16)  =(ap_int<8>) dataIn.range(7,0);
			      				(*dataptr2)  =(ap_int<8>) dataIn.range(7,0);
			      				break;
			      			case 3:
			      				//(*dataptr).range( 31,24) =(ap_int<8>) dataIn.range(7,0);
			      				(*dataptr2) =(ap_int<8>) dataIn.range(7,0);
			      				break;
			      		}
			          break;
			        case HALF:
			        	switch(addr.range(1,0)){
			        	case 2:
			        	case 3:
			        		//(*dataptr).range( 31,16) =(ap_int<16>) dataIn.range(15,0);
			        		(*dataptr2) =(ap_int<8>) dataIn.range(7,0);
			        		(*dataptr3) =(ap_int<8>) dataIn.range(15,8);
			      			break;
			        	case 0:
			        	case 1:
			        		//(*dataptr).range(15,0) = (ap_int<16>)dataIn.range(15,0);
			        		(*dataptr0) =(ap_int<8>) dataIn.range(7,0);
			        		(*dataptr1) =(ap_int<8>) dataIn.range(15,8);
			      			break;
			      		}
			          break;
			        case WORD:
			        	//(*dataptr).range( 31,0)= dataIn.range( 31,0);
		        		(*dataptr0) =(ap_int<8>) dataIn.range(7,0);
		        		(*dataptr1) =(ap_int<8>) dataIn.range(15,8);
		        		(*dataptr2) =(ap_int<8>) dataIn.range(23,16);
		        		(*dataptr3) =(ap_int<8>) dataIn.range(31,24);
			          break;
			        default:
			        break;
			      }

			      break;
			    case LD:
					//dataOut.range( 31,0) = (*dataptr).range( 31,0);
			    //	dataOut  = (*dataptr).range( 31,0);
			    	//dataOut = *dataptr;
			    	//loaddata = *dataptr;
			      switch(mask) {
			      	  case BYTE: //|
			      		switch(addr.range(1,0)){
			      			case 0:
			      				//dataOut = (ap_int<8>)((*dataptr).range( 7,0)) ;
			      				dataOut = (ap_int<8>)(*dataptr0) ;
			      				break;
			      			case 1:
			      				//dataOut = (ap_int<8>)((*dataptr).range(15,8)) ;
			      				dataOut = (ap_int<8>)(*dataptr1) ;
			      				break;
			      			case 2:
			      				//dataOut = (ap_int<8>)((*dataptr).range( 23,16)) ;
			      				dataOut = (ap_int<8>)(*dataptr2) ;
			      				break;
			      			case 3:
			      				//dataOut = (ap_int<8>)((*dataptr).range( 31,24)) ;
			      				dataOut = (ap_int<8>)(*dataptr3) ;
			      				break;
			      		}
			          break;
			        case HALF:
			        	switch(addr.range(1,0)){
			        	case 2:
			        	case 3:
			      			//dataOut = (ap_int<16>)((*dataptr).range(31,16)) ;
			        		temp16 = ((*dataptr3) << 8) | (*dataptr2);
			      			dataOut = (ap_int<16>)(temp16) ;
			      			break;
			        	case 0:
			        	case 1:
			      			//dataOut = (ap_int<16>)((*dataptr).range( 15,0)) ;
			        		temp16 = ((*dataptr1) << 8) | (*dataptr0);
			      			dataOut = (ap_int<16>)(temp16) ;
			      			break;
			      		}
			          break;
			        case WORD:
			        	//dataOut = (*dataptr).range( 31,0);
		        		  dataOut.range(7,0)  =(*dataptr0) ;
		        		  dataOut.range(15,8) =(*dataptr1) ;
		        		  dataOut.range(23,16)=(*dataptr2) ;
		        		  dataOut.range(31,24)=(*dataptr3) ;
			          break;
			        case BYTE_U:
			      		switch(addr.range(1,0)){
			      			case 0:
			      				//dataOut = (ap_uint<8>)((*dataptr).range( 7,0)) ;
			      				dataOut = (ap_uint<8>)(*dataptr0) ;
			      				break;
			      			case 1:
			      				//dataOut = (ap_uint<8>)((*dataptr).range(15,8)) ;
			      				dataOut = (ap_uint<8>)(*dataptr1) ;
			      				break;
			      			case 2:
			      				//dataOut = (ap_uint<8>)((*dataptr).range( 23,16)) ;
			      				dataOut = (ap_uint<8>)(*dataptr2) ;
			      				break;
			      			case 3:
			      				//dataOut = (ap_uint<8>)((*dataptr).range( 31,24)) ;
			      				dataOut = (ap_uint<8>)(*dataptr3) ;
			      				break;
			      		}
			          break;
			        case HALF_U:
			        	switch(addr.range(1,0)){
			        	case 2:
			        	case 3:
			      			//dataOut = (ap_uint<16>)((*dataptr).range(31,16)) ;
			        		temp16 = ((*dataptr3) << 8) | (*dataptr2);
			      			dataOut = (ap_uint<16>)(temp16) ;
			      			break;
			        	case 0:
			        	case 1:
			      			//dataOut = (ap_uint<16>)((*dataptr).range( 15,0)) ;
			        		temp16 = ((*dataptr1) << 8) | (*dataptr0);
			      			dataOut = (ap_uint<16>)(temp16) ;
			      			break;
			      		}
			          break;
			        default:
			        	break;
			      }

			      break;
				    case NONE:
						//dataOut = *dataptr;

				    	//dataOut =dataIn;
			      		break;
			  }
//return dataOut;
		}
#endif



void callmem(ap_uint<32> address[par_num+1], memMask mask, memOpType OpType, ap_int<32> valueToWrite[par_num] ,ap_uint<32> result[par_num+1], bool addr_arr[par_num+1],bool isparmode_or_addr_arr[par_num+1], Executeregs executereg,volatile data_type * mem_extra,bool &cache_accessfailed,cache_address_s		&cache_addr_1){
//#pragma HLS dataflow

#pragma  HLS inline
#pragma HLS PIPELINE
#pragma HLS latency min=0 max=0
#pragma HLS INTERFACE ap_stable port=address
#pragma HLS INTERFACE ap_stable port=mask
#pragma HLS INTERFACE ap_stable port=OpType
#pragma HLS INTERFACE ap_stable port=valueToWrite
#pragma HLS INTERFACE ap_stable port=result
//(ap_int<8>*)mem_par[0],

#ifdef parallel_mode
	GETSET_MACRO
	getset_mem(address[par_num-1], mask, (isparmode_or_addr_arr[par_num-1])?OpType:NONE, valueToWrite[(par_num-1)], result[(par_num-1)] );
#else
	getset_mem(address[par_num-1], mask, OpType, valueToWrite[(par_num-1)], result[(par_num-1)] );
#endif
//	riscv_cache_access(address[par_num] , valueToWrite[(par_num-1)] ,  (rf_addr == (memorybits)  (par_num))?OpType:NONE,mem_extra,executereg.funct3,result[(par_num)]);

#ifdef activatecache
	riscv_cache_access(address[par_num], valueToWrite[(par_num-1)] ,   (addr_arr[par_num])?OpType:NONE,executereg.funct3,result[par_num-1], cache_accessfailed,mem_extra,cache_addr_1);
#endif
	//	riscv_cache_access_failed(address[par_num], valueToWrite[(par_num-1)] ,   (rf_addr == (memorybits)  (par_num))?OpType:NONE,mem_extra,executereg.funct3,result[par_num-1], cache_accessfailed);



}


/*
 *
 *
 *
 */

void memorycall(struct Memoryregs &memoryreg,struct Executeregs executereg,volatile data_type * mem_extra){
#pragma HLS inline off
#pragma HLS PIPELINE

	ap_uint<1> 		cpu_mem_request_r_w;
	ap_uint<3> 		cpu_mem_request_funct3;
	ap_uint<32> 	cpu_mem_request_address;
	ap_uint<32> 	cpu_mem_request_write_data;
	ap_uint<2>		last_bits;
	data_type 		read_data;
	unsigned int	inner_addr;

	 Memoryregs temp_memory;
	memory(temp_memory,executereg);

	callmem( temp_memory.address, temp_memory.mask, temp_memory.OpType, temp_memory.valueToWrite ,temp_memory.result,  temp_memory.addr_arr, temp_memory.isparmode_or_addr_arr, executereg,mem_extra,temp_memory.cacheaccessfailed,temp_memory.cache_addr_1);


	memoryreg = temp_memory;
return;
}

void fetchcall(struct FetchRegs &fetchreg, ap_uint<32> pc, int parloopcount){
#pragma HLS inline off

	fetchreg.nextPC=   pc+ 4;
	fetchreg.pc=   pc;


#ifdef parallel_mode
	fetchreg.isparmode = ( (pc<= Par_end_addr[parloopcount]) && (pc>=Par_start_addr[parloopcount]));

	if(pc==Par_end_addr[parloopcount] && parloopcount!=(number_of_parloops))
		fetchreg.parloopcount = parloopcount+1;
	else
		fetchreg.parloopcount = parloopcount;

#else
	fetchreg.isparmode = false;
#endif
	 ap_uint<32> temp_instruction;
	 getsetint(pc, WORD, LD, 0, (ap_int<32>*)inst_mem, temp_instruction );
	 fetchreg.instruction1 = temp_instruction;
	 fetchreg.instruction2 = temp_instruction;
	 fetchreg.isopcodeBranch = (temp_instruction.range(6,0) ==  RISCV_BR);
}

template <class myType>
void setCore (myType &a, myType b,myType c, bool isflush, bool isstall) {
	#pragma HLS inline off
	#pragma HLS PIPELINE
	if(!isstall){
		if(isflush){
			   a =c ;

		}else{
			   a =b ;

		}
	}
}

static const struct FetchRegs EmptyFetch = {}; // {0,0,0};
static const struct Decoderegs EmptyDecode= {}; // {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static const struct Executeregs EmptyExecute= {};
static const struct Memoryregs EmptyMemory= {};
bool pcorcond ;
static uint32_t rf[par_num][32];

void stallCore (Core core, bool &stall,int &instcounter) {
#pragma HLS inline off

	ap_int<32> instruction = core.fetchreg.instruction2;
	ap_uint<5> rs2 = instruction.range(24,20);
	ap_uint<5> rs1 =  instruction.range(19,15);
	ap_uint<7> opCode = instruction.range(6,0);

	bool executereguseRd = core.decodereg.useRd;
	ap_uint<6> executeregrd    = core.decodereg.rd;

	bool memoryreguseRd  =  core.executereg.useRd;
	ap_uint<6> memoryregrd     =  core.executereg.rd;

	bool writebackreguseRd = core.memoryreg.useRd;
	ap_uint<6> writebackregrd = core.memoryreg.rd;

	bool useRs1 = 0;
	bool useRs2 = 0;

	switch (opCode)
	{

	case RISCV_LUI:
		useRs1 = 0;
		useRs2 = 0;
		break;

	case RISCV_AUIPC:
		useRs1 = 0;
		useRs2 = 0;
		break;

	case RISCV_JAL:
		useRs1 = 0;
		useRs2 = 0;

		break;

	case RISCV_JALR:
		useRs1 = 1;
		useRs2 = 0;

		break;

	case RISCV_BR:
		useRs1 = 1;
		useRs2 = 1;

		break;

	case RISCV_LD:
		useRs1 = 1;
		useRs2 = 0;
		break;

	case RISCV_ST:
		useRs1 = 1;
		useRs2 = 1;
		break;

	case RISCV_OPI:
		useRs1 = 1;
		useRs2 = 0;
		break;

	case RISCV_OP:

		useRs1 = 1;
		useRs2 = 1;
		break;

	default:
		break;
	}

	bool decodestall_rs1 = useRs1 && ( ((rs1==executeregrd) && executereguseRd)  ||
												 ((rs1==memoryregrd) && memoryreguseRd)     ||
												 ((rs1==writebackregrd) && writebackreguseRd)
																									);
	bool decodestall_rs2 = useRs2 && ( ((rs2==executeregrd) && executereguseRd)  ||
												 ((rs2==memoryregrd) && memoryreguseRd)     ||
												 ((rs2==writebackregrd) && writebackreguseRd)
																									);
	bool decodestall = decodestall_rs1 || decodestall_rs2;


	stall = decodestall;

	if(core.stall){
		instcounter = core.instcounter;

	}else {
		if(core.decodereg.isBranch){
			instcounter = core.instcounter ;

		}else{
			instcounter = core.instcounter +1;

		}
	}

}


void setCore (Core &core, FetchRegs fetchreg,  Executeregs executereg , Memoryregs memoryreg, Decoderegs decodereg, Writebackregs writebackreg,bool stall,int  instcounter,ap_uint<32> Branchpc, bool prevbranchraken,ap_uint<32> cachefailresult) {
#pragma HLS inline off

	setCore(core.memoryreg , memoryreg,EmptyMemory, false, false);
#ifdef activatecache
	core.memoryreg.result[par_num-1] = memoryreg.cacheaccessfailed ? cachefailresult : memoryreg.result[par_num-1];
#endif
//	setCore(core.fetchreg ,fetchreg , EmptyFetch, decodereg.isBranch || executereg.isBranch, decodestall);
	setCore(core.fetchreg ,fetchreg , EmptyFetch, decodereg.isopcodeBranchorJump && (decodereg.ismispredict || (decodereg.isBranch != prevbranchraken)) , stall);
//	setCore(core.decodereg , decodereg,EmptyDecode, executereg.isBranch || executeflush, false);
	setCore(core.decodereg , decodereg,EmptyDecode, stall, false);
//	branchUnit(fetchreg.nextPC, decodereg.nextPCDC, decodereg.isBranch, executereg.nextPC, executereg.isBranch,core.pc, decodestall);
	branchUnit(fetchreg.nextPC, decodereg.nextPCDC, decodereg.isBranch, instcounter, core.pc, stall, core.stall , core.instcounter, fetchreg.isopcodeBranch,Branchpc, core.Branchpc, prevbranchraken, core.prevbranchraken ,decodereg.isopcodeBranch  , decodereg.ismispredict, decodereg.nextPCMispredict, decodereg.isopcodeBranchorJump );
	core.instcounter = instcounter;
	core.stall = stall;
	core.parloopcount=fetchreg.parloopcount;

}

void executeCore (Core core, FetchRegs &fetchreg,  Executeregs &executereg , Memoryregs &memoryreg, Decoderegs &decodereg, Writebackregs &writebackreg, bool &stall, int & instcounter, volatile data_type * mem_extra ,ap_uint<32> &Branchpc, bool &prevbranchraken) {
#pragma HLS inline off
#pragma HLS latency min=0 max=0
#pragma HLS Pipeline

	 fetchcall(fetchreg,core.pc,core.parloopcount);
	 //execute(executereg,core.decodereg); //8.51 mul 2.5 mux
	 decwb(decodereg,writebackreg,core.fetchreg,core.memoryreg,rf,core.prevbranchraken, core.Branchpc);


	stallCore(core,  stall, instcounter );

	Branchpc = core.Branchpc;
	prevbranchraken = core.prevbranchraken;

	memorycall(memoryreg, core.executereg,mem_extra);

}


void myrun (volatile data_type mem_extra[extramem_size] ) {
#pragma HLS INTERFACE ap_memory depth=32 port=mem_extra

	 Core core  ;
	 core.pc =0;
	 core.fetchreg.pc = 0;
	 core.fetchreg.nextPC = 0;
	 core.fetchreg.instruction2 = 0;
	 core.fetchreg.parloopcount = 0;

	 core.decodereg.rf_addr = 0;
	 core.decodereg.useRd = 0;
	 core.decodereg.rd = 0;
	 core.decodereg.opCode = 0;
	 core.decodereg.parloopcount = 0;

	 core.executereg.useRd = 0;
	 core.executereg.opCode = 0;
	 core.executereg.parloopcount = 0;
	 core.executereg.pc = 0;

	 core.memoryreg.useRd = 0;
	 core.memoryreg.rd = 0;
	 core.memoryreg.isstartparmode = 0;
	 core.memoryreg.isparmode = false;
	 core.memoryreg.parloopcount = 0;
	 core.stall = false;
	 core.instcounter =0;
	 core.Branchpc = false;
	 core.prevbranchraken = false;

	 core.parloopcount = 0;

	 while(true) {
#ifndef activatecache
#pragma HLS latency max=0 min=0
#else
#pragma HLS latency max=1
#endif
	 fprintf(myfile,"%x:\t%08x " ,(uint32_t)core.pc , (uint32_t)inst_mem[core.pc/4]);
	//fprintf(myfile," ra = %d  sp = %d gp = %d tp = %d t0 = % d , t1 = % d , t2 = % d , s0 = % d, s1 = % d, a0 = % d, a1 = % d, a2 = % d, a3 = % d, a4 = % d, a5 = % d, a6 = % d, a7 = % d, s2 = % d, s3 = % d, s4 = % d, s5 = % d, s6 = % d, s7 = % d, s8 = % d, s9 = % d, sA = % d, sB = % d, t3 = % d, t4 = % d, t5 = % d, t6 = % d\n" ,rf[0][1] , rf[0][2] ,rf[0][3],rf[0][4],rf[0][5],rf[0][6],rf[0][7],rf[0][8],rf[0][9],rf[0][10],rf[0][11],rf[0][12],rf[0][13] ,rf[0][14] ,rf[0][15],rf[0][16],rf[0][17],rf[0][18],rf[0][19],rf[0][20],rf[0][21],rf[0][22],rf[0][23],rf[0][24],rf[0][25],rf[0][26],rf[0][27],rf[0][28],rf[0][29],rf[0][30],rf[0][31]  );
	fprintf(myfile," ra = %d  sp = %d gp = %d tp = %d t0 = % d , t1 = % d , t2 = % d , s0 = % d, s1 = % d, a0 = % d, a1 = % d, a2 = % d, a3 = % d, a4 = % d, a5 = % d, a6 = % d, a7 = % d, s2 = % d, s3 = % d, s4 = % d, s5 = % d, s6 = % d, s7 = % d, s8 = % d, s9 = % d, sA = % d, sB = % d, t3 = % d, t4 = % d, t5 = % d, t6 = % d\n" ,rf[par_num-1][1] , rf[par_num-1][2] ,rf[par_num-1][3],rf[par_num-1][4],rf[par_num-1][5],rf[par_num-1][6],rf[par_num-1][7],rf[par_num-1][8],rf[par_num-1][9],rf[par_num-1][10],rf[par_num-1][11],rf[par_num-1][12],rf[par_num-1][13] ,rf[par_num-1][14] ,rf[par_num-1][15],rf[par_num-1][16],rf[par_num-1][17],rf[par_num-1][18],rf[par_num-1][19],rf[par_num-1][20],rf[par_num-1][21],rf[par_num-1][22],rf[par_num-1][23],rf[par_num-1][24],rf[par_num-1][25],rf[par_num-1][26],rf[par_num-1][27],rf[par_num-1][28],rf[par_num-1][29],rf[par_num-1][30],rf[par_num-1][31]  );

	printf("%x: %08x \n" , (uint32_t)core.pc, (uint32_t)inst_mem[core.pc/4]);

	struct  FetchRegs fetchreg ;
	struct Executeregs executereg;
	struct Memoryregs memoryreg;
	struct Decoderegs decodereg;
	struct Writebackregs writebackreg;
	ap_uint<32> cachefailresult;
	bool stall ;
	int instcounter;
	ap_uint<32> Branchpc;
	bool prevbranchraken;

#ifdef parallel_mode
	#pragma HLS ARRAY_PARTITION variable=mem_par complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=mem_par complete  dim=3
	#pragma HLS ARRAY_PARTITION variable=for_init complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=for_end complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=for_delta complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=Par_num_from_c complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=start_reg complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=end_reg complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=Par_start_addr complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=Par_end_addr complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=init_simd_offset complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=final_end_core complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=final_start_core complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=for_array_init complete
	#pragma HLS ARRAY_PARTITION variable=for_array_parstart complete
	#pragma HLS ARRAY_PARTITION variable=for_array_end complete
	#pragma HLS ARRAY_PARTITION variable=final_Par_num complete
#endif


	#pragma HLS ARRAY_PARTITION variable=mem complete  dim=2

	#pragma HLS ARRAY_PARTITION variable=core.decodereg.isactive complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=core.executereg.result complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=core.executereg.address complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=core.executereg.isactive complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=core.memoryreg.result complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=core.memoryreg.valueToWrite complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=core.memoryreg.address complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=core.memoryreg.addr_arr complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=core.memoryreg.isparmode_or_addr_arr complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=core.memoryreg.isactive complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=core.memoryreg.forarrayparstart complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=core.memoryreg.forarrayend complete  dim=1

	#pragma HLS ARRAY_PARTITION variable=decodereg.isactive complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=executereg.result complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=executereg.address complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=executereg.isactive complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=memoryreg.result complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=memoryreg.valueToWrite complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=memoryreg.address complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=memoryreg.addr_arr complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=memoryreg.isparmode_or_addr_arr complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=memoryreg.isactive complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=memoryreg.forarrayparstart complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=memoryreg.forarrayend complete  dim=1

	#pragma HLS dependence variable=core inter false
	#pragma HLS dependence variable=fetchreg inter false
	#pragma HLS dependence variable=executereg inter false
	#pragma HLS dependence variable=memoryreg inter false
	#pragma HLS dependence variable=decodereg inter false
	#pragma HLS dependence variable=writebackreg inter false
	#pragma HLS dependence variable=stall inter false

	#pragma HLS dependence variable=core intra false
	#pragma HLS dependence variable=fetchreg intra false
	#pragma HLS dependence variable=executereg intra false
	#pragma HLS dependence variable=memoryreg intra false
	#pragma HLS dependence variable=decodereg intra false
	#pragma HLS dependence variable=writebackreg intra false
	#pragma HLS dependence variable=stall intra false


	#pragma HLS resource variable=rf core=RAM_2P_1S
	#pragma HLS resource variable=inst_mem core=ROM_1P_1S
#pragma HLS ARRAY_PARTITION variable=rf complete  dim=1



  /*  if (core.pc == 0x1e4) {
		 		 int x = 1;
		 		// return  false  ;
		 	}*/

	//if (core.executereg.rf_addr != par_num){



	executeCore (core, fetchreg,   executereg ,  memoryreg,  decodereg,writebackreg, stall, instcounter,mem_extra,Branchpc, prevbranchraken);
	execute(executereg,core.decodereg);

#ifdef activatecache
	if(memoryreg.cacheaccessfailed){
		cachefailresult = riscv_cache_access_failed(memoryreg.address[par_num], memoryreg.valueToWrite[(par_num-1)] ,   (memoryreg.addr_arr[par_num])?memoryreg.OpType:NONE,mem_extra,executereg.funct3, memoryreg.cacheaccessfailed,		memoryreg.cache_addr_1);
	}else {
		cachefailresult =0 ;
	}
#else
	cachefailresult =0 ;
#endif


	if(executereg.boolval){
		setCore(core.executereg , executereg,EmptyExecute, false, false);
		setCore (core, fetchreg,   executereg ,  memoryreg,  decodereg,writebackreg,stall, instcounter,Branchpc, prevbranchraken,cachefailresult) ;
	}else {
		break;
	}


//8.51 mul 2.5 mux




	};
		fprintf(myfile,"instruction number = %d\n " ,core.instcounter );
}




