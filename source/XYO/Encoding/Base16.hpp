// Encoding
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_ENCODING_BASE16_HPP
#define XYO_ENCODING_BASE16_HPP

#ifndef XYO_ENCODING_STRING_HPP
#	include <XYO/Encoding/String.hpp>
#endif
namespace XYO::Encoding::Base16 {

	XYO_ENCODING_EXPORT String encode(const String &toEncode);
	XYO_ENCODING_EXPORT bool decode(const String &toDecode, String &out);

};

#endif
