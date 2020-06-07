#include <CL/cl.h>

#include "load.h"
#include "m13100_a0-pure.h"

static void
data_free(m13100_data_t* d) {
	//TBD release buffers
	return;
}

static void
data_alloc(m13100_data_t* d, cl_context ctx) {
	int S = 4;
	d->pws = clCreateBuffer(ctx, CL_MEM_READ_WRITE, sizeof (pw_t) * 100, NULL, &status);
	check("clCreateBuffer");
	d->rules_buf = clCreateBuffer(ctx, CL_MEM_READ_WRITE, 32768, NULL, &status);
	check("clCreateBuffer");
	d->combs_buf = clCreateBuffer(ctx, CL_MEM_READ_WRITE, 266240, NULL, &status);
	check("clCreateBuffer");
	d->g_bfs_buf = clCreateBuffer(ctx, CL_MEM_READ_WRITE, 4096, NULL, &status);
	check("clCreateBuffer");
	d->tmps = clCreateBuffer(ctx, CL_MEM_READ_WRITE, S, NULL, &status);
	check("clCreateBuffer");
	d->hooks = clCreateBuffer(ctx, CL_MEM_READ_WRITE, S, NULL, &status);
	check("clCreateBuffer");
	d->bitmaps_buf_s1_a = clCreateBuffer(ctx, CL_MEM_READ_WRITE, 262144, NULL, &status);
	check("clCreateBuffer");
	d->bitmaps_buf_s1_b = clCreateBuffer(ctx, CL_MEM_READ_WRITE, 262144, NULL, &status);
	check("clCreateBuffer");
	d->bitmaps_buf_s1_c = clCreateBuffer(ctx, CL_MEM_READ_WRITE, 262144, NULL, &status);
	check("clCreateBuffer");
	d->bitmaps_buf_s1_d = clCreateBuffer(ctx, CL_MEM_READ_WRITE, 262144, NULL, &status);
	check("clCreateBuffer");
	d->bitmaps_buf_s2_a = clCreateBuffer(ctx, CL_MEM_READ_WRITE, 262144, NULL, &status);
	check("clCreateBuffer");
	d->bitmaps_buf_s2_b = clCreateBuffer(ctx, CL_MEM_READ_WRITE, 262144, NULL, &status);
	check("clCreateBuffer");
	d->bitmaps_buf_s2_c = clCreateBuffer(ctx, CL_MEM_READ_WRITE, 262144, NULL, &status);
	check("clCreateBuffer");
	d->bitmaps_buf_s2_d = clCreateBuffer(ctx, CL_MEM_READ_WRITE, 262144, NULL, &status);
	check("clCreateBuffer");
	d->plains_buf = clCreateBuffer(ctx, CL_MEM_READ_WRITE, 32, NULL, &status);
	check("clCreateBuffer");
	d->digests_buf = clCreateBuffer(ctx, CL_MEM_READ_WRITE, 16, NULL, &status);
	check("clCreateBuffer");
	d->hashes_shown = clCreateBuffer(ctx, CL_MEM_READ_WRITE, 4, NULL, &status);
	check("clCreateBuffer");
	d->salt_bufs = clCreateBuffer(ctx, CL_MEM_READ_WRITE, 560, NULL, &status);
	check("clCreateBuffer");
	d->esalt_bufs = clCreateBuffer(ctx, CL_MEM_READ_WRITE, 560, NULL, &status);
	check("clCreateBuffer");
	d->d_return_buf = clCreateBuffer(ctx, CL_MEM_READ_WRITE, 4, NULL, &status);
	check("clCreateBuffer");
	d->d_extra0_buf = clCreateBuffer(ctx, CL_MEM_READ_WRITE, 1, NULL, &status);
	check("clCreateBuffer");
	d->d_extra1_buf = clCreateBuffer(ctx, CL_MEM_READ_WRITE, 1, NULL, &status);
	check("clCreateBuffer");
	d->d_extra2_buf = clCreateBuffer(ctx, CL_MEM_READ_WRITE, 1, NULL, &status);
	check("clCreateBuffer");
	d->d_extra3_buf = clCreateBuffer(ctx, CL_MEM_READ_WRITE, 1, NULL, &status);
	check("clCreateBuffer");
	d->bitmap_mask = clCreateBuffer(ctx, CL_MEM_READ_WRITE, S, NULL, &status);
	check("clCreateBuffer");
	d->bitmap_shift1 = clCreateBuffer(ctx, CL_MEM_READ_WRITE, S, NULL, &status);
	check("clCreateBuffer");
	d->bitmap_shift2 = clCreateBuffer(ctx, CL_MEM_READ_WRITE, S, NULL, &status);
	check("clCreateBuffer");
	d->salt_pos = clCreateBuffer(ctx, CL_MEM_READ_WRITE, S, NULL, &status);
	check("clCreateBuffer");
	d->loop_pos = clCreateBuffer(ctx, CL_MEM_READ_WRITE, S, NULL, &status);
	check("clCreateBuffer");
	d->loop_cnt = clCreateBuffer(ctx, CL_MEM_READ_WRITE, S, NULL, &status);
	check("clCreateBuffer");
	d->il_cnt = clCreateBuffer(ctx, CL_MEM_READ_WRITE, S, NULL, &status);
	check("clCreateBuffer");
	d->digests_cnt = clCreateBuffer(ctx, CL_MEM_READ_WRITE, S, NULL, &status);
	check("clCreateBuffer");
	d->digests_offset = clCreateBuffer(ctx, CL_MEM_READ_WRITE, S, NULL, &status);
	check("clCreateBuffer");
	d->combs_mode = clCreateBuffer(ctx, CL_MEM_READ_WRITE, S, NULL, &status);
	check("clCreateBuffer");
	d->gid_max = clCreateBuffer(ctx, CL_MEM_READ_WRITE, 8, NULL, &status);
	check("clCreateBuffer");

exit:
	data_free(d);
	return;
}


static int
data2link(m13100_link_t *l, m13100_data_t *d) {
	cl_mem *p = l->ptr;

	*p = d->pws;			p++;
	*p = d->rules_buf;		p++;
	*p = d->combs_buf;		p++;
	*p = d->g_bfs_buf;		p++;
	*p = d->tmps;			p++;
	*p = d->hooks;			p++;
	*p = d->bitmaps_buf_s1_a;	p++;
	*p = d->bitmaps_buf_s1_b;	p++;
	*p = d->bitmaps_buf_s1_c;	p++;
	*p = d->bitmaps_buf_s1_d;	p++;
	*p = d->bitmaps_buf_s2_a;	p++;
	*p = d->bitmaps_buf_s2_b;	p++;
	*p = d->bitmaps_buf_s2_c;	p++;
	*p = d->bitmaps_buf_s2_d;	p++;
	*p = d->plains_buf;		p++;
	*p = d->digests_buf;		p++;
	*p = d->hashes_shown;		p++;
	*p = d->salt_bufs;		p++;
	*p = d->esalt_bufs;		p++;
	*p = d->d_return_buf;		p++;
	*p = d->d_extra0_buf;		p++;
	*p = d->d_extra1_buf;		p++;
	*p = d->d_extra2_buf;		p++;
	*p = d->d_extra3_buf;		p++;
	*p = d->bitmap_mask;		p++;
	*p = d->bitmap_shift1;		p++;
	*p = d->bitmap_shift2;		p++;
	*p = d->salt_pos;		p++;
	*p = d->loop_pos;		p++;
	*p = d->loop_cnt;		p++;
	*p = d->il_cnt;			p++;
	*p = d->digests_cnt;		p++;
	*p = d->digests_offset;		p++;
	*p = d->combs_mode;		p++;
	*p = d->gid_max;		p++;
}

void
setup_args(cl_kernel kernel, cl_context ctx, cl_command_queue queue) {
	m13100_data_t d;
	m13100_link_t l;
	uint64_t val64 = 10000;
	uint32_t val32 = 10000;

	data_alloc(&d, ctx);
	data2link(&l, &d);

	status = clEnqueueWriteBuffer(queue, d.il_cnt, CL_TRUE, 0, sizeof(unsigned int), (void*)&val32, 0, NULL, NULL);
	check("setup il_cnt");

	status = clEnqueueWriteBuffer(queue, d.gid_max, CL_TRUE, 0, sizeof(unsigned int), (void*)&val64, 0, NULL, NULL);
	check("setup gid_max");
/*
	status = clEnqueueReadBuffer(queue, d.gid_max, CL_TRUE, 0, sizeof(unsigned int), (void*)&val64, 0, NULL, NULL);
	printf("gid_max = %ld\n", val64);
	check("setup gid_max");
*/
	for (int i = 0; i < 35; i++) {
		status = clSetKernelArg(kernel, i, sizeof(cl_mem), (void*)&l.ptr[i]);
		if (status != CL_SUCCESS) {
			printf("ERROR: clSetKernelArg [%d]\n", i);
			goto exit;
		}
	}

exit:
	return;
}

