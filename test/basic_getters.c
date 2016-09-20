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
 * This test create an image for each type
 * and test some basic getter like width, height, etc
 */

#include <sil/simage.h>

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define TYPES 4

stype_t types[] = {SIL_IMAGE_GRAY_8,
                   SIL_IMAGE_GRAY_16,
                   SIL_IMAGE_RGB_24,
                   SIL_IMAGE_RGB_48};

size_t bpp[] = {1, 2, 3, 6};

int main()
{
    srand(time(NULL));
    const int max_size = 100;
    const int min_size = 10;

    size_t width = rand() % max_size + min_size;
    size_t height = rand() % max_size + min_size;

    for (int i = 0; i < TYPES; ++i)
    {
        simage_t *img = sil_image_new(width, height, types[i]);
        if (!img)
        {
            perror("[ERROR] getters: cannot allocate image\n");
            return 1;
        }

        if (width != sil_image_get_width(img))
        {
            perror("[ERROR] getters: width mismatch\n");
            return 1;
        }

        if (height != sil_image_get_height(img))
        {
            perror("[ERROR] getters: height mismatch\n");
            return 1;
        }

        if (types[i] != sil_image_get_type(img))
        {
            perror("[ERROR] getters: type mismatch\n");
            return 1;
        }

        if (bpp[i] != sil_image_byte_per_pixel(img))
        {
            perror("[ERROR] getters: BPP mismatch\n");
        }
        sil_image_free(img);
    }
    printf("Test basic_getters [OK]\n");
    return 0;
}
