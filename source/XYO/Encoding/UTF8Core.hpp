// Encoding
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_ENCODING_UTF8CORE_HPP
#define XYO_ENCODING_UTF8CORE_HPP

#ifndef XYO_ENCODING_DEPENDENCY_HPP
#	include <XYO/Encoding/Dependency.hpp>
#endif

namespace XYO::Encoding {

	typedef char utf8;

	namespace UTF8Core {

		inline size_t elementSize(const utf8 x) {
			if ((x & 0x80) == 0x00) {
				return 1;
			};
			if ((x & 0xE0) == 0xC0) {
				return 2;
			};
			if ((x & 0xF0) == 0xE0) {
				return 3;
			};
			if ((x & 0xF8) == 0xF0) {
				return 4;
			};
			if ((x & 0xFC) == 0xF8) {
				return 5;
			};
			if ((x & 0xFE) == 0xFC) {
				return 6;
			};
			return 0;
		};

		inline bool elementIsValid(const utf8 *x) {
			size_t sz;
			sz = elementSize(*x);
			if (sz == 0) {
				return false;
			};
			++x;
			--sz;
			while (sz) {
				if ((*x & 0xC0) != 0x80) {
					return false;
				};
				++x;
				--sz;
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
