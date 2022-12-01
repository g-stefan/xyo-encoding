// Encoding
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/Encoding/String.hpp>
#include <XYO/Encoding/Base16.hpp>

//
// https://en.wikipedia.org/wiki/Hexadecimal
//

namespace XYO::Encoding::Base16 {

	String encode(const String &toEncode) {
		TPointer<StringReference> retV(TMemory<StringReference>::newMemory());
		retV->init(toEncode.length() * 2);
		size_t k;
		const char *scan;
		scan = toEncode.value();
		for (k = 0; k < toEncode.length(); ++k, ++scan) {
			retV->concatenateX(THex<char>::encodeUppercase(((*scan) >> 4) & 0x0F));
			retV->concatenateX(THex<char>::encodeUppercase((*scan) & 0x0F));
		};
		return retV;
	};

	bool decode(const String &toDecode, String &out) {
		TPointer<StringReference> retV(TMemory<StringReference>::newMemory());
		retV->init((toDecode.length() / 2));
		size_t k;
		const char *scan;
		scan = toDecode.value();
		for (k = 0; k < toDecode.length() / 2; ++k, scan += 2) {
			retV->concatenateX(THex<char>::decode(*scan) << 4 | THex<char>::decode(*(scan + 1)));
		};
		out = retV;
		return true;
	};

};
