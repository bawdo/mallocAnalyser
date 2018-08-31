
A simple Heap Analyser
---------------------

![picture alt](https://github.com/gokulvasan/mallocAnalyser/blob/master/DonaldThePlumber.jpg "Leak Fixer")

**Author:** Gokul Vasan

Addresses two kind of memory leak:

* **Double malloc:** same pointer is malloc assigned sequentially twice without a free in betweeen.
* **Forgotten Free:** Malloced but forgotten.

Warning: Its still a lazy implementation, needs many potential improvements.

Procedure:
-----------
	* gcc malloc_analyser.c -o malloc_analyser
	* alloc_analyser.sh <File containing the malloc trace>
	
Malloc trace should be in the format:
-------------------------------------
	* [MALLOC_DBG], address-id, file, function, line, size, type, ptr
	* The non-string variables are expected to be %lld or %llu. 
	* please refer the test.c's my_malloc and my_free for reference.
	
What will this spit:
--------------------
	* The possible locations of the leak.
	* legend: [MALLOC_DBG], address-id, file, function, line, size, type, ptr
```

Possible Leaks in:
[MALLOC_DBG], 10638, test.c, growAndShrink, 62, 4, 1, 94186296386144
[MALLOC_DBG], 10638, test.c, growAndShrink, 62, 4, 1, 94186296390288
[MALLOC_DBG], 10638, test.c, growAndShrink, 62, 4, 1, 94186296390320
[MALLOC_DBG], 10638, test.c, growAndShrink, 62, 4, 1, 94186296390352
[MALLOC_DBG], 10638, test.c, growAndShrink, 62, 4, 1, 94186296390384
[MALLOC_DBG], 10638, test.c, growAndShrink, 62, 4, 1, 94186296390416
[MALLOC_DBG], 10638, test.c, growAndShrink, 62, 4, 1, 94186296390448
[MALLOC_DBG], 10638, test.c, growAndShrink, 62, 4, 1, 94186296390480
[MALLOC_DBG], 10638, test.c, growAndShrink, 62, 4, 1, 94186296390512
[MALLOC_DBG], 10638, test.c, growAndShrink, 62, 4, 1, 94186296390544
[MALLOC_DBG], 10638, test.c, growAndShrink, 62, 4, 1, 94186296390576
[MALLOC_DBG], 10638, test.c, growAndShrink, 62, 4, 1, 94186296390608
[MALLOC_DBG], 10638, test.c, growAndShrink, 62, 4, 1, 94186296390640
[MALLOC_DBG], 10638, test.c, growAndShrink, 62, 4, 1, 94186296390672
[MALLOC_DBG], 10638, test.c, growAndShrink, 62, 4, 1, 94186296390704
[MALLOC_DBG], 10638, test.c, growAndShrink, 62, 4, 1, 94186296390736
[MALLOC_DBG], 10638, test.c, growAndShrink, 62, 4, 1, 94186296390768
[MALLOC_DBG], 10638, test.c, growAndShrink, 62, 4, 1, 94186296390800
[MALLOC_DBG], 10638, test.c, growAndShrink, 62, 4, 1, 94186296390832
[MALLOC_DBG], 10638, test.c, growAndShrink, 62, 4, 1, 94186296390864

```
	
TODO:
------
	* Improve speed.
	* Add statistics functionality.
	* Add Temporal sequencing.
	* Use size as one among the delimiter, enabling reduced search space.

please refer the test.c's my_malloc and my_free for reference.


