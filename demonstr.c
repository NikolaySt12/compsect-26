#include <stdio.h>
#include <openssl/evp.h>

void print_hash(const char *file, const EVP_MD *type) {

    FILE *f = fopen(file, "rb");
    if (!f) return;

    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    unsigned char buffer[1024];
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int len;

    EVP_DigestInit_ex(ctx, type, NULL);

    int bytes;
    while ((bytes = fread(buffer, 1, 1024, f)) > 0) {
        EVP_DigestUpdate(ctx, buffer, bytes);
    }

    EVP_DigestFinal_ex(ctx, hash, &len);

    printf("%s: ", file);
    for (unsigned int i = 0; i < len; i++)
        printf("%02x", hash[i]);
    printf("\n");

    EVP_MD_CTX_free(ctx);
    fclose(f);
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Usage: %s file1 file2\n", argv[0]);
        return 1;
    }

    printf("MD5:\n");
    print_hash(argv[1], EVP_md5());
    print_hash(argv[2], EVP_md5());

    printf("\nSHA256:\n");
    print_hash(argv[1], EVP_sha256());
    print_hash(argv[2], EVP_sha256());

    return 0;
}