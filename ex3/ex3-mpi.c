#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

char* read_file(char *filename, long* filesize);
int count_number_spaces(char* string, long stringsize);

int main(){
    long filesize;
    int nspaces;
    char *filename = "example.in";
    char *file_content = read_file(filename, &filesize);
    nspaces = count_number_spaces(file_content, filesize);
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
    int total_count = 0;
    int process_rank;
    int lower_limit, upper_limit;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
    lower_limit = process_rank * ((int) stringsize / 4);
    upper_limit = lower_limit + ((int) stringsize / 4);
    if (process_rank == 3){
        upper_limit += (int) stringsize % 4;
    }
    for (i = lower_limit; i < upper_limit; i++){
        if (*(string + i) == ' '){
            count++;
        }
    }
    MPI_Reduce(&count, &total_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (process_rank == 0){
        printf("size %ld spaces %d\n", stringsize, total_count);
    }
    MPI_Finalize();
    return total_count;
}
