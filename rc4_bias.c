#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define KEY_LEN 16
#define NUM_TESTS 100000

typedef struct {
    unsigned char S[256];
    int i, j;
} RC4_STATE;

void rc4_init(RC4_STATE *state, unsigned char *key, int keylen) {
    for (int i = 0; i < 256; i++) {
        state->S[i] = i;
    }

    state->i = 0;
    state->j = 0;

    int j = 0;
    for (int i = 0; i < 256; i++) {
        j = (j + state->S[i] + key[i % keylen]) % 256;

        unsigned char temp = state->S[i];
        state->S[i] = state->S[j];
        state->S[j] = temp;
    }
}

unsigned char rc4_get_byte(RC4_STATE *state) {
    state->i = (state->i + 1) % 256;
    state->j = (state->j + state->S[state->i]) % 256;

    unsigned char temp = state->S[state->i];
    state->S[state->i] = state->S[state->j];
    state->S[state->j] = temp;

    return state->S[(state->S[state->i] + state->S[state->j]) % 256];
}

int main() {
    srand(time(NULL));

    int zero_count = 0;

    for (int t = 0; t < NUM_TESTS; t++) {
        unsigned char key[KEY_LEN];

        for (int i = 0; i < KEY_LEN; i++) {
            key[i] = rand() % 256;
        }

        RC4_STATE state;
        rc4_init(&state, key, KEY_LEN);

        unsigned char first = rc4_get_byte(&state);
        unsigned char second = rc4_get_byte(&state);

        if (second == 0x00) {
            zero_count++;
        }
    }

    double probability = (double)zero_count / NUM_TESTS;

    printf("Tests: %d\n", NUM_TESTS);
    printf("Second byte = 0x00: %d times\n", zero_count);
    printf("Probability: %.6f\n", probability);

    return 0;
}