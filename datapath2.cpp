

#include "datapath2.h"


//struct FtoDC ftoDC,struct Decodereg &Decodereg,ap_int<32> registerFile[64])
template <class T,class U>
void loopunrollassign_arrtoarr(T* from,U* to){
    initlize_par:for(int i = 0;i<par_num;i++){
        #pragma HLS UNROLL
    to[i] = from[i];
    }
}

template <class T,class U>
void loopunrollassign_valtoarr(T from,U* to){
    initlize_par:for(int i = 0;i<par_num;i++){
        #pragma HLS UNROLL
    	to[i] = from;
    }
}


void decwb(struct Decoderegs &Decodereg,struct Writebackregs &Writebackreg,struct FetchRegs FetchReg,struct Memoryregs Memoryreg,uint32_t rf[par_num][32], bool prevbranchraken ,ap_uint<32> Branchpc)
{
	#pragma HLS inline off
#pragma HLS dependence variable=rf inter false
  //Register access
    ap_uint<32> valueReg1[par_num];
    ap_uint<32> valueReg2[par_num];
#pragma HLS ARRAY_PARTITION variable=valueReg1 complete  dim=1
#pragma HLS ARRAY_PARTITION variable=valueReg2 complete  dim=1
#pragma HLS ARRAY_PARTITION variable=Memoryreg.result complete  dim=1


    Decodereg.isactive[par_num] = 1;

    Decodereg.isparmode = FetchReg.isparmode;
    	Writebackreg.useRd = Memoryreg.useRd;
    	Writebackreg.rd = Memoryreg.rd;
    	loopunrollassign_valtoarr(0,Writebackreg.value);
#ifdef parallel_mode
#pragma HLS ARRAY_PARTITION variable=final_end_core complete  dim=1
#pragma HLS ARRAY_PARTITION variable=for_array_end complete  dim=1
#pragma HLS ARRAY_PARTITION variable=for_array_parstart complete  dim=1

    	Decodereg.finalendcore= 	final_end_core[FetchReg.parloopcount];
    	Decodereg.forarrayend= 	for_array_end[final_end_core[FetchReg.parloopcount]];
    	Decodereg.forarrayparstart = for_array_parstart[final_end_core[FetchReg.parloopcount]];
    	Decodereg.fordelta	=	for_delta[FetchReg.parloopcount];
    	Decodereg.forend =  for_end[FetchReg.parloopcount];
#endif


    initlize_par:for(int i = 0;i<par_num;i++){
            #pragma HLS UNROLL
    	Decodereg.isactive[i] = i <= final_end_core[FetchReg.parloopcount] && i>= final_start_core[FetchReg.parloopcount];

    	valueReg1[i] = (ap_uint<32>)rf[i][FetchReg.instruction2.range(19,15)];
    	valueReg2[i] = (ap_uint<32>)rf[i][FetchReg.instruction2.range(24,20)];
        }

	if( Memoryreg.useRd|| Memoryreg.isstartparmode ){//
//
	if (Memoryreg.isparmode){
			set_r1:for(int i = 0;i<par_num;i++){
					#pragma HLS UNROLL
				//	   rf[i][Memoryreg.rd] = (uint32_t)Memoryreg.result[i] ;
				//problem
				rf[i][Memoryreg.rd] = Memoryreg.isactive[i] ? (uint32_t)Memoryreg.result[i] : (uint32_t)Memoryreg.result[Memoryreg.finalendcore ];
				//rf[i][Memoryreg.rd] = (uint32_t)Memoryreg.result[i] ;

			}
	} else {
#ifdef parallel_mode


if ( Memoryreg.isstartparmode) {//todo
	    		startpar:for(int i = 0 ;i<par_num;i++){
	        					#pragma HLS UNROLL
	    			//	        					rf[i ][start_reg] = for_array_parstart[i ];
					//							   rf[i ][end_reg]= for_array_end[i ] ;

	    			rf[i ][ start_reg[Memoryreg.parloopcount]] = (uint32_t) Memoryreg.forarrayparstart[i  ];
	    			rf[i ][ end_reg[Memoryreg.parloopcount]	]= (uint32_t)Memoryreg.forarrayend[i];

	    			//rf[i ][Memoryreg.startreg] = Memoryreg.isactive[i] ? for_array_parstart[i  ] :for_array_parstart[Memoryreg.finalendcore ];
	    			//rf[i ][Memoryreg.endreg]= Memoryreg.isactive[i]? for_array_end[i  ]:	 for_array_end[Memoryreg.finalendcore ];

	        	}
				//decode(mycore.decodereg, mycore.fetchreg);
			}else {
#endif
				 SETRF_MACRO<0>(Memoryreg.rf_addr,Memoryreg.rd,  Memoryreg.result, rf );
#ifdef parallel_mode
			}
#endif
			}

	}



	ap_int<32> instruction = FetchReg.instruction2;
	ap_uint<5> rs2 = instruction.range(24,20);
	ap_uint<5> rs1 =  instruction.range(19,15);

	// R-type instruction
	ap_uint<7> funct7 = instruction.range(31,25);
	ap_uint<5> rd = instruction.range(11, 7);
	ap_uint<3> funct3 = instruction.range(14, 12);
	ap_uint<7> opCode = instruction.range(6,0);  // could be reduced to 5 bits because 1:0 is always 11

	//Construction of different immediate values
	ap_uint<12> imm12_I = instruction.range(31, 20);
	ap_uint<12> imm12_S = 0;
	imm12_S.range(4, 0)  = instruction.range(11, 7);
	imm12_S.range(11, 5)  = instruction.range(31, 25);
	ap_int<12> imm12_I_signed = instruction.range(31,20);
	ap_int<12> imm12_S_signed =(ap_int<12>) imm12_S;

	ap_uint<13> imm13 = 0;
	imm13[12] = instruction[31];
	imm13.range(10,5)= instruction.range(30,25);
	imm13.range(4,1)=instruction.range(11,8);
	imm13[11] = instruction[7];

	ap_int<13> imm13_signed = (ap_int<13>) imm13;

	ap_int<32> imm31_12 = 0;
	imm31_12.range(31,12)= instruction.range(31,12);

	ap_uint<21> imm21_1 = 0;
	imm21_1.range(19,12)= instruction.range(19,12);
	imm21_1[11] = instruction[20];
	imm21_1.range(10,1)=instruction.range(30,21);
	imm21_1[20] = instruction[31];

	ap_int<21> imm21_1_signed = (ap_int<21>) imm21_1;
	Decodereg.nextPCDC = 0;
	Decodereg.nextPCMispredict = FetchReg.nextPC;
	Decodereg.useRd = 0;
	loopunrollassign_valtoarr(0,Decodereg.datac);

	loopunrollassign_valtoarr(0,Decodereg.lhs);
    loopunrollassign_valtoarr(0,Decodereg.rhs);
	Decodereg.useRs1 = 0;
	Decodereg.useRs2 = 0;
	Decodereg.isBranch = 0;
	Decodereg.rf_addr = 0;
	Decodereg.isopcodeBranch = FetchReg.isopcodeBranch;
	Decodereg.isopcodeBranchorJump = 0;
	Decodereg.isopcodeStoreorLoad = 0;
	Decodereg.parloopcount = FetchReg.parloopcount;


	bool temp_useRd;
	if (rd == 0){
		temp_useRd= 0;
	}else {
		temp_useRd=1;
	}

	switch (opCode)
	{

#ifdef LUI
	case RISCV_LUI:
	    loopunrollassign_valtoarr(imm31_12,Decodereg.lhs);

		Decodereg.useRs1 = 0;
		Decodereg.useRs2 = 0;
		Decodereg.useRd = temp_useRd;
		Decodereg.isBranch = 0;
		Decodereg.rf_addr = 0;
		break;
#endif
#ifdef AUIPC

	case RISCV_AUIPC:
		       // Decodereg.lhs = FetchReg.pc;
		        loopunrollassign_valtoarr(FetchReg.pc,Decodereg.lhs);

		       // Decodereg.rhs = imm31_12;
		        loopunrollassign_valtoarr(imm31_12,Decodereg.rhs);
		Decodereg.useRs1 = 0;
		Decodereg.useRs2 = 0;
		Decodereg.useRd = temp_useRd;
		Decodereg.isBranch = 0;
		Decodereg.rf_addr = 0;
		break;
#endif
#ifdef JAL
	case RISCV_JAL:
		       // Decodereg.lhs = FetchReg.pc+4;
		        loopunrollassign_valtoarr(FetchReg.pc+4,Decodereg.lhs);
		        //Decodereg.rhs = 0;
		        loopunrollassign_valtoarr(0,Decodereg.rhs);
		Decodereg.nextPCDC = FetchReg.pc + imm21_1_signed;
		Decodereg.useRs1 = 0;
		Decodereg.useRs2 = 0;
		Decodereg.useRd = temp_useRd;
		Decodereg.isBranch = 1;
		Decodereg.rf_addr = 0;
		Decodereg.isopcodeBranchorJump = 1;
		break;
#endif
#ifdef JALR
	case RISCV_JALR:
	        //Decodereg.lhs = valueReg1;
	        loopunrollassign_arrtoarr(valueReg1,Decodereg.lhs);
	        //Decodereg.rhs = imm12_I_signed;
	        loopunrollassign_valtoarr(imm12_I_signed,Decodereg.rhs);
		Decodereg.useRs1 = 1;
		Decodereg.useRs2 = 0;
		Decodereg.useRd = temp_useRd;
//		Decodereg.isBranch = 0;
		Decodereg.isBranch = 1;
		Decodereg.rf_addr = 0;
		Decodereg.nextPCDC = Decodereg.rhs[par_num-1] + Decodereg.lhs[par_num-1];
		Decodereg.isopcodeBranchorJump = 1;

		break;
#endif
#ifdef BRANCH
	case RISCV_BR:
		       // Decodereg.lhs = valueReg1;
		        loopunrollassign_arrtoarr(valueReg1,Decodereg.lhs);
		        //Decodereg.rhs = valueReg2;
		        loopunrollassign_arrtoarr(valueReg2,Decodereg.rhs);
		Decodereg.useRs1 = 1;
		Decodereg.useRs2 = 1;
		Decodereg.useRd = 0;
		Decodereg.isBranch = 0;
		Decodereg.rf_addr = 0;

		Decodereg.isopcodeBranchorJump = 1;

		Decodereg.nextPCDC = FetchReg.pc + imm13_signed;
		switch(funct3)
		{

		case RISCV_BR_BEQ:
			Decodereg.isBranch = (Decodereg.lhs[par_num-1] == Decodereg.rhs[par_num-1]);
			break;
		case RISCV_BR_BNE:
			Decodereg.isBranch = (Decodereg.lhs[par_num-1] != Decodereg.rhs[par_num-1]);
			break;
		case RISCV_BR_BLT:
			Decodereg.isBranch = (Decodereg.lhs[par_num-1] < Decodereg.rhs[par_num-1]);
			break;
		case RISCV_BR_BGE:
			Decodereg.isBranch = (Decodereg.lhs[par_num-1] >= Decodereg.rhs[par_num-1]);
			break;
		case RISCV_BR_BLTU:
			Decodereg.isBranch = ((ap_uint<32>)Decodereg.lhs[par_num-1] < (ap_uint<32>)Decodereg.rhs[par_num-1]);
			break;
		case RISCV_BR_BGEU:
			Decodereg.isBranch = ((ap_uint<32>)Decodereg.lhs[par_num-1] >= (ap_uint<32>)Decodereg.rhs[par_num-1]);
			break;
		}

		break;
#endif
#ifdef LOAD
	case RISCV_LD:
		      //  Decodereg.lhs = valueReg1;
		        loopunrollassign_arrtoarr(valueReg1,Decodereg.lhs);
		       // Decodereg.rhs = imm12_I_signed;
		        loopunrollassign_valtoarr(imm12_I_signed,Decodereg.rhs);
		Decodereg.useRs1 = 1;
		Decodereg.useRs2 = 0;
		Decodereg.useRd = temp_useRd;
		Decodereg.rf_addr = (ap_int<32>)valueReg1[par_num-1] + (ap_int<32>)imm12_I_signed-mem_start_adress;
		Decodereg.isBranch = 0;
		Decodereg.isopcodeStoreorLoad = 1;

		break;
#endif
#ifdef STORE
	case RISCV_ST:
		       // Decodereg.lhs = valueReg1;
		        loopunrollassign_arrtoarr(valueReg1,Decodereg.lhs);
		       // Decodereg.rhs = imm12_S_signed;
		        loopunrollassign_valtoarr(imm12_S_signed,Decodereg.rhs);

		        //Decodereg.datac = valueReg2; //Value to store in memory
		        loopunrollassign_arrtoarr(valueReg2,Decodereg.datac);
		Decodereg.useRs1 = 1;
		Decodereg.useRs2 = 0;
		Decodereg.useRd = 0;
		Decodereg.rf_addr = (ap_int<32>)valueReg1[par_num-1] + (ap_int<32>)imm12_S_signed-mem_start_adress;
		Decodereg.isBranch = 0;
		Decodereg.isopcodeStoreorLoad = 1;

		break;
#endif
	case RISCV_OPI:
	    	  //  Decodereg.lhs = valueReg1;
	    		        loopunrollassign_arrtoarr(valueReg1,Decodereg.lhs);
	    		       // Decodereg.rhs = imm12_I_signed;
	    		        loopunrollassign_valtoarr(imm12_I_signed,Decodereg.rhs);
		Decodereg.useRs1 = 1;
		Decodereg.useRs2 = 0;
		Decodereg.useRd = temp_useRd;
		Decodereg.isBranch = 0;
		Decodereg.rf_addr = 0;
		break;

	case RISCV_OP:

	    	   //  Decodereg.lhs = valueReg1;
	    		        loopunrollassign_arrtoarr(valueReg1,Decodereg.lhs);
	    		      //  Decodereg.rhs = valueReg2;
	    		        loopunrollassign_arrtoarr(valueReg2,Decodereg.rhs);
		Decodereg.useRs1 = 1;
		Decodereg.useRs2 = 1;
		Decodereg.useRd = temp_useRd;
		Decodereg.isBranch = 0;
		Decodereg.rf_addr = 0;
		break;

	default:
		break;
	}

	//Decodereg.opCode = FetchReg.opCode;
	Decodereg.opCode = opCode;

	Decodereg.rs2 = rs2;
	Decodereg.rs1 = rs1;
	Decodereg.rd = rd;
	Decodereg.funct3 = funct3;
	Decodereg.funct7 = funct7;
	Decodereg.instruction = instruction;
	Decodereg.pc =  FetchReg.pc;
	//Initialization of control bits
	//Decodereg.useRd = 0;

	Decodereg.ismispredict =  (prevbranchraken ?  ( (FetchReg.pc + imm13_signed) != Branchpc) : true); //&& (Decodereg.isBranch != prevbranchraken)



}


void execute(struct Executeregs &Executereg,struct Decoderegs Decodereg) {
#pragma HLS inline off
#pragma HLS latency max=0
#pragma HLS ARRAY_PARTITION variable=Decodereg.isactive complete  dim=1

#ifdef parallel_mode
#pragma HLS resource variable=addrlut core=ROM_1P_1S
#pragma HLS ARRAY_PARTITION variable=for_array_end complete

#endif

	Executereg.pc = Decodereg.pc;
	Executereg.opCode = Decodereg.opCode;
	Executereg.funct3 = Decodereg.funct3;
	//Executereg.isBranch = 0;

	Executereg.rd = Decodereg.rd;
	Executereg.useRd = Decodereg.useRd;
	Executereg.parloopcount = Decodereg.parloopcount;
	Executereg.finalendcore = Decodereg.finalendcore ;

	Executereg.isLongInstruction = 0;
	Executereg.instruction = Decodereg.instruction;
#ifdef parallel_mode
	Executereg.rf_addr = (Decodereg.rf_addr> addlutsize) ?  ext :  addrlut[Decodereg.rf_addr.range(31,2)];
	Executereg.forarrayend =   Decodereg.forarrayend;
    Executereg.forarrayparstart = Decodereg.forarrayparstart;
	//Executereg.isparmode = Decodereg.isparmode && (	Decodereg.isopcodeStoreorLoad ? ((Decodereg.rf_addr<for_end[0] ) && (Decodereg.rf_addr>=(for_end[0]-for_delta[0]))) : true  );//parmode and common memory
	//pðroblem 1 Executereg.isparmode
	//Executereg.isparmode = Decodereg.isparmode && (	Decodereg.isopcodeStoreorLoad ? ((Decodereg.rf_addr<for_end[Decodereg.parloopcount-1] ) && (Decodereg.rf_addr>=(for_end[Decodereg.parloopcount-1]-for_delta[Decodereg.parloopcount-1]))) : true  );//parmode and common memory
	Executereg.isparmode = Decodereg.isparmode && (	Decodereg.isopcodeStoreorLoad ? ((Decodereg.rf_addr<Decodereg.forend ) && (Decodereg.rf_addr>=(Decodereg.forend -Decodereg.fordelta))) : true  );//parmode and common memory

	addrset:for(int i = 0;i<par_num+1;i++){
		#pragma HLS UNROLL
		Executereg.addr_arr[i]	= Executereg.rf_addr == (memorybits) i ;
		if(Decodereg.isactive[i] ){ //only activate necessary cores
			Executereg.isparmode_or_addr_arr[i]	= Executereg.isparmode || Executereg.addr_arr[i] ;
		}else {
			Executereg.isparmode_or_addr_arr[i]	= Executereg.addr_arr[i];
		}
	}


#else
	Executereg.rf_addr = (memorybits) 0;
	Executereg.addr_arr[0] = true;
	Executereg.isparmode_or_addr_arr[0] = true;
	Executereg.isparmode = Decodereg.isparmode;
#endif

	Executereg.iscalloutsidemem  = Executereg.rf_addr == ext;

	//Executereg.nextPC = 0;
	loopunrollassign_valtoarr(0,Executereg.datac);

	Executereg.boolval = true;

	ap_uint<13> imm13 = 0;
    imm13[12] = Decodereg.instruction[31];
    imm13.range(10,5)= Decodereg.instruction.range(30,25);
    imm13.range(4,1)=Decodereg.instruction.range(11,8);
    imm13[11] = Decodereg.instruction[7];
    ap_int<13> imm13_signed = (ap_int<13>) imm13;
	ap_int<5> shamt = Decodereg.instruction.range(25,20);

    zero:for(int i = 0;i<par_num;i++){
		#pragma HLS UNROLL
		Executereg.address[i] = 0;
		Executereg.result[i] = 0;
		Executereg.isactive[i] = Decodereg.isactive[i];
	}


    Executereg.address[par_num] = 0;
  //  bool returnval = true;

	switch(Decodereg.opCode)
	{
#ifdef LUI
	case RISCV_LUI:
		loopunrollassign_valtoarr(Decodereg.lhs[par_num-1],Executereg.result);
		break;
#endif
#ifdef AUIPC
	case RISCV_AUIPC:
		loopunrollassign_valtoarr(Decodereg.lhs[par_num-1]+Decodereg.rhs[par_num-1],Executereg.result);
		break;
#endif
#ifdef JAL
	case RISCV_JAL:
		//Note: in current version, the addition is made in the decode stage
		//The value to store in rd (pc+4) is stored in lhs
		loopunrollassign_valtoarr(Decodereg.lhs[par_num-1],Executereg.result);
		break;
#endif
#ifdef JALR
	case RISCV_JALR:
		//Note: in current version, the addition is made in the decode stage
		//The value to store in rd (pc+4) is stored in lhs
		//Executereg.nextPC = Decodereg.rhs + Decodereg.lhs;
		//Executereg.isBranch = 1;

		loopunrollassign_valtoarr(Decodereg.pc+4,Executereg.result);
		break;
#endif
#ifdef BRANCH
	case RISCV_BR:
		 loopunrollassign_valtoarr(0,Executereg.result);

		break;
#endif
#ifdef LOAD
	case RISCV_LD:
		Executereg.isLongInstruction = 1;
		LD_par:for(int i = 0;i<par_num;i++){
			#pragma HLS UNROLL
			Executereg.address[i] = Executereg.isparmode ? (Decodereg.lhs[i] + Decodereg.rhs[i] - for_array_init [i] ):(Decodereg.lhs[par_num-1] + Decodereg.rhs[par_num-1] - for_array_init [i] );
		}
		Executereg.address[par_num] = Executereg.isparmode ? (Decodereg.lhs[par_num-1] + Decodereg.rhs[par_num-1] - for_array_init [par_num-1] - addlutsize ):(Decodereg.lhs[par_num-1] + Decodereg.rhs[par_num-1] - for_array_init [par_num-1] - addlutsize);
		loopunrollassign_valtoarr(0,Executereg.result);

		break;
#endif
#ifdef STORE
	case RISCV_ST:
		ST_par:for(int i = 0;i<par_num;i++){
			#pragma HLS UNROLL
			Executereg.datac[i] = Decodereg.datac[i];
			Executereg.address[i] = Executereg.isparmode ? (Decodereg.lhs[i] + Decodereg.rhs[i] - for_array_init [i]):(Decodereg.lhs[par_num-1] + Decodereg.rhs[par_num-1] - for_array_init [i] );
		}
		Executereg.address[par_num] = Executereg.isparmode ? (Decodereg.lhs[par_num-1] + Decodereg.rhs[par_num-1] - for_array_init [par_num-1] - addlutsize ):(Decodereg.lhs[par_num-1] + Decodereg.rhs[par_num-1] - for_array_init [par_num-1] - addlutsize);
		loopunrollassign_valtoarr(0,Executereg.result);

		break;
#endif
	case RISCV_OPI:
		switch(Decodereg.funct3)
		{
		case RISCV_OPI_ADDI:
l1:for(int i = 0;i<par_num;i++){
		        		                #pragma HLS UNROLL
		        		            	Executereg.result[i] = Decodereg.lhs[i] + Decodereg.rhs[i];
		        		            }
			break;
		case RISCV_OPI_SLTI:
l2:for(int i = 0;i<par_num;i++){
		        		                #pragma HLS UNROLL
		        		            	Executereg.result[i] = Decodereg.lhs[i] < Decodereg.rhs[i];
		        		            }
			break;
		case RISCV_OPI_SLTIU:
l3:for(int i = 0;i<par_num;i++){
		        		                #pragma HLS UNROLL
		        		            	Executereg.result[i] =(ap_uint<32>) Decodereg.lhs[i] <(ap_uint<32>) Decodereg.rhs[i];
		        		            }
			break;
		case RISCV_OPI_XORI:
	l4:for(int i = 0;i<par_num;i++){
		        		                #pragma HLS UNROLL
		        		            	Executereg.result[i] = Decodereg.lhs[i] ^ Decodereg.rhs[i];
		        		            }
			break;
		case RISCV_OPI_ORI:
	l5:for(int i = 0;i<par_num;i++){
		        		                #pragma HLS UNROLL
		        		            	Executereg.result[i] = Decodereg.lhs[i] | Decodereg.rhs[i];
		        		            }
			break;
		case RISCV_OPI_ANDI:
l6:for(int i = 0;i<par_num;i++){
		        			                #pragma HLS UNROLL
		        			            	Executereg.result[i] = Decodereg.lhs[i] & Decodereg.rhs[i];
		        			            }
			break;
		case RISCV_OPI_SLLI: // cast rhs as 5 bits, otherwise generated hardware is 32 bits
			// & shift amount held in the lower 5 bits (riscv spec)
l7:for(int i = 0;i<par_num;i++){
		        			                #pragma HLS UNROLL
		        			            	Executereg.result[i] = Decodereg.lhs[i] << (ap_uint<5>) Decodereg.rhs[i];
		        			            }
			break;
		case RISCV_OPI_SRI:
			if (Decodereg.funct7.get_bit(5)) //SRAI
l8:for(int i = 0;i<par_num;i++){
		        								#pragma HLS UNROLL
		        								Executereg.result[i] = Decodereg.lhs[i]>> (ap_uint<5>)shamt;
		        							}
			else //SRLI
l9:for(int i = 0;i<par_num;i++){
		        								#pragma HLS UNROLL

		        								Executereg.result[i] = (ap_uint<32>)Decodereg.lhs[i]>> (ap_uint<5>)shamt;
		        							}
			break;
		}
		break;
	case RISCV_OP:
#ifdef MULTI
		if(Decodereg.funct7.get_bit(0))     // M Extension
		{
			  ap_uint<32> quotient=0, remainder=0;
				//ac_int<33, false>
				ap_uint<6> state = 0;
				bool resIsNeg=false;
		        	    ap_uint<32> dataAUnsigned[par_num];
						ap_uint<32> dataBUnsigned[par_num];
						ap_uint<64> resultU[par_num];
                        ap_uint<64> resultS[par_num];
                        ap_uint<64> resultSU[par_num];
                        ap_uint<64> mem_resultU;
                        ap_uint<64> mem_resultS;
                        ap_uint<64> mem_resultSU;
#pragma HLS ARRAY_PARTITION variable=dataAUnsigned complete  dim=1
#pragma HLS ARRAY_PARTITION variable=dataBUnsigned complete  dim=1
#pragma HLS ARRAY_PARTITION variable=resultU complete  dim=1
#pragma HLS ARRAY_PARTITION variable=resultS complete  dim=1
#pragma HLS ARRAY_PARTITION variable=resultSU complete  dim=1
				//no need to fill in the output register fields, the first ALU has that taken care of
			  bool valRet = false;

		//  if (state == 0) {
						il1:for(int i = 0;i<par_num;i++){
			                #pragma HLS UNROLL
			        		dataAUnsigned[i] = (ap_uint<32>)Decodereg.lhs[i];
			        		dataBUnsigned[i] = (ap_uint<32>)Decodereg.rhs[i];

			        	}
#ifdef MUL
	#ifndef resultS_use
#define 	resultS_use
	#endif
#endif

#ifdef MULH
	#ifndef resultS_use
#define 	resultS_use
	#endif
#endif


						//mult results
						il12:for(int i = 0;i<par_num;i++){
							#pragma HLS UNROLL
#ifdef MULHSU

							resultU[i] = dataAUnsigned[i] * dataBUnsigned[i];
#endif
#ifdef resultS_use

							resultS[i] = Decodereg.lhs[i] * Decodereg.rhs[i];
#endif
#ifdef MULHU
							resultSU[i] = Decodereg.lhs[i] * dataBUnsigned[i];
#endif

						}


					switch (Decodereg.funct3){
#ifdef MUL
						case RISCV_OP_M_MUL:
									ml1:for(int i = 0;i<par_num;i++){
																		#pragma HLS UNROLL
																		Executereg.result[i] = resultS[i].range(31,0);
																	}									valRet = true;

						break;
#endif
#ifndef MULH
						case RISCV_OP_M_MULH:
							ml2:for(int i = 0;i<par_num;i++){
																	#pragma HLS UNROLL
																	Executereg.result[i] =  1;
																}									valRet = true;
						break;
#endif
#ifdef MULH
								case RISCV_OP_M_MULH:
									ml2:for(int i = 0;i<par_num;i++){
																			#pragma HLS UNROLL
																			Executereg.result[i] = resultS[i].range(63,32);
																		}									valRet = true;
								break;
#endif
#ifdef MULHSU
								case RISCV_OP_M_MULHSU:
									ml3:for(int i = 0;i<par_num;i++){
																			#pragma HLS UNROLL
																			Executereg.result[i] = resultSU[i].range(63,32);
																		}									valRet = true;
								break;
#endif
#ifdef MULHU
								case RISCV_OP_M_MULHU:
									ml4:for(int i = 0;i<par_num;i++){
																			#pragma HLS UNROLL
																			Executereg.result[i] =  resultU[i].range(63,32);
																		}									valRet = true;
								break;
#endif

					/*	case RISCV_OP_M_DIV:
							if(Decodereg.lhs[31]) {
								dataAUnsigned = -Decodereg.lhs;
							}
							if(Decodereg.rhs[31]) {
								dataBUnsigned = -Decodereg.rhs;
							}
							//printf("Dividing %d by %d\n", dataAUnsigned, dataBUnsigned);
						case RISCV_OP_M_DIVU:
							if(dataBUnsigned == 0) {
								Executereg.result[par_num-1] = -1;
							valRet = true;
							}
							else {
								state = 32;
								quotient = 0;
								remainder = 0;
							}
						break;
#endif*/

						/*case RISCV_OP_M_REM:
							if(Decodereg.lhs[31]) {
								dataAUnsigned = -Decodereg.lhs;
							}
							if(Decodereg.rhs[31]) {
								dataBUnsigned = -Decodereg.rhs;
							}
							//printf("Moduling %d by %d\n", dataAUnsigned, dataBUnsigned);
						case RISCV_OP_M_REMU:
							if(dataBUnsigned == 0) {
								Executereg.result = dataAUnsigned;
							}
							else {
								state = 32;
								quotient = 0;
								remainder = 0;
							}
						break;
						*/
						}
				//	}

			  /* if(state != 0){
						//Loop for the division
				   for(int j = 0; j < 8; j++)
						{
						for(int i = 0; i < 4; i++)
						{
							state--;
							remainder = remainder  << 1;
							remainder[0] = dataAUnsigned[state];
							if(remainder >= dataBUnsigned) {
								remainder = remainder - dataBUnsigned;
								quotient[state] = 1;
							}
						}
					}


					switch(Decodereg.funct3) {
					case RISCV_OP_M_DIV:
						if(resIsNeg)
							Executereg.result = -quotient;
						else
							Executereg.result = quotient;
					valRet = true;
					break;
					case RISCV_OP_M_DIVU:
						Executereg.result = quotient;
					valRet = true;
					break;
					case RISCV_OP_M_REM:
						if(dataAUnsigned[31])
							Executereg.result = -remainder;
						else
							Executereg.result = remainder;
					valRet = true;
					break;
					case RISCV_OP_M_REMU:
						Executereg.result = remainder;
					valRet = true;
					break;
					}
					//printf("result : %d\n", extoMem.result);
			   }*/

		} else{
			#endif
		            switch(Decodereg.funct3){
		            case RISCV_OP_ADD:
		            	  if (Decodereg.funct7.get_bit(5)) {  // SUB
		            			                 //   Executereg.result = Decodereg.lhs - Decodereg.rhs;
		            				        	il2:for(int i = 0;i<par_num;i++){
		            				                #pragma HLS UNROLL
		            				            	Executereg.result[i] = Decodereg.lhs[i] - Decodereg.rhs[i];
		            				            }
		            			                }else{   // ADD
		            			                  //  Executereg.result = Decodereg.lhs + Decodereg.rhs;
		            				        	il3:for(int i = 0;i<par_num;i++){
		            				                #pragma HLS UNROLL
		            				            	Executereg.result[i] = Decodereg.lhs[i] + Decodereg.rhs[i];
		            				            }
		            			                }
		            	  break;
		            case RISCV_OP_SLL:
		            	il4:for(int i = 0;i<par_num;i++){
		            			                #pragma HLS UNROLL
		            			            	Executereg.result[i] = Decodereg.lhs[i] << (ap_uint<5>) Decodereg.rhs[i];
		            			            }		                break;
		            case RISCV_OP_SLT:
		            	il5:for(int i = 0;i<par_num;i++){
		            				                #pragma HLS UNROLL
		            				            	Executereg.result[i] = Decodereg.lhs[i] < Decodereg.rhs[i];
		            				            }		                break;
		            case RISCV_OP_SLTU:
		            	il6:for(int i = 0;i<par_num;i++){
		            				                #pragma HLS UNROLL
		            				            	Executereg.result[i] = (ap_uint<32>)Decodereg.lhs[i] < (ap_uint<32>) Decodereg.rhs[i];
		            				            }		                break;
		            case RISCV_OP_XOR:
		            	il7:for(int i = 0;i<par_num;i++){
		            				                #pragma HLS UNROLL
		            				            	Executereg.result[i] = Decodereg.lhs[i] ^ Decodereg.rhs[i];
		            				            }		                break;







		            case RISCV_OP_SR:
		            	 if(Decodereg.funct7.get_bit(5)) {  // SRA
		            			                    //Executereg.result = Decodereg.lhs >> (ap_uint<5>)Decodereg.rhs;
		            								il8:for(int i = 0;i<par_num;i++){
		            									#pragma HLS UNROLL
		            									Executereg.result[i] = Decodereg.lhs[i] >> (ap_uint<5>) Decodereg.rhs[i];
		            								}
		            			                } else{  // SRL
		            			                   // Executereg.result = (ap_uint<32>)Decodereg.lhs >> (ap_uint<5>)Decodereg.rhs;
		            								il9:for(int i = 0;i<par_num;i++){
		            									#pragma HLS UNROLL
		            									Executereg.result[i] =  (ap_uint<32>)Decodereg.lhs[i] >> (ap_uint<5>) Decodereg.rhs[i];
		            								}
		            			                } break;
		            case RISCV_OP_OR:
		            	il10:for(int i = 0;i<par_num;i++){
		            								#pragma HLS UNROLL
		            								Executereg.result[i] =  Decodereg.lhs[i] | Decodereg.rhs[i];
		            							}		                break;
		            case RISCV_OP_AND:
		            	il11:for(int i = 0;i<par_num;i++){
		            								#pragma HLS UNROLL
		            								Executereg.result[i] =  Decodereg.lhs[i]& Decodereg.rhs[i];
		            							}		                break;
		            }
#ifdef MULTI
		        }
#endif
		break;
	case RISCV_MISC_MEM:    // this does nothing because all memory accesses are ordered and we have only one core
		 loopunrollassign_valtoarr(0,Executereg.result);
		break;

	case RISCV_SYSTEM:
		switch(Decodereg.funct3)     {
		case RISCV_SYSTEM_ENV:    // lhs is from csr, rhs is from reg[rs1]
		 loopunrollassign_valtoarr(0,Executereg.result);

			 Executereg.boolval = false;
			// returnval = false;
			 //exit(0);
			break;
		default:
			break;
		}
		break;
	default:
		 Executereg.boolval = true;
		break;
	}
}



void memory(struct Memoryregs &Memoryreg,struct Executeregs Executereg){
#pragma HLS inline

	ap_uint<2> datasize = Executereg.funct3.range(1,0);
    bool signenable = !Executereg.funct3.get_bit(2);
    Memoryreg.useRd = Executereg.useRd;
    Memoryreg.rd = Executereg.rd;
    Memoryreg.isparmode = Executereg.isparmode;
    Memoryreg.parloopcount = Executereg.parloopcount;
    Memoryreg.isstartparmode = Executereg.pc == (Par_start_addr[Executereg.parloopcount]-init_simd_offset[Executereg.parloopcount]);

    ap_uint<32> mem_read;
    memOpType OpType;

    Memoryreg.finalendcore = Executereg.finalendcore ;
	 Memoryreg.startreg		=	   start_reg[Executereg.parloopcount];
	 Memoryreg.endreg 		=				 end_reg[Executereg.parloopcount]		;

    mem:for(int i = 0;i<par_num;i++){
  	  #pragma HLS UNROLL
 		Memoryreg.isactive[i] =Executereg.isactive[i]; //  i <= final_end_core[Executereg.parloopcount-1] && i>= final_start_core[Executereg.parloopcount-1];
#ifdef parallel_mode
    	 Memoryreg.forarrayparstart[i]=Executereg.isactive[i] ? (int) for_array_parstart[i  ] :Executereg.forarrayparstart;
     	Memoryreg.forarrayend[i]= Executereg.isactive[i]? (int)for_array_end[i  ]:	 Executereg.forarrayend ;
#endif
    	//rf[i ][Memoryreg.startreg] = Memoryreg.isactive[i] ? for_array_parstart[i  ] :for_array_parstart[Memoryreg.finalendcore ];
		//rf[i ][Memoryreg.endreg]= Memoryreg.isactive[i]? for_array_end[i  ]:	 for_array_end[Memoryreg.finalendcore ];
     }


    memMask mask;
    memOpType tempoptype;
    switch (Executereg.funct3) {
       case 0:
     	  mask = BYTE;
        break;
       case 1:
     	  mask = HALF;
        break;
      case 2:
     	 mask = WORD;
        break;
      case 4:
     	 mask = BYTE_U;
        break;
      case 5:
     	 mask = HALF_U;
        break;
      //Should NEVER happen
      default:
     	 mask = WORD;
        break;
     }

    switch(Executereg.opCode)
    {

    case RISCV_LD:

        //Memoryreg.rd = Executereg.rd;

        Memoryreg.isLoad = 1;
    	Memoryreg.isStore = 0;
    	addrset1:for(int i = 0;i<par_num+1;i++){
    		#pragma HLS UNROLL
    		Memoryreg.addr_arr[i]	= Executereg.addr_arr[i];
    		Memoryreg.isparmode_or_addr_arr[i]	= Executereg.isparmode_or_addr_arr[i];

    	}



        Memoryreg.rf_addr = Executereg.rf_addr ;
        OpType =  LD;

        break;
    case RISCV_ST:
        	Memoryreg.isStore = 1;
        	Memoryreg.isLoad = 0;
        	Memoryreg.byteEnable = 0xf;
        	addrset2:for(int i = 0;i<par_num+1;i++){
        		#pragma HLS UNROLL
        		Memoryreg.addr_arr[i]	= Executereg.addr_arr[i];
        		Memoryreg.isparmode_or_addr_arr[i]	= Executereg.isparmode_or_addr_arr[i];
        	}

            ST_mem:for(int i = 0;i<par_num;i++){
          	            #pragma HLS UNROLL
                  	Memoryreg.valueToWrite[i]  = Executereg.datac[i] ;

                  }



            Memoryreg.rf_addr = Executereg.rf_addr ;
            OpType =  ST;

        break;
    default:
    	Memoryreg.isStore = 0;
    	Memoryreg.isLoad = 0;

        OpType =  NONE;

    	addrset3:for(int i = 0;i<par_num+1;i++){
    		#pragma HLS UNROLL
    		Memoryreg.addr_arr[i]	= false;
    		Memoryreg.isparmode_or_addr_arr[i]	= Executereg.isparmode;
    	}

        DEF_mem:for(int i = 0;i<par_num;i++){
        	            #pragma HLS UNROLL
        	Memoryreg.result[i] = Executereg.result[i];
        	//Memoryreg.result[i] = (i <= final_end_core[Executereg.parloopcount-1] && i>= final_start_core[Executereg.parloopcount-1]) ? (uint32_t)Executereg.result[i] : (uint32_t)Executereg.result[final_end_core[Executereg.parloopcount-1]  ];


                }

    	Memoryreg.addr_arr[par_num-1] = true;
    	Memoryreg.isparmode_or_addr_arr[par_num-1] = true;
        Memoryreg.rf_addr = (memorybits) (par_num-1) ;

    	break;
    }

    END_mem:for(int i = 0;i<par_num+1;i++){
#pragma HLS UNROLL

    Memoryreg.address[i]  =  (ap_uint<32>)Executereg.address[i];

    }
    Memoryreg.OpType = OpType;
    Memoryreg.mask =  mask;
    Memoryreg.pc =  Executereg.pc;

}



void branchUnit(ap_uint<32> nextPC_fetch,ap_uint<32> nextPC_decode,bool isBranch_decode,int instcount_decode, ap_uint<32> &PC, bool isstall, bool &corestall, int &coreinstcount, bool fetchreg_isopcodeBranch ,ap_uint<32> Branchpc, ap_uint<32> &CoreBranchPC, bool prevbranchraken ,bool &Core_prevbranchraken, bool decodereg_isopcodeBranch, bool decodereg_ismispredict, ap_uint<32> decodereg_mispc, bool decodereg_isopcodeBranchorJump ){
#pragma HLS inline off //ap_uint<32> nextPC_execute,bool isBranch_execute,

	//coreinstcount = instcount_decode;
//	corestall = isstall;
	if(!isstall){ //decodereg.isopcodeBranchorJump && (decodereg.ismispredict || (decodereg.isBranch != prevbranchraken))
		if(decodereg_isopcodeBranchorJump && ( decodereg_ismispredict ||  (isBranch_decode != prevbranchraken)) ){
			if(isBranch_decode){
				PC = nextPC_decode;
			}else {
				PC = decodereg_mispc;
			}
		}else {
			if(fetchreg_isopcodeBranch && prevbranchraken){
				PC = Branchpc;
			}else {
				PC = nextPC_fetch;
			}
		}
		if (decodereg_isopcodeBranch){
			Core_prevbranchraken =  isBranch_decode;
		}

		if (decodereg_isopcodeBranch && isBranch_decode) {
			CoreBranchPC = nextPC_decode;
		}
	}


}


