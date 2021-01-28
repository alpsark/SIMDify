
cd ..

cd   ./parallel.c   
g++ -Wall -g -std=c++11  -c parallel.cpp -o obj\Debug\parallel.o
g++  -o bin\Debug\Parallel.exe obj\Debug\parallel.o   
START ./bin/Debug/Parallel.exe

cd  ..  

Rem syntesize generated SIMD core
echo ".................syntesizing design"
CALL ./_UserDefinedPrimitives/call_vivado_syn.bat


cd ./_UserDefinedPrimitives

