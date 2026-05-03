#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>

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

int main() {

    unsigned char key[32] = "01234567890123456789012345678901";
    unsigned char iv[16]  = "0123456789012345";

    unsigned char plaintext[] = "Hello, EVP encryption!";
    unsigned char ciphertext[128];
    unsigned char decryptedtext[128];

    int ciphertext_len, decrypted_len;

    ciphertext_len = encrypt(
        plaintext,
        strlen((char*)plaintext),
        key,
        iv,
        ciphertext
    );

    if (ciphertext_len < 0) {
        printf("Encryption failed\n");
        return 1;
    }

    printf("Ciphertext length: %d\n", ciphertext_len);

    decrypted_len = decrypt(
        ciphertext,
        ciphertext_len,
        key,
        iv,
        decryptedtext
    );

    if (decrypted_len < 0) {
        printf("Decryption failed\n");
        return 1;
    }

    decryptedtext[decrypted_len] = '\0';

    printf("Original : %s\n", plaintext);
    printf("Decrypted: %s\n", decryptedtext);

    return 0;
}