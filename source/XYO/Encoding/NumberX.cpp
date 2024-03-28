// Encoding
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/Encoding/NumberX.hpp>

namespace XYO::Encoding::NumberX {

	int countDigits(int value) {
		int digits = 0;
		do {
			value /= 10;
			digits++;
		} while (value != 0);

		return digits;
	};

	String leftPadByDigits(int index, int indexLn) {
		char strIndex[32];
		char strProcess[16];
		strProcess[0] = '%';
		strProcess[1] = '0';
		sprintf(&strProcess[2], "%d", countDigits(indexLn));
		strcat(strProcess, "d");
		sprintf(strIndex, strProcess, index);
		return strIndex;
	};

};
