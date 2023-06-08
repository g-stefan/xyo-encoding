// Encoding
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_ENCODING_NUMBERX_HPP
#define XYO_ENCODING_NUMBERX_HPP

#ifndef XYO_ENCODING_STRING_HPP
#	include <XYO/Encoding/String.hpp>
#endif

namespace XYO::Encoding::NumberX {

	int countDigits(int value);
	String leftPadByDigits(int index, int indexLn);

};

#endif
