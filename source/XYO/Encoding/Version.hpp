// Encoding
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_ENCODING_VERSION_HPP
#define XYO_ENCODING_VERSION_HPP

#ifndef XYO_ENCODING_DEPENDENCY_HPP
#	include <XYO/Encoding/Dependency.hpp>
#endif

namespace XYO::Encoding::Version {

	XYO_ENCODING_EXPORT const char *version();
	XYO_ENCODING_EXPORT const char *build();
	XYO_ENCODING_EXPORT const char *versionWithBuild();
	XYO_ENCODING_EXPORT const char *datetime();

};

#endif
