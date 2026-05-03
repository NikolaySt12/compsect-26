#include <stdio.h>
#include <string.h>

#define MAX_USERS 100
#define MAX_LEN 64

typedef struct {
    char username[MAX_LEN];
    char hash[MAX_LEN];
} User;

int match(const char *hash, const char *guess) {
    return strcmp(hash, guess) == 0;
}

int dictionary_attack(const char *hash, const char *dict_file, char *found) {
    FILE *f = fopen(dict_file, "r");
    if (!f) return 0;

    char word[MAX_LEN];

    while (fgets(word, sizeof(word), f)) {
        word[strcspn(word, "\n")] = 0;

        if (match(hash, word)) {
            strcpy(found, word);
            fclose(f);
            return 1;
        }
    }

    fclose(f);
    return 0;
}

int brute_force_pin(const char *hash, char *found) {
    char pin[5];

    for (int i = 0; i <= 9999; i++) {
        sprintf(pin, "%04d", i);

        if (match(hash, pin)) {
            strcpy(found, pin);
            return 1;
        }
    }

    return 0;
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Usage: %s shadow.txt dictionary.txt\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        printf("Error opening shadow file\n");
        return 1;
    }

    User users[MAX_USERS];
    int count = 0;

    char line[128];

    while (fgets(line, sizeof(line), f) && count < MAX_USERS) {

        char *username = strtok(line, ":");
        char *hash = strtok(NULL, "\n");

        if (username && hash) {
            strcpy(users[count].username, username);
            strcpy(users[count].hash, hash);
            count++;
        }
    }

    fclose(f);

    printf("\n--- Results ---\n");

    for (int i = 0; i < count; i++) {

        char result[MAX_LEN] = "";

        if (dictionary_attack(users[i].hash, argv[2], result)) {
            printf("%s: %s (dictionary)\n", users[i].username, result);
        }
        else if (brute_force_pin(users[i].hash, result)) {
            printf("%s: %s (brute-force)\n", users[i].username, result);
        }
        else {
            printf("%s: [NOT CRACKED]\n", users[i].username);
        }
    }

    return 0;
}