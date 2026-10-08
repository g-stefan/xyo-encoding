// Encoding
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/Encoding/Base64.hpp>

//
// http://en.wikipedia.org/wiki/Base64
//

namespace XYO::Encoding::Base64 {

	static const char *base64Code = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	// Reverse lookup table: maps an encoded byte back to its 6-bit value, or -1
	// if it is not part of the alphabet. Built once at startup so decoding is a
	// single array read per character instead of a linear scan of base64Code.
	struct Base64Reverse {
			int8_t value[256];
			Base64Reverse() {
				for (int k = 0; k < 256; ++k) {
					value[k] = -1;
				};
				for (int k = 0; k < 64; ++k) {
					value[(uint8_t)base64Code[k]] = (int8_t)k;
				};
			};
	};

	static const Base64Reverse base64Reverse;

	String encode(const String &toEncode) {
		size_t co = toEncode.length();

		TPointer<StringReference> retV(TMemory<StringReference>::newMemory());
		retV->init(((co + 2) / 3) * 4);

		const char *src = toEncode.value();
		size_t c;
		uint32_t n;
		uint32_t n0, n1, n2, n3;
		for (c = 0; c < co; c += 3) {

			n = (((uint32_t)((uint8_t)src[c])) << 16);
			if (c + 1 < co) {
				n += (((uint32_t)((uint8_t)src[c + 1])) << 8);
			};
			if (c + 2 < co) {
				n += (((uint32_t)((uint8_t)src[c + 2])));
			};

			n0 = (n >> 18) & 0x3F;
			n1 = (n >> 12) & 0x3F;
			n2 = (n >> 6) & 0x3F;
			n3 = (n)&0x3F;

			retV->concatenateX((char)base64Code[n0]);
			retV->concatenateX((char)base64Code[n1]);
			retV->concatenateX((char)((c + 1 < co) ? base64Code[n2] : '='));
			retV->concatenateX((char)((c + 2 < co) ? base64Code[n3] : '='));
		};

		return retV;
	};

	bool decode(const String &toDecode, String &out) {
		size_t co = toDecode.length();
		const char *src = toDecode.value();

		// complete groups of 4 only, never read past the end of the input
		if (co % 4) {
			return false;
		};

		TPointer<StringReference> retV(TMemory<StringReference>::newMemory());
		retV->init((co / 4) * 3);

		size_t c;
		size_t k;
		uint32_t n;
		int8_t v;
		size_t q;
		uint8_t p;
		for (c = 0; c < co; c += 4) {

			// q = count of alphabet characters before the first '=',
			// anything after the first '=' must also be '='
			q = 4;
			n = 0;
			for (k = 0; k < 4; ++k) {
				v = base64Reverse.value[(uint8_t)src[c + k]];
				if (v < 0) {
					if (src[c + k] != '=') {
						return false;
					};
					if (q == 4) {
						q = k;
					};
					v = 0;
				} else if (q != 4) {
					return false;
				};
				n <<= 6;
				n |= ((uint32_t)v);
			};

			// valid groups: xxxx, xxx= or xx==, padding only in the last group
			if (q < 2) {
				return false;
			};
			if ((q < 4) && (c + 4 < co)) {
				return false;
			};
			p = (uint8_t)(q - 1);

			retV->concatenateX((char)((n >> 16) & 0x00FF));
			if (p >= 2) {
				retV->concatenateX((char)((n >> 8) & 0x00FF));
			};
			if (p >= 3) {
				retV->concatenateX((char)((n)&0x00FF));
			};
		};

		out = retV;
		return true;
	};

};
