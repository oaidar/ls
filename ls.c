#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define FULLNAMEBUF_SIZE 1024
#define TIMEBUF_SIZE 64

void print_long_file_info(const char *name, struct stat *file_stat) {
    struct passwd *pw;
    struct group *gr;
    struct tm *tm_info;
    char timebuf[TIMEBUF_SIZE];

    printf((S_ISDIR(file_stat->st_mode)) ? "d" : "-");
    printf((file_stat->st_mode & S_IRUSR) ? "r" : "-");
    printf((file_stat->st_mode & S_IWUSR) ? "w" : "-");
    printf((file_stat->st_mode & S_IXUSR) ? "x" : "-");
    printf((file_stat->st_mode & S_IRGRP) ? "r" : "-");
    printf((file_stat->st_mode & S_IWGRP) ? "w" : "-");
    printf((file_stat->st_mode & S_IXGRP) ? "x" : "-");
    printf((file_stat->st_mode & S_IROTH) ? "r" : "-");
    printf((file_stat->st_mode & S_IWOTH) ? "w" : "-");
    printf((file_stat->st_mode & S_IXOTH) ? "x" : "-");
    printf(" %3ld", file_stat->st_nlink);
    pw = getpwuid(file_stat->st_uid);
    gr = getgrgid(file_stat->st_gid);
    printf(" %s", pw->pw_name);
    printf(" %s", gr->gr_name);
    printf(" %10lld", (long long)file_stat->st_size);
    tm_info = localtime(&file_stat->st_mtime);
    strftime(timebuf, sizeof(timebuf), "%b %2d %H:%M %Y", tm_info);
    printf(" %s", timebuf);
    printf(" %s\n", name);
}

void list_directory(const char *argv0, const char *path, int long_format) {
    DIR *dir;
    struct dirent *ds;

    dir = opendir(path);
    if (dir == NULL) {
        fprintf(stderr, "%s: cannot access '%s': ", argv0, path);
        perror("");
        return;
    }
    while ((ds = readdir(dir)) != 0) {
        if (ds->d_name[0] == '.')
            continue;
        if (long_format) {
            struct stat file_stat;
            char fullnamebuf[FULLNAMEBUF_SIZE];
            snprintf(fullnamebuf, sizeof(fullnamebuf), "%s/%s", path, ds->d_name);
            if (stat(fullnamebuf, &file_stat) == -1) {
                perror("stat()");
                continue;
            }
            print_long_file_info(ds->d_name, &file_stat);
        } else {
            printf("%s\n", ds->d_name);
        }
    }
    closedir(dir);
}

int main(int argc, char **argv) {
    int long_format;
    const char *path;

    long_format = 0;
    path = ".";
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-l") == 0)
            long_format = 1;
        else
            path = argv[i];
    }
    list_directory(argv[0], path, long_format);

    return 0;
}
