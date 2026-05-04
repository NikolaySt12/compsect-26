#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>

void sha256 (char *str, char output [65]){
    unsigned char hash [SHA256_DIGEST_LENGTH];
    SHA256 ((unsigned char*)str, strlen(str), hash);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i ++){
        sprintf(output + (i * 2), "%02x", hash[i]);
    }
    output[64] = 0;
}

int main (){
    char target[]="ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad";
    char str[4];
    char hash[65];

    for (char a = 'a'; a <= 'z'; a++) {
        for(char b = 'a'; b<= 'z'; b++){
            for(char c ='a'; c<='z';c++ ){
                str[0] = a;
                str[1] = b;
                str[2] = c;
                str[3]= '\0';

                sha256(str, hash);
                if(strcmp(hash, target) == 0){
                    printf("Password: %s\n", str);
                    return 0;
                }
            }
        }
    }
    printf("Not found\n");
    return 0;
}