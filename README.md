# rgrep

A simple recursive grep utility for POSIX systems.

This tool searches for lines matching a given regular expression pattern in a specified file or directory. When given a directory, it recursively searches through all files inside.

Features:
- Supports regular expressions using the POSIX regex library.
- Recursively searches directories.
- Prints matching lines with file paths.

## Usage

```bash
./rgrep '<regex pattern>' <file or directory>
