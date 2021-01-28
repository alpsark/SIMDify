
prompt $d $t $_$P$G

cd ..

Rem  generate define_instructions.h
echo ".................generating define_instructions.h"
cd   ./Define_instruction.c   
mkdir  obj\Debug
mkdir  bin\Debug
g++ -Wall -g -std=c++11  -c define_instruction.cpp -o obj\Debug\define_instruction.o
g++  -o bin\Debug\define_instruction.exe obj\Debug\define_instruction.o 
START ./bin/Debug/Define_instruction.exe
cd  ..  

Rem  generate new_instructions.h
echo ".................generating new_instructions.h"
CALL ./_UserDefinedPrimitives/call_vivado.bat

Rem  parallel.h
echo ".................generating parallel.h"
cd   ./parallel.c  
mkdir  obj\Debug
mkdir  bin\Debug
g++ -Wall -g -std=c++11  -c parallel.cpp -o obj\Debug\parallel.o
g++  -o bin\Debug\Parallel.exe obj\Debug\parallel.o   
START ./bin/Debug/Parallel.exe
cd  ..  


Rem syntesize generated SIMD core
echo ".................syntesizing design"
CALL ./_UserDefinedPrimitives/call_vivado_syn.bat

cd ./_UserDefinedPrimitives

echo "process finished"

