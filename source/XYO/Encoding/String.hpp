// Encoding
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_ENCODING_STRING_HPP
#define XYO_ENCODING_STRING_HPP

#ifndef XYO_ENCODING_TSTRINGX_HPP
#	include <XYO/Encoding/TStringX.hpp>
#endif

namespace XYO::Encoding {

	typedef TStringCore<char> StringCore;
	typedef TStringReference<char> StringReference;
	typedef TString<char> String;
	typedef TStringX<char> StringX;

};

#endif
