// Encoding
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/Encoding/Base32.hpp>

//
// http://en.wikipedia.org/wiki/Base32
// RFC 4648
//

namespace XYO::Encoding::Base32 {

	static const char *base32Code = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

	// Reverse lookup table: maps an encoded byte back to its 5-bit value, or -1
	// if it is not part of the alphabet. Built once at startup so decoding is a
	// single array read per character instead of a linear scan of base32Code.
	struct Base32Reverse {
			int8_t value[256];
			Base32Reverse() {
				for (int k = 0; k < 256; ++k) {
					value[k] = -1;
				};
				for (int k = 0; k < 32; ++k) {
					value[(uint8_t)base32Code[k]] = (int8_t)k;
				};
			};
	};

	static const Base32Reverse base32Reverse;

	String encode(const String &toEncode) {
		size_t co = toEncode.length();

		TPointer<StringReference> retV(TMemory<StringReference>::newMemory());
		retV->init(((co + 4) / 5) * 8);

		const char *src = toEncode.value();
		size_t c;
		uint64_t n;
		uint32_t n0, n1, n2, n3, n4, n5, n6, n7;
		for (c = 0; c < co; c += 5) {

			n = (((uint64_t)((uint8_t)src[c])) << 32);
			if (c + 1 < co) {
				n += (((uint64_t)((uint8_t)src[c + 1])) << 24);
			};
			if (c + 2 < co) {
				n += (((uint64_t)((uint8_t)src[c + 2])) << 16);
			};
			if (c + 3 < co) {
				n += (((uint64_t)((uint8_t)src[c + 3])) << 8);
			};
			if (c + 4 < co) {
				n += (((uint64_t)((uint8_t)src[c + 4])));
			};

			n0 = (uint32_t)((n >> 35) & 0x1F);
			n1 = (uint32_t)((n >> 30) & 0x1F);
			n2 = (uint32_t)((n >> 25) & 0x1F);
			n3 = (uint32_t)((n >> 20) & 0x1F);
			n4 = (uint32_t)((n >> 15) & 0x1F);
			n5 = (uint32_t)((n >> 10) & 0x1F);
			n6 = (uint32_t)((n >> 5) & 0x1F);
			n7 = (uint32_t)((n)&0x1F);

			retV->concatenateX((char)base32Code[n0]);
			retV->concatenateX((char)base32Code[n1]);
			retV->concatenateX((char)((c + 1 < co) ? base32Code[n2] : '='));
			retV->concatenateX((char)((c + 1 < co) ? base32Code[n3] : '='));
			retV->concatenateX((char)((c + 2 < co) ? base32Code[n4] : '='));
			retV->concatenateX((char)((c + 3 < co) ? base32Code[n5] : '='));
			retV->concatenateX((char)((c + 3 < co) ? base32Code[n6] : '='));
			retV->concatenateX((char)((c + 4 < co) ? base32Code[n7] : '='));
		};

		return retV;
	};

	bool decode(const String &toDecode, String &out) {
		size_t co = toDecode.length();
		const char *src = toDecode.value();

		// complete groups of 8 only, never read past the end of the input
		if (co % 8) {
			return false;
		};

		TPointer<StringReference> retV(TMemory<StringReference>::newMemory());
		retV->init((co / 8) * 5);

		size_t c;
		size_t k;
		uint64_t n;
		int8_t v;
		size_t q;
		uint8_t p;
		for (c = 0; c < co; c += 8) {

			// q = count of alphabet characters before the first '=',
			// anything after the first '=' must also be '='
			q = 8;
			n = 0;
			for (k = 0; k < 8; ++k) {
				v = base32Reverse.value[(uint8_t)src[c + k]];
				if (v < 0) {
					if (src[c + k] != '=') {
						return false;
					};
					if (q == 8) {
						q = k;
					};
					v = 0;
				} else if (q != 8) {
					return false;
				};
				n <<= 5;
				n |= ((uint64_t)v);
			};

			// valid groups end with 0, 1, 3, 4 or 6 '=', padding only in the last group
			switch (q) {
			case 2:
				p = 1;
				break;
			case 4:
				p = 2;
				break;
			case 5:
				p = 3;
				break;
			case 7:
				p = 4;
				break;
			case 8:
				p = 5;
				break;
			default:
				return false;
			};
			if ((q < 8) && (c + 8 < co)) {
				return false;
			};

			retV->concatenateX((char)((n >> 32) & 0x00FF));
			if (p >= 2) {
				retV->concatenateX((char)((n >> 24) & 0x00FF));
			};
			if (p >= 3) {
				retV->concatenateX((char)((n >> 16) & 0x00FF));
			};
			if (p >= 4) {
				retV->concatenateX((char)((n >> 8) & 0x00FF));
			};
			if (p >= 5) {
				retV->concatenateX((char)((n)&0x00FF));
			};
		};

		out = retV;
		return true;
	};

};
