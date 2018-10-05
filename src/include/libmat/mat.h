#ifndef MAT_H
#define MAT_H

#include <stdint.h>
#include <libfixed/fixed.h>

typedef struct {
	uint16_t dims[10];
	uint16_t len_dims;
	uint16_t strides[10];
	fixed *data;
	struct {
		uint16_t dims[10];
		uint16_t len_dims;
		uint16_t *offsets;
		uint16_t *sizes;
	} sparse;
} mat_t;

#define MAT_DECLARE(buf) mat_t mat_ ## buf

#define MAT_PTR(buf) (&mat_ ## buf)

#define MAT_NUMARGS(...)  (sizeof((uint16_t[]){__VA_ARGS__}) / sizeof(uint16_t))

#define MAT_RESHAPE(m, ...) (mat_reshape(m, ((uint16_t[]){__VA_ARGS__}),		\
								MAT_NUMARGS(__VA_ARGS__)))

#define MAT_GET(buf, ...) (														\
	(MAT_NUMARGS(__VA_ARGS__) == 1) ? 											\
		buf[((uint16_t[]){__VA_ARGS__})[0]] :							\
	(MAT_NUMARGS(__VA_ARGS__) == 2) ? 											\
		buf[((uint16_t[]){__VA_ARGS__})[0] *	 						\
			MAT_PTR(buf)->strides[0] + ((uint16_t[]){__VA_ARGS__})[1]]:					\
	(MAT_NUMARGS(__VA_ARGS__) == 3) ? 											\
		buf[((uint16_t[]){__VA_ARGS__})[0] * MAT_PTR(buf)->strides[0] + 			\
			MAT_PTR(buf)->strides[1] * ((uint16_t[]){__VA_ARGS__})[1] + 					\
			((uint16_t[]){__VA_ARGS__})[2]]:									\
	buf[mat_idx(MAT_PTR(buf), ((uint16_t[]){__VA_ARGS__}),	\
		MAT_NUMARGS(__VA_ARGS__))])

#define MAT_SET(buf, val, ...) (													\
	(MAT_NUMARGS(__VA_ARGS__) == 1) ? 											\
		(buf[((uint16_t[]){__VA_ARGS__})[0]] = val) :						\
	(MAT_NUMARGS(__VA_ARGS__) == 2) ? 											\
		(buf[((uint16_t[]){__VA_ARGS__})[0] * 							\
			MAT_PTR(buf)->strides[0] + ((uint16_t[]){__VA_ARGS__})[1]] = val) :				\
	(MAT_NUMARGS(__VA_ARGS__) == 3) ? 											\
		(buf[((uint16_t[]){__VA_ARGS__})[0] * MAT_PTR(buf)->strides[0] + 			\
			MAT_PTR(buf)->strides[1] * ((uint16_t[]){__VA_ARGS__})[1] + 					\
			((uint16_t[]){__VA_ARGS__})[2]] = val) :							\
	(buf[mat_idx(MAT_PTR(buf), ((uint16_t[]){__VA_ARGS__}),	\
		MAT_NUMARGS(__VA_ARGS__))] = val))

#define MAT_GET_DIM(buf, axis) (mat_get_dim(MAT_PTR(buf), axis))

void mat_reshape(mat_t *, uint16_t[], uint16_t);
uint16_t mat_idx(mat_t *, uint16_t[], uint16_t);
uint16_t mat_get_dim(mat_t *, uint16_t);

#endif

