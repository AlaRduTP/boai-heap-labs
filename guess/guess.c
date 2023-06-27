#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
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

#define MAX_NOTES   0x8
#define NOTE_SIZE   (0x410 / sizeof(int64_t))

int64_t *notes[MAX_NOTES] = {NULL};

void new_note()
{
    size_t idx = 0;
    while (idx < MAX_NOTES && notes[idx])
        ++idx;
    if (idx == MAX_NOTES) {
        puts("* ran out of memories");
        puts("* no more notes can be allocated :(");
        return;
    }
    notes[idx] = malloc(NOTE_SIZE * sizeof(int64_t));
    printf("* new note [%zu] is allocated\n", idx);
}

void edit_note()
{
    size_t idx = 0;
    printf("Which note?  (index)\n"
           "> ");
    scanf("%zu", &idx);
    if (idx >= MAX_NOTES || !notes[idx]) {
        puts("* bad note index");
        return;
    }
    size_t off = 0;
    printf("Which entry? (index)\n"
           "> ");
    scanf("%zu", &off);
    printf("New value?\n"
           "> ");
    scanf("%" SCNd64, &notes[idx][off]);
    puts("* note is updated");
}

void del_note()
{
    size_t idx = 0;
    printf("Which note?  (index)\n"
           "> ");
    scanf("%zu", &idx);
    if (idx >= MAX_NOTES || !notes[idx]) {
        puts("* bad note index");
        return;
    }
    free(notes[idx]);
    notes[idx] = NULL;
    printf("* note [%zu] is deleted\n", idx);
}

void guess(int64_t *secret, char *flag)
{
    int64_t guess = 0;
    printf("> ");
    scanf("%" SCNd64, &guess);
    if (guess == *secret) {
        puts("* good job!");
        printf("* here is the flag: %s\n", flag);
        exit(0);
    }
    puts(guess > *secret ? "too big!" : "too small!");
    *secret = (int64_t)rand() * rand();
}

int main()
{
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);

    char *flag = NULL;
    get_flag(&flag);
    setuid(getuid());

    srand(time(NULL));
    int64_t secret = (int64_t)rand() * rand();
    printf("* address of the secret number = %p = %lu\n", &secret, (unsigned long)&secret);

    /* initialize the first note */
    notes[0] = malloc(NOTE_SIZE * sizeof(int64_t));
    printf("* address of the  first note   = %p = %lu\n", &notes[0], (unsigned long)&notes[0]);

    printf(" [ G U E S S ]\n"
           "1) edit note\n"
           "   You can write down at most 130 the wrong numbers here!\n"
           "2) new note\n"
           "   You can create a new note in case there are too many wrong numbers!\n"
           "3) delete note\n"
           "   You can delete a note if you don't need it any more!\n"
           "4) guess the secret number\n"
           "   You have only one chance!\n");

    while (1) {
        int opt;
        printf("> ");
        if(scanf("%d", &opt) != 1)
            exit(1);
        switch (opt) {
        case 1: edit_note();            break;
        case 2: new_note();             break;
        case 3: del_note();             break;
        case 4: guess(&secret, flag);   break;
        }
    }
}
