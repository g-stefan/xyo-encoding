// Encoding
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_ENCODING_TMEMORYCORE_CHAR_HPP
#define XYO_ENCODING_TMEMORYCORE_CHAR_HPP

#ifndef XYO_ENCODING_TMEMORYCORE_HPP
#	include <XYO/Encoding/TMemoryCore.hpp>
#endif

namespace XYO::Encoding {

	template <>
	struct TMemoryCore<char> {

			static inline void copyN(char *x, const char *y, size_t ln) {
				memcpy(x, y, ln);
			};

			static inline int compareN(char *x, const char *y, size_t ln) {
				return memcmp(x, y, ln);
			};

			static inline void setN(char *x, const char y, size_t ln) {
				memset(x, y, ln);
			};
	};

};

#endif
