g++ test_ringbuffer.t.cpp -std=c++17 -I /home/daowen/boost_1_75_0 -L /home/daowen/boost_1_75_0/stage/lib -lboost_unit_test_framework -static -o test_ringbuffer.tsk
echo "compiled test_ringbuffer.tsk"

g++ test_bytearray.t.cpp -std=c++17 -I /home/daowen/boost_1_75_0 -L /home/daowen/boost_1_75_0/stage/lib -lboost_unit_test_framework -static -o test_bytearray.tsk
echo "compiled test_bytearray.tsk"
