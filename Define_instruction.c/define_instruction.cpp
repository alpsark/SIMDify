#include <iostream>
#include <vector>
#include <stdio.h>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <algorithm>

//stoi requires c++11 standard
//#define UBUNTU
#ifdef UBUNTU
	#include <stdint.h>
#endif

int main()
{
	uint32_t inst;
	int i =0 ;
	int instruction_num = 0;

	std::vector <std::string> def_name_inst_vector,def_funct_vector,def_reg_vector ;
	std::vector <uint32_t> def_bit_inst_vector;
	std::string temp,line;
	std::ifstream inputData_fp,inputData,inputData2,inputData3;
	std::string filepath_filepath ="../_UserDefinedPrimitives/Define_instruction_filepath.txt" ;

	std::string filepath_LUT, filepath_HEX,filepath_GLUT,filepath_mem ;

    uint32_t mem_start_adress; // = 0x4000  ;
	int    memory_size ; //=  4*32*1024/4;
    uint32_t adress_par_start   ;
    uint32_t adress_par_num ;
    uint32_t adress_arr_str ;
    uint32_t adress_arr_end ;


	char * ptr_name_fp = &filepath_filepath[0];
	inputData_fp.open(ptr_name_fp);

	if(!inputData_fp.is_open()){
		std::cout << "ERROR COULD NOT OPEN filepath.txt\n"  ;
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
	filepath_LUT = temp;
	else if (i_fp == 1)
	filepath_GLUT = temp;
	else if (i_fp == 2)
	filepath_HEX = temp;
	else if (i_fp == 3)
	filepath_mem = temp;
    else if (i_fp == 4)
    mem_start_adress = std::stoi(temp);
    else if (i_fp == 5)
    memory_size = std::stoi(temp);
    else if (i_fp == 6)
    adress_par_start = std::stoi(temp);
    else if (i_fp == 7)
    adress_par_num = std::stoi(temp);
    else if (i_fp == 8)
    adress_arr_str = std::stoi(temp);
    else if (i_fp == 9)
    adress_arr_end = std::stoi(temp);

	getline (inputData_fp,temp) ;
	i_fp++;

	}

	}

	//default path to lut and hex
	/* ="../LUT.txt" ;
	std::string  ="../hex/1distance.hex" ;
	std::string  ="../GLUT.txt" ;//
	std::string  ="../hex/1dmem.hex" ; // */

	/**<std::string filepath_addr ="../hex/1mem.dump" ;*/

	//path to lut and hex
	// while(true){
	std::cout << "Default path for LUT.TXT: "  << filepath_LUT << std::endl  ;
	std::cout << "Default path for GLUT.TXT: "  << filepath_GLUT << std::endl  ;
	std::cout << "Default path for hex file: " << filepath_HEX << std::endl   ;
	std::cout << "Default path for mem file: " << filepath_mem << std::endl   ;
	/**<  std::cout << "Default path for mem dump file: " << filepath_addr << std::endl   ;*/
	/**<  D to change path of mem dump file,*/
	/**  std::cout << "Press L to change path of LUT file, G to change path of GLUT file, H to change path of hex file,M to change path of mem file, C to continue" << std::endl   ;
	getline(std::cin, temp);
	//std::cout << temp <<  std::endl   ;
	if(temp == "L" || temp =="l" ){
	std::cout << "Press enter to path of LUT file, press q if you changed you mind" << std::endl   ;
	getline(std::cin, temp);
	if(temp != "q" || temp =="Q")
	filepath_LUT = temp;
	}else if(temp == "g" ||temp == "G" ){
	std::cout << "Press enter to path of GLUT file, press q if you changed you mind" << std::endl   ;
	getline(std::cin, temp);
	if(temp != "q" || temp =="Q")
	filepath_GLUT = temp;
	}else if(temp == "h" ||temp == "H" ){
	std::cout << "Press enter to path of hex file, press q if you changed you mind" << std::endl   ;
	getline(std::cin, temp);
	if(temp != "q" || temp =="Q")
	filepath_HEX = temp;
	}else if(temp == "m" ||temp == "M" ){
	std::cout << "Press enter to path of mem file, press q if you changed you mind" << std::endl   ;
	getline(std::cin, temp);
	if(temp != "q" || temp =="Q")
	filepath_mem = temp;
	/**< } else if(temp == "d" ||temp == "D" ){
	std::cout << "Press enter to path of mem dump file, press q if you changed you mind" << std::endl   ;
	getline(std::cin, temp);
	if(temp != "q" || temp =="Q")
	filepath_addr = temp;*/
	/** }else if(temp == "c" || temp =="C" ){
	break;
	}else{
	std::cout << "Invalid character entered, please try again!" << std::endl   ;
	}

	}*/

	/**<
	//Open lut
	char * ptr_name = &filepath_addr[0];
	inputData3.open(ptr_name);
	if(!inputData3.is_open()){
	std::cout << "ERROR COULD NOT OPEN Mem.dump"  ;
	return 0;
	}
	getline (inputData3,temp) ;// get empty
	getline (inputData3,temp) ;// get 1knn.exe:     file format elf32-littleriscv
	getline (inputData3,temp) ;// get empty
	getline (inputData3,temp) ;// Contents of section .rodata:
	inputData3 >> mem_start_adress;// get start adress
	*/
	//TODO add Common File Dialog GUI https://msdn.microsoft.com/en-us/library/windows/desktop/bb776913%28v=vs.85%29.aspx#usage
	//Open lut
	char * ptr_name = &filepath_LUT[0];
	inputData.open(ptr_name);
	if(!inputData.is_open()){
	std::cout << "ERROR COULD NOT OPEN LUT.TXT\n"  ;
	}else{

	//Get Instructions in the LUT file to def_name_inst_vector and def_bit_inst_vector
	inputData >> temp;// get mnemonic
	std::cout << "Instructions in the LUT file:" << std::endl;
	while (temp != "ENDOFLUT")
	{
	if(temp == "#"){
	getline (inputData,temp) ;// get comment
	inputData >> temp;// get start of the next line
	continue;
	}
	def_name_inst_vector.push_back(temp);
	std::cout << temp << " " ;

	inputData >> temp;// 3 bit register code
	def_reg_vector.push_back(temp);


	inputData >> temp;// 3 bit funct code
	def_funct_vector.push_back(temp);
	//
	inputData >> temp;// 32 bit machine code
	char * ptr;
	uint32_t parsed = strtoul(temp.c_str(), & ptr, 2);
	//std::cout << parsed << std::endl ;
	printf("%08x \n" ,parsed);
	def_bit_inst_vector.push_back( parsed);
	inputData >> temp;//  mnemonic
	}
	std::cout << std::endl;
	}
	std::vector <std::string> def_name_group_vector ;
	std::vector <std::string> def_name_mnemonic_vector ;
	//Open glut
	ptr_name = &filepath_GLUT[0];
	inputData2.open(ptr_name);
	if(!inputData2.is_open()){
	std::cout << "ERROR COULD NOT OPEN GLUT.TXT\n"  ;
	}else{



	//Get Instructions in the GLUT file to def_name_group_vector and def_name_mnemonic_vector
	inputData2 >> temp;// get group
	std::cout << "Groups in the GLUT file:"  << std::endl;
	while (temp != "ENDOFLUT")
	{
	if(temp == "#"){
	getline (inputData2,temp) ;// get comment
	inputData2 >> temp;// get start of the next line
	continue;
	}
	inputData2 >> std::ws;
	getline(inputData2,line);
	std::string delimiter = "-";
	size_t pos = 0;
	std::string token;
	std::cout << temp <<  " : " ;
	while ((pos = line.find(delimiter)) != std::string::npos) {
	token = line.substr(0, pos);
	token.erase(std::find_if(token.rbegin(), token.rend(), std::bind1st(std::not_equal_to<char>(), ' ')).base(), token.end());
	token.erase(token.begin(), std::find_if(token.begin(), token.end(), std::bind1st(std::not_equal_to<char>(), ' ')));
	std::cout << token << " ";
	def_name_group_vector.push_back(temp);
	def_name_mnemonic_vector.push_back(token);
	line.erase(0, pos + delimiter.length());
	}
	line.erase(std::find_if(line.rbegin(), line.rend(), std::bind1st(std::not_equal_to<char>(), ' ')).base(), line.end());
	line.erase(line.begin(), std::find_if(line.begin(), line.end(), std::bind1st(std::not_equal_to<char>(), ' ')));
	def_name_group_vector.push_back(temp);
	def_name_mnemonic_vector.push_back(line);
	std::cout << line <<  std::endl;;

	inputData2 >> temp;//  mnemonic
	}
	std::cout << std::endl;
	}
	std::vector <std::string> mnemonic_vector ;
	std::vector<std::string>::iterator it;

	//Open hex
	std::vector <uint32_t> inst_vector ;
	ptr_name = &filepath_HEX[0];
	FILE * inst_file = fopen(ptr_name ,"r+b");
	if (inst_file == 0){
	printf("Hex file read error\n" );
	}else{

	//Get Instructions in the hex file and compare with lut to fill mnemonic_vector

	while( 1 == fread(&inst,sizeof(uint32_t),1,inst_file) ){
	inst_vector.push_back(inst);
	instruction_num = instruction_num +1;
	}

	std::cout << "There are "<< instruction_num  << " Instructions in the hex file:" << std::endl;



	while (i < inst_vector.size()){

	//printf("%x: %08x \n" ,i*4 , inst_vector[i]);
	int j =0 ;
	//PROBLEM if LH is not defined but LB is defined.
	std::string mnemonic = "0";
	//myvector.begin()
	while (j < def_bit_inst_vector.size()){
	uint32_t xored = inst_vector[i] ^ def_bit_inst_vector[j] ;
	// std::cout <<  << " " <<inst_vector[i] << def_bit_inst_vector[j] <<  std::endl ;
	if((xored << 25) == 0 ){ //check opcode first
	if(def_funct_vector[j][2] == '1'){ //check check func3
	if((((xored >> 12) << 29 ) == 0)){//check func3
	if(def_funct_vector[j][1]== '1'){//check check func7
	if((xored >> 25) == 0 ){ //check funct7
	mnemonic = def_name_inst_vector[j] ;
	break;
	}else
	j = j+1;
	continue;
	}else if(def_funct_vector[j][0] == '1' ){//check check func12
	if((xored >> 20) == 0 ){ //check funct12
	mnemonic = def_name_inst_vector[j] ;
	break;
	}else
	j = j+1;
	continue;
	}else{
	mnemonic = def_name_inst_vector[j] ;
	break;
	}
	}else
	j = j+1;
	continue;

	//mnemonic = def_name_inst_vector[j] ;
	}else {
	mnemonic = def_name_inst_vector[j] ;
	break;
	}
	//mnemonic = def_name_inst_vector[j] ;
	}
	j = j+1;
	}

	if(mnemonic == "0"){
	printf("%x: %08x is not defined in the LUT.txt\n" ,i*4 , inst_vector[i]);
	}else{
	printf("%x: %08x is " ,i*4 , inst_vector[i]);
	std::cout << mnemonic << std::endl ;
	it = find (mnemonic_vector.begin(), mnemonic_vector.end(), mnemonic);
	if (it == mnemonic_vector.end())
	mnemonic_vector.push_back(mnemonic);
	}
	i = i+1 ;
	}
	}

	printf("Inst end adress is %x and ",i);

	// uint32_t mem;
	uint8_t mem;
	i =0 ;
	//int    memory_size =  32*1024/4;



	//Open mem
	std::vector <uint8_t> mem_vector ;
	ptr_name = &filepath_mem[0];
	FILE * mem_file = fopen(ptr_name ,"r+b");
	if (mem_file == 0){
	printf("Mem file read error\n" );
	}else{

	while( 1 == fread(&mem,sizeof(uint8_t),1,mem_file) ){
	mem_vector.push_back(mem);
	}


	std::cout <<  "there are "<< mem_vector.size()  << " data in the mem file:" << std::endl;
	}

	if(!inputData.is_open() || !inputData2.is_open() || inst_file == 0 || mem_file == 0){
	printf("ERROR Some of the file(s) could not opened\n");
	}else{
	//type define using mnemonic_vector
	//std::ofstream myfile;
	FILE *myfile = fopen("../define_instructions.h", "w");
	FILE *myfile_c = fopen("../parallel.c/define_instructions.h", "w");

	int prev_size =  mnemonic_vector.size();


	fprintf(myfile, "//Hex file path %s  \n" ,  &filepath_HEX[0]);
	fprintf(myfile, "//Mem file path %s  \n" ,  &filepath_mem[0]);

	fprintf(myfile, "//required inst memory size \n");
    fprintf(myfile, "#include \"riscv_cache_l1.hpp\"  \n");

	fprintf(myfile, "static ap_int<32> inst_mem[ %d ] = { ",instruction_num);


	for(int i =0 ; i < instruction_num ; i++) {
	if(i == instruction_num-1 ){
	fprintf(myfile, "%u  ", inst_vector[i]);

	}else{
	fprintf(myfile, "%u , ", inst_vector[i]);
	}
	}
	uint32_t bytesize = (memory_size-mem_start_adress)/4;

	fprintf(myfile_c, "const uint32_t mem_start_adress  = %d  ;\n",mem_start_adress);
	fprintf(myfile_c, "const uint32_t whole_memory_size  = %d  ;\n",memory_size);
    fprintf(myfile_c, "const uint32_t data_memory_size  = %d  ;\n",memory_size-mem_start_adress);


	fprintf(myfile, "};\n\n" );
	fprintf(myfile, "//memory address offset  \n");
	fprintf(myfile, "const uint32_t mem_start_adress  = %d  ;\n",mem_start_adress);
	fprintf(myfile, "const uint32_t whole_memory_size  = %d  ;\n",memory_size);
    fprintf(myfile, "const uint32_t data_memory_size  = %d  ;\n",memory_size-mem_start_adress);

    fprintf(myfile, "const uint32_t par_start_address = %d   ;\n",adress_par_start);
    fprintf(myfile, "const uint32_t par_num_address    = %d   ;\n",adress_par_num);
    fprintf(myfile, "const uint32_t arr_str_address   = %d    ;\n",adress_arr_str);
    fprintf(myfile, "const uint32_t arr_end_address   = %d   ;\n",adress_arr_end);

	fprintf(myfile, "//required data memory size \n");
	fprintf(myfile, "static ap_int<8> mem[ %d ][4] = { ",bytesize);

	fprintf(myfile_c, "int mem[ %d ][4] = { ",bytesize);

	for(int i =0 ; i < bytesize*4 ; ) {
	if(i !=  bytesize*4-1 && i !=  0   ){
	fprintf(myfile, " , ");
	fprintf(myfile_c, " , ");

	}
	fprintf(myfile, " {");
	fprintf(myfile_c, " {");

	for(int j =0 ; j < 4 ; j++) {

	if(i<  mem_vector.size() ){
	//fprintf(myfile, "%u , ", mem_vector[i]);
	fprintf(myfile, "%u  ", mem_vector[i]);
	fprintf(myfile_c, "%u  ", mem_vector[i]);
	i++;
	}else{
	fprintf(myfile, "0  ");
	fprintf(myfile_c, "0  ");
		i++;
	}

	if(j != 3 ){
	fprintf(myfile, " , ");
	fprintf(myfile_c, " , ");

	}



	}
	fprintf(myfile, " }");
	fprintf(myfile_c, " }");

	}
	fprintf(myfile, "};\n\n" );
	fprintf(myfile_c, "};\n\n" );

	fprintf(myfile, "//LUT instructions \n" );


	for(i = 0 ;i < mnemonic_vector.size() ; i++){
	fprintf(myfile, "#define %s \n ", &mnemonic_vector[i][0]);

	if(i == prev_size-1){
	fprintf(myfile, "//GLUT instructions \n" );

	}
	int j = 0;
	while (j < def_name_mnemonic_vector.size()){
	if(def_name_mnemonic_vector[j] == mnemonic_vector[i]){
	it = find (mnemonic_vector.begin(), mnemonic_vector.end(), def_name_group_vector[j]);
	if (it == mnemonic_vector.end())
	mnemonic_vector.push_back(def_name_group_vector[j]);
	}
	j  = j+1;
	}
	}

	fclose(myfile);
	fclose(myfile_c);
	inputData.close();
	inputData2.close();
	}
	//std::cout << "Press any key then enter to end program:"  << std::endl  ;
	//std::cin >> temp ;
	return 0;
	}
