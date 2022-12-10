// Encoding
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_ENCODING_COPYRIGHT_HPP
#define XYO_ENCODING_COPYRIGHT_HPP

#ifndef XYO_ENCODING_DEPENDENCY_HPP
#	include <XYO/Encoding/Dependency.hpp>
#endif

namespace XYO::Encoding::Copyright {
	XYO_ENCODING_EXPORT const char *copyright();
	XYO_ENCODING_EXPORT const char *publisher();
	XYO_ENCODING_EXPORT const char *company();
	XYO_ENCODING_EXPORT const char *contact();
};

#endif
