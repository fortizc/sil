/*
 * This file is part of SIL a Simple Image Library.
 * Copyright (C) 2016  Felipe A. Ortiz C. <fortizc@gmail.com>
 *
 * SIL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SIL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with SIL.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SIL_IMAGE_H
#define SIL_IMAGE_H

#include <stdint.h>
#include <stddef.h>

struct simage;
typedef struct simage simage_t;

enum sil_image_type
{
    SIL_IMAGE_GRAY_8,
    SIL_IMAGE_GRAY_16,
    SIL_IMAGE_RGB_24,
    SIL_IMAGE_RGB_48
};
typedef enum sil_image_type stype_t;

simage_t *sil_image_new(size_t width, size_t height, stype_t type);
simage_t *sil_image_zero_new(size_t width, size_t height, stype_t type);
simage_t *sil_image_copy(const simage_t *img);

void sil_image_free(simage_t *img);
void sil_image_roi(simage_t *img, size_t top, size_t left, size_t width, size_t height);
void sil_image_zero(simage_t *img);

void sil_image_set_pixel(simage_t *img, size_t x, size_t y, uint64_t value);
uint64_t sil_image_get_pixel(const simage_t *img, size_t x, size_t y);

uint8_t *sil_image_data8(const simage_t *img);
uint16_t *sil_image_data16(const simage_t *img);
uint32_t *sil_image_data32(const simage_t *img);

uint8_t *sil_image_data_row8(const simage_t *img, size_t y);
uint16_t *sil_image_data_row16(const simage_t *img, size_t y);
uint32_t *sil_image_data_row32(const simage_t *img, size_t y);

size_t sil_image_get_width(const simage_t *img);
size_t sil_image_get_height(const simage_t *img);
size_t sil_image_get_stride(const simage_t *img);
size_t sil_image_byte_per_pixel(const simage_t *img);
stype_t sil_image_get_type(const simage_t *img);

#endif
