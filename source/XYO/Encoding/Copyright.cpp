// Encoding
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/Encoding/Copyright.hpp>
#include <XYO/Encoding/Copyright.rh>

namespace XYO::Encoding::Copyright {

	static const char *copyright_ = XYO_ENCODING_COPYRIGHT;
	static const char *publisher_ = XYO_ENCODING_PUBLISHER;
	static const char *company_ = XYO_ENCODING_COMPANY;
	static const char *contact_ = XYO_ENCODING_CONTACT;

	const char *copyright() {
		return copyright_;
	};

	const char *publisher() {
		return publisher_;
	};

	const char *company() {
		return company_;
	};

	const char *contact() {
		return contact_;
	};

};
