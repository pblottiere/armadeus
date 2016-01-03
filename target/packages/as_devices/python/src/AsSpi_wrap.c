/*
**    The ARMadeus Project
**
**    Copyright (C) 2012 The armadeus systems team
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
*
*/

#include "AsSpi_wrap.h"
#include "as_spi.h"

/* Methods definitions */
static PyMethodDef AsSpi_wrap_methods[] = {
	{"spi_open", spi_open, METH_VARARGS, "as_spi function wrapped"},
	{"spi_close", spi_close, METH_VARARGS, "as_spi function wrapped"},
	{"spi_set_lsb", spi_set_lsb, METH_VARARGS, "as_spi function wrapped"},
	{"spi_get_lsb", spi_get_lsb, METH_VARARGS, "as_spi function wrapped"},
	{"spi_set_mode", spi_set_mode, METH_VARARGS, "as_spi function wrapped"},
	{"spi_get_mode", spi_get_mode, METH_VARARGS, "as_spi function wrapped"},
	{"spi_set_speed", spi_set_speed, METH_VARARGS,
		"as_spi function wrapped"},
	{"spi_get_speed", spi_get_speed, METH_VARARGS,
		"as_spi function wrapped"},
	{"spi_get_bits_per_word", spi_get_bits_per_word, METH_VARARGS,
		"as_spi function wrapped"},
	{"spi_set_bits_per_word", spi_set_bits_per_word, METH_VARARGS,
		"as_spi function wrapped"},
	{"spi_msg", spi_msg, METH_VARARGS, "as_spi function wrapped"},
	{NULL, NULL, 0, NULL}	/* Sentinel */
};

/* Init Module */
void initAsSpi_wrap()
{				/* called on first import *//* name matter if called dynamically */
	(void)Py_InitModule("AsSpi_wrap", AsSpi_wrap_methods);	/* mod name, table ptr */
}

static PyObject *spi_open(PyObject * self, PyObject * args)
{
	/* parameters */
	unsigned char *aSpidevName;

	int dev;

	PyObject *ret;
	char buff[300];
	/* Get arguments */
	if (!PyArg_ParseTuple(args, "s", &aSpidevName)) {
		PyErr_SetString(PyExc_IOError, "Wrong parameters.");
		return NULL;
	}
	dev = as_spi_open(aSpidevName);
	if (dev < 0) {
		snprintf(buff, 300, "Can't open spidev %s", aSpidevName);
		PyErr_SetString(PyExc_IOError, buff);
		return NULL;
	}

	ret = Py_BuildValue("l", (long)dev);
	return ret;
}

static PyObject *spi_close(PyObject * self, PyObject * args)
{
	/* parameters */
	long aDev;

	/* Get arguments */
	if (!PyArg_ParseTuple(args, "l", (long *)&aDev)) {
		PyErr_SetString(PyExc_IOError, "Wrong parameters.");
		return NULL;
	}

	as_spi_close((int)aDev);

	return Py_BuildValue("i", 0);
}

static PyObject *spi_set_lsb(PyObject * self, PyObject * args)
{
	/* parameters */
	long aDev;
	int aLsb;

	int ret;

	/* Get arguments */
	if (!PyArg_ParseTuple(args, "li", (long *)&aDev, &aLsb)) {
		PyErr_SetString(PyExc_IOError, "Wrong parameters.");
		return NULL;
	}

	ret = as_spi_set_lsb((int)aDev, aLsb);
	if (ret < 0) {
		PyErr_SetString(PyExc_IOError, "Can't set lsb");
		return NULL;
	}

	return Py_BuildValue("i", ret);
}

static PyObject *spi_get_lsb(PyObject * self, PyObject * args)
{
	/* parameters */
	long aDev;

	int ret;

	/* Get arguments */
	if (!PyArg_ParseTuple(args, "l", (long *)&aDev)) {
		PyErr_SetString(PyExc_IOError, "Wrong parameters.");
		return NULL;
	}

	ret = as_spi_get_lsb((int)aDev);
	if (ret < 0) {
		PyErr_SetString(PyExc_IOError, "Can't get lsb");
		return NULL;
	}

	return Py_BuildValue("i", ret);
}

static PyObject *spi_set_mode(PyObject * self, PyObject * args)
{
	/* parameters */
	long aDev;
	int aMode;

	int ret;

	/* Get arguments */
	if (!PyArg_ParseTuple(args, "li", (long *)&aDev, &aMode)) {
		PyErr_SetString(PyExc_IOError, "Wrong parameters.");
		return NULL;
	}

	ret = as_spi_set_mode((int)aDev, aMode);
	if (ret < 0) {
		PyErr_SetString(PyExc_IOError, "Can't set mode");
		return NULL;
	}

	return Py_BuildValue("i", ret);
}

static PyObject *spi_get_mode(PyObject * self, PyObject * args)
{
	/* parameters */
	long aDev;

	int ret;

	/* Get arguments */
	if (!PyArg_ParseTuple(args, "l", (long *)&aDev)) {
		PyErr_SetString(PyExc_IOError, "Wrong parameters.");
		return NULL;
	}

	ret = as_spi_get_mode((int)aDev);
	if (ret < 0) {
		PyErr_SetString(PyExc_IOError, "Can't get lsb");
		return NULL;
	}

	return Py_BuildValue("i", ret);
}

static PyObject *spi_set_speed(PyObject * self, PyObject * args)
{
	/* parameters */
	long aDev;
	int aSpeed;

	int ret;

	/* Get arguments */
	if (!PyArg_ParseTuple(args, "li", (long *)&aDev, &aSpeed)) {
		PyErr_SetString(PyExc_IOError, "Wrong parameters.");
		return NULL;
	}

	ret = as_spi_set_speed((int)aDev, aSpeed);
	if (ret < 0) {
		PyErr_SetString(PyExc_IOError, "Can't set speed");
		return NULL;
	}

	return Py_BuildValue("i", ret);
}

static PyObject *spi_get_speed(PyObject * self, PyObject * args)
{
	/* parameters */
	long aDev;

	int ret;

	/* Get arguments */
	if (!PyArg_ParseTuple(args, "l", (long *)&aDev)) {
		PyErr_SetString(PyExc_IOError, "Wrong parameters.");
		return NULL;
	}

	ret = as_spi_get_speed((int)aDev);
	if (ret < 0) {
		PyErr_SetString(PyExc_IOError, "Can't get speed");
		return NULL;
	}

	return Py_BuildValue("i", ret);
}

static PyObject *spi_get_bits_per_word(PyObject * self, PyObject * args)
{
	/* parameters */
	long aDev;

	int ret;

	/* Get arguments */
	if (!PyArg_ParseTuple(args, "l", (long *)&aDev)) {
		PyErr_SetString(PyExc_IOError, "Wrong parameters.");
		return NULL;
	}

	ret = as_spi_get_bits_per_word((int)aDev);
	if (ret < 0) {
		PyErr_SetString(PyExc_IOError, "Can't get bit per word");
		return NULL;
	}

	return Py_BuildValue("i", ret);
}

static PyObject *spi_set_bits_per_word(PyObject * self, PyObject * args)
{
	/* parameters */
	long aDev;
	int aBits;

	int ret;

	/* Get arguments */
	if (!PyArg_ParseTuple(args, "li", (long *)&aDev, &aBits)) {
		PyErr_SetString(PyExc_IOError, "Wrong parameters.");
		return NULL;
	}

	ret = as_spi_set_bits_per_word((int)aDev, aBits);
	if (ret < 0) {
		PyErr_SetString(PyExc_IOError, "Can't set speed");
		return NULL;
	}

	return Py_BuildValue("i", ret);
}

static PyObject *spi_msg(PyObject * self, PyObject * args)
{
	/* parameters */
	long aDev;
	int aSpeed;
	int aLen;
	char msg[8];

	long long ret, value;

	/* Get arguments */
	if (!PyArg_ParseTuple
	    (args, "liibbbbbbbb", (long *)&aDev, &aSpeed, &aLen, &msg[0],
	     &msg[1], &msg[2], &msg[3], &msg[4], &msg[5], &msg[6], &msg[7])) {
		PyErr_SetString(PyExc_IOError, "Wrong parameters.");
		return NULL;
	}

	value = 0;
	value += (long long)msg[0] << (7 - 0) * 8;
	value += (long long)msg[1] << (7 - 1) * 8;
	value += (long long)msg[2] << (7 - 2) * 8;
	value += (long long)msg[3] << (7 - 3) * 8;
	value += (long long)msg[4] << (7 - 4) * 8;
	value += (long long)msg[5] << (7 - 5) * 8;
	value += (long long)msg[6] << (7 - 6) * 8;
	value += (long long)msg[7] << (7 - 7) * 8;

	ret = as_spi_msg((int)aDev, value, aLen, aSpeed);
	if (ret < 0) {
		PyErr_SetString(PyExc_IOError, "Can't make message");
		return NULL;
	}

	return Py_BuildValue("bbbbbbbb", msg[0], msg[1], msg[2], msg[3],
			     msg[4], msg[5], msg[6], msg[7]);
}
