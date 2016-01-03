/*
**    THE ARMadeus Systems
**
**    Copyright (C) 2010, 2011  The armadeus systems team
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
 * @brief Provide Python wrapper for GPIOs
 *
 * @ingroup python_wrappers
 */

#include <Python.h>

/* Init module */
void initAsGpio_wrap();

/*********************/
/* Functions wrapped */
/*********************/
static PyObject *gpio_open(PyObject * self, PyObject * args);

static PyObject *setPinDirection(PyObject * self, PyObject * args);
static PyObject *getPinDirection(PyObject * self, PyObject * args);
static PyObject *setPinValue(PyObject * self, PyObject * args);
static PyObject *getPinValue(PyObject * self, PyObject * args);

static PyObject *blockingGetPinValue(PyObject * self, PyObject * args);
static PyObject *getIrqMode(PyObject * self, PyObject * args);
static PyObject *setIrqMode(PyObject * self, PyObject * args);
static PyObject *getPinNumber(PyObject * self, PyObject * args);
static PyObject *getPortLetter(PyObject * self, PyObject * args);

static PyObject *gpio_close(PyObject * self, PyObject * args);
