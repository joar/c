#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

void print_usage(char **argv)
{
    fprintf(stderr, "Usage: %s [DIR]\n", argv[0]);
}

int main(int argc, char **argv)
{
    char dir_name[1024];

    if (argc > 2)
    {
        print_usage(argv);
        return 1;
    }
    else if (argc == 2)
    {
        sprintf(dir_name, "%s", argv[1]);
    }
    else
    {
        if (getcwd(dir_name, sizeof(dir_name)) == NULL)
        {
            perror("Could not getcwd()");
        }
    }
#ifdef REUSE_STATBUF
    printf("Compiled with REUSE_STATBUF\n");
#endif /* REUSE_STATBUF */

    printf("dir: %s\n", dir_name);

    DIR *dir;

    dir = opendir(dir_name);

    struct dirent *entry;

#ifdef REUSE_STATBUF
    struct stat *pbuf;

    pbuf = malloc(sizeof(struct stat));
#endif /* REUSE_STATBUF */

    while ((entry = readdir(dir)) != NULL)
    {
#ifndef REUSE_STATBUF
        struct stat buf;
#endif /* REUSE_STATBUF */
        char *ppath;
        char full_path[1024];
        int *uid;

        ppath = &entry->d_name;
        sprintf(full_path, "%s/%s", dir_name, ppath);

#ifdef REUSE_STATBUF
        stat(&full_path, pbuf);
#else
        stat(&full_path, &buf);
#endif /* REUSE_STATBUF */

        char *accesses[] = {
            "---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx"};

        char mode[9];
        char *pmode = mode;

        for (int i = 6; i >= 0; i -= 3)
        {
           pmode += sprintf(pmode, "%s",
#ifdef REUSE_STATBUF
                    accesses[(pbuf->st_mode >> i) & 7]
#else
                    accesses[(buf.st_mode >> i) & 7]
#endif /* REUSE_STATBUF */
                    );
        }

        struct passwd *user_info;
        struct group *group_info;

#ifdef REUSE_STATBUF
        user_info = getpwuid(pbuf->st_uid);
        group_info = getgrgid(pbuf->st_gid);
#else
        user_info = getpwuid(buf.st_uid);
        group_info = getgrgid(buf.st_gid);
#endif /* REUSE_STATBUF */

        printf("-%s 1 %s %s %d Month 00 00:00 %s\n",
                mode,
                user_info->pw_name,
                group_info->gr_name,
#ifdef REUSE_STATBUF
                pbuf->st_size,
#else
                buf.st_size,
#endif /* REUSE_STATBUF */
                entry->d_name);
    }

#ifdef REUSE_STATBUF
    free(pbuf);
#endif /* REUSE_STATBUF */

    return 0;
}
