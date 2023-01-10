// Encoding
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/Encoding/UTF.hpp>
#include <XYO/Encoding/UTFStream.hpp>

namespace XYO::Encoding::UTF {

	const utf8 utf8StringQuestionMark[] = {'?', 0};
	const utf16 utf16StringQuestionMark[] = {'?', 0};
	const utf32 utf32StringQuestionMark[] = {'?', 0};

	size_t elementUTF32FromUTF8(utf32 *out, const utf8 *in) {
		size_t in_sz;
		in_sz = UTF8Core::elementSize(*in);
		if (in_sz == 0) {
			return 0;
		};
		if (!UTF8Core::elementIsValid(in)) {
			return 0;
		};
		*out = 0;
		switch (in_sz) {
		case 1:
			*out = (utf32)*in;
			break;
		case 2:
			*out = (utf32)(*in & 0x1F);
			*out <<= 6;
			++in;
			*out |= (utf32)(*in & 0x3F);
			break;
		case 3:
			*out = (utf32)(*in & 0x0F);
			*out <<= 6;
			++in;
			*out |= (utf32)(*in & 0x3F);
			*out <<= 6;
			++in;
			*out |= (utf32)(*in & 0x3F);
			break;
		case 4:
			*out = (utf32)(*in & 0x07);
			*out <<= 6;
			++in;
			*out |= (utf32)(*in & 0x3F);
			*out <<= 6;
			++in;
			*out |= (utf32)(*in & 0x3F);
			*out <<= 6;
			++in;
			*out |= (utf32)(*in & 0x3F);
			break;
		case 5:
			*out = (utf32)(*in & 0x03);
			*out <<= 6;
			++in;
			*out |= (utf32)(*in & 0x3F);
			*out <<= 6;
			++in;
			*out |= (utf32)(*in & 0x3F);
			*out <<= 6;
			++in;
			*out |= (utf32)(*in & 0x3F);
			*out <<= 6;
			++in;
			*out |= (utf32)(*in & 0x3F);
			break;
		case 6:
			*out = (utf32)(*in & 0x01);
			*out <<= 6;
			++in;
			*out |= (utf32)(*in & 0x3F);
			*out <<= 6;
			++in;
			*out |= (utf32)(*in & 0x3F);
			*out <<= 6;
			++in;
			*out |= (utf32)(*in & 0x3F);
			*out <<= 6;
			++in;
			*out |= (utf32)(*in & 0x3F);
			*out <<= 6;
			++in;
			*out |= (utf32)(*in & 0x3F);
			break;
		};
		if (UTF32Core::elementIsValid(*out)) {
			return 1;
		};
		return 0;
	};

	size_t elementUTF8FromUTF32Size(utf32 in) {
		if (!UTF32Core::elementIsValid(in)) {
			return 0;
		};

		// ISO-10646-UTF-8
		if ((in & 0xFFFFFF80) == 0x00000000) {
			return 1;
		};
		if ((in & 0xFFFFF800) == 0x00000000) {
			return 2;
		};
		if ((in & 0xFFFF0000) == 0x00000000) {
			return 3;
		};
		if ((in & 0xFFE00000) == 0x00000000) { // <-- never here ...
			return 4;
		};
		if ((in & 0xFC000000) == 0x00000000) {
			return 5;
		};
		if ((in & 0x80000000) == 0x00000000) {
			return 6;
		};
		return 0;
	};

	size_t elementUTF8FromUTF32(utf8 *out, utf32 in) {
		size_t sz;
		sz = elementUTF8FromUTF32Size(in);
		if (sz == 0) {
			return 0;
		};
		*out = 0;
		switch (sz) {
		case 1:
			*out = (utf8)in;
			break;
		case 2:
			out += 1;
			*out = (utf8)((in & 0x3F) | 0x80);
			--out;
			in >>= 6;
			*out = (utf8)((in & 0x1F) | 0xC0);
			break;
		case 3:
			out += 2;
			*out = (utf8)((in & 0x3F) | 0x80);
			--out;
			in >>= 6;
			*out = (utf8)((in & 0x3F) | 0x80);
			--out;
			in >>= 6;
			*out = (utf8)((in & 0x0F) | 0xE0);
			break;
		case 4:
			out += 3;
			*out = (utf8)((in & 0x3F) | 0x80);
			--out;
			in >>= 6;
			*out = (utf8)((in & 0x3F) | 0x80);
			--out;
			in >>= 6;
			*out = (utf8)((in & 0x3F) | 0x80);
			--out;
			in >>= 6;
			*out = (utf8)((in & 0x07) | 0xF0);
			break;
		case 5:
			out += 4;
			*out = (utf8)((in & 0x3F) | 0x80);
			--out;
			in >>= 6;
			*out = (utf8)((in & 0x3F) | 0x80);
			--out;
			in >>= 6;
			*out = (utf8)((in & 0x3F) | 0x80);
			--out;
			in >>= 6;
			*out = (utf8)((in & 0x3F) | 0x80);
			--out;
			in >>= 6;
			*out = (utf8)((in & 0x03) | 0xF8);
			break;
		case 6:
			out += 5;
			*out = (utf8)((in & 0x3F) | 0x80);
			--out;
			in >>= 6;
			*out = (utf8)((in & 0x3F) | 0x80);
			--out;
			in >>= 6;
			*out = (utf8)((in & 0x3F) | 0x80);
			--out;
			in >>= 6;
			*out = (utf8)((in & 0x3F) | 0x80);
			--out;
			in >>= 6;
			*out = (utf8)((in & 0x3F) | 0x80);
			--out;
			in >>= 6;
			*out = (utf8)((in & 0x01) | 0xFC);
			break;
		};
		return sz;
	};

	size_t elementUTF16FromUTF32Size(utf32 in) {
		// ISO-10646-UTF-16
		if (!UTF32Core::elementIsValid(in)) {
			return 0;
		};
		if (in <= 0x0000FFFF) {
			return 1;
		};
		return 2;
	};

	size_t elementUTF16FromUTF32(utf16 *out, utf32 in) {
		size_t sz;
		sz = elementUTF16FromUTF32Size(in);
		if (sz == 0) {
			return 0;
		};
		*out = 0;
		switch (sz) {
		case 1:
			*out = (utf16)in;
			break;
		case 2:
			in -= 0x00010000;
			++out;
			*out = (utf16)((in % 0x0400) + 0xDC00);
			--out;
			*out = (utf16)((in / 0x0400) + 0xD800);
			break;
		};
		return sz;
	};

	size_t elementUTF32FromUTF16(utf32 *out, const utf16 *in) {
		size_t sz;
		sz = UTF16Core::elementSize(*in);
		if (sz == 0) {
			return 0;
		};
		if (!UTF16Core::elementIsValid(in)) {
			return false;
		};
		*out = 0;
		switch (sz) {
		case 1:
			*out = (utf32)*in;
			break;
		case 2:
			*out = (*in - 0xD800);
			*out *= 0x400;
			++in;
			*out += (*in - 0xDC00);
			*out += 0x0010000;
			break;
		};
		if (UTF32Core::elementIsValid(*out)) {
			return 1;
		};
		return 0;
	};

	size_t elementUTF8FromUTF16Size(const utf16 *in) {
		if ((*in & 0xFC00) == 0xD800) {
			++in;
			if ((*in & 0xFF80) == 0x0000) {
				return 3;
			};
			if ((*in & 0xF800) == 0x0000) {
				return 4;
			};
			return 5;
		};

		if ((*in & 0xFC00) == 0xDC00) {
			return 0;
		}
		if (*in >= 0xFFFE) {
			return 0;
		};

		if ((*in & 0xFF80) == 0x0000) {
			return 1;
		};
		if ((*in & 0xF800) == 0x0000) {
			return 2;
		};
		return 3;
	};

	String utf8FromUTF16(const utf16 *in, const utf8 *err) {
		String retV;
		utf8 chr[8];
		utf32 tmp;
		size_t sz;

		while (*in) {

			sz = UTF16Core::elementSize(*in);
			if (sz) {
				if (elementUTF32FromUTF16(&tmp, in)) {
					chr[elementUTF8FromUTF32(chr, tmp)] = 0;
					retV += chr;
				} else {
					retV += err;
				};
				in += sz;
				continue;
			};

			retV += err;
			++in;
		};
		return retV;
	};

	String utf8FromUTF32(const utf32 *in, const utf8 *err) {
		String retV;
		utf8 chr[8];
		size_t sz;

		while (*in) {

			sz = UTF32Core::elementSize(*in);
			if (sz) {
				chr[elementUTF8FromUTF32(chr, *in)] = 0;
				retV += chr;
				in += sz;
				continue;
			};

			retV += err;
			++in;
		};
		return retV;
	};

	StringUTF16 utf16FromUTF8(const utf8 *in, const utf16 *err) {
		StringUTF16 retV;
		utf32 tmp;
		utf16 chr[4];
		size_t sz;

		while (*in) {

			sz = UTF8Core::elementSize(*in);
			if (sz) {
				if (elementUTF32FromUTF8(&tmp, in)) {
					chr[elementUTF16FromUTF32(chr, tmp)] = 0;
					retV += chr;
				} else {
					retV += err;
				};
				in += sz;
				continue;
			};

			retV += err;
			++in;
		};

		return retV;
	};

	StringUTF16 utf16FromUTF32(const utf32 *in, const utf16 *err) {
		StringUTF16 retV;
		utf16 chr[4];
		size_t sz;

		while (*in) {

			sz = UTF32Core::elementSize(*in);
			if (sz) {
				chr[elementUTF16FromUTF32(chr, *in)] = 0;
				retV += chr;
				continue;
			};

			retV += err;
			++in;
		};

		return retV;
	};

	StringUTF32 utf32FromUTF8(const utf8 *in, const utf32 *err) {
		StringUTF32 retV;
		utf32 chr[2];
		size_t sz;

		while (*in) {

			sz = UTF8Core::elementSize(*in);
			if (sz) {
				chr[elementUTF32FromUTF8(chr, in)] = 0;
				retV += chr;
				in += sz;
				continue;
			};

			retV += err;
			++in;
		};

		return retV;
	};

	StringUTF32 utf32FromUTF16(const utf16 *in, const utf32 *err) {
		StringUTF32 retV;
		utf32 chr[2];
		size_t sz;

		while (*in) {
			sz = UTF16Core::elementSize(*in);
			if (sz) {
				chr[elementUTF32FromUTF16(chr, in)] = 0;
				retV += chr;
				in += sz;
				continue;
			};

			retV += err;
			++in;
		};

		return retV;
	};

	size_t utf16FromUTF8Length(const utf8 *in, const utf16 *err) {
		size_t ln = 0;
		utf32 tmp;
		size_t sz;

		while (*in) {
			sz = UTF8Core::elementSize(*in);
			if (sz) {
				if (elementUTF32FromUTF8(&tmp, in)) {
					ln += elementUTF16FromUTF32Size(tmp);
				} else {
					ln += StringUTF16Core::length(err);
				};
				in += sz;
				continue;
			};

			ln += StringUTF16Core::length(err);
			++in;
		};
		return ln;
	};

	size_t utf16FromUTF32Length(const utf32 *in, const utf16 *err) {
		size_t ln = 0;
		size_t sz;

		while (*in) {
			sz = UTF32Core::elementSize(*in);
			if (sz) {
				ln += elementUTF16FromUTF32Size(*in);
				in += sz;
				continue;
			};

			ln += StringUTF16Core::length(err);
			++in;
		};
		return ln;
	};

	size_t utf32FromUTF8Length(const utf8 *in, const utf32 *err) {
		size_t ln = 0;
		size_t lnX;
		size_t sz;
		utf32 chr;

		while (*in) {
			sz = UTF8Core::elementSize(*in);
			if (sz) {
				ln += elementUTF32FromUTF8(&chr, in);
				in += sz;
				continue;
			};

			ln += StringUTF32Core::length(err);
			++in;
		};
		return ln;
	};

	size_t utf32FromUTF16Length(const utf16 *in, const utf32 *err) {
		size_t ln = 0;
		size_t lnX;
		size_t sz;
		utf32 chr;

		while (*in) {
			sz = UTF16Core::elementSize(*in);
			if (sz) {
				ln += elementUTF32FromUTF16(&chr, in);
				in += sz;
				continue;
			};

			ln += StringUTF32Core::length(err);
			++in;
		};
		return ln;
	};

};
