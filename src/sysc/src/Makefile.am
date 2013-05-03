#
# This file is part of OpTiMSoC.
#
# OpTiMSoC is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation, either version 3 of
# the License, or (at your option) any later version.
#
# OpTiMSoC is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with OpTiMSoC. If not, see <http://www.gnu.org/licenses/>.
#
# ============================================================================
#
# (c) 2013 by the author(s)
#
# Author(s):
#    Stefan Wallentowitz <stefan.wallentowitz@tum.de>
#

lib_LTLIBRARIES = liboptimsocsysc.la

AM_CPPFLAGS = $(SYSTEMC_CPPFLAGS)
AM_LDFLAGS = $(SYSTEMC_LDFLAGS)

liboptimsocsysc_la_SOURCES =\
    debug/DebugConnector.cpp \
    debug/VerilatedDebugConnector.cpp \
    debug/VerilatedSTM.cpp

liboptimsocsysc_la_LDFLAGS = $(AM_LDFLAGS) $(LTLDFLAGS)
liboptimsocsysc_la_CPPFLAGS = $(AM_CPPFLAGS)