g++ test_ringbuffer.t.cpp -std=c++2a -I ~/boost_1_75_0 -L ~/boost_1_75_0/stage/lib -lboost_unit_test_framework -static -o test_ringbuffer.tsk
echo "compiled test_ringbuffer.tsk"

g++ test_bytearray.t.cpp -std=c++2a -I ~/boost_1_75_0 -L ~/boost_1_75_0/stage/lib -lboost_unit_test_framework -static -o test_bytearray.tsk
echo "compiled test_bytearray.tsk"

g++ test_timestamp.t.cpp -std=c++2a -I ~/boost_1_75_0 -L ~/boost_1_75_0/stage/lib -lboost_unit_test_framework -static -o test_timestamp.tsk
echo "compiled test_timestamp.tsk"
