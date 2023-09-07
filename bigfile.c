#define DEBUG

/*
 *  @(!--#) @(#) bigfile.c, version 001, 07-september-2023
 *
 *  create a file, typically of large size, for testing
 *
 */

/**********************************************************************/

/*
 *  includes
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


/**********************************************************************/

/*
 *  defines
 */

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define DEFAULT_FILENAME "bigfile.bin"
#define DEFAULT_BLOCK_SIZE 512
#define DEFAULT_BYTE_VALUE 0
#define DEFAULT_SIZE "1m"

/**********************************************************************/

/*
 *  globals
 */

char *progname;

/**********************************************************************/

/**********************************************************************/

/*
 *  Main
 */

int main(argc, argv)
  int   argc;
  char *argv[];
{
	char		*fname;
	int		fd;
	int		blocksize;
	int		arg;
	unsigned char	*block;
	unsigned char	value;
	char		*sizestring;
	unsigned long	sizelong;
	int		i;
	int		n;
	int		n2;
	char		lastc;

	progname = argv[0];

	fname = DEFAULT_FILENAME;
	blocksize = DEFAULT_BLOCK_SIZE;
	value = DEFAULT_BYTE_VALUE;
	sizestring = DEFAULT_SIZE;

	for (arg = 1; arg < argc; arg++) {
		if (strcmp(argv[arg], "-b") == 0) {
			arg++;

			if (arg >= argc) {
				fprintf(stderr, "%s: expecting block size after -b command line option\n", progname);
				exit(1);
			}

			blocksize = atoi(argv[arg]);
		} else if (strcmp(argv[arg], "-v") == 0) {
			arg++;

			if (arg >= argc) {
				fprintf(stderr, "%s: expecting byte value after -v command line option\n", progname);
                                exit(1);
                        }

                        value = (unsigned char)atoi(argv[arg]);
		} else if (strcmp(argv[arg], "-f") == 0) {
			arg++;

			if (arg >= argc) {
				fprintf(stderr, "%s: expecting filename after -f command line option\n", progname);
                                exit(1);
                        }

                        fname = argv[arg];
		} else if (strcmp(argv[arg], "-s") == 0) {
			arg++;

			if (arg >= argc) {
				fprintf(stderr, "%s: expecting size after -s command line option\n", progname);
                                exit(1);
                        }

                        sizestring = argv[arg];
		} else {
			fprintf(stderr, "%s: unrecognised command line option \"%s\"\n", progname, argv[arg]);
			exit(1);
		}
	}

#ifdef DEBUG
	printf("Filename ..............: %s\n", fname);
	printf("Block size ............: %d bytes\n", blocksize);
	printf("Byte value     ........: %d\n", value);
	printf("Size string ...........: %s\n", sizestring);
#endif

	if ((block = calloc(blocksize, sizeof(unsigned char))) == NULL) {
		fprintf(stderr, "%s: unable to allocate write block of %d bytes\n", progname, blocksize);
		exit(1);
	}

	for (i = 0; i < blocksize; i++) {
		block[i] = value;
	}

	if ((fd = creat(fname, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0) {
		fprintf(stderr, "%s: unable to create file \"%s\" to write to it\n", progname, fname);
		exit(1);
	}

	sizelong = atol(sizestring);

	if (strlen(sizestring) >= 2) {
		lastc = sizestring[strlen(sizestring) - 1];

		lastc = toupper(lastc);

		switch (lastc) {
			case 'K':
				sizelong = sizelong * 1024;
				break;
			case 'M':
				sizelong = sizelong * 1024 * 1024;
				break;
		}
	}

#ifdef DEBUG
	printf("Size ..................: %ld bytes\n", sizelong);
#endif

	while (sizelong > 0) {
		if (sizelong > blocksize) {
			n = blocksize;
		} else {
			n = sizelong;
		}

		n2 = write(fd, block, n);

		if (n2 != n) {
			fprintf(stderr, "%s: write error\n", progname);
			exit(1);
		}

		sizelong = sizelong - n2;
	}

	close(fd);

	return 0;
}

/**********************************************************************/

/* end of file */
