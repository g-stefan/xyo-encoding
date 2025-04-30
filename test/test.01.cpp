// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

#include <XYO/Encoding.hpp>

using namespace XYO::Encoding;

void test() {
	String info("Hello world!");

	printf("String: %s\n", info.value());
	printf("Base16: %s\n", (Base16::encode(info)).value());
	printf("Base32: %s\n", (Base32::encode(info)).value());
	printf("Base64: %s\n", (Base64::encode(info)).value());

	printf("Done.\r\n");
};

int main(int cmdN, char *cmdS[]) {

	try {
		test();
		return 0;
	} catch (const std::exception &e) {
		printf("* Error: %s\n", e.what());
	} catch (...) {
		printf("* Error: Unknown\n");
	};

	return 1;
};
