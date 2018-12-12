__kernel void countSpaces(__global char* string, long stringsize, __global int *result){
    int i = 0;
    int count = 0;
    for (i = 0; i < (int) stringsize; i++){
        if (*(string + i) == ' '){
            count++;
        }
    }
	*(result) = count;
}
