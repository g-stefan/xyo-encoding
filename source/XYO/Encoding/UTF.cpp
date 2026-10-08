// Encoding
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/Encoding/UTF.hpp>
#include <XYO/Encoding/UTFStream.hpp>

namespace XYO::Encoding::UTF {

	const utf8 utf8StringQuestionMark[] = {'?', 0};
	const utf16 utf16StringQuestionMark[] = {'?', 0};
	const utf32 utf32StringQuestionMark[] = {'?', 0};

	size_t elementUTF32FromUTF8(utf32 *out, const utf8 *in) {
		if (!UTF8Core::elementIsValid(in)) {
			return 0;
		};
		switch (UTF8Core::elementSize(*in)) {
		case 1:
			*out = (utf32)((uint8_t)in[0]);
			break;
		case 2:
			*out = (utf32)(in[0] & 0x1F);
			*out <<= 6;
			*out |= (utf32)(in[1] & 0x3F);
			break;
		case 3:
			*out = (utf32)(in[0] & 0x0F);
			*out <<= 6;
			*out |= (utf32)(in[1] & 0x3F);
			*out <<= 6;
			*out |= (utf32)(in[2] & 0x3F);
			break;
		case 4:
			*out = (utf32)(in[0] & 0x07);
			*out <<= 6;
			*out |= (utf32)(in[1] & 0x3F);
			*out <<= 6;
			*out |= (utf32)(in[2] & 0x3F);
			*out <<= 6;
			*out |= (utf32)(in[3] & 0x3F);
			break;
		default:
			return 0;
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
		// RFC 3629, valid code points are at most U+10FFFF, 4 bytes
		if (in < 0x00000080) {
			return 1;
		};
		if (in < 0x00000800) {
			return 2;
		};
		if (in < 0x00010000) {
			return 3;
		};
		return 4;
	};

	size_t elementUTF8FromUTF32(utf8 *out, utf32 in) {
		size_t sz;
		sz = elementUTF8FromUTF32Size(in);
		switch (sz) {
		case 1:
			out[0] = (utf8)in;
			break;
		case 2:
			out[1] = (utf8)((in & 0x3F) | 0x80);
			in >>= 6;
			out[0] = (utf8)((in & 0x1F) | 0xC0);
			break;
		case 3:
			out[2] = (utf8)((in & 0x3F) | 0x80);
			in >>= 6;
			out[1] = (utf8)((in & 0x3F) | 0x80);
			in >>= 6;
			out[0] = (utf8)((in & 0x0F) | 0xE0);
			break;
		case 4:
			out[3] = (utf8)((in & 0x3F) | 0x80);
			in >>= 6;
			out[2] = (utf8)((in & 0x3F) | 0x80);
			in >>= 6;
			out[1] = (utf8)((in & 0x3F) | 0x80);
			in >>= 6;
			out[0] = (utf8)((in & 0x07) | 0xF0);
			break;
		default:
			return 0;
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
			return 0;
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
		utf32 tmp;
		if (!elementUTF32FromUTF16(&tmp, in)) {
			return 0;
		};
		return elementUTF8FromUTF32Size(tmp);
	};

	// Decode one element, returns the number of input elements used or 0 if invalid.
	// On an invalid element the converters below skip a single input element, so they
	// never step over the terminator of a truncated sequence nor swallow valid data.

	static inline size_t decodeUTF8_(utf32 *out, const utf8 *in) {
		if (elementUTF32FromUTF8(out, in)) {
			return UTF8Core::elementSize(*in);
		};
		return 0;
	};

	static inline size_t decodeUTF16_(utf32 *out, const utf16 *in) {
		if (elementUTF32FromUTF16(out, in)) {
			return UTF16Core::elementSize(*in);
		};
		return 0;
	};

	String utf8FromUTF16(const utf16 *in, const utf8 *err) {
		TPointer<StringReference> retV(TMemory<StringReference>::newMemory());
		retV->init(utf8FromUTF16Length(in, err));

		size_t errLn = StringUTF8Core::length(err);
		utf8 chr[8];
		utf32 tmp;
		size_t sz;

		while (*in) {
			sz = decodeUTF16_(&tmp, in);
			if (sz) {
				retV->concatenateX(chr, elementUTF8FromUTF32(chr, tmp));
				in += sz;
				continue;
			};
			retV->concatenateX(err, errLn);
			++in;
		};
		return retV;
	};

	String utf8FromUTF32(const utf32 *in, const utf8 *err) {
		TPointer<StringReference> retV(TMemory<StringReference>::newMemory());
		retV->init(utf8FromUTF32Length(in, err));

		size_t errLn = StringUTF8Core::length(err);
		utf8 chr[8];
		size_t sz;

		while (*in) {
			sz = elementUTF8FromUTF32(chr, *in);
			if (sz) {
				retV->concatenateX(chr, sz);
			} else {
				retV->concatenateX(err, errLn);
			};
			++in;
		};
		return retV;
	};

	StringUTF16 utf16FromUTF8(const utf8 *in, const utf16 *err) {
		TPointer<StringUTF16Reference> retV(TMemory<StringUTF16Reference>::newMemory());
		retV->init(utf16FromUTF8Length(in, err));

		size_t errLn = StringUTF16Core::length(err);
		utf32 tmp;
		utf16 chr[4];
		size_t sz;

		while (*in) {
			sz = decodeUTF8_(&tmp, in);
			if (sz) {
				retV->concatenateX(chr, elementUTF16FromUTF32(chr, tmp));
				in += sz;
				continue;
			};
			retV->concatenateX(err, errLn);
			++in;
		};

		return retV;
	};

	StringUTF16 utf16FromUTF32(const utf32 *in, const utf16 *err) {
		TPointer<StringUTF16Reference> retV(TMemory<StringUTF16Reference>::newMemory());
		retV->init(utf16FromUTF32Length(in, err));

		size_t errLn = StringUTF16Core::length(err);
		utf16 chr[4];
		size_t sz;

		while (*in) {
			sz = elementUTF16FromUTF32(chr, *in);
			if (sz) {
				retV->concatenateX(chr, sz);
			} else {
				retV->concatenateX(err, errLn);
			};
			++in;
		};

		return retV;
	};

	StringUTF32 utf32FromUTF8(const utf8 *in, const utf32 *err) {
		TPointer<StringUTF32Reference> retV(TMemory<StringUTF32Reference>::newMemory());
		retV->init(utf32FromUTF8Length(in, err));

		size_t errLn = StringUTF32Core::length(err);
		utf32 tmp;
		size_t sz;

		while (*in) {
			sz = decodeUTF8_(&tmp, in);
			if (sz) {
				retV->concatenateX(tmp);
				in += sz;
				continue;
			};
			retV->concatenateX(err, errLn);
			++in;
		};

		return retV;
	};

	StringUTF32 utf32FromUTF16(const utf16 *in, const utf32 *err) {
		TPointer<StringUTF32Reference> retV(TMemory<StringUTF32Reference>::newMemory());
		retV->init(utf32FromUTF16Length(in, err));

		size_t errLn = StringUTF32Core::length(err);
		utf32 tmp;
		size_t sz;

		while (*in) {
			sz = decodeUTF16_(&tmp, in);
			if (sz) {
				retV->concatenateX(tmp);
				in += sz;
				continue;
			};
			retV->concatenateX(err, errLn);
			++in;
		};

		return retV;
	};

	size_t utf8FromUTF16Length(const utf16 *in, const utf8 *err) {
		size_t ln = 0;
		size_t errLn = StringUTF8Core::length(err);
		utf32 tmp;
		size_t sz;

		while (*in) {
			sz = decodeUTF16_(&tmp, in);
			if (sz) {
				ln += elementUTF8FromUTF32Size(tmp);
				in += sz;
				continue;
			};
			ln += errLn;
			++in;
		};
		return ln;
	};

	size_t utf8FromUTF32Length(const utf32 *in, const utf8 *err) {
		size_t ln = 0;
		size_t errLn = StringUTF8Core::length(err);
		size_t sz;

		while (*in) {
			sz = elementUTF8FromUTF32Size(*in);
			if (sz) {
				ln += sz;
			} else {
				ln += errLn;
			};
			++in;
		};
		return ln;
	};

	size_t utf16FromUTF8Length(const utf8 *in, const utf16 *err) {
		size_t ln = 0;
		size_t errLn = StringUTF16Core::length(err);
		utf32 tmp;
		size_t sz;

		while (*in) {
			sz = decodeUTF8_(&tmp, in);
			if (sz) {
				ln += elementUTF16FromUTF32Size(tmp);
				in += sz;
				continue;
			};
			ln += errLn;
			++in;
		};
		return ln;
	};

	size_t utf16FromUTF32Length(const utf32 *in, const utf16 *err) {
		size_t ln = 0;
		size_t errLn = StringUTF16Core::length(err);
		size_t sz;

		while (*in) {
			sz = elementUTF16FromUTF32Size(*in);
			if (sz) {
				ln += sz;
			} else {
				ln += errLn;
			};
			++in;
		};
		return ln;
	};

	size_t utf32FromUTF8Length(const utf8 *in, const utf32 *err) {
		size_t ln = 0;
		size_t errLn = StringUTF32Core::length(err);
		utf32 tmp;
		size_t sz;

		while (*in) {
			sz = decodeUTF8_(&tmp, in);
			if (sz) {
				++ln;
				in += sz;
				continue;
			};
			ln += errLn;
			++in;
		};
		return ln;
	};

	size_t utf32FromUTF16Length(const utf16 *in, const utf32 *err) {
		size_t ln = 0;
		size_t errLn = StringUTF32Core::length(err);
		utf32 tmp;
		size_t sz;

		while (*in) {
			sz = decodeUTF16_(&tmp, in);
			if (sz) {
				++ln;
				in += sz;
				continue;
			};
			ln += errLn;
			++in;
		};
		return ln;
	};

};
