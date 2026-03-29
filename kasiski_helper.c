#include <stdio.h>
#include <ctype.h>
#include <string.h>

void decrypt(char *text, int key) {

    for (int i = 0; i < strlen(text); i++) {

        char c = text[i];

        if (isupper(c)) {
            char d = (c - 'A' - key + 26) % 26 + 'A';
            printf("%c", d);
        }
        else {
            printf("%c", c);
        }
    }

    printf("\n");
}

int main() {

    char text[] = "WKLV LV D VHFUHW PHVVDJH";

    printf("Brute Force Results:\n\n");

    for (int key = 1; key <= 25; key++) {

        printf("Key %d: ", key);
        decrypt(text, key);
    }

    return 0;
}


