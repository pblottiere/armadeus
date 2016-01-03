/*
**    THE ARMadeus Systems
**
**    Copyright (C) 2012  The armadeus systems team
**    Fabien Marteau <fabien.marteau@armadeus.com>
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with this library; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/** @file
 * @brief Provide Python wrapper for SPI
 *
 * @ingroup python_wrappers
 */

#include <Python.h>

/* Init module */
void initAsSpi_wrap();

/*********************/
/* Functions wrapped */
/*********************/

static PyObject *spi_open(PyObject * self, PyObject * args);
static PyObject *spi_close(PyObject * self, PyObject * args);

static PyObject *spi_set_lsb(PyObject * self, PyObject * args);
static PyObject *spi_get_lsb(PyObject * self, PyObject * args);

static PyObject *spi_set_mode(PyObject * self, PyObject * args);
static PyObject *spi_get_mode(PyObject * self, PyObject * args);

static PyObject *spi_set_speed(PyObject * self, PyObject * args);
static PyObject *spi_get_speed(PyObject * self, PyObject * args);

static PyObject *spi_get_bits_per_word(PyObject * self, PyObject * args);
static PyObject *spi_set_bits_per_word(PyObject * self, PyObject * args);
static PyObject *spi_msg(PyObject * self, PyObject * args);
