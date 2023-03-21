// Encoding
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_ENCODING_COPYRIGHT_HPP
#define XYO_ENCODING_COPYRIGHT_HPP

#ifndef XYO_ENCODING_DEPENDENCY_HPP
#	include <XYO/Encoding/Dependency.hpp>
#endif

namespace XYO::Encoding::Copyright {
	XYO_ENCODING_EXPORT std::string copyright();
	XYO_ENCODING_EXPORT std::string publisher();
	XYO_ENCODING_EXPORT std::string company();
	XYO_ENCODING_EXPORT std::string contact();
};

#endif
