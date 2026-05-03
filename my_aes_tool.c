#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

#define KEY_SIZE 32
#define IV_SIZE 16

void derive_key_iv(const char *key_str, unsigned char *key, unsigned char *iv) {
    memset(key, 0, KEY_SIZE);
    memset(iv, 0, IV_SIZE);

    int len = strlen(key_str);
    if (len > KEY_SIZE) len = KEY_SIZE;
    memcpy(key, key_str, len);

    if (len > IV_SIZE) len = IV_SIZE;
    memcpy(iv, key_str, len);
}

int encrypt(unsigned char *plaintext, int plaintext_len,
            unsigned char *key, unsigned char *iv,
            unsigned char *ciphertext) {

    EVP_CIPHER_CTX *ctx;
    int len, ciphertext_len;

    ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return -1;

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        return -1;

    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        return -1;

    ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        return -1;

    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return ciphertext_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len,
            unsigned char *key, unsigned char *iv,
            unsigned char *plaintext) {

    EVP_CIPHER_CTX *ctx;
    int len, plaintext_len;

    ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return -1;

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        return -1;

    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        return -1;

    plaintext_len = len;

    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return plaintext_len;
}

unsigned char *read_file(const char *filename, long *size) {
    FILE *f = fopen(filename, "rb");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    *size = ftell(f);
    rewind(f);

    unsigned char *buffer = malloc(*size);
    if (!buffer) return NULL;

    fread(buffer, 1, *size, f);
    fclose(f);

    return buffer;
}

int write_file(const char *filename, unsigned char *data, int size) {
    FILE *f = fopen(filename, "wb");
    if (!f) return 0;

    fwrite(data, 1, size, f);
    fclose(f);

    return 1;
}

int main(int argc, char *argv[]) {

    if (argc != 5) {
        printf("Usage: %s [enc|dec] input_file output_file key\n", argv[0]);
        return 1;
    }

    unsigned char key[KEY_SIZE];
    unsigned char iv[IV_SIZE];

    derive_key_iv(argv[4], key, iv);

    long input_size;
    unsigned char *input_data = read_file(argv[2], &input_size);
    if (!input_data) {
        printf("Error reading input file\n");
        return 1;
    }

    unsigned char *output_data = malloc(input_size + 32);
    if (!output_data) {
        free(input_data);
        return 1;
    }

    int output_len;

    if (strcmp(argv[1], "enc") == 0) {
        output_len = encrypt(input_data, input_size, key, iv, output_data);
    } else if (strcmp(argv[1], "dec") == 0) {
        output_len = decrypt(input_data, input_size, key, iv, output_data);
    } else {
        printf("Invalid mode (use enc or dec)\n");
        free(input_data);
        free(output_data);
        return 1;
    }

    if (output_len < 0) {
        printf("Error during processing\n");
        free(input_data);
        free(output_data);
        return 1;
    }

    if (!write_file(argv[3], output_data, output_len)) {
        printf("Error writing output file\n");
    }

    free(input_data);
    free(output_data);

    return 0;
}