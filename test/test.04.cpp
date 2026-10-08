// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Base16 / Base32 / Base64 (RFC 4648 vectors, strict decoding, binary round trip), UConvert, NumberX

#include <XYO/Encoding.hpp>
#include <cstdio>
#include <cstring>

using namespace XYO::Encoding;

static int failures = 0;
#define CHECK(cond, ...)                                           \
	do {                                                       \
		if (!(cond)) {                                     \
			++failures;                                \
			printf("FAIL %s:%d ", __FILE__, __LINE__); \
			printf(__VA_ARGS__);                       \
			printf("\n");                              \
		};                                                 \
	} while (0)

typedef String (*EncodeFn)(const String &);
typedef bool (*DecodeFn)(const String &, String &);

struct Vector {
		const char *plain;
		const char *encoded;
};

static void checkVectors(const char *name, EncodeFn encode, DecodeFn decode, const Vector *vectors, size_t count) {
	for (size_t k = 0; k < count; ++k) {
		String encoded = encode(vectors[k].plain);
		CHECK(encoded == vectors[k].encoded, "%s encode '%s' -> '%s'", name, vectors[k].plain, encoded.value());
		String decoded("unchanged");
		CHECK(decode(vectors[k].encoded, decoded), "%s decode '%s' failed", name, vectors[k].encoded);
		CHECK(decoded == vectors[k].plain, "%s decode '%s' -> '%s'", name, vectors[k].encoded, decoded.value());
	};
};

static void checkRejected(const char *name, DecodeFn decode, const char *const *inputs, size_t count) {
	for (size_t k = 0; k < count; ++k) {
		String out("unchanged");
		CHECK(!decode(inputs[k], out), "%s accepted invalid '%s'", name, inputs[k]);
		CHECK(out == "unchanged", "%s modified output on invalid '%s'", name, inputs[k]);
	};
};

// every byte value, every tail length
static void checkBinary(const char *name, EncodeFn encode, DecodeFn decode) {
	char data[300];
	for (size_t k = 0; k < sizeof(data); ++k) {
		data[k] = (char)((k * 7 + 3) & 0xFF);
	};
	for (size_t ln = 0; ln <= sizeof(data); ln += (ln < 20) ? 1 : 37) {
		String plain(data, ln);
		String decoded;
		CHECK(decode(encode(plain), decoded), "%s binary decode length %zu", name, ln);
		CHECK(decoded.length() == ln && memcmp(decoded.value(), data, ln) == 0, "%s binary round trip length %zu", name, ln);
	};
};

static void base16() {
	const Vector vectors[] = {
	    {"", ""},
	    {"f", "66"},
	    {"fo", "666F"},
	    {"foobar", "666F6F626172"},
	    {"\xFF\x80", "FF80"}};
	checkVectors("base16", Base16::encode, Base16::decode, vectors, sizeof(vectors) / sizeof(vectors[0]));

	String lower;
	CHECK(Base16::decode("666f6F", lower) && lower == "foo", "base16 mixed case");

	const char *const invalid[] = {"6", "666", "6G", "G6", "66 6", "0x66"};
	checkRejected("base16", Base16::decode, invalid, sizeof(invalid) / sizeof(invalid[0]));

	checkBinary("base16", Base16::encode, Base16::decode);
};

static void base32() {
	const Vector vectors[] = {
	    {"", ""},
	    {"f", "MY======"},
	    {"fo", "MZXQ===="},
	    {"foo", "MZXW6==="},
	    {"foob", "MZXW6YQ="},
	    {"fooba", "MZXW6YTB"},
	    {"foobar", "MZXW6YTBOI======"}};
	checkVectors("base32", Base32::encode, Base32::decode, vectors, sizeof(vectors) / sizeof(vectors[0]));

	const char *const invalid[] = {
	    "MY=====",          // not a multiple of 8
	    "MZXW6YT",          // not a multiple of 8
	    "M=======",         // 7 padding
	    "MYZ=====",         // 5 padding
	    "MZXW6Y==",         // 2 padding
	    "========",         // only padding
	    "MZXQ===A",         // data after padding
	    "MY======MZXQ====", // padding before the last group
	    "mzxw6ytb",         // lowercase is not in the alphabet
	    "MZXW6YT1",         // 1 is not in the alphabet
	};
	checkRejected("base32", Base32::decode, invalid, sizeof(invalid) / sizeof(invalid[0]));

	checkBinary("base32", Base32::encode, Base32::decode);
};

static void base64() {
	const Vector vectors[] = {
	    {"", ""},
	    {"f", "Zg=="},
	    {"fo", "Zm8="},
	    {"foo", "Zm9v"},
	    {"foob", "Zm9vYg=="},
	    {"fooba", "Zm9vYmE="},
	    {"foobar", "Zm9vYmFy"},
	    {"\xFB\xFF", "+/8="}};
	checkVectors("base64", Base64::encode, Base64::decode, vectors, sizeof(vectors) / sizeof(vectors[0]));

	const char *const invalid[] = {
	    "Zg=",      // not a multiple of 4
	    "Zm9vY",    // not a multiple of 4
	    "Z===",     // 3 padding
	    "====",     // only padding
	    "Zg=A",     // data after padding
	    "Zg==Zm8=", // padding before the last group
	    "Zm9v!A==", // not in the alphabet
	    "Zm9v\nYg==",
	};
	checkRejected("base64", Base64::decode, invalid, sizeof(invalid) / sizeof(invalid[0]));

	checkBinary("base64", Base64::encode, Base64::decode);
};

static void uconvert() {
	CHECK(UConvert::u32LeftRotate(0x12345678, 0) == 0x12345678, "u32 left rotate 0");
	CHECK(UConvert::u32RightRotate(0x12345678, 0) == 0x12345678, "u32 right rotate 0");
	CHECK(UConvert::u32LeftRotate(0x80000001, 1) == 0x00000003, "u32 left rotate 1");
	CHECK(UConvert::u32RightRotate(0x00000003, 1) == 0x80000001, "u32 right rotate 1");
	CHECK(UConvert::u32LeftRotate(0x12345678, 8) == 0x34567812, "u32 left rotate 8");
	CHECK(UConvert::u32RightRotate(0x12345678, 8) == 0x78123456, "u32 right rotate 8");
	CHECK(UConvert::u32LeftRotate(0x12345678, 32) == 0x12345678, "u32 left rotate 32");
	CHECK(UConvert::u64LeftRotate(0x0123456789ABCDEFULL, 0) == 0x0123456789ABCDEFULL, "u64 left rotate 0");
	CHECK(UConvert::u64RightRotate(0x0123456789ABCDEFULL, 0) == 0x0123456789ABCDEFULL, "u64 right rotate 0");
	CHECK(UConvert::u64LeftRotate(0x8000000000000001ULL, 1) == 0x0000000000000003ULL, "u64 left rotate 1");
	CHECK(UConvert::u64RightRotate(0x0123456789ABCDEFULL, 16) == 0xCDEF0123456789ABULL, "u64 right rotate 16");

	uint8_t buf[8];
	UConvert::u32ToU8(0x11223344, buf);
	CHECK(buf[0] == 0x44 && buf[3] == 0x11, "u32ToU8 little endian");
	CHECK(UConvert::u32FromU8(buf) == 0x11223344, "u32FromU8");
	UConvert::u32ToU8Reversed(0x11223344, buf);
	CHECK(buf[0] == 0x11 && buf[3] == 0x44, "u32ToU8Reversed big endian");
	CHECK(UConvert::u32FromU8Reversed(buf) == 0x11223344, "u32FromU8Reversed");
	UConvert::u64ToU8(0x1122334455667788ULL, buf);
	CHECK(UConvert::u64FromU8(buf) == 0x1122334455667788ULL && buf[0] == 0x88, "u64 little endian");
	UConvert::u64ToU8Reversed(0x1122334455667788ULL, buf);
	CHECK(UConvert::u64FromU8Reversed(buf) == 0x1122334455667788ULL && buf[0] == 0x11, "u64 big endian");
};

static void numberX() {
	CHECK(NumberX::countDigits(0) == 1, "countDigits 0");
	CHECK(NumberX::countDigits(9) == 1, "countDigits 9");
	CHECK(NumberX::countDigits(12345) == 5, "countDigits 12345");
	CHECK(NumberX::countDigits(-12) == 2, "countDigits -12");
	CHECK(NumberX::leftPadByDigits(5, 100) == "005", "leftPadByDigits 5 of 100");
	CHECK(NumberX::leftPadByDigits(42, 9) == "42", "leftPadByDigits wider value");
};

int main(int cmdN, char *cmdS[]) {
	(void)cmdN;
	(void)cmdS;
	try {

		base16();
		base32();
		base64();
		uconvert();
		numberX();

		printf(failures ? "* FAILED: %d\n" : "test.04: all passed\n", failures);
		return failures ? 1 : 0;

	} catch (const std::exception &e) {
		printf("* Error: %s\n", e.what());
	} catch (...) {
		printf("* Error: Unknown\n");
	};

	return 1;
};
