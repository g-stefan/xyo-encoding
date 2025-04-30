// Encoding
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_ENCODING_TCOMPARATOR_TSTRING_HPP
#define XYO_ENCODING_TCOMPARATOR_TSTRING_HPP

#ifndef XYO_ENCODING_TSTRING_HPP
#	include <XYO/Encoding/TString.hpp>
#endif

namespace XYO::ManagedMemory {
	template <typename U>
	struct TComparator<XYO::Encoding::TString<U>> {
			typedef XYO::Encoding::TString<U> T;

			static inline bool isEqual(const T &x, const T &y) {
				return const_cast<T &>(x) == const_cast<T &>(y);
			};

			static inline bool isNotEqual(const T &x, const T &y) {
				return const_cast<T &>(x) != const_cast<T &>(y);
			};

			static inline bool isLess(const T &x, const T &y) {
				return const_cast<T &>(x) < const_cast<T &>(y);
			};

			static inline bool isGreater(const T &x, const T &y) {
				return const_cast<T &>(x) > const_cast<T &>(y);
			};

			static inline bool isLessOrEqual(const T &x, const T &y) {
				return const_cast<T &>(x) <= const_cast<T &>(y);
			};

			static inline bool isGreaterOrEqual(const T &x, const T &y) {
				return const_cast<T &>(x) >= const_cast<T &>(y);
			};

			static inline int compare(const T &x, const T &y) {
				return (const_cast<T &>(x)).compare(const_cast<T &>(y));
			};
	};

};

#endif
