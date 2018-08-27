
A simple Heap Analyser
---------------------

Warning: Its still is a lazy state, needs many potential improvements.

Procedure:
	gcc malloc_analyser.c -o malloc_analyser
	alloc_analyser.sh <File containing the malloc trace>

Malloc trace should be in the format 
	[MALLOC_DBG] File, address-id, function, line, type, size, ptr

please refer the test.c's my_malloc and my_free for reference.


