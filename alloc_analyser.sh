#! /usr/bin/env bash

SEARCH_STRING="MALLOC_DBG"
SEARCH="["${SEARCH_STRING}"]"
DIR="./tmp"
LEVEL_ONE="gen.txt"
PROCESS="pid_"
PROCESS_FILES=${DIR}/${PROCESS}
SIZE_BASED_DIR="size"

PTR="ptr_"
DIR1="_dir"
SIZE_FILE="size_"

LEAK_FILE="leak_file.txt"

PROCESS_ID_LOCATION=1
POINTER_LOCATION=7
TYPE_LOCATION=6
SIZE_LOCATION=5

OPERATION_BUCKET=2
OPERATION_ANALYSE=0
OPERATION_STAT=1


if [ -d $DIR ]; then
	rm -rf $DIR
fi

mkdir $DIR

#ground zero : cleanup the dirty log
cat $1 | grep $SEARCH | grep $SEARCH_STRING 1>${DIR}/${LEVEL_ONE} 2>/dev/null

#level one: segregate based on process id
./malloc_analyser -i ${DIR}/${LEVEL_ONE} -p ${DIR} -n ${PROCESS} -l 1 -o 2

#level two: Segreagate based on pointer
for f in ${PROCESS_FILES}*; do
       #echo "processing $f"
       mkdir ${f}_dir
       ./malloc_analyser -i ${f} -p ${f}_dir -n ${PTR} -l 7 -o 2
done

#level three: Segregate based on size from the pointer
for f in ${PROCESS_FILES}* ; do

	if [ -d $f ]; then
		#echo $f
		mkdir ${f}/${SIZE_BASED_DIR}${DIR1}
		for j in $f/*; do
			./malloc_analyser -i ${j} -p ${f}/${SIZE_BASED_DIR}${DIR1} -n ${SIZE_FILE}${f} -l ${SIZE_LOCATION} -o ${OPERATION_BUCKET} 
		done
	fi
done

#level four: Analyse the log
for f in ${PROCESS_FILES}* ; do

	if [ -d $f ]; then
		if [ -d ${f}/${SIZE_BASED_DIR}${DIR1} ]; then

			k=${f}/${SIZE_BASED_DIR}${DIR1}

			for j in $k/*; do
				#echo "analysing the $j"
				./malloc_analyser -i ${j} -l ${TYPE_LOCATION} -o 0 >> ${DIR}/${LEAK_FILE}
			done
		fi
	fi
done

echo "Possible Leaks in:"
cat ${DIR}/${LEAK_FILE} | xargs cat

