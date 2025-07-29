#ifndef SEARCH_H
#define SEARCH_H

int search(const char *pattern, const char *location);
int search_dir(const char *pattern, const char *dirname);
int search_file(const char *pattern, const char *filename);

#endif
