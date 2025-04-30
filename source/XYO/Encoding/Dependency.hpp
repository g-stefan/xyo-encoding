// Encoding
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_ENCODING_DEPENDENCY_HPP
#define XYO_ENCODING_DEPENDENCY_HPP

#ifndef XYO_DATASTRUCTURES_HPP
#	include <XYO/DataStructures.hpp>
#endif

// -- Export

#ifdef XYO_ENCODING_INTERNAL
#	define XYO_ENCODING_EXPORT XYO_PLATFORM_LIBRARY_EXPORT
#else
#	define XYO_ENCODING_EXPORT XYO_PLATFORM_LIBRARY_IMPORT
#endif
#ifdef XYO_ENCODING_LIBRARY
#	undef XYO_ENCODING_EXPORT
#	define XYO_ENCODING_EXPORT
#endif

// --

namespace XYO::Encoding {
	using namespace XYO::ManagedMemory;
};

#endif
