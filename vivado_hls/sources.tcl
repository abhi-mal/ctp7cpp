## Set the top level module
set_top algo_unpacked

#### Add source code
add_files src/algo_unpacked.cpp -cflags "-std=c++0x"
add_files src/${project_name}.cpp -cflags "-std=c++0x"
add_files src/et_3by3.cpp -cflags "-std=c++0x"
add_files src/PU_LUT.cpp -cflags "-std=c++0x"
add_files src/bitonicFun64.cpp -cflags "-std=c++0x"
add_files src/bitonicSort64.cpp -cflags "-std=c++0x"
add_files src/bitonic32Inc.cpp -cflags "-std=c++0x"
add_files src/bitonic32Dec.cpp -cflags "-std=c++0x"

### Add testbed files
add_files -tb src/algo_unpacked_tb.cpp  -cflags "-std=c++0x" 

### Add test input files
add_files -tb data/test3_inp.txt
add_files -tb data/test3_out_ref.txt
