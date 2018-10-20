#include <stdio.h>
#include <stdlib.h>

char* read_file(char *filename, long* filesize);
int count_number_spaces(char* string, long stringsize);

int main(){
    long filesize;
    int nspaces;
    char *filename = "example.in";
    char *file_content = read_file(filename, &filesize);
    nspaces = count_number_spaces(file_content, filesize);
    printf("The number of spaces is %d\n", nspaces);
    // we need to free the space we have used
    // to store the contents of the file
    free(file_content);
}

char* read_file(char *filename, long *fsize){
    FILE *fp = fopen(filename, "r+");
    // First we figure out the end of the file
    fseek(fp, 0, SEEK_END);
    *(fsize) = ftell(fp);
    // Now we go to the beginning
    fseek(fp, 0, SEEK_SET);
    // We allocate the memory necessary to store the whole file
    char *file_content = malloc(*fsize + 1);
    fread(file_content, *fsize, 1, fp);
    fclose(fp);
    return file_content;
}

int count_number_spaces(char *string, long stringsize){
    int i = 0;
    int count = 0;
    for (i = 0; i < (int) stringsize; i++){
        if (*(string + i) == ' '){
            count++;
        }
    }
    return count;
}
