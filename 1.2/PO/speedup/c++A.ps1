$compile = "g++ Floyd.cpp -O0 --std=c++11 -o Floyd_noopt.exe -Wl',--stack',10000000"
$command = ".\Floyd_noopt.exe"