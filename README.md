# slowcp

The `slowcp` command for Linux copies a file. The difference between the normal `cp` command and
the `slowcp` command is that after each chunk of data in the file is copied the command
sleeps for a period of time.

This means that the command can take longer to run.

Normally this would be a bad thing but it can come in handy when performing certain tests.

For example if you wanted to see what happens when a backup application copies an open file you could
initiate a `slowcp` command to copy a large file (say a file 5 gigabtes in size) with a delay that would
mean the copy would take at a minimum 10 minutes to complete. Once the `slowcp` command is started the
backup appilication could be started to backup up the file (which will be open) and the behaviour
can be observed.

## Command line options

The `slowcp` command has some command line options.

### Option `-b` for buffer size

Normally the `slowcp` command reads and writes content of the files with buffer of 512 bytes.

To use a different size use the `-b` command line option. For example to copy a file called
`sfile` to a file `dfile` with a block size of 8129 bytes run:

```
slowcp -b 8192 sfile dfile
```

### Options `-s` and `-n` for seconds and nano seconds delay

The delay between write operations is specified with two options.

The `-s` specifies the delay in seconds. The `-n` specifies the delay in nano seconds.

By default the seconds delay is 0 (zero) seconds and the nano seconds delay is 1000000 nano seconds which is 1 millisecond.

As a reminder 1 millisecond is one thousandth (1/1000) of a second.

To specify a delay of just 1000 nanoseconds run:

```
slowcp -n 1000 sfile dfile
```

To specify a delay of one and a half seconds run:

```
slowcp -s 1 -n 5000000000 sfile dfile
```

# bigfile

This repo also contains the `bigfile` command.

If you run the command without arguments:

```
bigfile
```

It will create a file called `bigfile.bin` in the current directory of size 1048576 bytes (exactly one megabyte). The content
will be all null bytes (0x00).

WARNING: if the file `bigfile.bin` already it exists it will be overwriten without warning.

## Command line options

The `bigile` command has some command line options.

### Option `-f` for file name

To specifiy a different file name use the `-f` command line option.

For example to create a file called `verylarge` run:

```
bigfile -f verylarge
```

WARNING: if the file specified by the `-f` command line option already it exists it will be overwriten without warning.

### Option `-b` for block size

TBC.

### Option `-b` for byte value

TBC.


----------------
End of README.md
