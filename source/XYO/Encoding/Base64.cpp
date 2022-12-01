// Encoding
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/Encoding/Base64.hpp>

//
// http://en.wikipedia.org/wiki/Base64
//

namespace XYO::Encoding::Base64 {

	static const char *base64Code = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	String encode(const String &toEncode) {
		String retV;
		uint32_t c;
		uint32_t co;
		uint32_t n;
		uint32_t n0, n1, n2, n3;
		char buf[2];
		buf[1] = 0;
		co = toEncode.length();
		const char *pad = "";
		c = 0;
		while (c < co) {

			n = (((uint32_t)((uint8_t)toEncode[c])) << 16);
			if (c + 1 < co) {
				n += (((uint32_t)((uint8_t)toEncode[c + 1])) << 8);
			};
			if (c + 2 < co) {
				n += (((uint32_t)((uint8_t)toEncode[c + 2])));
			};

			n0 = (n >> 18) & 0x3F;
			n1 = (n >> 12) & 0x3F;
			n2 = (n >> 6) & 0x3F;
			n3 = (n)&0x3F;

			buf[0] = base64Code[n0];
			retV << buf;
			buf[0] = base64Code[n1];
			retV << buf;
			pad = "==";
			if (c + 1 < co) {
				buf[0] = base64Code[n2];
				retV << buf;
				pad = "=";
			};
			if (c + 2 < co) {
				buf[0] = base64Code[n3];
				retV << buf;
				pad = "";
			};

			c += 3;
		};
		retV << pad;
		return retV;
	};

	bool decode(const String &toDecode, String &out) {
		uint32_t c;
		uint32_t co;
		uint32_t n;
		uint8_t n0, n1, n2, n3;
		uint8_t c0, c1, c2, ch;
		uint8_t z;
		co = toDecode.length();
		out = "";
		uint8_t p;
		for (c = 0; c < co; c += 4) {

			p = 3;

			ch = toDecode[c];
			for (z = 0; z < 0x40; ++z) {
				if (base64Code[z] == ch) {
					break;
				};
			};
			if (z == 0x40) {
				return false;
			};
			n0 = z;

			ch = toDecode[c + 1];
			for (z = 0; z < 0x40; ++z) {
				if (base64Code[z] == ch) {
					break;
				};
			};
			if (z == 0x40) {
				return false;
			};
			n1 = z;

			ch = toDecode[c + 2];
			for (z = 0; z < 0x40; ++z) {
				if (base64Code[z] == ch) {
					break;
				};
			};
			if (z == 0x40) {
				if (ch != '=') {
					return false;
				};
				p = 1;
				z = 0;
			};

			n2 = z;

			ch = toDecode[c + 3];
			for (z = 0; z < 0x40; ++z) {
				if (base64Code[z] == ch) {
					break;
				};
			};
			if (z == 0x40) {
				if (ch != '=') {
					return false;
				};
				if (p == 3) {
					p = 2;
				};
				z = 0;
			};
			n3 = z;

			n = ((uint32_t)n0);
			n <<= 6;
			n |= ((uint32_t)n1);
			n <<= 6;
			n |= ((uint32_t)n2);
			n <<= 6;
			n |= ((uint32_t)n3);

			c0 = ((n >> 16) & 0x00FF);
			c1 = ((n >> 8) & 0x00FF);
			c2 = ((n)&0x00FF);

			out << c0;
			if (p >= 2) {
				out << c1;
			};
			if (p >= 3) {
				out << c2;
			};
		};
		return true;
	};

};
