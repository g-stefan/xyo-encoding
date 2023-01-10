// Encoding
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_ENCODING_DEPENDENCY_HPP
#define XYO_ENCODING_DEPENDENCY_HPP

#ifndef XYO_DATASTRUCTURES_HPP
#	include <XYO/DataStructures.hpp>
#endif

// -- Export

#ifdef XYO_ENCODING_INTERNAL
#	define XYO_ENCODING_EXPORT XYO_LIBRARY_EXPORT
#else
#	define XYO_ENCODING_EXPORT XYO_LIBRARY_IMPORT
#endif

// --

namespace XYO::Encoding {
	using namespace XYO::ManagedMemory;
};

#endif
