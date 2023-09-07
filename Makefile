All: bigfile slowcp
	@sleep 1
	touch All

bigfile: bigfile.c
	gcc -o bigfile bigfile.c

slowcp: slowcp.c
	gcc -o slowcp slowcp.c
