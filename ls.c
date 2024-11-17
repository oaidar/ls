#include <stdio.h>
#include <dirent.h>

int main(int argc, char **argv) {
    const char *path;
    DIR *dir;
    struct dirent *ds;

    path = ".";
    for (int i = 1; i < argc; i++)
        path = argv[i];
    dir = opendir(path);
    if (dir == NULL) {
        fprintf(stderr, "%s: cannot access '%s': ", argv[0], path);
        perror("");
        return -1;
    }
    while ((ds = readdir(dir)) != 0) {
        if (ds->d_name[0] == '.')
            continue;
            printf("%s\t", ds->d_name);
    }
    printf("\n");
    closedir(dir);

    return 0;
}
