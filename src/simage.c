#include <sil/simage.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Define the word size in bytes
#ifdef ARCH32
#define ARCH_WORD 4
#else
#define ARCH_WORD 8
#endif

struct simage
{
    size_t width;
    size_t height;
    size_t stride;
    stype_t type;
#ifdef ARCH32
    uint32_t *data;
#else
    uint64_t *data;
#endif
};

static inline size_t bytes_per_pixel(stype_t type)
{
    switch (type)
    {
        case SIL_IMAGE_GRAY_8:
            return 1;
        case SIL_IMAGE_GRAY_16:
            return 2;
        case SIL_IMAGE_RGB_24:
            return 3;
        case SIL_IMAGE_RGB_48:
            return 6;
    }
    return 0;
}

static simage_t *allocate_image(size_t width, size_t height, stype_t type, int zero)
{
    simage_t *img = (simage_t *) malloc (sizeof(simage_t));

    if (!img)
        return NULL;

    img->data = 0;

    size_t total = width * bytes_per_pixel(type);
    if (!total)
    {
        sil_image_free(img);
        return NULL;
    }
    // Amounts of blocks to store an image row
    size_t blocks = total / ARCH_WORD + ((total % ARCH_WORD) != 0);

#ifdef ARCH32
    if (zero)
        img->data = (uint32_t *) calloc(blocks * height, sizeof(uint32_t));
    else
        img->data = (uint32_t *) malloc (sizeof(uint32_t) * blocks * height);
#else
    if (zero)
        img->data = (uint64_t *) calloc(blocks * height, sizeof(uint64_t));
    else
        img->data = (uint64_t *) malloc (sizeof(uint64_t) * blocks * height);
#endif

    if (!img->data)
    {
        sil_image_free(img);
        return NULL;
    }

    img->stride = blocks * ARCH_WORD;
    img->width = width;
    img->height = height;
    img->type = type;

    return img;
}

simage_t *sil_image_new(size_t width, size_t height, stype_t type)
{
    return allocate_image(width, height, type, 0);
}

simage_t *sil_image_zero_new(size_t width, size_t height, stype_t type)
{
    return allocate_image(width, height, type, 1);
}

simage_t *sil_image_copy(const simage_t *img)
{
    simage_t *dst = allocate_image(img->width, img->height, img->type, 0);
    if (!dst)
        return NULL;

    size_t size = img->stride * img->height;
#ifdef ARCH32
    memcpy(dst->data, img->data, size * sizeof(uint32_t));
#else
    memcpy(dst->data, img->data, size * sizeof(uint64_t));
#endif
    return dst;
}

void sil_image_free(simage_t *img)
{
    free (img->data);
    free (img);
}

void sil_image_roi(simage_t *img, size_t top, size_t left, size_t width, size_t height)
{
    assert (top <= img->height
        && height <= img->height
        && top + height <= img->height
        && left <= img->width
        && width <= img->width
        && left + width <= img->width);

    size_t n = top * img->stride + left * bytes_per_pixel(img->type);
    assert((n % ARCH_WORD) == 0);

    img->width = width;
    img->height = height;
    img->data += n / ARCH_WORD;
}

void sil_image_zero(simage_t *img)
{
    memset(img->data, 0, img->stride * img->height);
}

void sil_image_set_pixel(simage_t *img, size_t x, size_t y, uint64_t value)
{
    switch (img->type)
    {
        case SIL_IMAGE_GRAY_8:
        {
            sil_image_data_row8(img, y)[x] = value;
            break;
        }
        case SIL_IMAGE_GRAY_16:
        {
            uint8_t *p = sil_image_data_row8(img, y) + x * bytes_per_pixel(img->type);
            *p = value >> 8;
            *(p + 1) = value;
            break;
        }
        case SIL_IMAGE_RGB_24:
        {
            uint8_t *p = sil_image_data_row8(img, y) + x * bytes_per_pixel(img->type);
            *p = value >> 16;
            *(p + 1) = value >> 8;
            *(p + 2) = value;
            break;
        }
        case SIL_IMAGE_RGB_48:
        {
            uint8_t *p = sil_image_data_row8(img, y) + x * bytes_per_pixel(img->type);

            for (int i = 0, j = 40; i < 6; ++i, j -= 8)
                *(p + i) = value >> j;
            break;
        }
    }
}

uint64_t sil_image_get_pixel(const simage_t *img, size_t x, size_t y)
{
    switch (img->type)
    {
        case SIL_IMAGE_GRAY_8:
            return sil_image_data_row8(img, y)[x];
        case SIL_IMAGE_GRAY_16:
        {
            uint8_t *p = sil_image_data_row8(img, y) + x * bytes_per_pixel(img->type);
            return (*p << 8) | *(p + 1);
        }
        case SIL_IMAGE_RGB_24:
        {
            uint8_t *p = sil_image_data_row8(img, y) + x * bytes_per_pixel(img->type);
            return *p << 16 | *(p + 1) << 8 | *(p + 2) ;
        }
        case SIL_IMAGE_RGB_48:
        {
            uint8_t *p = sil_image_data_row8(img, y) + x * bytes_per_pixel(img->type);
            uint64_t a1 = *p << 16 | *(p + 1) << 8 | *(p + 2);
            uint64_t a2 = *(p + 3) << 16  | *(p + 4) << 8 | *(p + 5);
            return a1 << 24 | a2;
        }
    }
    return 0;
}

inline uint8_t *sil_image_data8(const simage_t *img)
{
    return (uint8_t *)img->data;
}

inline uint16_t *sil_image_data16(const simage_t *img)
{
    return (uint16_t *)img->data;
}

inline uint32_t *sil_image_data32(const simage_t *img)
{
    return (uint32_t *)img->data;
}

inline uint8_t *sil_image_data_row8(const simage_t *img, size_t y)
{
    return (uint8_t *)(img->data + img->stride * y / ARCH_WORD);
}

inline uint16_t *sil_image_data_row16(const simage_t *img, size_t y)
{
    return (uint16_t *)(img->data + img->stride * y / ARCH_WORD);
}

inline uint32_t *sil_image_data_row32(const simage_t *img, size_t y)
{
    return (uint32_t *)(img->data + img->stride * y / ARCH_WORD);
}

inline size_t sil_image_get_width(const simage_t *img)
{
    return img->width;
}

inline size_t sil_image_get_height(const simage_t *img)
{
    return img->height;
}

inline size_t sil_image_get_stride(const simage_t *img)
{
    return img->stride;
}

inline size_t sil_image_byte_per_pixel(const simage_t *img)
{
    return bytes_per_pixel(img->type);
}

inline stype_t sil_image_get_type(const simage_t *img)
{
    return img->type;
}

