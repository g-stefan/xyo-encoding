// Encoding
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_ENCODING_UTF8CORE_HPP
#define XYO_ENCODING_UTF8CORE_HPP

#ifndef XYO_ENCODING_DEPENDENCY_HPP
#	include <XYO/Encoding/Dependency.hpp>
#endif

namespace XYO::Encoding {

	typedef char utf8;

	namespace UTF8Core {

		// RFC 3629: lead bytes C0, C1 (always overlong) and F5..FF (above U+10FFFF,
		// or the obsolete 5 and 6 byte forms) are not valid
		inline size_t elementSize(const utf8 x) {
			uint8_t x_ = static_cast<uint8_t>(x);
			if (x_ < 0x80) {
				return 1;
			};
			if (x_ < 0xC2) {
				return 0;
			};
			if (x_ < 0xE0) {
				return 2;
			};
			if (x_ < 0xF0) {
				return 3;
			};
			if (x_ < 0xF5) {
				return 4;
			};
			return 0;
		};

		inline bool elementIsValid(const utf8 *x) {
			size_t sz;
			sz = elementSize(*x);
			if (sz == 0) {
				return false;
			};
			if (sz == 1) {
				return true;
			};
			// the second byte range rejects overlong forms (E0, F0),
			// surrogates (ED) and code points above U+10FFFF (F4)
			uint8_t lead = static_cast<uint8_t>(x[0]);
			uint8_t next = static_cast<uint8_t>(x[1]);
			uint8_t low = 0x80;
			uint8_t high = 0xBF;
			switch (lead) {
			case 0xE0:
				low = 0xA0;
				break;
			case 0xED:
				high = 0x9F;
				break;
			case 0xF0:
				low = 0x90;
				break;
			case 0xF4:
				high = 0x8F;
				break;
			default:
				break;
			};
			if ((next < low) || (next > high)) {
				return false;
			};
			// the terminator is not a continuation byte, the scan stops on it
			for (size_t k = 2; k < sz; ++k) {
				if ((static_cast<uint8_t>(x[k]) & 0xC0) != 0x80) {
					return false;
				};
			};
			return true;
		};

		inline bool check(const utf8 x) {
			return ((x & 0xC0) == 0x80);
		};

		inline bool elementIsEqual(const utf8 *x, const utf8 *y) {
			size_t sz;
			sz = elementSize(*x);
			if (sz != elementSize(*y)) {
				return false;
			};
			return (memcmp(x, y, sz) == 0);
		};

	};

};

#endif
