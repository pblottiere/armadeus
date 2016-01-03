#
#  Copyright (C) 2010-2011  The Armadeus Project - ARMadeus Systems
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software 
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
# 

GPIOLIB_DIR=/sys/class/gpio

# PE11 == pin 139
echo 139 > $GPIOLIB_DIR/export
echo out > $GPIOLIB_DIR/gpio139/direction
echo 0 > $GPIOLIB_DIR/gpio139/value
echo 1 > $GPIOLIB_DIR/gpio139/value
