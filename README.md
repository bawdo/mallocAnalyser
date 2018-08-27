
A simple Heap Analyser
---------------------

Author: Gokul Vasan

Warning: Its still is a lazy state, needs many potential improvements.

Procedure:
-----------

	* gcc malloc_analyser.c -o malloc_analyser
	* alloc_analyser.sh <File containing the malloc trace>
What will this spit:
--------------------
	* File locations containing the traces where the potential leak should be.

Malloc trace should be in the format:
-------------------------------------
	*[MALLOC_DBG] File, address-id, function, line, type, size, ptr
	
TODO:
------
	* Improve speed.
	* Add statistics functionality.
	* Add Temporal sequencing.

please refer the test.c's my_malloc and my_free for reference.


