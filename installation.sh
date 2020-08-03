echo "TEMPLATE = subdirs\nSUBDIRS += decoder/morse_conseq_decode.pro\nSUBDIRS += graph_visualization/graph_visualization.pro"> ./temp_build.pro 

qmake
make
cp decoder/morse_conseq_decode morse_decoder
cp decoder/morse	morse
cp graph_visualization/graph_visualization graphic_visualization

rm temp_build.pro
rm Makefile

python gen_test_code.py
echo "\n\nInstalation's over. 2 executables, namely \"morse_decoder\" and \"graphic_visualization\" generated and copied from the coresponding folders. Aslo 3 additional files were created. File \"test_code\" provides a testing example for the program.File \"gen_test_code.py\" allows you to automaticly generate new \"test_code\" files. File \"morse\" is just a morse table for reference."


