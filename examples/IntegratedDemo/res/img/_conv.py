# -*- coding: utf-8 -*-
#
# Arduboy Image Converter
#

import Image
import fnmatch
import sys
import os


def bmpConv(fname):
	list = []
	im = Image.open(fname)
	cx = im.size[0]
	cy = im.size[1]

	if cx % 8 != 0:
		print "size error: " + fname
		sys.exit(1)

	for y in range(0, cy):
		for x in range(7, cx, 8):
			bit = 0
			if im.getpixel((x-7, y)) == 255: bit += 0x80
			if im.getpixel((x-6, y)) == 255: bit += 0x40
			if im.getpixel((x-5, y)) == 255: bit += 0x20
			if im.getpixel((x-4, y)) == 255: bit += 0x10
			if im.getpixel((x-3, y)) == 255: bit += 0x08
			if im.getpixel((x-2, y)) == 255: bit += 0x04
			if im.getpixel((x-1, y)) == 255: bit += 0x02
			if im.getpixel((x-0, y)) == 255: bit += 0x01
			list.append(bit)

	fname2 = os.path.splitext(fname)[0]

	s =  ""
	s += "// " + fname + " / " + str(cx) + "x" + str(cy) + "\n"
	s += "PROGMEM const unsigned char bitmap" + fname2[0].upper() + fname2[1:] + "[] = {\n"
	s += "\t" + str(cx) + ", " + str(cy) + ","

	for i in range(len(list)):
		if i == 0 or i % cx == 0:
			s +=  "\n\t"

		s +=  "0x%02X" % list[i]

		if i != len(list)-1:
			s += ","

	s += "\n};\n"
	s += "\n"

	return s


bmp = fnmatch.filter(os.listdir("."), "*.bmp")
s = ""

for f in bmp:
	s += bmpConv(f)

h1 = ""
h1 += "#ifndef IMG_H\n"
h1 += "#define IMG_H\n"
h1 += "#ifdef __cplusplus\n"
h1 += "extern \"C\" {\n"
h1 += "#endif\n"
h1 += "\n"
h1 += "#include <avr/pgmspace.h>\n"
h1 += "\n"

h2 = ""
h2 += "\n"
h2 += "#ifdef __cplusplus\n"
h2 += "}\n"
h2 += "#endif\n"
h2 += "#endif\n"


f = open("img.h", 'w')
f.write(h1)
f.write(s)
f.write(h2)

f.close()
