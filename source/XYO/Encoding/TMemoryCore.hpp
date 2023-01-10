// Encoding
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_ENCODING_TMEMORYCORE_HPP
#define XYO_ENCODING_TMEMORYCORE_HPP

#ifndef XYO_ENCODING_DEPENDENCY_HPP
#	include <XYO/Encoding/Dependency.hpp>
#endif

namespace XYO::Encoding {

	template <typename T>
	struct TMemoryCore {

			static inline void copyN(T *x, const T *y, size_t ln) {

				while (ln != 0) {

					*x = *y;

					++x;
					++y;
					--ln;
				};
			};

			static inline int compareN(T *x, const T *y, size_t ln) {

				while ((ln != 0) && (*x == *y)) {
					++x;
					++y;
					--ln;
				};

				if (ln == 0) {
					return 0;
				};

				if ((*x - *y) < 0) {
					return -1;
				};

				return 1;
			};

			static inline void setN(T *x, const T y, size_t ln) {

				while (ln != 0) {

					*x = y;

					++x;
					--ln;
				};
			};
	};

};

#ifndef XYO_ENCODING_TMEMORYCORE_CHAR_HPP
#	include <XYO/Encoding/TMemoryCore-Char.hpp>
#endif

#endif
