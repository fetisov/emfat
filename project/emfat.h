/*
 * Copyright (C) 2015 by Sergey Fetisov <fsenok@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
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
	uint32_t    num_sectors;
	const char *vol_label;
	struct
	{
		uint32_t       boot_sect;
		uint32_t       fat1_sect;
		uint32_t       fat2_sect;
		uint32_t       root_sect;
		uint32_t       num_clust;
		emfat_entry_t *entries;
		emfat_entry_t *last_entry;
		int            num_entries;
	} priv;
} emfat_t;

bool emfat_init(emfat_t *emfat, const char *label, emfat_entry_t *entries);
void emfat_read(emfat_t *emfat, uint8_t *data, uint32_t sector, int num_sectors);
void emfat_write(emfat_t *emfat, const uint8_t *data, uint32_t sector, int num_sectors);

#ifdef __cplusplus
}
#endif

#endif
