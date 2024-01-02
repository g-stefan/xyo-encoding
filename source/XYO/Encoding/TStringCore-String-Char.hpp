// Encoding
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_ENCODING_TSTRINGCORE_STRING_CHAR_HPP
#define XYO_ENCODING_TSTRINGCORE_STRING_CHAR_HPP

#ifndef XYO_ENCODING_TSTRINGCORE_HPP
#	include <XYO/Encoding/TStringCore.hpp>
#endif

namespace XYO::Encoding {

	template <>
	inline size_t TStringCore<char>::length(const char *x) {
		return strlen(x);
	};

	template <>
	inline void TStringCore<char>::copy(char *x, const char *y) {
		strcpy(x, y);
	};

	template <>
	inline void TStringCore<char>::copyN(char *x, const char *y, size_t yLn) {
		strncpy(x, y, yLn);
	};

	template <>
	inline int TStringCore<char>::compare(const char *x, const char *y) {
		return strcmp(x, y);
	};

	template <>
	inline int TStringCore<char>::compareN(const char *x, const char *y, size_t ln) {
		return strncmp(x, y, ln);
	};

	template <>
	inline void TStringCore<char>::concatenate(char *x, const char *y) {
		strcat(x, y);
	};

	template <>
	inline void TStringCore<char>::concatenateN(char *x, const char *y, size_t yLn) {
		strncat(x, y, yLn);
	};

};

#endif
