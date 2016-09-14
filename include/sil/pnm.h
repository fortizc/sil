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

#ifndef SIL_PNM_H
#define SIL_PNM_H

#include <stdio.h>

struct simage *sil_pnm_read_path(const char *path);
struct simage *sil_pnm_read_stream(FILE *fd);
void sil_pnm_write_path(const struct simage *img, const char *path);
void sil_pnm_write_stream(const struct simage *img, FILE *fd);

#endif
