#include <string.h>
#ifndef CONFIG_RISCV
#include <libio/console.h>
#else
#include <libfemto/femto.h>
#define PRINTF(...) printf(__VA_ARGS__)
#endif

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
	memset(m->strides, 1, sizeof(uint16_t) * (len + 1));
	for(uint16_t i = 0; i < len; i++) {
		m->dims[i] = dims[i];
		m->strides[len - i - 1] = running_stride;
		running_stride *= dims[len - i - 1];
	}
}

uint16_t mat_idx(mat_t *m, uint16_t idxs[], uint16_t len) {
	return _offset_calc(m, idxs, len);
}

uint16_t mat_get_dim(mat_t *m, uint16_t axis) {
	return m->dims[axis];
}
