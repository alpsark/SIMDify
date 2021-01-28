#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <math.h>
#include "new_define_datapath2.h"


volatile data_type mem_extra[extramem_size];
int main()
{
	//uint32_t indx = 0;
	//uint32_t inst;
	//data_path(0x30000113); // init Stack Pointer 768
	//todo
/*
	data_path(0x7f010113); // init Stack Pointer 2032
	data_path(0x7f010113); // init Stack Pointer 4064
*/

	//PC = 0 ; //init Program Counter
	//FILE * imem_file = fopen("C:\\Users\\alpsark\\Desktop\\hls\\simple_hls.hex","r");
	//Assuming we are working in the debug mode
	//and hex file named "32I.hax" is in the same file with the tb.cpp file.
	//for example 32I.hex you can see the instructions in "inst set 32I" file.
//	FILE * imem_file = fopen("../../../../hex/test.bin","r+b");
//	if (imem_file == 0){
		//printf("No hex file in the current directory");
	//	return 0;
//	}
	//carry readed instructions to vector for easy reference.
//	std::vector <uint32_t> inst_vector ;
	//int i = 0; //debug hex
	//while( 1 == fread(&inst,sizeof(uint32_t),1,imem_file) ){
		//inst_vector.push_back(inst);
		//mem[indx++] = inst;
		//debug  hex
		//printf("%x: %08x \n" , i , inst_vector[i/4]);
		//i = i+4;
//	}
	//bool pass = false ;
	//pass = true; //debug  hex
	//int i = 1 ;
 	myfile =  	fopen("../../../../new_define.h", "w");
 	myfile_c =  	fopen("../../../../parallel.c/new_define.h", "w");
 	myfileinst =  	fopen("../../../../parallel.c/myfileinst.txt", "w");

 	myrun(mem_extra );

 	//myrun();
 	//while(myrun());
	/*****
	 *
	 while (true){
		//printf("%x: %08x " ,0x890 , inst_vector[0x890/4-1]);





		int PC = myrun();

		fprintf(myfile,"%x:\t%08x " ,PC , inst_mem2[PC/4]);
		fprintf(myfile," ra = %d  sp = %d gp = %d tp = %d t0 = % d , t1 = % d , t2 = % d , s0 = % d, s1 = % d, a0 = % d, a1 = % d, a2 = % d, a3 = % d, a4 = % d, a5 = % d, a6 = % d, a7 = % d, s2 = % d, s3 = % d, s4 = % d, s5 = % d, s6 = % d, s7 = % d, s8 = % d, s9 = % d, sA = % d, sB = % d, t3 = % d, t4 = % d, t5 = % d, t6 = % d\n" ,rf[1] , rf[2] ,rf[3],rf[4],rf[5],rf[6],rf[7],rf[8],rf[9],rf[10],rf[11],rf[12],rf[13] ,rf[14] ,rf[15],rf[16],rf[17],rf[18],rf[19],rf[20],rf[21],rf[22],rf[23],rf[24],rf[25],rf[26],rf[27],rf[28],rf[29],rf[30],rf[31],rf[32]  );


		printf("%x: %08x \n" ,PC , inst_mem2[PC/4]);

		if( PC == 0x6e0){
						int x = 5;
				}


		//riscv_core();


		if( inst_mem2[PC/4] == 0x00100073 ){
			break;
		}

		//printf("%d a4 = %d \n" ,PC , rf[14] );

		//terminate before auipc instruction
		//if(0x0068 == PC ){break;}


	}
*****///
	return 0;
}
