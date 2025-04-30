// Encoding
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/Encoding/Base32.hpp>

//
// http://en.wikipedia.org/wiki/Base32
// RFC 4648
//

namespace XYO::Encoding::Base32 {

	static const char *base32Code = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

	String encode(const String &toEncode) {
		String retV;
		uint32_t c;
		uint32_t co;
		uint64_t n;
		uint32_t n0, n1, n2, n3, n4, n5, n6, n7;
		char buf[2];
		buf[1] = 0;
		co = toEncode.length();
		const char *pad = "";
		c = 0;
		while (c < co) {

			n = (((uint64_t)((uint8_t)toEncode[c])) << 32);
			if (c + 1 < co) {
				n += (((uint64_t)((uint8_t)toEncode[c + 1])) << 24);
			};
			if (c + 2 < co) {
				n += (((uint64_t)((uint8_t)toEncode[c + 2])) << 16);
			};
			if (c + 3 < co) {
				n += (((uint64_t)((uint8_t)toEncode[c + 3])) << 8);
			};
			if (c + 4 < co) {
				n += (((uint64_t)((uint8_t)toEncode[c + 4])));
			};

			n0 = (n >> 35) & 0x1F;
			n1 = (n >> 30) & 0x1F;
			n2 = (n >> 25) & 0x1F;
			n3 = (n >> 20) & 0x1F;
			n4 = (n >> 15) & 0x1F;
			n5 = (n >> 10) & 0x1F;
			n6 = (n >> 5) & 0x1F;
			n7 = (n)&0x1F;

			buf[0] = base32Code[n0];
			retV << buf;
			buf[0] = base32Code[n1];
			retV << buf;
			pad = "======";
			if (c + 1 < co) {
				buf[0] = base32Code[n2];
				retV << buf;
				buf[0] = base32Code[n3];
				retV << buf;
				pad = "====";
			};
			if (c + 2 < co) {
				buf[0] = base32Code[n4];
				retV << buf;
				pad = "===";
			};
			if (c + 3 < co) {
				buf[0] = base32Code[n5];
				retV << buf;
				buf[0] = base32Code[n6];
				retV << buf;
				pad = "=";
			};
			if (c + 4 < co) {
				buf[0] = base32Code[n7];
				retV << buf;
				pad = "";
			};

			c += 5;
		};
		retV << pad;
		return retV;
	};

	bool decode(const String &toDecode, String &out) {
		uint32_t c;
		uint32_t co;
		uint64_t n;
		uint8_t n0, n1, n2, n3, n4, n5, n6, n7;
		uint8_t c0, c1, c2, c3, c4, ch;
		uint8_t z;
		co = toDecode.length();
		out = "";
		uint8_t p;
		for (c = 0; c < co; c += 8) {

			p = 5;

			ch = toDecode[c];
			for (z = 0; z < 0x20; ++z) {
				if (base32Code[z] == ch) {
					break;
				};
			};
			if (z == 0x20) {
				return false;
			};
			n0 = z;

			ch = toDecode[c + 1];
			for (z = 0; z < 0x20; ++z) {
				if (base32Code[z] == ch) {
					break;
				};
			};
			if (z == 0x20) {
				return false;
			};
			n1 = z;

			ch = toDecode[c + 2];
			for (z = 0; z < 0x20; ++z) {
				if (base32Code[z] == ch) {
					break;
				};
			};
			if (z == 0x20) {
				if (ch != '=') {
					return false;
				};
				p = 1;
				z = 0;
			};

			n2 = z;

			ch = toDecode[c + 3];
			for (z = 0; z < 0x20; ++z) {
				if (base32Code[z] == ch) {
					break;
				};
			};
			if (z == 0x20) {
				if (ch != '=') {
					return false;
				};
				z = 0;
			};
			n3 = z;

			ch = toDecode[c + 4];
			for (z = 0; z < 0x20; ++z) {
				if (base32Code[z] == ch) {
					break;
				};
			};
			if (z == 0x20) {
				if (ch != '=') {
					return false;
				};
				if (p == 5) {
					p = 2;
				};
				z = 0;
			};
			n4 = z;

			ch = toDecode[c + 5];
			for (z = 0; z < 0x20; ++z) {
				if (base32Code[z] == ch) {
					break;
				};
			};
			if (z == 0x20) {
				if (ch != '=') {
					return false;
				};
				if (p == 5) {
					p = 3;
				};
				z = 0;
			};
			n5 = z;

			ch = toDecode[c + 6];
			for (z = 0; z < 0x20; ++z) {
				if (base32Code[z] == ch) {
					break;
				};
			};
			if (z == 0x20) {
				if (ch != '=') {
					return false;
				};
				z = 0;
			};
			n6 = z;

			ch = toDecode[c + 7];
			for (z = 0; z < 0x20; ++z) {
				if (base32Code[z] == ch) {
					break;
				};
			};
			if (z == 0x20) {
				if (ch != '=') {
					return false;
				};
				if (p == 5) {
					p = 4;
				};
				z = 0;
			};
			n7 = z;

			n = ((uint64_t)n0);
			n <<= 5;
			n |= ((uint64_t)n1);
			n <<= 5;
			n |= ((uint64_t)n2);
			n <<= 5;
			n |= ((uint64_t)n3);
			n <<= 5;
			n |= ((uint64_t)n4);
			n <<= 5;
			n |= ((uint64_t)n5);
			n <<= 5;
			n |= ((uint64_t)n6);
			n <<= 5;
			n |= ((uint64_t)n7);

			c0 = ((n >> 32) & 0x00FF);
			c1 = ((n >> 24) & 0x00FF);
			c2 = ((n >> 16) & 0x00FF);
			c3 = ((n >> 8) & 0x00FF);
			c4 = ((n)&0x00FF);

			out << c0;
			if (p >= 2) {
				out << c1;
			};
			if (p >= 3) {
				out << c2;
			};
			if (p >= 4) {
				out << c3;
			};
			if (p >= 5) {
				out << c4;
			};
		};
		return true;
	};

};
