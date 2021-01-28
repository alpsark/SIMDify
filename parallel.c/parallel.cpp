#include <iostream>
#include <vector>
#include <stdio.h>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <algorithm>
#include <sstream>
#include <string.h>


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

int final_end_core [number_of_parloops];
int final_start_core [number_of_parloops];

using namespace std; 

int main()
{
	
    std::vector <uint32_t> def_bit_inst_vector;
	std::string temp,line;
	std::ifstream inputData_fp,inputData,inputData2,inputData3;
	std::string filepath_filepath ="../_UserDefinedPrimitives/parallel_primitives.txt" ;
    bool activate_cache = false;
    bool override_default_par = false;
    int new_default_par [50] ;
    bool is_new_core [50] ;
	int last_new_core_parnum = 0;
    int total_par_num = 0 ;
    int extramemsize = 0;
    int local_mem_size = 1;
	const char s[2] = " ";
	char * ptr_name_fp = &filepath_filepath[0];
	inputData_fp.open(ptr_name_fp);
	int count =0 ;
	
	//read parallel_primitives.txt
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
			else if (i_fp == 2){
				char * cstr = new char [temp.length()+1];
				strcpy (cstr, temp.c_str());
				
				char * token = strtok(cstr, s);

				
				while( token != NULL ) 
				{
					new_default_par[count]=atoi(token);
					count++;
					token = strtok(NULL, s );
				}
			}
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
	bool no_parallel = false ;
	
	if(!override_default_par || new_default_par[0] == 1) {	
		if (new_default_par[0] == 1)// temp solution if first is 1 all is non parallel
			no_parallel = true;  
			
		for (int i=0; i<number_of_parloops; i++){ 
        new_default_par[i] = Par_num_from_c[i];	
		final_start_core[i] =  start_core[i];
		final_end_core[i] =  end_core[i];
		
		}

		
	}else {
		for (int i=0; i<number_of_parloops; i++){ 
			for (int j=0; j<i; j++) {
				//same cores must have same parallelization
				if (start_core[i] == start_core[j] 	){
					if (new_default_par[i] != 	new_default_par[j]) {
						printf("PROBLEM, parallel loops  %d  and %d  use the same memory but its not divided same   \n" ,i ,j );
						fprintf(myfile, "PROBLEM, parallel loops  %d  and %d  use the same memory but its not divided same   \n" ,i ,j );
						
						fclose(myfile);
						return 9;				
					}			
					break;
				}
			}				
		}	
		
		for (int i=0; i<number_of_parloops; i++){ 
			bool is_used_multiple_times = false; 

			for (int j=0; j<i; j++) {
				//same cores must have same parallelization
				if (start_core[i] == start_core[j] 	){
					is_used_multiple_times = true; 
					final_start_core[i] =  final_start_core[j];
					final_end_core[i] =  final_end_core[j];
					break;
				}
			}	
			
			if (!is_used_multiple_times) {
				if (i != 0){
					final_start_core[i] = final_end_core[i-1] +1 ;
					}
				
				final_end_core[i] = final_start_core[i] + new_default_par[i] -1 ;

				}
			
			
		}	
		

			

	}


	//start core
	
	fprintf(myfile," \n const int final_start_core [%d]  = { " ,number_of_parloops);
	for (int i=0; i <number_of_parloops ; i++)
		fprintf(myfile," %d," ,final_start_core[i]);
	fprintf(myfile," } ; //cores starts at 0, if its i take i+Par_num_from_c-1  cores)" );


	//end_core
	fprintf(myfile," \n const int final_end_core [%d]  = { " ,number_of_parloops);
	for (int i=0; i <number_of_parloops ; i++)
		fprintf(myfile," %d," ,final_end_core[i]);
	fprintf(myfile," } ; //cores starts at 0, if its i take i+Par_num_from_c-1  cores)" );

	// par_num_from_c
	
	fprintf(myfile," \n const int final_Par_num [%d]  = { " ,number_of_parloops);
	for (int i=0; i <number_of_parloops ; i++)
		fprintf(myfile," %d," ,new_default_par[i]);
	fprintf(myfile," } ; // overwritten par_num value in the c code \n " );
	
			
	if (no_parallel)
		fprintf(myfile, "//#define  parallel_mode  \n" );
	else 
		fprintf(myfile, "#define  parallel_mode  \n" );
	


	//check divisibility
	for (int i=0; i<number_of_parloops; i++) {
		if(((for_end[i]-for_init[i])/4)%new_default_par[i] != 0){
			printf("PROBLEM, PAR %d IS NOT DIVISIBLE  \n" ,i );
			fprintf(myfile, "PROBLEM, PAR %d IS NOT DIVISIBLE  \n",i );
			fclose(myfile);
			return 8;
		}
		
		is_new_core [i] = true ; 
		for (int j=0; j<i; j++) {
			//same cores must have same parallelization
			if (final_start_core[i] == final_start_core[j] 	){
				if (new_default_par[i] != 	new_default_par[j]) {
					printf("PROBLEM, parallel loops  %d  and %d  use the same memory but its not divided same   \n" ,i ,j );
					fprintf(myfile, "PROBLEM, parallel loops  %d  and %d  use the same memory but its not divided same   \n" ,i ,j );
					
					fclose(myfile);
					return 9;				
				}
				
				is_new_core [i] = false ; 
				
				break;
			}
		}
		
		if (is_new_core [i])
			last_new_core_parnum=i;
		
		
	}
		
	
	total_par_num = *max_element (final_end_core,final_end_core+number_of_parloops  )+1 ;
	
	
    if (activate_cache){
		fprintf(myfile, "#define  activatecache  \n" );
		}else {
		fprintf(myfile, "//#define  activatecache  \n" );
		extramemsize = 1;
	}
	
	fprintf(myfile, "const int extramem_size = %d ; // size of outside memory 32xextramem_size \n" ,extramemsize );
    fprintf(myfile, "const uint32_t addlutsize = %d ; // size of local memory  \n" ,local_mem_size);
	
	//parnum
	fprintf(myfile, "#ifdef parallel_mode  \n" );
	fprintf(myfile, "const int par_num = %d ;//par_num \n",total_par_num  );
	fprintf(myfile, "#else \n" );
	fprintf(myfile, "const int par_num = 1 ;//par_num \n"  );
	fprintf(myfile, "#endif \n\n" );
	
	
	//memorybits
	fprintf(myfile, "#ifdef parallel_mode  \n" );
	fprintf(myfile, "typedef enum {  \n" );
	//fprintf(myfile, "0_0 = 0,  \n" );	
	int last_par ; 
	for (int i=0;i<number_of_parloops;i++) {
	    if (is_new_core[i]) {
			for (int j=0;j<new_default_par[i];j++){
				if (i == 0 && j == 0){
					fprintf(myfile, "_%d_%d = 0, \n",i,j  );	
					}else {
					fprintf(myfile, "_%d_%d , \n",i,j  );	
				}
				
				last_par = i;
			}			
		}
	}
	fprintf(myfile, "ext \n" );
	fprintf(myfile, "} memorybits; \n" );
	fprintf(myfile, "#else \n" );
	fprintf(myfile, "typedef enum { \n"  );
    fprintf(myfile, "_0_0 = 0,  \n" );
    fprintf(myfile, "ext \n" );
	fprintf(myfile, "} memorybits; \n" );
	fprintf(myfile, "#endif \n\n" );
	
	
	//addlut
    fprintf(myfile, "#ifdef parallel_mode  \n" );
    fprintf(myfile, "const memorybits addrlut[%d]={ ",local_mem_size/4 );
    int  addrlut[local_mem_size/4];
	

	
	//int for_init_start = for_init[0]/4;
	//int for_init2_ = for_init[number_of_parloops-1]/4;
    //int for_end_ = for_end[number_of_parloops-1]/4;
	
	int for_delta_   ;
	
	for (int i = 0;i<local_mem_size/4;i++){	
		bool is_partitioned_mem = false; 

		for (int j = 0;j<number_of_parloops;j++){		
			if (is_new_core[j] ) {				
				for_delta_ = (for_end[j]/4-for_init[j]/4)/new_default_par[j]  ;				
				if (i >= for_init[j]/4  &&  i < for_end[j]/4  ) { // check if its between 				
					writeaddlut(i,1,addrlut,for_delta_,for_init[j]/4 );					
					fprintf(myfile,"_%d_%d,",j,addrlut[i]);
					is_partitioned_mem = true; 
					break;					
				}	
			}		
		}
		
		if (!is_partitioned_mem) 
			fprintf(myfile,"_%d_%d,", last_par,new_default_par[last_par]-1 );			
		
		/* if ( i < for_init_start) {
			fprintf(myfile,"_%d_%d,",number_of_parloops-1,new_default_par-1);
			
            //addrlut[i] =  new_default_par-1;
			}else if ( i >= for_end_ - for_delta_) {
            //addrlut[i] =  new_default_par-1;
			fprintf(myfile,"_%d_%d,",number_of_parloops-1,new_default_par-1);
			
			}else {
			bool else_case = true;
			for (int j=0; j<number_of_parloops; j++) {				
			int for_init_temp = for_init[j]/4;
			int for_end_temp  = for_end[j]/4;
			int for_delta_temp  = (for_end_temp-for_init_temp)/new_default_par  ;
			
			if (i >= for_init_temp && i < for_end_temp){//inside
			writeaddlut(i,1,addrlut,for_delta_temp,for_init_temp );					
			fprintf(myfile,"_%d_%d,",j,addrlut[i]);
			else_case = false;
			break;
			}
			
			}
			if (else_case) {
			fprintf(myfile,"_%d_%d,",number_of_parloops-1,new_default_par-1);
			
			}
		}*/
	}
	

	
    /*for(int i =0; i< local_mem_size/4 ; i++ ){
        fprintf(myfile,"_%d,",addrlut[i]);
	}*/
    fprintf(myfile,"}; \n");
	fprintf(myfile, "#endif \n\n" );
			

			
	//getsetmacro
    fprintf(myfile, "#define GETSET_MACRO    	\\\n" );
	for(int i =0; i< total_par_num-2 ; i++ ){
        fprintf(myfile,"getset0<%d>(address[%d], mask,(isparmode_or_addr_arr[%d] )?OpType:NONE, valueToWrite[%d],result[%d] );    \\\n",i,i,i,i,i);
	}
	fprintf(myfile,"getset0<%d>(address[%d], mask,(isparmode_or_addr_arr[%d] )?OpType:NONE, valueToWrite[%d],result[%d] );     \n ",total_par_num-2,total_par_num-2,total_par_num-2,total_par_num-2,total_par_num-2);
	fprintf(myfile, " \n\n" );
	

			
	//SETRF_MACRO
    fprintf(myfile, "template <int dd> \n" );
    fprintf(myfile, "void SETRF_MACRO(memorybits rf_addr,ap_uint<6> rd,  ap_uint<32> * result , uint32_t rf[par_num][32]){ \n" );
    fprintf(myfile, "#pragma HLS inline \n" );
    fprintf(myfile, "#ifdef parallel_mode \n" );
    fprintf(myfile, "switch (rf_addr) { \n" );
	count = 0; 
	for(int i =0; i< number_of_parloops ; i++ ){
		if (is_new_core[i] ) {	
			for(int j =0; j< new_default_par[i] ; j++ ){
				fprintf(myfile, "case _%d_%d: \n",i,j );
				fprintf(myfile, "for(int i = 0;i<par_num;i++){ \n" );
				fprintf(myfile, "#pragma HLS UNROLL \n" );
				fprintf(myfile, " rf[i][rd] =  (uint32_t)result[%d] ; \n" ,count);
				fprintf(myfile, " }         break; \n" );
				count++;
			}
		}
	}
	fprintf(myfile, " default: \n" );
	fprintf(myfile, " break;\n" );
	fprintf(myfile, " }\n" );
	fprintf(myfile, "#else \n" );
	fprintf(myfile, " rf[0][rd] =  (uint32_t)result[0] ; \n"  );
	fprintf(myfile, "#endif \n" );
	fprintf(myfile, "} \n\n" );
	


	/////////////for arrays
	
	//for_delta
    fprintf(myfile, "#ifdef parallel_mode \n" );
	fprintf(myfile, " const int for_delta [%d] = {" , number_of_parloops );
	
	for(int i =0; i< number_of_parloops ; i++ ){

    fprintf(myfile, "(for_end[%d]-for_init[%d])/(final_Par_num[%d]), " ,i,i,i  );
		 
	}
	
	fprintf(myfile, "} ; // " );


	int largest_fordeltaby4 = 0; 
	for(int i =0; i< number_of_parloops ; i++ ){
	int fordeltaby4 = (for_end[i]-for_init[i])/(new_default_par[i]);
	if (fordeltaby4 > largest_fordeltaby4){
		largest_fordeltaby4 = fordeltaby4;
	}
    fprintf(myfile, " %d , " ,fordeltaby4 );
	
	}
	fprintf(myfile, "\n  " );
	
	largest_fordeltaby4 = largest_fordeltaby4/4; 
	
    fprintf(myfile, " const int for_delta_by4 = %d; //largest for_delta/4  \n \n" , largest_fordeltaby4);
	

				
	//for_array_init
	
    fprintf(myfile, " const ap_int<32> for_array_init[par_num] = { \n" );
	for (int j =0; j< number_of_parloops ; j++ ){
	    //fprintf(myfile, "  {  " );
		if (is_new_core[j]) {
			for(int i =0; i< new_default_par[j] ; i++ ){	   
				if (j == last_new_core_parnum && i==new_default_par[j]-1 )				
				fprintf(myfile, "  mem_start_adress " );
				else
				fprintf(myfile, "  mem_start_adress+for_init[%d]+%d *for_delta[%d] ,\n " ,j, i,j);
			}
			//fprintf(myfile, "  } , " );
		}
	}
    fprintf(myfile, "  } ; \n" );
	
	
	//for_array_parstart
		
		
	/*  fprintf(myfile, " const ap_int<32> for_array_parstart[par_num ] = { " );
		for(int i =0; i< new_default_par-1 ; i++ ){
		fprintf(myfile, " mem_start_adress+ for_init+%d *for_delta , " , i);
		}
	fprintf(myfile, "  mem_start_adress+for_init+%d *for_delta } ; \n " , new_default_par-1 );*/
	
    fprintf(myfile, " const ap_int<32> for_array_parstart[par_num ] = { \n" );
	for (int j =0; j< number_of_parloops ; j++ ){
	    //fprintf(myfile, "  {  " );
		if (is_new_core[j]) {
			for(int i =0; i< new_default_par[j] ; i++ ){	   
				fprintf(myfile, "  mem_start_adress+for_init[%d]+%d *for_delta[%d] ,\n " ,j, i,j);
			}
			//fprintf(myfile, "  } , " );
		}
	}
    fprintf(myfile, "  } ; \n" );
	

	
	//for_array_end
	
	/*   fprintf(myfile, " const ap_int<32> for_array_end[par_num ] = { " );
		for(int i =new_default_par-1; i> 0  ; i-- ){
		fprintf(myfile, "  mem_start_adress+for_end-%d *for_delta , " , i);
		}
	fprintf(myfile, " mem_start_adress+ for_end}; \n\n " );*/
	

	fprintf(myfile, " const ap_int<32> for_array_end[par_num ] = { \n" );
	for (int j =0; j<number_of_parloops  ; j++ ){
	    //fprintf(myfile, "  {  " );
		if (is_new_core[j]) {
			for(int i =new_default_par[j]-1; i>= 0 ; i-- ){	   
				fprintf(myfile, "  mem_start_adress+for_end[%d]-%d *for_delta[%d] ,\n " ,j, i,j);
			}
			//fprintf(myfile, "  } , " );
		}
	}
    fprintf(myfile, "  } ; \n\n" );
	

	// mempar
	
	fprintf(myfile, "  static ap_int<8> mem_par [par_num ][for_delta_by4][4] = {\n " );
	for (int z =0; z< number_of_parloops ; z++ ){
		// fprintf(myfile, "  { " );
		 if (is_new_core[z]) {
			for(int i =0; i< new_default_par[z]  ; i++ ){
				fprintf(myfile, "  { " );
				int for_delta_by4 = (for_end[z]-for_init[z])/(new_default_par[z]*4);
				for(int j =0; j< largest_fordeltaby4   ; j++ ){
					fprintf(myfile, "  { " );
					if (j <= for_delta_by4){
						for(int k =0; k< 4  ; k++ ){
							if(k == 3){
								fprintf(myfile, "  %d ", mem[for_init[z]/4+i*for_delta_by4+j][k] );
								}else {
								fprintf(myfile, "  %d, ", mem[for_init[z]/4+i*for_delta_by4+j][k] );
							}
						}
					}else {
						fprintf(myfile, "  9,   9,   9,   9 " );
					}
					
					fprintf(myfile, "  }, " );
						
					
				}
				
				fprintf(myfile, "  }, \n" );
				
				
			}
			//fprintf(myfile, "  }, \n" );
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
