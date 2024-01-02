// Encoding
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_ENCODING_UTF32CORE_HPP
#define XYO_ENCODING_UTF32CORE_HPP

#ifndef XYO_ENCODING_DEPENDENCY_HPP
#	include <XYO/Encoding/Dependency.hpp>
#endif

namespace XYO::Encoding {

	typedef uint32_t utf32;

	namespace UTF32Core {

		inline bool elementIsValid(const utf32 x) {
			if (x <= 0x0000D7FFUL) {
				return true;
			};
			if ((x >= 0x0000D800UL) && (x < 0x0000E000UL)) {
				return false;
			};
			if (x == 0x0000FFFFUL) {
				return false;
			};
			if (x >= 0x00110000UL) {
				return false;
			};
			return true;
		};

		inline size_t elementSize(const utf32 x) {
			return static_cast<size_t>(elementIsValid(x));
		};

	};

};

#endif
