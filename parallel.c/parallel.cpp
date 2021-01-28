#include <iostream>
#include <vector>
#include <stdio.h>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <algorithm>

#include "new_define.h"
#include "define_instructions.h"

//stoi requires c++11 standard
//#define UBUNTU
#ifdef UBUNTU
	#include <stdint.h>
#endif

void writeaddlut(int i,int counter, int * addrlut, int for_delta_, int for_init_){

    if(i < for_init_ + (for_delta_ * counter)) {
        addrlut[i] =  counter-1;
        return;
    }else {
        writeaddlut( i, counter+1, addrlut,for_delta_, for_init_);
    }


};


int main()
{



    std::vector <uint32_t> def_bit_inst_vector;
	std::string temp,line;
	std::ifstream inputData_fp,inputData,inputData2,inputData3;
	std::string filepath_filepath ="../_UserDefinedPrimitives/parallel_primitives.txt" ;
    bool activate_cache = false;
    bool override_default_par = false;
    int new_default_par = 1 ;
    int extramemsize = 0;
    int local_mem_size = 1;

	char * ptr_name_fp = &filepath_filepath[0];
	inputData_fp.open(ptr_name_fp);

	if(!inputData_fp.is_open()){
		std::cout << "ERROR COULD NOT OPEN parallel_primitives.txt\n"  ;
		}else{

		//Get Instructions in the LUT file to def_name_inst_vector and def_bit_inst_vector
		getline (inputData_fp,temp);
		int i_fp = 0;
		while (temp != "ENDOFLUT")
		{
			if(temp[0] == '#'){ //if comment
	getline (inputData_fp,temp) ;// get next line
	//inputData >> temp;// get start of the next line
	continue;
	}
	if (i_fp == 0)
    activate_cache = std::stoi(temp);
 	else if (i_fp == 1)
	override_default_par = std::stoi(temp);
	else if (i_fp == 2)
	new_default_par = std::stoi(temp);
	else if (i_fp == 3)
	extramemsize = std::stoi(temp);
	else if (i_fp == 4)
	local_mem_size = std::stoi(temp);

	getline (inputData_fp,temp) ;
	i_fp++;

	}

	}

	FILE *myfile = fopen("../parallel.h", "w");

	/*
	    bool activate_cache = false;
    bool override_default_par = false;
    int new_default_par = 1 ;
    int extramemsize = 0;
    int local_mem_size = 1;
	*/
	fprintf(myfile, "#include \"new_define.h\"  \n" );
	if(!override_default_par) {
        new_default_par = Par_num_from_c;
	}

	if (new_default_par == 1){
	fprintf(myfile, "//#define  parallel_mode  \n" );
	}else {
	fprintf(myfile, "#define  parallel_mode  \n" );
	//check divisibility
	if(((for_end-for_init)/4)%new_default_par != 0){
        fprintf(myfile, "PROBLEM, PAR IS NOT DIVISIBLE  \n" );
        return 8;
	}

	}

    if (activate_cache){
	fprintf(myfile, "#define  activatecache  \n" );
	}else {
	fprintf(myfile, "//#define  activatecache  \n" );
	extramemsize = 1;
	}

	fprintf(myfile, "const int extramem_size = %d ; // size of outside memory 32xextramem_size \n" ,extramemsize );
    fprintf(myfile, "const uint32_t addlutsize = %d ; // size of outside memory 32xextramem_size \n" ,local_mem_size);



//parnum
	fprintf(myfile, "#ifdef parallel_mode  \n" );
	fprintf(myfile, "const int par_num = %d ;//par_num \n",new_default_par  );
	fprintf(myfile, "#else \n" );
	fprintf(myfile, "const int par_num = 1 ;//par_num \n"  );
	fprintf(myfile, "#endif \n\n" );


//memorybits
	fprintf(myfile, "#ifdef parallel_mode  \n" );
	fprintf(myfile, "typedef enum {  \n" );
	fprintf(myfile, "_0 = 0,  \n" );
	for (int i=1;i<new_default_par;i++){
	fprintf(myfile, "_%d , \n",i  );
	}
	fprintf(myfile, "ext \n" );
	fprintf(myfile, "} memorybits; \n" );
	fprintf(myfile, "#else \n" );
	fprintf(myfile, "typedef enum { \n"  );
    fprintf(myfile, "_0 = 0,  \n" );
    	fprintf(myfile, "ext \n" );
	fprintf(myfile, "} memorybits; \n" );
	fprintf(myfile, "#endif \n\n" );


	//addlut
    fprintf(myfile, "#ifdef parallel_mode  \n" );
    fprintf(myfile, "const memorybits addrlut[%d]={ ",local_mem_size/4 );
    int  addrlut[local_mem_size/4];
    int for_init_ = for_init/4;
    int for_end_ = for_end/4;

int for_delta_ = (for_end_-for_init_)/new_default_par  ;

    for (int i = 0;i<local_mem_size/4;i++){
        if ( i < for_init_) {
            addrlut[i] =  new_default_par-1;
        }else if ( i >= for_end_ - for_delta_) {
            addrlut[i] =  new_default_par-1;
        }else {
            writeaddlut(i,1,addrlut,for_delta_,for_init_ );
        }
	}

    for(int i =0; i< local_mem_size/4-1 ; i++ ){
        fprintf(myfile,"_%d,",addrlut[i]);
    }
    fprintf(myfile,"_%d}; \n",addrlut[local_mem_size/4-1]);
	fprintf(myfile, "#endif \n\n" );

	//getsetmacro
    fprintf(myfile, "#define GETSET_MACRO    	\\\n" );
   for(int i =0; i< new_default_par-2 ; i++ ){
        fprintf(myfile,"getset0<%d>(address[%d], mask,(isparmode_or_addr_arr[%d] )?OpType:NONE, valueToWrite[%d],result[%d] );    \\\n",i,i,i,i,i);
    }
        fprintf(myfile,"getset0<%d>(address[%d], mask,(isparmode_or_addr_arr[%d] )?OpType:NONE, valueToWrite[%d],result[%d] );     \n ",new_default_par-2,new_default_par-2,new_default_par-2,new_default_par-2,new_default_par-2);
	fprintf(myfile, " \n\n" );


//SETRF_MACRO
    fprintf(myfile, "template <int dd> \n" );
    fprintf(myfile, "void SETRF_MACRO(memorybits rf_addr,ap_uint<6> rd,  ap_uint<32> * result , uint32_t rf[par_num][32]){ \n" );
    fprintf(myfile, "#pragma HLS inline \n" );
    fprintf(myfile, "#ifdef parallel_mode \n" );
    fprintf(myfile, "switch (rf_addr) { \n" );
   for(int i =0; i< new_default_par ; i++ ){
    fprintf(myfile, "case _%d: \n",i );
    fprintf(myfile, "for(int i = 0;i<par_num;i++){ \n" );
    fprintf(myfile, "#pragma HLS UNROLL \n" );
    fprintf(myfile, " rf[i][rd] =  (uint32_t)result[%d] ; \n" ,i);
    fprintf(myfile, " }         break; \n" );
    }
        fprintf(myfile, " default: \n" );
        fprintf(myfile, " break;\n" );
        fprintf(myfile, " }\n" );
	fprintf(myfile, "#else \n" );
	fprintf(myfile, " rf[0][rd] =  (uint32_t)result[0] ; \n"  );
	fprintf(myfile, "#endif \n" );
	fprintf(myfile, "} \n\n" );

	//for arrays

    fprintf(myfile, "#ifdef parallel_mode \n" );
    fprintf(myfile, " const int for_delta = (for_end-for_init)/par_num; //%d / %d =%d \n" ,(for_end-for_init), new_default_par, (for_end-for_init)/new_default_par  );
    fprintf(myfile, " const int for_delta_by4 = for_delta/4; //%d \n" , (for_end-for_init)/(new_default_par*4));
    fprintf(myfile, " const ap_int<32> for_array_init[par_num ] = { " );
       for(int i =0; i< new_default_par-1 ; i++ ){
    fprintf(myfile, "  mem_start_adress+for_init+%d *for_delta , " , i);
       }
    fprintf(myfile, "  mem_start_adress} ; \n" );

    fprintf(myfile, " const ap_int<32> for_array_parstart[par_num ] = { " );
       for(int i =0; i< new_default_par-1 ; i++ ){
    fprintf(myfile, " mem_start_adress+ for_init+%d *for_delta , " , i);
       }
    fprintf(myfile, "  mem_start_adress+for_init+%d *for_delta } ; \n " , new_default_par-1 );

        fprintf(myfile, " const ap_int<32> for_array_end[par_num ] = { " );
       for(int i =new_default_par-1; i> 0  ; i-- ){
    fprintf(myfile, "  mem_start_adress+for_end-%d *for_delta , " , i);
       }
    fprintf(myfile, " mem_start_adress+ for_end}; \n\n " );

// mempar

    int for_delta_by4 = (for_end-for_init)/(new_default_par*4);
        fprintf(myfile, "  static ap_int<8> mem_par [par_num][for_delta_by4][4] = {\n " );

       for(int i =0; i< new_default_par  ; i++ ){
        fprintf(myfile, "  { " );

           for(int j =0; j< for_delta_by4   ; j++ ){
            fprintf(myfile, "  { " );
            for(int k =0; k< 4  ; k++ ){
                    if(k == 3){
                        fprintf(myfile, "  %d ", mem[for_init/4+i*for_delta_by4+j][k] );
                    }else {
                        fprintf(myfile, "  %d, ", mem[for_init/4+i*for_delta_by4+j][k] );
                    }
           }
            if(j == for_delta_by4 -1 ){
                    fprintf(myfile, "  } " );

            }else {
                    fprintf(myfile, "  }, " );

            }
           }
            if(i == new_default_par -1 ){
                fprintf(myfile, "  } \n" );

            }else {
                fprintf(myfile, "  }, \n" );

            }
           }
                fprintf(myfile, "  }; \n" );

           fprintf(myfile, "  #else \n" );
           fprintf(myfile, "  const ap_int<32> for_array_init[par_num ]  ={mem_start_adress}; \n" );


    	fprintf(myfile, "#endif \n\n" );


	fclose(myfile);

	//std::cout << "Press any key then enter to end program:"  << std::endl  ;
	//std::cin >> temp ;
	return 0;
	}
