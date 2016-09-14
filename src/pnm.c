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

#include <sil/pnm.h>
#include <sil/simage.h>

#include <stdlib.h>

static int to_valid_position(FILE *fd)
{
    char c;
    while ((c = fgetc(fd)) && !feof(fd))
    {
        if (c == '#')
            while ((c = fgetc(fd)) && !feof(fd) && c != '\n');
        else if (c == '\n'
                || c == '\r'
                || c == '\t'
                || c == ' '
                || c == '\x0b'
                || c == '\x0c')
        {
            continue;
        }
        else
            break;
    }

    if (feof(fd))
    {
        fclose(fd);
        return 1;
    }

    ungetc(c, fd);
    return 0;
}

void get_magick_numbers(FILE *fd, char *number)
{
    rewind(fd);
    if (to_valid_position(fd) != 0)
    {
        fprintf(stderr, "[ERROR] PNM: cannot find the magick numbers\n");
        exit(1);
    }
    if (fread(number, 1, 2, fd) != 2)
    {
        fprintf(stderr, "[ERROR] PNM: error reading the magick numbers\n");
        exit(1);
    }
    number[2] = 0;
}

size_t get_value(FILE *fd)
{
    if (to_valid_position(fd) != 0)
    {
        fprintf(stderr, "[ERROR] PNM: cannot find value\n");
        exit(1);
    }

    char c;
    size_t val = 0;
    while ((c = fgetc(fd)) && !feof(fd) && c > 47 && c < 58)
    {
        val *= 10;
        val += c - 48;
    }

    return val;
}

static void create_stream(FILE **fd, const char *path, const char *mode)
{
    *fd = fopen(path, mode);

    if (!fd)
    {
        fprintf(stderr, "[ERROR] PNM: cannot open file %s\n", path);
        exit(1);
    }
}

void sil_pnm_write_stream(const simage_t *img, FILE *fd)
{
    size_t width = sil_image_get_width(img);
    size_t height = sil_image_get_height(img);
    unsigned maxval = 0;

    char magick_num[3];
    magick_num[0] = 'P';
    magick_num[2] = 0;

    switch (sil_image_get_type(img))
    {
        case SIL_IMAGE_GRAY_8:
            maxval = 255;
            magick_num[1] = '5';
            break;
        case SIL_IMAGE_GRAY_16:
            maxval = 65535;
            magick_num[1] = '5';
            break;
        case SIL_IMAGE_RGB_24:
            maxval = 255;
            magick_num[1] = '6';
            break;
        case SIL_IMAGE_RGB_48:
            maxval = 65535;
            magick_num[1] = '6';
            break;
    }
    size_t size = sil_image_byte_per_pixel(img);

    fprintf(fd, "%s\n%zd %zd\n%d\n", magick_num, width, height, maxval);

    for (size_t i = 0; i < height; ++i)
        fwrite(sil_image_data_row8(img, i), size, width, fd);
}

simage_t *sil_pnm_read_stream(FILE *fd)
{
    char magick_num[3];
    get_magick_numbers(fd, magick_num);

    size_t width = get_value(fd);
    size_t height = get_value(fd);
    size_t maxval = get_value(fd);

    stype_t type = 0;

    switch (magick_num[1])
    {
        case '5':
        {
            if (maxval < 256)
                type = SIL_IMAGE_GRAY_8;
            else if (maxval < 65536)
                type = SIL_IMAGE_GRAY_16;
            else
            {
                fprintf(stderr, "[ERROR] PNM: wrong max value\n");
                exit(1);
            }
            break;
        }
        case '6':
        {
            if (maxval < 256)
                type = SIL_IMAGE_RGB_24;
            else if (maxval < 65536)
                type = SIL_IMAGE_RGB_48;
            else
            {
                fprintf(stderr, "[ERROR] PNM: wrong max value\n");
                exit(1);
            }

            break;
        }
    }

    simage_t *img = sil_image_new(width, height, type);
    if (!img)
    {
        fprintf(stderr, "[ERROR] PNM: cannot allocate image\n");
        exit(1);
    }

    size_t size = sil_image_byte_per_pixel(img);

    for (size_t i = 0; i < height; ++i)
    {
        if (fread(sil_image_data_row8(img, i), size, width, fd) != width)
            break;
    }

    return img;
}

simage_t *sil_pnm_read_path(const char *path)
{
    FILE *fd = NULL;
    create_stream(&fd, path, "r");
    simage_t *img = sil_pnm_read_stream(fd);
    fclose(fd);
    return img;
}

void sil_pnm_write_path(const simage_t *img, const char *path)
{
    FILE *fd = NULL;
    create_stream(&fd, path, "w");
    sil_pnm_write_stream(img, fd);
    fclose(fd);
}
