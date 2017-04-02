/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 by Sergey Fetisov <fsenok@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * version: 1.0 (4.01.2015)
 */

#ifndef EMFAT_H
#define EMFAT_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*emfat_readcb_t)(uint8_t *dest, int size, uint32_t offset, size_t userdata);
typedef void (*emfat_writecb_t)(const uint8_t *data, int size, uint32_t offset, size_t userdata);

typedef struct emfat_entry emfat_entry_t;

struct emfat_entry
{
	const char     *name;
	bool            dir;
	int             level;
	uint32_t        offset;
	uint32_t        curr_size;
	uint32_t        max_size;
	size_t          user_data;
	uint32_t        cma_time[3]; /**< create/mod/access time in unix format */
	emfat_readcb_t  readcb;
	emfat_writecb_t writecb;
	struct
	{
		uint32_t       first_clust;
		uint32_t       last_clust;
		uint32_t       last_reserved;
		uint32_t       num_subentry;
		emfat_entry_t *top;
		emfat_entry_t *sub;
		emfat_entry_t *next;
	} priv;
};

typedef struct
{
	uint64_t    vol_size;
	uint32_t    disk_sectors;
	const char *vol_label;
	struct
	{
		uint32_t       boot_lba;
		uint32_t       fsinfo_lba;
		uint32_t       fat1_lba;
		uint32_t       fat2_lba;
		uint32_t       root_lba;
		uint32_t       num_clust;
		emfat_entry_t *entries;
		emfat_entry_t *last_entry;
		int            num_entries;
	} priv;
} emfat_t;

bool emfat_init(emfat_t *emfat, const char *label, emfat_entry_t *entries);
void emfat_read(emfat_t *emfat, uint8_t *data, uint32_t sector, int num_sectors);
void emfat_write(emfat_t *emfat, const uint8_t *data, uint32_t sector, int num_sectors);

#define EMFAT_ENCODE_CMA_TIME(D,M,Y,h,m,s) \
    ((((((Y)-1980) << 9) | ((M) << 5) | (D)) << 16) | \
    (((h) << 11) | ((m) << 5) | (s >> 1)))

static inline uint32_t emfat_encode_cma_time(int D, int M, int Y, int h, int m, int s)
{
    return EMFAT_ENCODE_CMA_TIME(D,M,Y,h,m,s);
}

uint32_t emfat_cma_time_from_unix(uint32_t unix_time);

#ifdef __cplusplus
}
#endif

#endif
