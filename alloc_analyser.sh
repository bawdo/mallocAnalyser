#! /usr/bin/env bash

SEARCH="[MALLOC_DBG]"
DIR="./tmp"
LEVEL_ONE="gen.txt"
PROCESS="pid_"
PROCESS_FILES=${DIR}/${PROCESS}
PTR="ptr_"
DIR1="_dir"
LEAK_FILE="leak_file.txt"



if [ -d $DIR ]; then
	rm -rf $DIR
fi

mkdir $DIR

cat $1 | grep $SEARCH > ${DIR}/${LEVEL_ONE}

# segregate based on process id
./malloc_analyser -i ${DIR}/${LEVEL_ONE} -p ${DIR} -n ${PROCESS} -l 1 -o 2

for f in ${PROCESS_FILES}*; do
       #echo "processing $f"
       mkdir ${f}_dir
       ./malloc_analyser -i ${f} -p ${f}_dir -n ${PTR} -l 7 -o 2
done

for f in ${PROCESS_FILES}* ; do

	if [ -d $f ]; then
		for j in $f/*; do
			#echo "processing the $j"
			./malloc_analyser -i ${j} -l 6 -o 0 >> ${DIR}/${LEAK_FILE}
		done
	fi
done

echo "Possible Leaks in:"
cat ${DIR}/${LEAK_FILE} | xargs cat
