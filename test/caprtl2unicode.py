#!/usr/bin/env python
"""A mapping of CapRTL to Unicode

A mapping of CapRTL to Unicode.  Used Unicode blocks in the range of
the mapping are:

- 0x0000-7f: Basic Latin
- 0x0080-ff: Latin-1 Supplement
- 0x0590-ff: Hebrew
- 0x0600-ff: Arabic
- 0x2000-6f: General Punctuation
- 0x20d0-ff: Combining Diacritical Marks for Symbols



Author:
  Behnam Esfahbod, 2005

Copyright (C) 2005 Sharif FarsiWeb, Inc

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this library, in a file named COPYING; if not, write to the
Free Software Foundation, Inc., 59 Temple Place, Suite 330,
Boston, MA 02111-1307, USA

For licensing issues, contact <license@farsiweb.info>.
"""



escape = {
    '_>': '\x04',
    '_<': '\x05',
    '_R': '\x0e',
    '_r': '\x0f',
    '_L': '\x10',
    '_l': '\x11',
    '_o': '\x12',
    }

mapping = [
    # 0x
    0x0040, 0x0060, 0x060e, 0x060f, 0x200e, 0x200f, 0x00a6, 0x00a7,
    0x00a8, 0x00a9, 0x00ae, 0x00ab, 0x00ac, 0x000d, 0x202e, 0x202b,
    # 1x
    0x202d, 0x202a, 0x202c, 0x2028, 0x00d7, 0x00f7, 0x00b6, 0x00b7,
    0x00a1, 0x2010, 0x2011, 0x00bb, 0x00bc, 0x00bd, 0x00be, 0x00bf,
    # 2x
    0x0020, 0x0021, 0x0022, 0x005c, 0x0024, 0x0025, 0x0026, 0x0027,
    0x0028, 0x0029, 0x002a, 0x0023, 0x002c, 0x007c, 0x002b, 0x002d,
    # 3x
    0x0040, 0x0041, 0x0042, 0x0043, 0x06f4, 0x06f5, 0x0666, 0x0667,
    0x0668, 0x0669, 0x003a, 0x003b, 0x003c, 0x003d, 0x003e, 0x003f,
    # 4x
    0x05d0, 0x0628, 0x062c, 0x062f, 0x0647, 0x0648, 0x0632, 0x05d7,
    0x05d8, 0x05d9, 0x05da, 0x05db, 0x05dc, 0x05dd, 0x05de, 0x05df,
    # 5x
    0x05e0, 0x05e1, 0x05e2, 0x05e3, 0x05e4, 0x05e5, 0x05e6, 0x05e7,
    0x05e8, 0x05e9, 0x05ea, 0x005b, 0x000a, 0x005d, 0x00f7, 0x005f,
    # 6x
    0x20d0, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067,
    0x0068, 0x0069, 0x006a, 0x006b, 0x006c, 0x006d, 0x006e, 0x006f,
    # 7x
    0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077,
    0x0078, 0x0079, 0x007a, 0x007b, 0x0009, 0x007d, 0x202f, 0x007e,
    ] 


def convert2unicode (caprtl):
    """Converter from CapRTL to Unicode"""

    mytext = str(caprtl)
    for x in escape.keys(): mytext = mytext.replace(x, escape[x])
    return unicode(mytext).translate(mapping)


def convert2utf8 (caprtl):
    """Converter from CapRTL to UTF-8"""

    mytext = str(caprtl)
    for x in escape.keys(): mytext = mytext.replace(x, escape[x])
    return unicode(mytext).translate(mapping).encode('utf-8')



if __name__ == '__main__':
    import sys

    for mytext in sys.stdin.readlines():
	mytext = mytext[:-1]
	sys.stdout.write(caprtl_to_utf8.convert(mytext))
	print

# vi:set ai sw=4:
