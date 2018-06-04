#include <string.h>

#include "mat.h"

uint16_t _offset_calc(mat_t *m, uint16_t idxs[], uint16_t len) {
	uint16_t offset = 0;
	for(uint16_t i = 0; i < len; i++) {
		offset += m->strides[i] * idxs[i];
	}
	return offset;
}

void mat_reshape(mat_t *m, uint16_t dims[], uint16_t len) {
	m->len_dims = len;
	uint16_t running_stride = 1;
	for(uint16_t i = 0; i < len; i++) {
		m->dims[i] = dims[i];
		m->strides[len - i - 1] = running_stride;
		running_stride *= dims[len - i - 1];
	}
}

mat_t mat_constrain(mat_t *m, uint16_t idxs[], uint16_t len) {
	uint16_t len_dims = m->len_dims - len;
	mat_t c_m;
	c_m.len_dims = len_dims;
	memcpy(&c_m.sparse, &m->sparse, sizeof(m->sparse));
	memcpy(&c_m.dims, (&m->dims) + len_dims, sizeof(uint16_t) * len_dims);
	memcpy(&c_m.strides, (&m->strides) + len_dims, sizeof(uint16_t) * len_dims);
	c_m.data = m->data + _offset_calc(m, idxs, len);
    return c_m;
}

fixed mat_get(mat_t *m, uint16_t idxs[], uint16_t len) {
	return *mat_ptr(m, idxs, len);
}

fixed *mat_ptr(mat_t *m, uint16_t idxs[], uint16_t len) {
	return m->data + _offset_calc(m, idxs, len);
}

void mat_set(mat_t *m, fixed v, uint16_t idxs[], uint16_t len) {
	*(m->data + _offset_calc(m, idxs, len)) = v;
}

uint16_t mat_get_dim(mat_t *m, uint16_t axis) {
	return m->dims[axis];
}

uint16_t mat_get_stride(mat_t *m, uint16_t axis) {
	return m->strides[axis];
}

void mat_transpose(mat_t *m) {
	uint16_t start = 0;
	uint16_t end = m->len_dims - 1;
	while(start < end) {
		uint16_t tmp = m->dims[start];
		m->dims[start] = m->dims[end];
		m->dims[end] = tmp;

		tmp = m->strides[start];
		m->strides[start] = m->strides[end];
		m->strides[end] = tmp;

		start++;
		end--;
	}
}

void mat_copy(mat_t *src, mat_t *dest) {
	memcpy(&dest->dims, &src->dims, sizeof(uint16_t) * src->len_dims);
	memcpy(&dest->strides, &src->strides, sizeof(uint16_t) * src->len_dims);
	memcpy(&dest->sparse.dims, &src->sparse.dims, 
		sizeof(uint16_t) * src->sparse.len_dims);
	dest->sparse.offset = src->sparse.offset;
	dest->sparse.sizes = src->sparse.sizes;
}
