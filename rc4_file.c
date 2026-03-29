#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned char S[256];
    int i, j;
} RC4_STATE;

void rc4_init(RC4_STATE *state, const unsigned char *key, int keylen) {
    for (int i = 0; i < 256; i++) {
        state->S[i] = i;
    }

    state->i = 0;
    state->j = 0;

    int j = 0;
    for (int i = 0; i < 256; i++) {
        j = (j + state->S[i] + key[i % keylen]) % 256;

        // swap
        unsigned char temp = state->S[i];
        state->S[i] = state->S[j];
        state->S[j] = temp;
    }
}


void rc4_crypt(RC4_STATE *state, unsigned char *data, int len) {
    for (int k = 0; k < len; k++) {
        state->i = (state->i + 1) % 256;
        state->j = (state->j + state->S[state->i]) % 256;

        unsigned char temp = state->S[state->i];
        state->S[state->i] = state->S[state->j];
        state->S[state->j] = temp;

        unsigned char K = state->S[(state->S[state->i] + state->S[state->j]) % 256];

        data[k] ^= K;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <input> <output> <password>\n", argv[0]);
        return 1;
    }

    char *input_file = argv[1];
    char *output_file = argv[2];
    char *password = argv[3];

    FILE *fin = fopen(input_file, "rb");
    if (!fin) {
        perror("Error opening input file");
        return 1;
    }

    FILE *fout = fopen(output_file, "wb");
    if (!fout) {
        perror("Error opening output file");
        fclose(fin);
        return 1;
    }

 
    fseek(fin, 0, SEEK_END);
    long filesize = ftell(fin);
    rewind(fin);

    
    unsigned char *buffer = (unsigned char *)malloc(filesize);
    if (!buffer) {
        perror("Memory allocation failed");
        fclose(fin);
        fclose(fout);
        return 1;
    }

    fread(buffer, 1, filesize, fin);

    
    RC4_STATE state;
    rc4_init(&state, (unsigned char *)password, strlen(password));
    rc4_crypt(&state, buffer, filesize);
    
    fwrite(buffer, 1, filesize, fout);

    free(buffer);
    fclose(fin);
    fclose(fout);

    printf("Done.\n");
    return 0;
}