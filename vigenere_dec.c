#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

char caesar_encrypt(char c, int key) {
    if (isupper(c))
        return (c - 'A' + key) % 26 + 'A';
    else if (islower(c))
        return (c - 'a' + key) % 26 + 'a';
    else
        return c;
}

char caesar_decrypt(char c, int key) {
    if (isupper(c))
        return (c - 'A' - key + 26) % 26 + 'A';
    else if (islower(c))
        return (c - 'a' - key + 26) % 26 + 'a';
    else
        return c;
}

int main(int argc, char *argv[]) {

    if (argc != 5) {
        printf("Usage:\n");
        printf("./caesar_tool -e input.txt key.txt output.txt\n");
        printf("./caesar_tool -d input.txt key.txt output.txt\n");
        return 1;
    }

    FILE *input = fopen(argv[2], "r");
    FILE *keyfile = fopen(argv[3], "r");
    FILE *output = fopen(argv[4], "w");

    if (!input || !keyfile || !output) {
        printf("Error opening files\n");
        return 1;
    }

    int key;
    fscanf(keyfile, "%d", &key);

    char c;

    if (argv[1][1] == 'e') {
        while ((c = fgetc(input)) != EOF) {
            fputc(caesar_encrypt(c, key), output);
        }
    }
    else if (argv[1][1] == 'd') {
        while ((c = fgetc(input)) != EOF) {
            fputc(caesar_decrypt(c, key), output);
        }
    }
    else {
        printf("Invalid flag. Use -e or -d\n");
    }

    fclose(input);
    fclose(keyfile);
    fclose(output);

    return 0;
}
