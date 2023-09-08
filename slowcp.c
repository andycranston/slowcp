#define DEBUG

/*
 *  @(!--#) @(#) slowcp.c, version 005, 08-september-2023
 *
 *  do a simple file copy but pause between each block/buffer
 *
 *  this is to keep a large copied file open for write for
 *  a long period of time to allow other testing (e.g. backup)
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

#define DEFAULT_BUFSIZ 512
#define DEFAULT_PAUSE_SECOND 0
#define DEFAULT_PAUSE_NANOSECOND 1000000

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
	char		*srcfname;
	char		*dstfname;
	int		srcfd;
	int		dstfd;
	int		bufsiz;
	time_t		pausesecond;
	long		pausenanosecond;
	struct timespec	pause;
	int		arg;
	unsigned char	*buf;
	int		n;
	int		n2;

	progname = argv[0];

	srcfname = (char *)NULL;
	dstfname = (char *)NULL;
	bufsiz = DEFAULT_BUFSIZ;
	pausesecond = DEFAULT_PAUSE_SECOND;
	pausenanosecond = DEFAULT_PAUSE_NANOSECOND;

	for (arg = 1; arg < argc; arg++) {
		if (strcmp(argv[arg], "-b") == 0) {
			arg++;

			if (arg >= argc) {
				fprintf(stderr, "%s: expecting buffer size after -b command line option\n", progname);
				exit(1);
			}

			bufsiz = atoi(argv[arg]);
		} else if (strcmp(argv[arg], "-s") == 0) {
			arg++;

			if (arg >= argc) {
				fprintf(stderr, "%s: expecting second delay after -s command line option\n", progname);
                                exit(1);
                        }

                        pausesecond = (time_t)atoi(argv[arg]);
		} else if (strcmp(argv[arg], "-n") == 0) {
			arg++;

			if (arg >= argc) {
				fprintf(stderr, "%s: expecting nanosecond delay after -s command line option\n", progname);
                                exit(1);
                        }

                        pausenanosecond = atol(argv[arg]);
		} else if (argv[arg][0] == '-') {
			fprintf(stderr, "%s: unrecognised command line option \"%s\"\n", progname, argv[arg]);
			exit(1);
		} else {
			/* printf("Break out with arg=%d\n", arg); */
			break;
		}
	}

	if (arg == argc) {
		fprintf(stderr, "%s: no source or destination filename specified on the command line\n", progname);
		exit(1);
	}

	if (arg == (argc -1)) {
		fprintf(stderr, "%s: no destination filename specified on the command line\n", progname);
		exit(1);
	}

	if (arg == (argc - 2)) {
		srcfname = argv[arg + 0];
		dstfname = argv[arg + 1];
	} else {
		fprintf(stderr, "%s: too many filenames specified on the command line\n", progname);
		exit(1);
	}

#ifdef DEBUG
	printf("Source filename........: %s\n", srcfname);
	printf("Destination filename...: %s\n", dstfname);
	printf("Buffer size ...........: %d bytes\n", bufsiz);
	printf("Pause second ..........: %ld seconds\n", pausesecond);
	printf("Pause nanosecond ......: %ld nanoseconds\n", pausenanosecond);
#endif

	if ((buf = calloc(bufsiz, sizeof(unsigned char))) == NULL) {
		fprintf(stderr, "%s: unable to allocate read/write buffer of %d bytes\n", progname, bufsiz);
		exit(1);
	}

	if ((srcfd = open(srcfname, O_RDONLY)) < 0) {
		fprintf(stderr, "%s: unable to open file \"%s\" for reading\n", progname, srcfname);
		exit(1);
	}


	if ((dstfd = creat(dstfname, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0) {
		fprintf(stderr, "%s: unable to create file \"%s\" to write to it\n", progname, dstfname);
		exit(1);
	}

	pause.tv_sec = pausesecond;
	pause.tv_nsec = pausenanosecond;

	while ((n = read(srcfd, buf, bufsiz)) > 0) {
		/* printf("Read %d bytes\n", n); */
		n2 = write(dstfd, buf, n);

		if (n2 != n) {
			fprintf(stderr, "%s: write error\n", progname);
			exit(1);
		}
		/* printf("Pausing...\n"); */
		nanosleep(&pause, NULL);
		/* printf("Done\n"); */
	}

	close(srcfd);
	close(dstfd);

	return 0;
}

/**********************************************************************/

/* end of file */
