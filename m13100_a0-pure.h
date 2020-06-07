#ifndef M13100_A0_PURE_H
#define M13100_A0_PURE_H

#include <CL/cl.h>
#include <stdint.h>
#include <stdio.h>

typedef uint32_t u32;
typedef uint64_t u64;

typedef struct pw
{
  u32 i[64];
  u32 pw_len;
} pw_t;

typedef struct bf
{
  u32  i;
} bf_t;

typedef struct
{
  u32 cmds[32];
} kernel_rule_t;

#define DGST_ELEM	(4)

typedef struct digest
{
  u32 digest_buf[DGST_ELEM];
} digest_t;

typedef struct plain
{
  u64  gidvid;
  u32  il_pos;
  u32  salt_pos;
  u32  digest_pos;
  u32  hash_pos;
  u32  extra1;
  u32  extra2;
} plain_t;

typedef struct salt
{
  u32 salt_buf[64];
  u32 salt_buf_pc[64];

  u32 salt_len;
  u32 salt_len_pc;
  u32 salt_iter;
  u32 salt_iteGLOBAL_AS;
  u32 salt_sign[2];

  u32 digests_cnt;
  u32 digests_done;

  u32 digests_offset;

  u32 scrypt_N;
  u32 scrypt_r;
  u32 scrypt_p;

} salt_t;

typedef struct krb5tgs {
	u32 account_info[512];
	u32 checksum[4];
	u32 edata2[5120];
	u32 edata2_len;
} krb5tgs_t;

typedef struct m13100_data_host {
	pw_t		*pws;
	const kernel_rule_t *rules_buf;
	const pw_t	*combs_buf;
	const bf_t	*g_bfs_buf;
	void		*tmps;
	void		*hooks;
	const u32	*bitmaps_buf_s1_a;
	const u32	*bitmaps_buf_s1_b;
	const u32	*bitmaps_buf_s1_c;
	const u32	*bitmaps_buf_s1_d;
	const u32	*bitmaps_buf_s2_a;
	const u32	*bitmaps_buf_s2_b;
	const u32	*bitmaps_buf_s2_c;
	const u32	*bitmaps_buf_s2_d;
	plain_t		*plains_buf;
	const digest_t 	*digests_buf;
	u32		*hashes_shown;
	const salt_t 	*salt_bufs;
	const krb5tgs_t 	*esalt_bufs;
	u32		*d_return_buf;
	void		*d_extra0_buf;
	void		*d_extra1_buf;
	void		*d_extra2_buf;
	void		*d_extra3_buf;
	const u32	bitmap_mask;
	const u32	bitmap_shift1;
	const u32	bitmap_shift2;
	const u32	salt_pos;
	const u32	loop_pos;
	const u32	loop_cnt;
	const u32	il_cnt;
	const u32	digests_cnt;
	const u32	digests_offset;
	const u32	combs_mode;
	const u64	gid_max;
} m13100_data_host_t;

typedef struct m13100_data {
	cl_mem pws;
	cl_mem rules_buf;
	cl_mem combs_buf;
	cl_mem g_bfs_buf;
	cl_mem tmps;
	cl_mem hooks;
	cl_mem bitmaps_buf_s1_a;
	cl_mem bitmaps_buf_s1_b;
	cl_mem bitmaps_buf_s1_c;
	cl_mem bitmaps_buf_s1_d;
	cl_mem bitmaps_buf_s2_a;
	cl_mem bitmaps_buf_s2_b;
	cl_mem bitmaps_buf_s2_c;
	cl_mem bitmaps_buf_s2_d;
	cl_mem plains_buf;
	cl_mem digests_buf;
	cl_mem hashes_shown;
	cl_mem salt_bufs;
	cl_mem esalt_bufs;
	cl_mem d_return_buf;
	cl_mem d_extra0_buf;
	cl_mem d_extra1_buf;
	cl_mem d_extra2_buf;
	cl_mem d_extra3_buf;
	cl_mem bitmap_mask;		//24
	cl_mem bitmap_shift1;
	cl_mem bitmap_shift2;
	cl_mem salt_pos;
	cl_mem loop_pos;
	cl_mem loop_cnt;
	cl_mem il_cnt;
	cl_mem digests_cnt;
	cl_mem digests_offset;
	cl_mem combs_mode;
	cl_mem gid_max;
} m13100_data_t;

typedef struct m13100_link {
	cl_mem ptr[35];
} m13100_link_t;

void setup_args(cl_kernel kernel, cl_context ctx, cl_command_queue queue);

#endif //M13100_A0_PURE_H

