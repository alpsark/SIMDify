#!/bin/sh
now=$(date +"%T")
echo "$now"
cd ..

#  generate define_instructions.h
echo ".................generating define_instructions.h"
cd   ./Define_instruction.c 
bash ./define_instructions.sh
cd  ..  

#  generate new_instructions.h
echo ".................generating new_instructions.h"
bash ./_UserDefinedPrimitives/call_vivado.sh

#  parallel.h
echo ".................generating parallel.h"
cd   ./parallel.c 
bash ./parallel.sh


cd  ..  

#syntesize generated SIMD core
echo ".................syntesizing design"
bash ./_UserDefinedPrimitives/call_vivado_syn.sh


cd ./_UserDefinedPrimitives
now=$(date +"%T")
echo "process finished in $now"
