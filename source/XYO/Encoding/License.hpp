// Encoding
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_ENCODING_LICENSE_HPP
#define XYO_ENCODING_LICENSE_HPP

#ifndef XYO_ENCODING_DEPENDENCY_HPP
#	include <XYO/Encoding/Dependency.hpp>
#endif

namespace XYO::Encoding::License {

	XYO_ENCODING_EXPORT const char *licenseHeader();
	XYO_ENCODING_EXPORT const char *licenseBody();
	XYO_ENCODING_EXPORT const char *shortLicense();

};

#endif
