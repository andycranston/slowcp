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




----------------
End of README.md
