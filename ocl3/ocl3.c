#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#define MAX_SOURCE_SIZE (0x100000)

char* read_file(char *filename, long *fsize);

int main() {
    // Load the source code containing the kernel
    FILE *fp;
    char fileName[] = "./ocl3.cl";
    char *fname = "example.in";
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
    printf("%d\n", (int) ret);
    // Create OpenCL Kernel
    #define NUM_KERNELS 1
    #define COUNTSPACES 0

    cl_kernel kernel[NUM_KERNELS];
    kernel[COUNTSPACES] = clCreateKernel(program, "countSpaces", &ret);

    long filesize;
    int result[1] = {50};
    char* file_content = read_file(fname, &filesize);

    printf("filesize is %ld\n", filesize);

    // Create Device Buffer Objects for all three matrices
    cl_mem content_mobj = NULL;
    cl_mem result_mobj = NULL;
    content_mobj = clCreateBuffer(context, CL_MEM_READ_WRITE,
            filesize*sizeof(char), NULL, &ret);
    result_mobj = clCreateBuffer(context, CL_MEM_READ_WRITE,
            sizeof(int), NULL, &ret);

    ret = clSetKernelArg(kernel[COUNTSPACES], 0, sizeof(cl_mem), (void *)&content_mobj);
    ret = clSetKernelArg(kernel[COUNTSPACES], 1, sizeof(long), (void *)&filesize);
    ret = clSetKernelArg(kernel[COUNTSPACES], 2, sizeof(cl_mem), (void *)&result_mobj);

    ret = clEnqueueWriteBuffer(command_queue, content_mobj, CL_TRUE, 0,
            filesize*sizeof(char), file_content, 0, NULL, NULL);
    ret = clEnqueueWriteBuffer(command_queue, result_mobj, CL_TRUE, 0,
            sizeof(int), file_content, 0, NULL, NULL);

    ret = clEnqueueTask(command_queue, kernel[COUNTSPACES], 0, NULL, NULL);

    ret = clEnqueueReadBuffer(command_queue, result_mobj, CL_TRUE, 0, sizeof(int),
            result, 0, NULL, NULL);

    printf("%d\n", *(result));
    // Get the transpose of B
    ret = clReleaseMemObject(content_mobj);

    // Finalization
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);

    int i;
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
