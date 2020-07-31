echo "TEMPLATE = subdirs\nSUBDIRS += decoder/morse_conseq_decode.pro\nSUBDIRS += graph_visualization/graph_visualization.pro"> ./temp_build.pro 

qmake
make
cp decoder/morse_conseq_decode morse_decoder
cp decoder/morse	morse
cp graph_visualization/graph_visualization graphic_visualization

rm temp_build.pro
rm Makefile

echo ".\n..\n...\n-.-.-\n--..-.-\n-.ty\nqwe\n-..-.\n------\n\n\n------------\n..-.\n..--\n--.---.--.--\n..-" > ./test_code

echo "\n\nInstalation's over. 2 executables, namely \"morse_decoder\" and \"graphic_visualization\" generated and copied from the coresponding folders. Aslo 2 additional files were created. File \"test_code\" provides a testing example for the program. File \"morse"\ is just a morse table for reference."


