BEGINNANO=$(date +%s%N)

PROGRAM=${@: -2: 1}
TESTDIR=${@: -1: 1}

SHFLAGS=${@: 1: $(($#-2))}

INPUTFLAGS= 
MEMCHECK=false
TIMES=false

if [[ $SHFLAGS == *"-v"* ]]; then INPUTFLAGS=-v; fi #checks program's debug output
if [[ $SHFLAGS == *"-m"* ]]; then MEMCHECK=true; fi #runs an additional execution supervised by valgrind
if [[ $SHFLAGS == *"-t"* ]]; then TIMES=true; fi    #prints out execution times for all tests 

for test in $TESTDIR/*.in; do 
	testname=$(basename "${test/.in/}") 

	if [[ $MEMCHECK == true ]]; then 
		beginNano=$(date +%s%N)
		valgrind -q --log-file="buf.out" ./$PROGRAM $INPUTFLAGS < $test &> /dev/null 
		elapsed=$((($(date +%s%N) - $beginNano) / 100))
		elapsedSecs=$(($elapsed / 10000000))
		elapsedNano=$(($elapsed % 10000000))
		cat buf.out
		if [[ $TIMES == true ]]; then printf "%s memchckd in %d.%07d seconds\n" $testname $elapsedSecs $elapsedNano; fi
	fi

	beginNano=$(date +%s%N)
	./$PROGRAM $INPUTFLAGS < $test > buf.out 2> buf.err
	elapsed=$((($(date +%s%N) - $beginNano) / 100))
	elapsedSecs=$(($elapsed / 10000000))
	elapsedNano=$(($elapsed % 10000000))
	if [[ $TIMES == true ]]; then printf "%s computed in %d.%07d seconds\n" $testname $elapsedSecs $elapsedNano; fi

	diff -q buf.out ${test/in/out} 
	if [ $? -ne 0 ]; then echo "$(basename "${test/.in/}") - bad standard out!"; fi

	if [[ $INPUTFLAGS == -v ]]; then 
		diff -q buf.err ${test/in/err}  
		if [ $? -ne 0 ]; then echo "$(basename "${test/.in/}") - bad diagnostic out!"; fi
	fi 
done

rm buf.out buf.err

elapsed=$((($(date +%s%N) - $BEGINNANO) / 100))
elapsedSecs=$(($elapsed / 10000000))
elapsedNano=$(($elapsed % 10000000))
	
if [[ $TIMES == true ]]; then printf "\nTotal testing time: %d.%07d seconds\n" $elapsedSecs $elapsedNano; fi

