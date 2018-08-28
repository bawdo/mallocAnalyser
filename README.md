
A simple Heap Analyser
---------------------

![picture alt](https://github.com/gokulvasan/mallocAnalyser/blob/master/DonaldThePlumber.jpg "Leak Fixer")

Author: Gokul Vasan

Addresses two kind of memory leak:

* Double malloc: same pointer is malloc assigned sequentially twice without a free in betweeen.
* Forgotten Free: Malloced but forgotten.

Warning: Its still a lazy implementation, needs many potential improvements.

Procedure:
-----------
	* gcc malloc_analyser.c -o malloc_analyser
	* alloc_analyser.sh <File containing the malloc trace>
	
Malloc trace should be in the format:
-------------------------------------
	* [MALLOC_DBG], address-id, file, function, line, type, size, ptr
	* The variables should be %lld. 
	* please refer the test.c's my_malloc and my_free for reference.
	
What will this spit:
--------------------
	* File locations containing the traces where the potential leak should be.
	
TODO:
------
	* Improve speed.
	* Add statistics functionality.
	* Add Temporal sequencing.

please refer the test.c's my_malloc and my_free for reference.


