//Top block for RISCV design
#include <stdint.h>
#include <stdio.h>
//#include "datapath.hpp"
#include "new_define_datapath2.h"

//ap_uint<32> PC;

FILE *myfile;
FILE *myfile_c;
FILE *myfileinst;

bool loopflag = 0;
bool endflag = 0;

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

			const int adress =  (ap_uint<32>)  addr.range(31,2);			//ap_int<32>* dataptr = &mem_par[0][adress];;
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

void fetchcall(struct FetchRegs &fetchreg, ap_uint<32> pc){
#pragma HLS inline off

	fetchreg.nextPC=   pc+ 4;
	fetchreg.pc=   pc;


#ifdef parallel_mode
	fetchreg.isparmode = ( (pc<= Par_end_addr) && (pc>=Par_start_addr));
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
}

void executeCore (Core core, FetchRegs &fetchreg,  Executeregs &executereg , Memoryregs &memoryreg, Decoderegs &decodereg, Writebackregs &writebackreg, bool &stall, int & instcounter, volatile data_type * mem_extra ,ap_uint<32> &Branchpc, bool &prevbranchraken) {
#pragma HLS inline off
#pragma HLS latency min=0 max=0
#pragma HLS Pipeline

	 fetchcall(fetchreg,core.pc);
	 //execute(executereg,core.decodereg); //8.51 mul 2.5 mux
	 decwb(decodereg,writebackreg,core.fetchreg,core.memoryreg,rf,core.prevbranchraken, core.Branchpc);


	stallCore(core,  stall, instcounter );

	Branchpc = core.Branchpc;
	prevbranchraken = core.prevbranchraken;

	memorycall(memoryreg, core.executereg,mem_extra);

}

ap_uint<32> load_word(ap_uint<32> adress){
	ap_uint<32> dataOut;
	ap_uint<32> new_addr = adress-mem_start_adress;
	getset_mem(new_addr, WORD, LD, 0, dataOut);
	return dataOut;
}

void myrun (volatile data_type mem_extra[extramem_size] ) {
#pragma HLS INTERFACE ap_memory depth=32 port=mem_extra

	 Core core  ;
	 core.pc =0;
	 core.fetchreg.pc = 0;
	 core.fetchreg.nextPC = 0;
	 core.fetchreg.instruction2 = 0;

	 core.decodereg.rf_addr = 0;
	 core.decodereg.useRd = 0;
	 core.decodereg.rd = 0;
	 core.decodereg.opCode = 0;

	 core.executereg.useRd = 0;
	 core.executereg.opCode = 0;

	 core.memoryreg.useRd = 0;
	 core.memoryreg.rd = 0;
	 core.stall = false;
	 core.instcounter =0;
	 core.Branchpc = false;
	 core.prevbranchraken = false;

		//ap_uint<5> start_reg ;
		//ap_uint<5> end_reg;
		ap_uint<32> branch_start_pc;
		ap_uint<32> branch_end_pc;
		ap_uint<32> par_start_change_pc;
		ap_uint<32> initial_par_start_change_pc;
		bool changed_register_Arr[32] = {};
		bool isbranchexists = false;


		//header variables
		  int for_init [50]  ; //start address value (start_reg volue in first branch-mem start)
		  int for_end [50]  ;//end address value (end_regin  value -mem start)
		  int Par_num_from_c [50] ;//par_num value in the c code
		  int start_reg [50]  ;//start val register (is incremented)
		  int end_reg [50]  ;//end val register (constant)
		  int Par_start_addr[50] ; //pc value of the par loop start  branched address
		  int Par_end_addr [50] ; //pc value of the par loop end branch instruction address
		  int init_simd_offset [50]  ; //pc value offset between brach and Par_start_addr which start or end reg is written
		  int start_core [50] ; //cores starts at 0, if its i take i+Par_num_from_c-1  cores
		  int end_core [50] ;
		  int number_of_parloops =0;

	 while(true) {
#ifndef activatecache
#pragma HLS latency min=0 max=0
#else
#pragma HLS latency max=1
#endif
	//fprintf(myfile," ra = %d  sp = %d gp = %d tp = %d t0 = % d , t1 = % d , t2 = % d , s0 = % d, s1 = % d, a0 = % d, a1 = % d, a2 = % d, a3 = % d, a4 = % d, a5 = % d, a6 = % d, a7 = % d, s2 = % d, s3 = % d, s4 = % d, s5 = % d, s6 = % d, s7 = % d, s8 = % d, s9 = % d, sA = % d, sB = % d, t3 = % d, t4 = % d, t5 = % d, t6 = % d\n" ,rf[0][1] , rf[0][2] ,rf[0][3],rf[0][4],rf[0][5],rf[0][6],rf[0][7],rf[0][8],rf[0][9],rf[0][10],rf[0][11],rf[0][12],rf[0][13] ,rf[0][14] ,rf[0][15],rf[0][16],rf[0][17],rf[0][18],rf[0][19],rf[0][20],rf[0][21],rf[0][22],rf[0][23],rf[0][24],rf[0][25],rf[0][26],rf[0][27],rf[0][28],rf[0][29],rf[0][30],rf[0][31]  );

		 fprintf(myfileinst,"%x:\t%08x " ,(uint32_t)core.pc , (uint32_t)inst_mem[core.pc/4]);
		//fprintf(myfile," ra = %d  sp = %d gp = %d tp = %d t0 = % d , t1 = % d , t2 = % d , s0 = % d, s1 = % d, a0 = % d, a1 = % d, a2 = % d, a3 = % d, a4 = % d, a5 = % d, a6 = % d, a7 = % d, s2 = % d, s3 = % d, s4 = % d, s5 = % d, s6 = % d, s7 = % d, s8 = % d, s9 = % d, sA = % d, sB = % d, t3 = % d, t4 = % d, t5 = % d, t6 = % d\n" ,rf[0][1] , rf[0][2] ,rf[0][3],rf[0][4],rf[0][5],rf[0][6],rf[0][7],rf[0][8],rf[0][9],rf[0][10],rf[0][11],rf[0][12],rf[0][13] ,rf[0][14] ,rf[0][15],rf[0][16],rf[0][17],rf[0][18],rf[0][19],rf[0][20],rf[0][21],rf[0][22],rf[0][23],rf[0][24],rf[0][25],rf[0][26],rf[0][27],rf[0][28],rf[0][29],rf[0][30],rf[0][31]  );
		fprintf(myfileinst," ra = %d  sp = %d gp = %d tp = %d t0 = % d , t1 = % d , t2 = % d , s0 = % d, s1 = % d, a0 = % d, a1 = % d, a2 = % d, a3 = % d, a4 = % d, a5 = % d, a6 = % d, a7 = % d, s2 = % d, s3 = % d, s4 = % d, s5 = % d, s6 = % d, s7 = % d, s8 = % d, s9 = % d, sA = % d, sB = % d, t3 = % d, t4 = % d, t5 = % d, t6 = % d\n" ,rf[par_num-1][1] , rf[par_num-1][2] ,rf[par_num-1][3],rf[par_num-1][4],rf[par_num-1][5],rf[par_num-1][6],rf[par_num-1][7],rf[par_num-1][8],rf[par_num-1][9],rf[par_num-1][10],rf[par_num-1][11],rf[par_num-1][12],rf[par_num-1][13] ,rf[par_num-1][14] ,rf[par_num-1][15],rf[par_num-1][16],rf[par_num-1][17],rf[par_num-1][18],rf[par_num-1][19],rf[par_num-1][20],rf[par_num-1][21],rf[par_num-1][22],rf[par_num-1][23],rf[par_num-1][24],rf[par_num-1][25],rf[par_num-1][26],rf[par_num-1][27],rf[par_num-1][28],rf[par_num-1][29],rf[par_num-1][30],rf[par_num-1][31]  );


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
#endif

	#pragma HLS ARRAY_PARTITION variable=mem complete  dim=2

	#pragma HLS ARRAY_PARTITION variable=core.executereg.result complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=core.executereg.address complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=core.memoryreg.result complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=core.memoryreg.valueToWrite complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=core.memoryreg.address complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=core.memoryreg.addr_arr complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=core.memoryreg.isparmode_or_addr_arr complete  dim=1

	#pragma HLS ARRAY_PARTITION variable=executereg.result complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=executereg.address complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=memoryreg.result complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=memoryreg.valueToWrite complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=memoryreg.address complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=memoryreg.addr_arr complete  dim=1
	#pragma HLS ARRAY_PARTITION variable=memoryreg.isparmode_or_addr_arr complete  dim=1

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

	#pragma HLS ARRAY_PARTITION variable=for_array_init complete  dim=1

/*	if (core.pc == 0x160) {
		 		 int x = 1;
		 		// return  false  ;
		 	}*/

	//if (core.executereg.rf_addr != par_num){


	//execute pipelined core
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

//8.51 mul 2.5 mux

	if(executereg.boolval){
		setCore(core.executereg , executereg,EmptyExecute, false, false);
		setCore (core, fetchreg,   executereg ,  memoryreg,  decodereg,writebackreg,stall, instcounter,Branchpc, prevbranchraken,cachefailresult) ;
	}else {
		break;
	}


	//check until startpar=1
	if(load_word(par_start_address) == 1 && loopflag == 0 && endflag == 0){
		loopflag = 1;
		isbranchexists = false;
		//fprintf(myfile," //this header is generated by running the code once and simulating necessary values \n");
		//fprintf(myfile," #include \"define_instructions.h\" \n");
		//fprintf(myfile," //necessary values ");
		for_init[number_of_parloops] = (int)load_word(arr_str_address)-(int)mem_start_adress;
		for_end[number_of_parloops] =(int)load_word(arr_end_address)-(int)mem_start_adress;
		Par_num_from_c[number_of_parloops] =(int)load_word(par_num_address);
		//fprintf(myfile," \n const int for_init  = %d ; //start address value (start_reg volue in first branch-mem start)" ,(int)load_word(arr_str_address)-(int)mem_start_adress);
		//fprintf(myfile," \n const int for_end  = %d ;//end address value (end_regin  value -mem start)" ,(int)load_word(arr_end_address)-(int)mem_start_adress);
		//fprintf(myfile," \n const ap_int<32> Par_num_from_c  = %d ;//par_num value in the c code" ,(int)load_word(par_num_address));

		//fprintf(myfile_c," \n const int for_init  = %d ; //start address value (start_reg volue in first branch-mem start)" ,(int)load_word(arr_str_address)-(int)mem_start_adress);
		//fprintf(myfile_c," \n const int for_end  = %d ;//end address value (end_regin  value -mem start)" ,(int)load_word(arr_end_address)-(int)mem_start_adress);
		//fprintf(myfile_c," \n const int Par_num_from_c  = %d ;//par_num value in the c code" ,(int)load_word(par_num_address));

		par_start_change_pc = memoryreg.pc;
		initial_par_start_change_pc = memoryreg.pc;
	}

	//now we are inside the loop note changed register values
	if(load_word(par_start_address) == 1 && loopflag == 1 && endflag == 0 && core.memoryreg.pc>=(par_start_change_pc+0x20)){ //

		if( memoryreg.useRd){
			changed_register_Arr[memoryreg.rd] = true;
		}
	}

	//now compare changed register values to find which one is start and end
	// constant register will be the end
	if (memoryreg.isopcodeBranchorJump) { // bne is must be guaranteed
		isbranchexists = true;
		if(changed_register_Arr[memoryreg.rs2]){
			start_reg[number_of_parloops]=(int) memoryreg.rs2 ;
			end_reg[number_of_parloops]= (int)memoryreg.rs1 ;
		}else{
			start_reg[number_of_parloops]= (int)memoryreg.rs1 ;
			end_reg[number_of_parloops]= (int)memoryreg.rs2 ;
		}
		branch_start_pc = memoryreg.nextPCDC;
		branch_end_pc = memoryreg.pc;
	}

	//now wait for start par to be 0
	if(load_word(par_start_address) == 0 && loopflag == 1 && endflag == 0 ){

		if (isbranchexists){

		}else {

			printf("ERROR, THERE IS NO BRANCH TO PARALLELIZE!!");
			return ;

		}

		loopflag = 0;
		endflag = 1;

		//start_reg	[number_of_parloops]	= (int)start_reg;
		//end_reg		[number_of_parloops]	=(int)end_reg;
		Par_start_addr	[number_of_parloops]=(int)branch_start_pc;
		Par_end_addr[number_of_parloops]	=(int)branch_end_pc;

		//fprintf(myfile," \n const ap_uint<5> start_reg  = %d ;//start val register (is incremented)" ,(int)start_reg); //start val register
		//fprintf(myfile," \n const ap_uint<5> end_reg  = %d ;//end val register (constant)" ,(int)end_reg); //end val register
		//fprintf(myfile," \n  const ap_int<32> Par_start_addr  = 0x%x; //pc value of the par loop start  branched address" ,(int)branch_start_pc); //brach return value
		//fprintf(myfile," \n const ap_int<32> Par_end_addr  = 0x%x ; //pc value of the par loop end branch instruction address " ,(int)branch_end_pc); //one after branch

//now find rf write instruction between par_start_change_pc and Par_start_addr
//there should be at least one

	ap_int<32> inst_temp;
	ap_uint<5>  rd ;
	int value ;
	bool found = false;
		while(true){
			 inst_temp = inst_mem[par_start_change_pc/4];
			 rd = inst_temp.range(11, 7);
			 	if(rd == start_reg[number_of_parloops] || rd ==end_reg[number_of_parloops] ){
					found = true;
					value = (int)branch_start_pc-(int)par_start_change_pc;
					 par_start_change_pc = par_start_change_pc +4;
				}else{
				
				if(0x00000013 == inst_temp){//check nop instruction
					found = true;
					value = (int)branch_start_pc-(int)par_start_change_pc;
				}				
					par_start_change_pc = par_start_change_pc +4;
			 	}

			 	if(par_start_change_pc == branch_start_pc){
					if(found) {
						init_simd_offset[number_of_parloops] =value;
					//fprintf(myfile," \n const ap_int<32> init_simd_offset  = 0x%x ; //pc value offset between brach and Par_start_addr which start or end reg is written" ,value); //one after branch
					printf("SUCCESS\n");
					//now reset
					number_of_parloops = number_of_parloops +1;
					loopflag =0;
					endflag =0;
					break;
			 		//return;
						}else {
					printf("value is not a branch. Theory is wrong, there is no register set between PC = %x and %x abandon ship!!! \n",(uint32_t)initial_par_start_change_pc , (uint32_t)branch_start_pc  );
					
					printf ("Please add 'asm volatile (\"nop\");' between startpar = 1 and SIMD loop"); 
					
					return ;
						}
					
			 	}
		}


	}

	};


		fprintf(myfile," //this header is generated by running the code once and simulating necessary values \n");
		fprintf(myfile," #include \"define_instructions.h\" \n");
		fprintf(myfile," //necessary values ");

		//numer of parloops
		fprintf(myfile, "  \n const int number_of_parloops = %d ; // how many SIMD loops are in the algorithm", number_of_parloops);
		fprintf(myfile_c, "  \n const int number_of_parloops = %d ; // how many SIMD loops are in the algorithm", number_of_parloops);

		// start_core
		start_core[0] = 0;
		end_core[0] = Par_num_from_c [0]-1;

		for (int i=1; i <number_of_parloops ; i++){
			for (int j=0; j <i ; j++){
				if (for_init[i] == for_init[j] &&  for_end[i] == for_end[j] ) {
					start_core[i] = start_core[j];
					end_core[i] = end_core[j];
					break;
				}else {
					start_core[i] = start_core[i-1] + Par_num_from_c [i-1];
					end_core[i]   = start_core[i] + Par_num_from_c [i] -1 ;
				}

			}
		}
		fprintf(myfile," \n const int start_core [%d]  = { " ,number_of_parloops);
		for (int i=0; i <number_of_parloops ; i++)
			fprintf(myfile," %d," ,start_core[i]);
		fprintf(myfile," } ; //cores starts at 0, if its i take i+Par_num_from_c-1  cores)" );

		fprintf(myfile_c," \n int start_core [%d]  = { " ,number_of_parloops);
		for (int i=0; i <number_of_parloops ; i++)
			fprintf(myfile_c," %d," ,start_core[i]);
		fprintf(myfile_c," } ; //cores starts at 0, if its i take i+Par_num_from_c-1  cores)" );


		// end_core

		fprintf(myfile," \n const int end_core [%d]  = { " ,number_of_parloops);
		for (int i=0; i <number_of_parloops ; i++)
			fprintf(myfile," %d," ,end_core[i]);
		fprintf(myfile," } ; //cores starts at 0, if its i take i+Par_num_from_c-1  cores)" );

		fprintf(myfile_c," \n int end_core [%d]  = { " ,number_of_parloops);
		for (int i=0; i <number_of_parloops ; i++)
			fprintf(myfile_c," %d," ,end_core[i]);
		fprintf(myfile_c," } ; //cores starts at 0, if its i take i+Par_num_from_c-1  cores)" );



		//for init
		//fprintf(myfile," \n const int for_init  = %d ; //start address value (start_reg value in first branch-mem start)" ,(int)load_word(arr_str_address)-(int)mem_start_adress);
		fprintf(myfile," \n const int for_init [%d]  = { " ,number_of_parloops);
		for (int i=0; i <number_of_parloops ; i++)
			fprintf(myfile," %d," ,for_init[i]);
		fprintf(myfile," } ; //start address value (start_reg value in first branch-mem start)" );
		//fprintf(myfile_c," \n const int for_init  = %d ; //start address value (start_reg value in first branch-mem start)" ,(int)load_word(arr_str_address)-(int)mem_start_adress);
		fprintf(myfile_c," \n const int for_init [%d]  = { " ,number_of_parloops);
		for (int i=0; i <number_of_parloops ; i++)
			fprintf(myfile_c," %d," ,for_init[i]);
		fprintf(myfile_c," } ; //start address value (start_reg value in first branch-mem start)" );

		//for end
		//fprintf(myfile," \n const int for_end  = %d ;//end address value (end_reg  value -mem start)" ,(int)load_word(arr_end_address)-(int)mem_start_adress);
		fprintf(myfile," \n const int for_end [%d]  = { " ,number_of_parloops);
		for (int i=0; i <number_of_parloops ; i++)
			fprintf(myfile," %d," ,for_end[i]);
		fprintf(myfile," } ; //end address value (end_reg  value -mem start)" );
		//fprintf(myfile_c," \n const int for_end  = %d ;//end address value (end_reg  value -mem start)" ,(int)load_word(arr_end_address)-(int)mem_start_adress);
		fprintf(myfile_c," \n const int for_end [%d]  = { " ,number_of_parloops);
		for (int i=0; i <number_of_parloops ; i++)
			fprintf(myfile_c," %d," ,for_end[i]);
		fprintf(myfile_c," } ; //end address value (end_reg  value -mem start)" );

		//Par_num_from_c
		//fprintf(myfile," \n const ap_int<32> Par_num_from_c  = %d ;//par_num value in the c code" ,(int)load_word(par_num_address));
		fprintf(myfile," \n const ap_int<32> Par_num_from_c [%d]  = { " ,number_of_parloops);
		for (int i=0; i <number_of_parloops ; i++)
			fprintf(myfile," %d," ,Par_num_from_c[i]);
		fprintf(myfile," } ; //par_num value in the c code" );

		//fprintf(myfile_c," \n const int Par_num_from_c  = %d ;//par_num value in the c code" ,(int)load_word(par_num_address));
		fprintf(myfile_c," \n const int Par_num_from_c [%d]  = { " ,number_of_parloops);
		for (int i=0; i <number_of_parloops ; i++)
			fprintf(myfile_c," %d," ,Par_num_from_c[i]);
		fprintf(myfile_c," } ; //par_num value in the c code" );

		//start_reg
		//fprintf(myfile," \n const ap_uint<5> start_reg  = %d ;//start val register (is incremented)" ,(int)start_reg); //start val register
		fprintf(myfile," \n const ap_uint<5> start_reg [%d]  = { " ,number_of_parloops);
		for (int i=0; i <number_of_parloops ; i++)
			fprintf(myfile," %d," ,start_reg[i]);
		fprintf(myfile," } ; //start val register (is incremented)" );


		//end_reg
		//fprintf(myfile," \n const ap_uint<5> end_reg  = %d ;//end val register (constant)" ,(int)end_reg); //end val register
		fprintf(myfile," \n const ap_uint<5> end_reg [%d]  = { " ,number_of_parloops);
		for (int i=0; i <number_of_parloops ; i++)
			fprintf(myfile," %d," ,end_reg[i]);
		fprintf(myfile," } ; //end val register (constant)" );

		//Par_start_addr
		//fprintf(myfile," \n  const ap_int<32> Par_start_addr  = 0x%x; //pc value of the par loop start  branched address" ,(int)branch_start_pc); //brach return value
		fprintf(myfile," \n const ap_int<32> Par_start_addr [%d]  = { " ,number_of_parloops);
		for (int i=0; i <number_of_parloops ; i++)
			fprintf(myfile," 0x%x," ,Par_start_addr[i]);
		fprintf(myfile," } ; //pc value of the par loop start  branched address " );

		//Par_end_addr
		//fprintf(myfile," \n const ap_int<32> Par_end_addr  = 0x%x ; //pc value of the par loop end branch instruction address " ,(int)branch_end_pc); //one after branch
		fprintf(myfile," \n const ap_int<32> Par_end_addr [%d]  = { " ,number_of_parloops);
		for (int i=0; i <number_of_parloops ; i++)
			fprintf(myfile," 0x%x," ,Par_end_addr[i]);
		fprintf(myfile," } ;//pc value of the par loop end branch instruction address " );

		//init_simd_offset
		//fprintf(myfile," \n const ap_int<32> init_simd_offset  = 0x%x ; //pc value offset between brach and Par_start_addr which start or end reg is written" ,value); //one after branch
		fprintf(myfile," \n const ap_int<32> init_simd_offset [%d]  = { " ,number_of_parloops);
		for (int i=0; i <number_of_parloops ; i++)
			fprintf(myfile," 0x%x," ,init_simd_offset[i]);
		fprintf(myfile," } ;//pc value offset between brach and Par_start_addr which start or end reg is written " );


}




