g++ test_buffer.cpp -std=c++2a -I ~/boost_1_75_0 -L ~/boost_1_75_0/stage/lib -lboost_unit_test_framework -static -o test_buffer.tsk
echo "compiled test_buffer.tsk"

g++ test_bytearray.cpp -std=c++2a -I ~/boost_1_75_0 -L ~/boost_1_75_0/stage/lib -lboost_unit_test_framework -static -o test_bytearray.tsk
echo "compiled test_bytearray.tsk"

g++ test_timestamp.cpp -std=c++2a -I ~/boost_1_75_0 -L ~/boost_1_75_0/stage/lib -lboost_unit_test_framework -static -o test_timestamp.tsk
echo "compiled test_timestamp.tsk"

g++ test_packing.cpp -std=c++2a -I ~/boost_1_75_0 -L ~/boost_1_75_0/stage/lib -lboost_unit_test_framework -static -o test_packing.tsk
echo "compiled test_packing.tsk"
