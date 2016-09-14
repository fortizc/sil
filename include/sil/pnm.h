#ifndef SIL_PNM_H
#define SIL_PNM_H

#include <stdio.h>

struct simage *sil_pnm_read_path(const char *path);
struct simage *sil_pnm_read_stream(FILE *fd);
void sil_pnm_write_path(const struct simage *img, const char *path);
void sil_pnm_write_stream(const struct simage *img, FILE *fd);

#endif
