#include <stdio.h>
#include <openssl/evp.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "rb");
    if (!f) {
        printf("Error opening file\n");
        return 1;
    }

    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (!ctx) {
        fclose(f);
        return 1;
    }

    if (1 != EVP_DigestInit_ex(ctx, EVP_sha256(), NULL)) {
        EVP_MD_CTX_free(ctx);
        fclose(f);
        return 1;
    }

    unsigned char buffer[BUF_SIZE];
    size_t bytes;

    while ((bytes = fread(buffer, 1, BUF_SIZE, f)) > 0) {
        if (1 != EVP_DigestUpdate(ctx, buffer, bytes)) {
            EVP_MD_CTX_free(ctx);
            fclose(f);
            return 1;
        }
    }

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    if (1 != EVP_DigestFinal_ex(ctx, hash, &hash_len)) {
        EVP_MD_CTX_free(ctx);
        fclose(f);
        return 1;
    }

    EVP_MD_CTX_free(ctx);
    fclose(f);

    for (unsigned int i = 0; i < hash_len; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");

    return 0;
}