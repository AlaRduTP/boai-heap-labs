#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

ssize_t get_flag(char **flag, size_t *linecap)
{
    FILE *fp = NULL;
    if (!(fp = fopen("/flag", "r"))) {
        return 0;
    }
    ssize_t linelen = getline(flag, linecap, fp);
    fclose(fp);
    if (linelen > 0 && (*flag)[linelen - 1] == '\n') {
        (*flag)[--linelen] = '\0';
    }
    return linelen;
}

int main()
{
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);

    char *flag = NULL;
    size_t flagcap = 0;
    if (get_flag(&flag, &flagcap) > 0)
        printf("flag: %s\nflagcap: %zu\n", flag, flagcap);
    free(flag);

    setuid(getuid());

    char *argv[1] = {NULL};
    execvp("sh", argv);
}
