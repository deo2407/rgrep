#include "search.h"

#include <linux/limits.h>
#include <stdio.h>
#include <regex.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <limits.h>

int search(const char *pattern, const char *location) {
    struct stat path_stat;
    stat(location, &path_stat);

    if (S_ISDIR(path_stat.st_mode)) {
        return search_dir(pattern, location);
    } else if (S_ISREG(path_stat.st_mode)) {
        return search_file(pattern, location);
    }
    return 1;
}

void rec_search(const char *pattern, const char *dirname, int *found) {
    DIR *d = opendir(dirname);
    if (d == NULL) {
        return;
    }
    
    struct dirent *entry;
    while ((entry = readdir(d)) != NULL) {
        if (entry->d_type == DT_REG) {
            char subpath[PATH_MAX];
            snprintf(subpath, sizeof subpath, "%s/%s", dirname, entry->d_name);           
            *found |= search_file(pattern, subpath);
        }
    }

    rewinddir(d);

    while ((entry = readdir(d)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            char subpath[PATH_MAX];
            snprintf(subpath, sizeof subpath, "%s/%s", dirname, entry->d_name);
            rec_search(pattern, subpath, found);
        }
    }
    closedir(d);
}

int search_dir(const char *pattern, const char *dirname) {
    DIR *d = opendir(dirname);
    if (d == NULL) {
        fprintf(stderr, "no such directory found.\n");
        return 1;
    }
    closedir(d);

    int found = 0;
    rec_search(pattern, dirname, &found);
    
    return found ? 0 : 1;
}

int search_file(const char *pattern, const char *filename) {
    char buff[512];
    regex_t regex;
    int reti;
    int matched = 0;

    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        fprintf(stderr, "%s failed to open.\n", filename);
        return 1;
    }

    reti = regcomp(&regex, pattern, REG_EXTENDED);
    if (reti) {
        char ebuff[256];
        regerror(reti, &regex, ebuff, sizeof buff);
        fprintf(stderr, "could not compile regex: %s.\n", ebuff);
        fclose(file);
        return 1;
    }

    // check each line for the pattern
    printf("%s: ", filename);
    while (fgets(buff, sizeof buff, file) != NULL) {
        reti = regexec(&regex, buff, 0, NULL, 0); 
        
        if (!reti) {
            printf("%s", buff);
            matched = 1;
        } 
    }
    printf("\n");
    
    regfree(&regex);
    fclose(file);

    return matched ? 0 : 1;
}

