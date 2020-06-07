#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "load.h"
#include "m13100_a0-pure.h"

cl_int status = CL_SUCCESS;

cl_program
create_program(const char *name, cl_context ctx) {
	size_t size;
	char *src = NULL;
	cl_program program;

	FILE *fp = fopen(name, "r");
	if (!fp) {
		status = !CL_SUCCESS;
		return NULL;
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	src = (char*) malloc((size+1)*sizeof(char));
	size = fread(src, 1, size, fp);
	fclose(fp);
	src[size] = '\0';

	program = clCreateProgramWithSource(ctx, 1, (const char **) &src, &size, &status);

	free(src);

	return program;
}

void
build_program(cl_program program, cl_device_id *devices) {
	const char options[] =
		"-I ../hashcat/OpenCL/ "
		"-D KERNEL_STATIC -I OpenCL "
		"-D DEVICE_TYPE=2 "
		"-D LOCAL_MEM_TYPE=2 -D VENDOR_ID=8 -D CUDA_ARCH=0 -D HAS_ADD=0 -D HAS_ADDC=0 -D HAS_SUB=0 -D HAS_SUBC=0 -D HAS_VADD=0 -D HAS_VADDC=0 -D HAS_VADD_CO=0 -D HAS_VADDC_CO=0 -D HAS_VSUB=0 -D HAS_VSUBB=0 -D HAS_VSUB_CO=0 -D HAS_VSUBB_CO=0 -D HAS_VPERM=0 -D HAS_VADD3=0 -D HAS_VBFE=0 -D HAS_BFE=0 -D HAS_LOP3=0 -D HAS_MOV64=0 -D HAS_PRMT=0 "
		"-D VECT_SIZE=8 -D DGST_R0=0 -D DGST_R1=1 -D DGST_R2=2 -D DGST_R3=3 -D DGST_ELEM=4 -D KERN_TYPE=13100 -D ATTACK_EXEC=11 -D ATTACK_KERN=0"
		;

//		"-D DGST_R0=0 -D DGST_R1=1 -D DGST_R2=2 -D DGST_R3=3 -D DGST_ELEM=4";

	status = clBuildProgram(program, 1, &devices[0], options, NULL, NULL);
	if(status == CL_BUILD_PROGRAM_FAILURE) {
		printf("ERROR: clBuildProgram (%d)\n", status);
		cl_int logStatus;
		char * buildLog = NULL;
		size_t buildLogSize = 0;
		logStatus = clGetProgramBuildInfo (program, devices[0], CL_PROGRAM_BUILD_LOG, buildLogSize, buildLog, &buildLogSize);
		if(logStatus != CL_SUCCESS) {
			printf("ERROR: clGetProgramBuildInfo (%d)\n", logStatus);
			status = !CL_SUCCESS;
			return;
		}

		buildLog = (char*)malloc(buildLogSize);
		if (!buildLog) {
			printf("ERROR: Failed to allocate host memory. (buildLog)\n");
			status = !CL_SUCCESS;
			return;
		}
		memset(buildLog, 0, buildLogSize);

		logStatus = clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, buildLogSize, buildLog, NULL);
		if(logStatus != CL_SUCCESS) {
			printf("ERROR: clGetProgramBuildInfo (%d)\n", logStatus);
			free(buildLog);
			status = !CL_SUCCESS;
			return;
		}

		printf("BUILD LOG:\n%s\n", buildLog);
		free(buildLog);
	}
}

void
hw_detect(cl_platform_id *platform, cl_device_id **devices, cl_context *context, cl_command_queue *queue) {
	cl_device_type device_type = CL_DEVICE_TYPE_GPU; //CPU; //CL_DEVICE_TYPE_GPU;
	cl_context_properties props[3] = {CL_CONTEXT_PLATFORM, 0, 0};
	cl_uint dev_num = 0;

	cl_uint plat_num;
	cl_platform_id plat_id = NULL;

	status = clGetPlatformIDs(0, NULL, &plat_num);
	check("clGetPlatformIDs");

	if (plat_num > 0) {
		cl_platform_id* plat_ids = (cl_platform_id* )malloc(plat_num* sizeof(cl_platform_id));
		status = clGetPlatformIDs(plat_num, plat_ids, NULL);
		check("clGetPlatformIDs");
		plat_id = plat_ids[0];
		free(plat_ids);
	} else {
		printf("ERROR: no OpenCL platform\n");
		goto exit;
	}

	status = clGetDeviceIDs(plat_id, CL_DEVICE_TYPE_GPU, 0, NULL, &dev_num);
//	check("clGetDeviceIDs GPU");
	if (dev_num == 0 && status != CL_SUCCESS) {
		status == CL_SUCCESS;

		status = clGetDeviceIDs(plat_id, CL_DEVICE_TYPE_CPU, 0, NULL, &dev_num);	
		check("clGetDeviceIDs CPU");

		if (dev_num == 0) {
			printf("no CPU device\n");
			status = !CL_SUCCESS;
			goto exit;
		}

		device_type = CL_DEVICE_TYPE_CPU;
	}

	printf("use %s[%d]\n", (device_type == CL_DEVICE_TYPE_CPU) ? "CPU" : "GPU", dev_num);
	*devices = (cl_device_id*)malloc(dev_num * sizeof(cl_device_id));
	status = clGetDeviceIDs(plat_id, device_type, dev_num, *devices, NULL);
	check("clGetDeviceIDs");

	*context = clCreateContext(NULL, 1, *devices , NULL, NULL, &status);
	check("clCreateContext");

	*queue = clCreateCommandQueue(*context, (*devices)[0], CL_QUEUE_PROFILING_ENABLE, &status);
	check("clCreateCommandQueue");

	return;
exit:
	status = !CL_SUCCESS;
	return;
}

#define TEST_COUNT	(10)

void
launch_kernel(cl_command_queue queue, cl_kernel kernel) {
	cl_event event = NULL;
	size_t global_work_size[1] = { 1 }, local_work_size[1]= { 1 };
	cl_ulong time_start;
	cl_ulong time_end;
	double time_total;

	for (unsigned i = 0; i < TEST_COUNT; i++) {
		status = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, global_work_size, local_work_size, 0, NULL, &event);
		check("clEnqueueNDRangeKernel SXX");

		status = clWaitForEvents(1, &event);
		check("clWaitForEvents");

		status = clFinish(queue);
		check("clFinish");

		clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
		clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);

		double time = (time_end - time_start) / 1000000.0;
		printf("time: %0.3f ms \n", time);

		time_total += time;
	}

	printf("average time: %0.3f ms \n", time_total / TEST_COUNT);

exit:
	return;
}

cl_int
main(int argc, char **argv) {
	cl_program program;
	cl_device_id *devices;
	cl_platform_id platform;
	cl_context context;
	cl_command_queue queue;
	cl_kernel kernel;

	hw_detect(&platform, &devices, &context, &queue);
	check("HW detect");

	program = create_program("./hashcat/OpenCL/m13100_a0-pure.cl", context);
	check("create program");

	build_program(program, devices);
	check("build program");

	kernel = clCreateKernel(program, "m13100_sxx", &status);
	check("clCreateKernel SXX");

	setup_args(kernel, context, queue);
	check("setup_arg");

	launch_kernel(queue, kernel);

exit:
	//TBD!!!
	//free resources
	clReleaseKernel(kernel);
	clReleaseProgram(program);

	return status;
}

