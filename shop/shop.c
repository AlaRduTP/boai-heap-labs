#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
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

struct Bag {
    uint64_t items;
};

struct User {
    uint64_t money;
    struct Bag *bag;
};

#define MAX_USERS 0x7

struct User users[MAX_USERS];

void *_new_bag()
{
    return calloc(1, sizeof(struct Bag));
}

void _del_bag(struct Bag *bag)
{
    free(bag);
}

void _reset_user(struct User *user)
{
    user->money = 256;
    _del_bag(user->bag);
    user->bag = _new_bag();
}

void init_users() {
    for (size_t i = 0; i < MAX_USERS; ++i) {
        _reset_user(&users[i]);
    }
}

size_t get_user_idx()
{
    size_t idx = 0;
    printf("Which user?  (index)\n"
           "> ");
    scanf("%zu", &idx);
    if (idx >= MAX_USERS) {
        puts("* bad index");
        exit(1);
    }
    int c;
    do {} while ((c = getchar()) != '\n' && c != EOF);
    return idx;
}

void new_bag()
{
    size_t idx = get_user_idx();
    users[idx].bag = _new_bag();
    printf("* user [%zu] created a new bag\n", idx);
}

void del_bag()
{
    size_t idx = get_user_idx();
    _del_bag(users[idx].bag);
    printf("* user [%zu] deleted his bag\n", idx);
}

void reset_user()
{
    size_t idx = get_user_idx();
    _reset_user(&users[idx]);
    printf("* user [%zu] is reset\n", idx);
}

void buy_char() {
    size_t idx = get_user_idx();
    printf("Which char?\n"
           "> ");
    uint64_t c = (uint64_t)getchar();
    if (c > users[idx].money) {
        printf("* needs $%" PRIu64 " to buy a `%c`, but you only have $%" PRIu64 "\n",
               c, (char)c, users[idx].money);
        return;
    }
    users[idx].money -= c;
    users[idx].bag->items <<= 8;
    users[idx].bag->items |= c;
    printf("* user [%zu] bought a `%c`\n", idx, (char)c);
    printf("* money is $%" PRIu64 " left\n", users[idx].money);
}

#define F    (1 << 0)
#define L    (1 << 1)
#define A    (1 << 2)
#define G    (1 << 3)
#define FLAG (F | L | A | G)

void buy_flag(char *flag) {
    size_t idx = get_user_idx();
    uint64_t items = users[idx].bag->items;
    uint8_t check = FLAG;
    printf("* your bag = [");
    for (size_t i = 0; i < 8; ++i) {
        char c = (char)(items & 0xff);
        items >>= 8;
        if (!c) {
            continue;
        }
        switch (c) {
        case 'f': check &= ~F; break;
        case 'l': check &= ~L; break;
        case 'a': check &= ~A; break;
        case 'g': check &= ~G; break;
        }
        printf(" %c", c);
    }
    puts(" ]");
    if (check) {
        puts("* you need to buy at least `f`, `l`, `a`, and `g` to get the flag");
        return;
    }
    puts("* good job!");
    printf("* here is the flag: %s\n", flag);
    exit(0);
}

int main()
{
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);

    char *flag = NULL;
    get_flag(&flag);
    setuid(getuid());

    init_users();

    printf(" [ S H O P ]\n"
           "1) delete bag\n"
           "   User can delete his bag!\n"
           "2) new bag\n"
           "   User can get a new bag!\n"
           "3) reset user\n"
           "   User can reset his money and his bag!\n"
           "4) Buy character\n"
           "   User can buy a character!\n"
           "5) Buy flag\n"
           "   User can buy the flag!\n");

    while (1) {
        int opt;
        printf("> ");
        if(scanf("%d", &opt) != 1)
            exit(1);
        switch (opt) {
        case 1: del_bag();      break;
        case 2: new_bag();      break;
        case 3: reset_user();   break;
        case 4: buy_char();     break;
        case 5: buy_flag(flag); break;
        }
    }
}
