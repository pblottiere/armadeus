/*
**    The ARMadeus Project
**
**    Copyright (C) 2010  The armadeus systems team
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
**
** TODO:
**    - Change name of function "blockingGetPinValue" to "WaitEvent" as named
**      under C library
*/

#include "AsGpio_wrap.h"
#include "as_gpio.h"

/* Methods definitions */
static PyMethodDef AsGpio_wrap_methods[] = {
	{"gpio_open", gpio_open, METH_VARARGS, "Initialize gpio"},
	{"setPinDirection", setPinDirection, METH_VARARGS, "Set pin direction"},
	{"getPinDirection", getPinDirection, METH_VARARGS, "Get pin direction"},
	{"setPinValue", setPinValue, METH_VARARGS, "Set pin value"},
	{"getPinValue", getPinValue, METH_VARARGS, "Get pin value"},
	{"blockingGetPinValue", blockingGetPinValue, METH_VARARGS,
		"Blocking read pin value"},
	{"getIrqMode", getIrqMode, METH_VARARGS, "Get IRQ Mode"},
	{"setIrqMode", setIrqMode, METH_VARARGS, "Set IRQ Mode"},
	{"getPinNumber", getPinNumber, METH_VARARGS, "Get pin number"},
	{"gpio_close", gpio_close, METH_VARARGS, "Close gpio"},
	{NULL, NULL, 0, NULL}	/* Sentinel */
};

/* Init module */
void initAsGpio_wrap()
{				/* called on first import *//* name matter if called dynamically */
	(void)Py_InitModule("AsGpio_wrap", AsGpio_wrap_methods);	/* mod name, table ptr */
}

/** @brief Initialize pin port access
 * @param aGpioNum pin number on port
 * @return PyObject
 */
static PyObject *gpio_open(PyObject * self, PyObject * args)
{
	/* parameters */
	int aGpioNum;

	struct as_gpio_device *dev;
	PyObject *ret;
	char buff[300];
	/* Get arguments */
	if (!PyArg_ParseTuple(args, "i", &aGpioNum)) {
		PyErr_SetString(PyExc_IOError, "Wrong parameters.");
		return NULL;
	}

	dev = as_gpio_open(aGpioNum);
	if (dev == NULL) {
		snprintf(buff, 300,
			 "Initialization error %d. Is kernel module loaded ?",
			 aGpioNum);
		PyErr_SetString(PyExc_IOError, buff);
		return NULL;
	}

	ret = Py_BuildValue("l", (long)dev);

	return ret;
}

/** @brief  Set pin direction
 *
 * @param aFdev as_gpio_device structure pointer
 * @param aDirection direction "in" / "out"
 *
 * @return error if negative value
 */
static PyObject *setPinDirection(PyObject * self, PyObject * args)
{
	/* parameters */
	struct as_gpio_device *aFdev;
	char *aDirection;

	int ret;

	/* Get arguments */
	if (!PyArg_ParseTuple(args, "ls", (long *)&aFdev, &aDirection)) {
		PyErr_SetString(PyExc_IOError, "Wrong parameters.");
		return NULL;
	}

	ret = as_gpio_set_pin_direction(aFdev, aDirection);
	if (ret < 0) {
		PyErr_SetString(PyExc_IOError, "Can't set pin direction");
		return NULL;
	}

	return Py_BuildValue("i", ret);
}

static PyObject *getPinDirection(PyObject * self, PyObject * args)
{
	/* parameters */
	struct as_gpio_device *aFdev;

	const char *dir;

	/* Get arguments */
	if (!PyArg_ParseTuple(args, "l", (long *)&aFdev)) {
		PyErr_SetString(PyExc_IOError, "Wrong parameters.");
		return NULL;
	}

	dir = as_gpio_get_pin_direction(aFdev);
	if (strcmp(dir, "??")) {
		PyErr_SetString(PyExc_IOError, "Can't get pin direction");
		return NULL;
	}

	return Py_BuildValue("s", dir);
}

/** @brief Set pin value
 *
 * @param aFdev as_gpio_device structure pointer
 * @param aValue value of pin (1 or 0)
 *
 * @return error if negative
 */
static PyObject *setPinValue(PyObject * self, PyObject * args)
{
	/* parameters */
	struct as_gpio_device *aFdev;
	int aValue;

	int ret;

	/* Get arguments */
	if (!PyArg_ParseTuple(args, "li", (long *)&aFdev, &aValue)) {
		PyErr_SetString(PyExc_IOError, "Wrong parameters.");
		return NULL;
	}

	ret = as_gpio_set_pin_value(aFdev, aValue);
	if (ret < 0) {
		PyErr_SetString(PyExc_IOError, "Can't set pin value");
		return NULL;
	}

	return Py_BuildValue("i", ret);
}

/** @brief Get pin value
 *
 * @param aFdev as_gpio_device structure pointer
 * @param aPinNum pin number
 *
 * @return pin value if positive or null, error if negative
 */
static PyObject *getPinValue(PyObject * self, PyObject * args)
{
	/* parameters */
	struct as_gpio_device *aFdev;

	int ret;

	/* Get arguments */
	if (!PyArg_ParseTuple(args, "l", (long *)&aFdev)) {
		PyErr_SetString(PyExc_IOError, "Wrong parameters.");
		return NULL;
	}

	ret = as_gpio_get_pin_value(aFdev);
	if (ret < 0) {
		PyErr_SetString(PyExc_IOError, "Can't get pin value");
		return NULL;
	}

	return Py_BuildValue("i", ret);
}

/** @brief Get pin value, blocking until interrupt occur
 *
 * @param aFdev as_gpio_device structure pointer
 * @param aDelay_s seconds delay
 * @param aDelay_us useconds delay
 *
 * @return pin value if positive or null, error if negative
 */
static PyObject *blockingGetPinValue(PyObject * self, PyObject * args)
{
	/* parameters */
	struct as_gpio_device *aFdev;
	int aDelay_ms;

	int ret;

	/* Get arguments */
	if (!PyArg_ParseTuple(args, "li", (long *)&aFdev, &aDelay_ms)) {
		PyErr_SetString(PyExc_IOError, "Wrong parameters.");
		return NULL;
	}

	Py_BEGIN_ALLOW_THREADS ret = as_gpio_wait_event(aFdev, aDelay_ms);
	Py_END_ALLOW_THREADS if (ret == -ETIMEDOUT) {
		PyErr_SetString(PyExc_IOError, "TIMEOUT");
		return NULL;
	}
	if (ret < 0) {
		PyErr_SetString(PyExc_IOError, "Can't get pin value");
		return NULL;
	}

	return Py_BuildValue("i", ret);
}

/** @brief Set pin irq mode
 *
 * @param aFdev as_gpio_device structure pointer
 * @param aMode irq mode
 *
 * @return error if negative
 */
static PyObject *setIrqMode(PyObject * self, PyObject * args)
{
	/* parameters */
	struct as_gpio_device *aFdev;
	char *aMode;

	int ret;

	/* Get arguments */
	if (!PyArg_ParseTuple(args, "ls", (long *)&aFdev, &aMode)) {
		PyErr_SetString(PyExc_IOError, "Wrong parameters.");
		return NULL;
	}

	ret = as_gpio_set_irq_mode(aFdev, aMode);
	if (ret < 0) {
		PyErr_SetString(PyExc_IOError, "Can't get pin value");
		return NULL;
	}

	return Py_BuildValue("i", ret);
}

/** @brief Get pin irq mode value
 *
 * @param aFDev as_gpio_device pointer structure
 *
 * @return pin mode ("falling"/"rising"/"both"/"none")
 */
static PyObject *getIrqMode(PyObject * self, PyObject * args)
{
	/* parameters */
	struct as_gpio_device *aFdev;

	const char *mode;

	/* Get arguments */
	if (!PyArg_ParseTuple(args, "l", (long *)&aFdev)) {
		PyErr_SetString(PyExc_IOError, "Wrong parameters.");
		return NULL;
	}

	mode = as_gpio_get_irq_mode(aFdev);
	if (strcmp(mode, "??")) {
		PyErr_SetString(PyExc_IOError, "Can't get pin value");
		return NULL;
	}

	return Py_BuildValue("s", mode);
}

/** @brief Get pin number value
 *
 * @param aDev as_gpio_device pointer structure
 *
 * @return pin number, error if negative
 */
static PyObject *getPinNumber(PyObject * self, PyObject * args)
{
	/* parameters */
	struct as_gpio_device *aFdev;

	int ret;

	/* Get arguments */
	if (!PyArg_ParseTuple(args, "l", (long *)&aFdev)) {
		PyErr_SetString(PyExc_IOError, "Wrong parameters.");
		return NULL;
	}

	ret = as_gpio_get_pin_num(aFdev);
	if (ret < 0) {
		PyErr_SetString(PyExc_IOError, "Can't get pin value");
		return NULL;
	}

	return Py_BuildValue("i", ret);
}

/** @brief Close port access
 *
 * @param aFdev as_gpio_device structure pointer
 *
 * @return pin value if positive or null, error if negative
 */
static PyObject *gpio_close(PyObject * self, PyObject * args)
{
	/* parameters */
	struct as_gpio_device *aFdev;

	int ret;

	/* Get arguments */
	if (!PyArg_ParseTuple(args, "l", (long *)&aFdev)) {
		PyErr_SetString(PyExc_IOError, "Wrong parameters.");
		return NULL;
	}

	ret = as_gpio_close(aFdev);
	if (ret < 0) {
		PyErr_SetString(PyExc_IOError, "Can't close port");
		return NULL;
	}

	return Py_BuildValue("i", ret);
}
