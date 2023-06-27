#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

ssize_t get_flag(char **flag)
{
    FILE *fp = NULL;
    if (!(fp = fopen("/flag", "r"))) {
        return 0;
    }
    size_t linecap = 0;
    ssize_t linelen = getline(flag, &linecap, fp);
    fclose(fp);
    if (linelen > 0 && (*flag)[linelen - 1] == '\n') {
        (*flag)[--linelen] = '\0';
    }
    return linelen;
}

void rand_str(char *str, size_t len)
{
    srand(time(NULL));
    for (size_t i = 0; i < len; ++i) {
        str[i] = 33 + rand() % 94;
    }
    str[len] = '\0';
}

struct User {
    char username[8];
    char password[8];
};

void reset_admin(struct User **admin)
{
    free(*admin);
    *admin = malloc(sizeof(struct User));
    strcpy((*admin)->username, "admin");
    rand_str((*admin)->password, 7);
}

int login(struct User *admin, char *username, char *password)
{
    return !(strcmp(admin->username, username)
             || strcmp(admin->password, password));
}

void gets(char *buf) {
    fgets(buf, 0x10, stdin);
}

int main()
{
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);

    char *flag = NULL;
    get_flag(&flag);
    setuid(getuid());

    struct User *admin = NULL;
    reset_admin(&admin);

    char username[8] = {0};
    char password[8] = {0};

    uint64_t chance = 0x25;

    printf("* address of chance      = %p\n", &chance);
    printf("* address of password[0] = %p\n", &password[0]);
    printf("* address of username[0] = %p\n", &username[0]);
    printf("* address of admin       = %p\n", &admin);

    while (chance--) {
        printf("\nchance: %#" PRIx64 "\n", chance);
        printf("username> ");
        gets(username);
        printf("password> ");
        gets(password);
        if (login(admin, username, password)) {
            puts("* good job!");
            printf("* here is the flag: %s\n", flag);
            exit(0);
        } else {
            puts("* you are not admin!");
            puts("* for the safety, reset admin's password");
            reset_admin(&admin);
        }
    }
}
