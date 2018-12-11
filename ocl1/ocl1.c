#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#define MAX_SOURCE_SIZE (0x100000)
int main() {
    // Load the source code containing the kernel
    FILE *fp;
    char fileName[] = "./ocl1.cl";
    char *source_str;
    size_t source_size;
    fp = fopen(fileName, "r");
    if (!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }
    source_str = (char *)malloc(MAX_SOURCE_SIZE);
    source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose(fp);
    // Get Platform and Device Info
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;
    cl_uint ret_num_devices, ret_num_platforms;
    cl_int ret;
    ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1,
            &device_id, &ret_num_devices);
    // Create OpenCL context
    cl_context context = NULL;
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
    // Create Command Queue
    cl_command_queue command_queue = NULL;
    command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
    // Create Kernel Program from the source
    cl_program program = NULL;
    program = clCreateProgramWithSource(context, 1,
            (const char**) &source_str,
            (const size_t *)&source_size,
            &ret);
    // Build Kernel Program
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    // Create OpenCL Kernel
    #define NUM_KERNELS 1
    #define MAXMUL 0
    #define ARRSIZE 2

    cl_kernel kernel[NUM_KERNELS];
    kernel[MAXMUL] = clCreateKernel(program, "matrixMul", &ret);

    // Init random
    srand((unsigned int)time(NULL));

    // Book main memory for all three matrices
    int i, j;
    float *A, *B, *C;
    A = (float *)malloc(ARRSIZE*ARRSIZE*sizeof(float));
    B = (float *)malloc(ARRSIZE*ARRSIZE*sizeof(float));
    C = (float *)malloc(ARRSIZE*ARRSIZE*sizeof(float));

    for (i=0; i < ARRSIZE; i++){
        for(j=0; j < ARRSIZE; j++){
            A[i*ARRSIZE+j] = ((float) rand() / (float)(RAND_MAX)) * 10;
            B[i*ARRSIZE+j] = ((float) rand() / (float)(RAND_MAX)) * 10;
        }
    }

    printf("A\n");
    for (i=0; i < ARRSIZE; i++){
        for(j=0; j < ARRSIZE; j++){
            printf("%2.1f\t", A[i*ARRSIZE+j]);
        }
        printf("\n");
    }
    printf("B\n");
    for (i=0; i < ARRSIZE; i++){
        for(j=0; j < ARRSIZE; j++){
            printf("%2.1f\t", B[i*ARRSIZE+j]);
        }
        printf("\n");
    }
    printf("\n");

    // Create Device Buffer Objects for all three matrices
    cl_mem Amobj = NULL;
    cl_mem Bmobj = NULL;
    cl_mem Cmobj = NULL;
    Amobj = clCreateBuffer(context, CL_MEM_READ_WRITE,
            ARRSIZE*ARRSIZE*sizeof(float), NULL, &ret);
    Bmobj = clCreateBuffer(context, CL_MEM_READ_WRITE,
            ARRSIZE*ARRSIZE*sizeof(float), NULL, &ret);
    Cmobj = clCreateBuffer(context, CL_MEM_READ_WRITE,
            ARRSIZE*ARRSIZE*sizeof(float), NULL, &ret);

    int wA = ARRSIZE;
    int wB = ARRSIZE;
    ret = clSetKernelArg(kernel[MAXMUL], 0, sizeof(cl_mem), (void *)&Amobj);
    ret = clSetKernelArg(kernel[MAXMUL], 1, sizeof(cl_mem), (void *)&Bmobj);
    ret = clSetKernelArg(kernel[MAXMUL], 2, sizeof(int), (void *)&wA);
    ret = clSetKernelArg(kernel[MAXMUL], 3, sizeof(int), (void *)&wB);
    ret = clSetKernelArg(kernel[MAXMUL], 4, sizeof(cl_mem), (void *)&Cmobj);

    ret = clEnqueueWriteBuffer(command_queue, Amobj, CL_TRUE, 0,
                               ARRSIZE*ARRSIZE*sizeof(float), A, 0, NULL, NULL);
    ret = clEnqueueWriteBuffer(command_queue, Bmobj, CL_TRUE, 0,
                               ARRSIZE*ARRSIZE*sizeof(float), B, 0, NULL, NULL);
    // Execute OpenCL kernel as data parallel
    size_t global_item_size[2] = {ARRSIZE, ARRSIZE};
    size_t local_item_size[2] = {1, 1};
    ret = clEnqueueNDRangeKernel(command_queue, kernel[MAXMUL], 2, NULL,
            global_item_size, local_item_size,
            0, NULL, NULL);
    // Copy back the resulting matrix C from device buffer
    ret = clEnqueueReadBuffer(command_queue, Cmobj, CL_TRUE, 0,
            ARRSIZE*ARRSIZE*sizeof(float), C, 0, NULL, NULL);

    // Display results
    printf("Result\n");
    for (i=0; i < ARRSIZE; ++i) {
        for (j=0; j < ARRSIZE; ++j) {
            printf("%2.1f\t",C[i*ARRSIZE+j]);
        }
        printf("\n");
    }
    printf("\n");
    // Free device buffers
    ret = clReleaseMemObject(Amobj);
    ret = clReleaseMemObject(Bmobj);
    ret = clReleaseMemObject(Cmobj);

    // Finalization
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    for (i=0; i < NUM_KERNELS; i++) {
        ret = clReleaseKernel(kernel[i]);
    }
    ret = clReleaseProgram(program);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);
    // free kernel code string
    free(source_str);
    return 0;
}
