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

/*
 * This test creates an image and set random pixels with random
 * colors and then check each one
 */

#include <sil/simage.h>
#include <sil/pnm.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define PIX 10
#define WIDTH 20
#define HEIGHT 20
#define TYPES 4

stype_t types[] = {SIL_IMAGE_GRAY_8,
                   SIL_IMAGE_GRAY_16,
                   SIL_IMAGE_RGB_24,
                   SIL_IMAGE_RGB_48};
struct pixel
{
    size_t x, y;
    uint64_t color;
};

struct pixel pix_vec[PIX];

static uint64_t get_color(int bpp)
{
    uint64_t color = 0;
    for (int i = 0; i < bpp; ++i)
        color = (color << 8) | rand() % 0xff;
    return color;
}

static int is_unused_position(size_t x, size_t y)
{
    for (int i = 0; i < PIX; ++i)
    {
        if (pix_vec[i].x == x && pix_vec[i].y == y)
            return 0;
    }
    return 1;
}

int main()
{

    srand(time(NULL));

    for (int k = 0; k < TYPES; ++k)
    {
        simage_t *img = sil_image_new(WIDTH, HEIGHT, types[k]);
        if (!img)
        {
            perror("[ERROR] get_set_pixel: cannot allocate image\n");
            return 1;
        }

        for (int i = 0; i < PIX; ++i)
        {
            size_t x, y;

            do
            {
                x = rand() % (HEIGHT - 1);
                y = rand() % (WIDTH - 1);
            }
            while(!is_unused_position(x, y));

            pix_vec[i].x = x;
            pix_vec[i].y = y;
            pix_vec[i].color = get_color((int) sil_image_byte_per_pixel(img));

            sil_image_set_pixel(img, pix_vec[i].x, pix_vec[i].y, pix_vec[i].color);
        }

        for (int i = 0; i < PIX; ++i)
        {
            if (sil_image_get_pixel(img, pix_vec[i].x, pix_vec[i].y) != pix_vec[i].color)
            {
                perror("[ERROR] get_set_pixel: pixel mismatch\n");
                sil_image_free(img);
                return 1;
            }
        }
        sil_image_free(img);
    }

    printf("Test get_set_pixel [OK]\n");
    return 0;
}

