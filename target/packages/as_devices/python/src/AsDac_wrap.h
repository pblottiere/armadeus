/*
**    THE ARMadeus Systems
**
**    Copyright (C) 2011  The armadeus systems team
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
 * @brief Provide Python wrapper for DAC
 *
 * @ingroup python_wrappers
 */

#include <Python.h>

/* Init module */
void initAsDac_wrap();

/*********************/
/* Functions wrapped */
/*********************/
static PyObject *dac_open(PyObject * self, PyObject * args);
static PyObject *dac_setValueInMillivolts(PyObject * self, PyObject * args);
static PyObject *dac_close(PyObject * self, PyObject * args);
