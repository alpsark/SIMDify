
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "new_define_parallel.h"
#include "riscvISA.h"

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <math.h>

extern FILE *myfile;
extern FILE *myfile_c;
extern FILE *myfileinst;


/* ap_int built in methods
 * a.lenght
 * hi.concat(lo)
 * a.range(hi,lo)
 * .and_reduce
 * .or_reduce
 * .xor_reduce
 * .reverse
 * .test(bitno) (return true false)
 * set_bit (no)
 * get_bit (no)
 * .set / clear
 *  .invert(bitno)
 *  .rrotate. (N bits to right)
 *  .lrotate(N bits to left)
 *  .b_not (bitwise invert)
 * /*/

/*----------------------------------------------------------------
|  						MEMORY								   	 |
*----------------------------------------------------------------*/
typedef enum {
  BYTE = 0,
  HALF,
  WORD,
  BYTE_U,
  HALF_U
} memMask;


typedef enum {
  NONE = 0,
  ST,
  LD
} memOpType;




class Memory {

public:
	ap_int<32> *data;

	//initilize
	Memory(ap_int<32> *arg){
		  data = arg;
	  }

	void getset(ap_uint<32> addr, memMask mask, memOpType opType, ap_int<32> dataIn, ap_uint<32>& dataOut){
			#pragma  HLS inline
	//#pragma HLS latency  max=1

			const ap_uint<12> adress = addr.range(13,2);
			ap_int<32>* dataptr = &data[adress] ;

	    	//dataptr = &data[adress] ;
			 ap_int<32> loaddata;// = *dataptr;
			//ap_uint<32> dataouts;
			switch(opType) {
			    case ST:
			    	//(* dataptr) = &data[adress] ;
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
			      //dataouts = 0;
			      break;
			    case LD:
			    	//dataOut = *dataptr;
			    	loaddata = *dataptr;
			      switch(mask) {
			      	  case BYTE: //|
			      		switch(addr.range(1,0)){
			      			case 0:
			      				dataOut = (ap_int<8>)(loaddata.range( 7,0)) ;
			      				break;
			      			case 1:
			      				dataOut = (ap_int<8>)(loaddata.range(15,8)) ;
			      				break;
			      			case 2:
			      				dataOut = (ap_int<8>)(loaddata.range( 23,16)) ;
			      				break;
			      			case 3:
			      				dataOut = (ap_int<8>)(loaddata.range( 31,24)) ;
			      				break;
			      		}
			          break;
			        case HALF:
			        	switch(addr.range(1,0)){
			        	case 2:
			        	case 3:
			      			dataOut = (ap_int<16>)(loaddata.range(31,16)) ;
			      			break;
			        	case 0:
			        	case 1:
			      			dataOut = (ap_int<16>)(loaddata.range( 15,0)) ;
			      			break;
			      		}
			          break;
			        case WORD:
			        	dataOut = loaddata.range( 31,0);
			          break;
			        case BYTE_U:
			      		switch(addr.range(1,0)){
			      			case 0:
			      				dataOut = (ap_uint<8>)(loaddata.range( 7,0)) ;
			      				break;
			      			case 1:
			      				dataOut = (ap_uint<8>)(loaddata.range(15,8)) ;
			      				break;
			      			case 2:
			      				dataOut = (ap_uint<8>)(loaddata.range( 23,16)) ;
			      				break;
			      			case 3:
			      				dataOut = (ap_uint<8>)(loaddata.range( 31,24)) ;
			      				break;
			      		}
			          break;
			        case HALF_U:
			        	switch(addr.range(1,0)){
			        	case 2:
			        	case 3:
			      			dataOut = (ap_uint<16>)(loaddata.range(31,16)) ;
			      			break;
			        	case 0:
			        	case 1:
			      			dataOut = (ap_uint<16>)(loaddata.range( 15,0)) ;
			      			break;
			      		}
			          break;
			        default:
			        	break;
			      }

			      break;
				    case NONE:
				    	//dataOut =dataIn;
			      		break;
			  }
		}


};
/*----------------------------------------------------------------
|  						Fetch								   	 |
*----------------------------------------------------------------*/
struct FetchRegs
{

    ap_uint<32> pc;           	// PC where to fetch
    ap_uint<32> instruction1;  	// Instruction to execute
    ap_uint<32> instruction2;  	// Instruction to execute
    ap_uint<32> nextPC;      // Next pc according to fetch
    bool isparmode;
    bool isopcodeBranch;
};

void fetch(struct FetchRegs &FetchReg, ap_uint<32> instruction,ap_uint<32> PC );

/*----------------------------------------------------------------
|  						Decode								   	 |
*----------------------------------------------------------------*/
struct Decoderegs
{
    ap_uint<5> rs1;       // rs1    = instruction[19:15]
    ap_uint<5>  rs2;       // rs2    = instruction[24:20]
    ap_uint<5>  rd;        // rd     = instruction[11:7]


    ap_uint<32> pc;       // used for branch
    ap_uint<32> instruction;

    ap_uint<7> opCode;    // opCode = instruction[6:0]
    ap_uint<7> funct7;    // funct7 = instruction[31:25]
    ap_uint<3> funct3;    // funct3 = instruction[14:12]

    ap_int<32> lhs[par_num];   //  left hand side : operand 1
    ap_int<32> rhs[par_num];   // right hand side : operand 2
    ap_int<32> datac[par_num]; // ST, BR, JAL/R,


    //For branch unit
    ap_uint<32> nextPCDC;
    bool isBranch;

    //Information for forward/stall unit
    bool useRs1;
    bool useRs2;
    bool useRd;
    bool isparmode;
    ap_int<32>  rf_addr;
    bool isopcodeBranch;
    bool ismispredict;
    bool isopcodeBranchorJump ;
    ap_uint<32> nextPCMispredict ;
    bool 	isopcodeStoreorLoad ;
};

void decwb(struct Decoderegs &Decodereg,struct Writebackregs &Writebackreg,struct FetchRegs FetchReg,struct Memoryregs Memoryreg, uint32_t rf[par_num][32],bool prevbranchraken ,ap_uint<32> Branchpc );

/*----------------------------------------------------------------
|  						Execute								   	 |
*----------------------------------------------------------------*/
struct Executeregs
{


		//branch_end_pc = Decodereg.pc;
    bool isopcodeBranchorJump ;
    ap_uint<5> rs1;       // rs1    = instruction[19:15]
    ap_uint<5>  rs2;       // rs2    = instruction[24:20]
    ap_uint<32> nextPCDC;

    ap_uint<32> pc;
    ap_uint<32> instruction;

    ap_uint<6> rd;        // destination register
    bool useRd;
    bool isLongInstruction;
    ap_uint<7> opCode;    // LD or ST (can be reduced to 2 bits)
    ap_uint<3> funct3;    // datasize and sign extension bit

    ap_int<32> datac[par_num];     // data to be stored in memory or csr result

    //For branch unit
    ap_uint<32> nextPC;
    bool isBranch;
    //Register for all stages
    bool we;
    bool isparmode;
    ap_int<32> result[par_num];    // result of the EX stage
    ap_int<32> address[par_num+1];    // result of the EX stage
    memorybits  rf_addr;
     bool boolval;
     bool iscalloutsidemem;
     bool addr_arr[par_num+1];
     bool isparmode_or_addr_arr[par_num+1];
};


void execute(struct Executeregs &Executereg,struct Decoderegs Decodereg);

/*----------------------------------------------------------------
|  						Memory								   	 |
*----------------------------------------------------------------*/
struct Memoryregs
{
    ap_uint<6> rd;        // destination register
    bool useRd;
    ap_uint<32> pc;

    bool isopcodeBranchorJump ;
    ap_uint<5> rs1;       // rs1    = instruction[19:15]
    ap_uint<5>  rs2;       // rs2    = instruction[24:20]
    ap_uint<32> nextPCDC;

    bool isparmode;
    ap_uint<4> byteEnable;
    bool isStore;
    bool isLoad;
    memOpType OpType;
    memorybits  rf_addr;
    ap_uint<32> result[par_num+1];    // Result to be written back
    ap_uint<32> address[par_num+1];
    ap_int<32> valueToWrite[par_num];
    memMask mask;
    bool cacheaccessfailed;
    cache_address_s cache_addr_1;
    bool addr_arr[par_num+1];
    bool isparmode_or_addr_arr[par_num+1];
};


void memory(struct Memoryregs &Memoryreg,struct Executeregs Executereg);

/*----------------------------------------------------------------
|  						WriteBack								   	 |
*----------------------------------------------------------------*/
struct Writebackregs
{
	ap_uint<32> value[par_num];
	ap_uint<6> rd;
	bool useRd;
    bool isparmode;
};

/*----------------------------------------------------------------
|  						CORE								   	 |
*----------------------------------------------------------------*/

struct Core {
    memorybits  rf_addr;

	 FetchRegs fetchreg;
	 Decoderegs decodereg;
	 Executeregs executereg;
	 Memoryregs memoryreg;
	 Writebackregs writebackreg;

	Memory * instmem,* datamem;
    int instcounter;
	bool stall;
	ap_uint<32> pc;
	ap_uint<32> Branchpc;
	bool prevbranchraken;
	//bool isparmode;
	//ap_uint<32>  nextInst ;
	//ap_int<32> rf[32];

};


void branchUnit(ap_uint<32> nextPC_fetch,ap_uint<32> nextPC_decode,bool isBranch_decode,int instcount_decode, ap_uint<32> &PC, bool isstall, bool &corestall, int &coreinstcount, bool fetchreg_isopcodeBranch ,ap_uint<32> Branchpc, ap_uint<32> &CoreBranchPC, bool prevbranchraken ,bool &Core_prevbranchraken, bool decodereg_isopcodeBranch,bool decodereg_ismispredict, ap_uint<32> decodereg_mispc, bool decodereg_isopcodeBranchorJump );

//void  myrun( );
void myrun (volatile data_type mem_extra[extramem_size] );
