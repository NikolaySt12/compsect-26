#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

char decrypt_char(char c, int key){
    if (isupper(c)){
        return (c-'A'- key + 26) % 26 + 'A';
    }
        if (islower(c)){
            return (c - 'a'- key +26) % 26 +'a';
    }
    return c;
}
int main (int argc, char *argv []){
    if (argc !=4){
        printf("Usage: %s input.txt key.txt output.txt\n",argv [0]);
        return 1;
    }
    FILE *fin = fopen (argv[1], "r");
    FILE *fkey = fopen (argv[2], "r");
    FILE *fout = fopen (argv[3], "w");
if (!fin || !fkey || !fout){
    printf("error opening file.\n");
    return 1;
}
int key;
fscanf(fkey,"%d", &key);

int c;
while ((c= fgetc(fin)) != EOF){
    fputc(decrypt_char(c, key), fout);
}
fclose(fin);
fclose(fkey);
fclose(fout);

return 0;
}