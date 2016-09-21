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
 * Create an image and set some pixels then check for any
 * non zero byte
 */

#include <sil/simage.h>
#include <sil/pnm.h>

#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>

#define WIDTH 20
#define HEIGHT 20
#define RND_PIX 30
#define TYPES 4

stype_t types[] = {SIL_IMAGE_GRAY_8,
                   SIL_IMAGE_GRAY_16,
                   SIL_IMAGE_RGB_24,
                   SIL_IMAGE_RGB_48};

static uint64_t get_color(int bpp)
{
    uint64_t color = 0;
    for (int i = 0; i < bpp; ++i)
        color = (color << 8) | rand() % 0xff;
    return color;
}

int main()
{
    srand(time(NULL));

    for (int k = 0; k < TYPES; ++k)
    {
        simage_t *img = sil_image_new(WIDTH, HEIGHT, types[k]);

        if (!img)
        {
            perror("[ERROR] zero_new: canot allocate image\n");
            return 1;
        }

        // Setting random pixels
        for (int i = 0; i < RND_PIX; ++i)
        {
            size_t w = rand() % (WIDTH - 1);
            size_t h = rand() % (HEIGHT - 1);
            uint64_t color = get_color((int) sil_image_byte_per_pixel(img));
            sil_image_set_pixel(img, w, h, color);
        }

        sil_image_zero(img);

        // Check for non zero bytes
        for (size_t i = 0; i < HEIGHT; ++i)
        {
            uint8_t *row = sil_image_data_row8(img, i);
            for (size_t j = 0; j < WIDTH; ++j)
            {
                if (row[i] != 0)
                {
                    sil_image_free(img);
                    perror("[ERROR] zero_new: a non zero byte was detected\n");
                    return 1;
                }
            }
        }
        sil_image_free(img);
    }
    printf("Test zero [OK]\n");
    return 0;
}

