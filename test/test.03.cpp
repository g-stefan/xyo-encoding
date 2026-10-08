// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// UTF: round trips, strict UTF-8 validation, invalid / truncated input, UTF8Read / UTF8Write

#include <XYO/Encoding.hpp>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

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

template <typename T>
static bool isEqualN(const TString<T> &s, const T *expect, size_t ln) {
	if (s.length() != ln) {
		return false;
	};
	for (size_t k = 0; k < ln; ++k) {
		if (s.elementAt(k) != expect[k]) {
			return false;
		};
	};
	return true;
};

// exact size heap copy, so reading past the terminator is caught by sanitizers
template <typename T>
static T *heapCopy(const T *x, size_t ln) {
	T *retV = static_cast<T *>(malloc((ln + 1) * sizeof(T)));
	memcpy(retV, x, ln * sizeof(T));
	retV[ln] = 0;
	return retV;
};

// A, e acute, euro, grinning face: 1, 2, 3 and 4 byte UTF-8
static const char *sample8 = "A\xC3\xA9\xE2\x82\xAC\xF0\x9F\x98\x80";
static const utf16 sample16[] = {0x0041, 0x00E9, 0x20AC, 0xD83D, 0xDE00, 0};
static const utf32 sample32[] = {0x0041, 0x00E9, 0x20AC, 0x1F600, 0};

static void roundTrip() {
	StringUTF16 s16 = UTF::utf16FromUTF8(sample8);
	StringUTF32 s32 = UTF::utf32FromUTF8(sample8);
	CHECK(isEqualN(s16, sample16, 5), "utf16FromUTF8");
	CHECK(isEqualN(s32, sample32, 4), "utf32FromUTF8");
	CHECK(UTF::utf8FromUTF16(sample16) == sample8, "utf8FromUTF16");
	CHECK(UTF::utf8FromUTF32(sample32) == sample8, "utf8FromUTF32");
	CHECK(isEqualN(UTF::utf16FromUTF32(sample32), sample16, 5), "utf16FromUTF32");
	CHECK(isEqualN(UTF::utf32FromUTF16(sample16), sample32, 4), "utf32FromUTF16");

	CHECK(UTF::utf16FromUTF8Length(sample8) == 5, "utf16FromUTF8Length");
	CHECK(UTF::utf32FromUTF8Length(sample8) == 4, "utf32FromUTF8Length");
	CHECK(UTF::utf8FromUTF16Length(sample16) == 10, "utf8FromUTF16Length");
	CHECK(UTF::utf8FromUTF32Length(sample32) == 10, "utf8FromUTF32Length");
	CHECK(UTF::utf16FromUTF32Length(sample32) == 5, "utf16FromUTF32Length");
	CHECK(UTF::utf32FromUTF16Length(sample16) == 4, "utf32FromUTF16Length");

	CHECK((TUTFConvert<utf16, utf8>::from(sample8) == UTF::utf16FromUTF8(sample8)), "TUTFConvert");

	// boundaries of every sequence length
	const utf32 edges[] = {0x7F, 0x80, 0x7FF, 0x800, 0xD7FF, 0xE000, 0xFFFD, 0x10000, 0x10FFFF, 0};
	String e8 = UTF::utf8FromUTF32(edges);
	CHECK(isEqualN(UTF::utf32FromUTF8(e8), edges, 9), "edge code points round trip");
	CHECK(isEqualN(UTF::utf32FromUTF16(UTF::utf16FromUTF32(edges)), edges, 9), "edge code points utf16 round trip");
};

static void elementSizes() {
	CHECK(UTF::elementUTF8FromUTF32Size(0x7F) == 1, "size 0x7F");
	CHECK(UTF::elementUTF8FromUTF32Size(0x80) == 2, "size 0x80");
	CHECK(UTF::elementUTF8FromUTF32Size(0x7FF) == 2, "size 0x7FF");
	CHECK(UTF::elementUTF8FromUTF32Size(0x800) == 3, "size 0x800");
	CHECK(UTF::elementUTF8FromUTF32Size(0x10000) == 4, "size 0x10000");
	CHECK(UTF::elementUTF8FromUTF32Size(0x10FFFF) == 4, "size 0x10FFFF");
	CHECK(UTF::elementUTF8FromUTF32Size(0x110000) == 0, "size 0x110000");
	CHECK(UTF::elementUTF8FromUTF32Size(0xD800) == 0, "size surrogate");

	const utf16 pair[] = {0xD83D, 0xDE00, 0};
	const utf16 loneHigh[] = {0xD83D, 'A', 0};
	const utf16 loneLow[] = {0xDE00, 0};
	const utf16 e9[] = {0x00E9, 0};
	const utf16 euro[] = {0x20AC, 0};
	CHECK(UTF::elementUTF8FromUTF16Size(pair) == 4, "utf16 pair size %zu", UTF::elementUTF8FromUTF16Size(pair));
	CHECK(UTF::elementUTF8FromUTF16Size(loneHigh) == 0, "utf16 lone high");
	CHECK(UTF::elementUTF8FromUTF16Size(loneLow) == 0, "utf16 lone low");
	CHECK(UTF::elementUTF8FromUTF16Size(sample16) == 1, "utf16 ascii");
	CHECK(UTF::elementUTF8FromUTF16Size(e9) == 2, "utf16 2 byte");
	CHECK(UTF::elementUTF8FromUTF16Size(euro) == 3, "utf16 3 byte");
};

static bool isRejected(const char *x) {
	utf32 out = 0x55555555;
	return UTF::elementUTF32FromUTF8(&out, x) == 0;
};

// overlong forms, surrogates, above U+10FFFF, obsolete 5 / 6 byte forms
static void strictUTF8() {
	CHECK(isRejected("\xC0\x80"), "overlong NUL");
	CHECK(isRejected("\xC0\xAF"), "overlong slash 2 byte");
	CHECK(isRejected("\xC1\xBF"), "overlong 2 byte");
	CHECK(isRejected("\xE0\x80\xAF"), "overlong slash 3 byte");
	CHECK(isRejected("\xE0\x9F\xBF"), "overlong 3 byte");
	CHECK(isRejected("\xF0\x80\x80\xAF"), "overlong slash 4 byte");
	CHECK(isRejected("\xF0\x8F\xBF\xBF"), "overlong 4 byte");
	CHECK(isRejected("\xED\xA0\x80"), "surrogate D800");
	CHECK(isRejected("\xED\xBF\xBF"), "surrogate DFFF");
	CHECK(isRejected("\xF4\x90\x80\x80"), "above U+10FFFF");
	CHECK(isRejected("\xF5\x80\x80\x80"), "lead F5");
	CHECK(isRejected("\xF8\x88\x80\x80\x80"), "5 byte form");
	CHECK(isRejected("\xFC\x84\x80\x80\x80\x80"), "6 byte form");
	CHECK(isRejected("\x80"), "lone continuation");
	CHECK(isRejected("\xC3"), "truncated 2 byte");
	CHECK(isRejected("\xE2\x82"), "truncated 3 byte");
	CHECK(isRejected("\xC3\x41"), "bad continuation");

	CHECK(!isRejected("\xE0\xA0\x80"), "U+0800");
	CHECK(!isRejected("\xED\x9F\xBF"), "U+D7FF");
	CHECK(!isRejected("\xEE\x80\x80"), "U+E000");
	CHECK(!isRejected("\xF0\x90\x80\x80"), "U+10000");
	CHECK(!isRejected("\xF4\x8F\xBF\xBF"), "U+10FFFF");

	// the conversion must not produce the hidden characters
	String decoded = UTF::utf8FromUTF32(UTF::utf32FromUTF8("a\xC0\xAF"
	                                                       "b\xC0\x80"
	                                                       "c"));
	CHECK(decoded == "a??b??c", "overlong replaced '%s'", decoded.value());
};

static void invalidInput() {
	const utf32 q = '?';

	// truncated sequence at the very end, the terminator must not be skipped
	char *t8 = heapCopy("A\xE2\x82", 3);
	const utf32 t8expect32[] = {'A', q, q};
	const utf16 t8expect16[] = {'A', q, q};
	CHECK(isEqualN(UTF::utf32FromUTF8(t8), t8expect32, 3), "truncated utf8 to utf32");
	CHECK(isEqualN(UTF::utf16FromUTF8(t8), t8expect16, 3), "truncated utf8 to utf16");
	CHECK(UTF::utf32FromUTF8Length(t8) == 3, "truncated utf8 utf32 length");
	CHECK(UTF::utf16FromUTF8Length(t8) == 3, "truncated utf8 utf16 length");
	free(t8);

	const utf16 highAtEnd[] = {'A', 0xD83D};
	utf16 *t16 = heapCopy(highAtEnd, 2);
	CHECK(UTF::utf8FromUTF16(t16) == "A?", "truncated utf16 to utf8");
	const utf32 t16expect[] = {'A', q};
	CHECK(isEqualN(UTF::utf32FromUTF16(t16), t16expect, 2), "truncated utf16 to utf32");
	CHECK(UTF::utf8FromUTF16Length(t16) == 2, "truncated utf16 utf8 length");
	CHECK(UTF::utf32FromUTF16Length(t16) == 2, "truncated utf16 utf32 length");
	free(t16);

	// a bad sequence must not swallow the valid characters after it
	const utf32 swallowExpect[] = {q, 'A', 'B'};
	CHECK(isEqualN(UTF::utf32FromUTF8("\xE2"
	                                  "AB"),
	               swallowExpect, 3),
	      "bad lead keeps following data");
	const utf16 loneHigh[] = {0xD83D, 'A', 0};
	CHECK(UTF::utf8FromUTF16(loneHigh) == "?A", "lone high surrogate keeps following data");

	// every converter replaces invalid elements with err, the length functions agree
	const utf32 bad32[] = {'a', 0xD800, 'b', 0x110000, 0};
	CHECK(UTF::utf8FromUTF32(bad32) == "a?b?", "utf8FromUTF32 invalid");
	CHECK(UTF::utf8FromUTF32Length(bad32) == 4, "utf8FromUTF32Length invalid");
	const utf16 bad32to16[] = {'a', q, 'b', q};
	CHECK(isEqualN(UTF::utf16FromUTF32(bad32), bad32to16, 4), "utf16FromUTF32 invalid");
	const utf32 bad8to32[] = {'a', q, 'b'};
	CHECK(isEqualN(UTF::utf32FromUTF8("a\xFF"
	                                  "b"),
	               bad8to32, 3),
	      "utf32FromUTF8 invalid");
	const utf16 bad16[] = {'a', 0xDC00, 'b', 0};
	CHECK(isEqualN(UTF::utf32FromUTF16(bad16), bad8to32, 3), "utf32FromUTF16 invalid");

	// custom error marker
	const utf32 err32[] = {'<', '>', 0};
	const utf32 errExpect[] = {'a', '<', '>', 'b'};
	CHECK(isEqualN(UTF::utf32FromUTF8("a\xFF"
	                                  "b",
	                                  err32),
	               errExpect, 4),
	      "custom err");
	CHECK(UTF::utf32FromUTF8Length("a\xFF"
	                               "b",
	                               err32) == 4,
	      "custom err length");
	CHECK(UTF::utf8FromUTF16(bad16, "[bad]") == "a[bad]b", "custom err utf8");
	CHECK(UTF::utf8FromUTF16Length(bad16, "[bad]") == 7, "custom err utf8 length");
};

// in memory stream for UTF8Read / UTF8Write
class MemoryIO : public virtual IRead,
                 public virtual IWrite {
		XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(MemoryIO);

	public:
		std::string data;
		size_t position = 0;

		MemoryIO() {};

		size_t read(void *output, size_t length) {
			size_t available = data.size() - position;
			if (length > available) {
				length = available;
			};
			memcpy(output, data.data() + position, length);
			position += length;
			return length;
		};

		size_t write(const void *input, size_t length) {
			data.append(static_cast<const char *>(input), length);
			return length;
		};
};

static std::string bytes(const void *x, size_t ln) {
	return std::string(static_cast<const char *>(x), ln);
};

static void streamWrite() {
	MemoryIO memory;
	UTF8Write writer;
	writer.open(&memory, UTFStreamMode::UTF16);

	CHECK(writer.write("A\xE2\x82\xAC", 4) == 4, "write utf16");
	const utf16 expect16[] = {'A', 0x20AC};
	CHECK(memory.data == bytes(expect16, sizeof(expect16)), "write utf16 data");

	// an invalid lead byte is refused and must not poison the next write
	memory.data.clear();
	CHECK(writer.write("\x80", 1) == 0, "write invalid lead refused");
	CHECK(writer.write("B", 1) == 1, "write after invalid lead");
	const utf16 expectB[] = {'B'};
	CHECK(memory.data == bytes(expectB, sizeof(expectB)), "write after invalid lead data");

	// a sequence cut by a non continuation byte
	memory.data.clear();
	CHECK(writer.write("\xE2\x82", 2) == 2, "write partial sequence buffered");
	CHECK(writer.write("C", 1) == 0, "write broken sequence refused");
	CHECK(writer.write("C", 1) == 1, "write after broken sequence");
	const utf16 expectC[] = {'C'};
	CHECK(memory.data == bytes(expectC, sizeof(expectC)), "write after broken sequence data");

	// a sequence split across writes
	memory.data.clear();
	CHECK(writer.write("\xF0\x9F", 2) == 2, "write split first half");
	CHECK(writer.write("\x98\x80", 2) == 2, "write split second half");
	const utf16 expectPair[] = {0xD83D, 0xDE00};
	CHECK(memory.data == bytes(expectPair, sizeof(expectPair)), "write split data");

	MemoryIO memory32;
	UTF8Write writer32;
	writer32.open(&memory32, UTFStreamMode::UTF32);
	CHECK(writer32.write(sample8, strlen(sample8)) == strlen(sample8), "write utf32");
	CHECK(memory32.data == bytes(sample32, 4 * sizeof(utf32)), "write utf32 data");
	CHECK(writer32.write("\xC0\x80", 2) == 0, "write utf32 overlong refused");
	CHECK(writer32.write("D", 1) == 1, "write utf32 after overlong");
};

static void streamRead() {
	char buffer[32];

	MemoryIO memory16;
	memory16.data = bytes(sample16, 5 * sizeof(utf16));
	UTF8Read reader16;
	reader16.open(&memory16, UTFStreamMode::UTF16);
	size_t ln = reader16.read(buffer, sizeof(buffer));
	CHECK(bytes(buffer, ln) == sample8, "read utf16");

	// invalid code point in UTF32 input used to produce a NUL byte
	MemoryIO memory32;
	const utf32 bad32[] = {0x110000, 'A'};
	memory32.data = bytes(bad32, sizeof(bad32));
	UTF8Read reader32;
	reader32.open(&memory32, UTFStreamMode::UTF32);
	memset(buffer, 'X', sizeof(buffer));
	ln = reader32.read(buffer, 1);
	CHECK(ln == 0, "read utf32 invalid stops, got %zu byte(s) 0x%02X", ln, (unsigned)(uint8_t)buffer[0]);
	ln = reader32.read(buffer, 1);
	CHECK(ln == 1 && buffer[0] == 'A', "read utf32 continues");

	// output split in the middle of a multi byte sequence
	MemoryIO memory32b;
	memory32b.data = bytes(sample32, 4 * sizeof(utf32));
	UTF8Read reader32b;
	reader32b.open(&memory32b, UTFStreamMode::UTF32);
	std::string all;
	while ((ln = reader32b.read(buffer, 3)) > 0) {
		all.append(buffer, ln);
	};
	CHECK(all == sample8, "read utf32 in small pieces");
};

int main(int cmdN, char *cmdS[]) {
	(void)cmdN;
	(void)cmdS;
	try {

		roundTrip();
		elementSizes();
		strictUTF8();
		invalidInput();
		streamWrite();
		streamRead();

		printf(failures ? "* FAILED: %d\n" : "test.03: all passed\n", failures);
		return failures ? 1 : 0;

	} catch (const std::exception &e) {
		printf("* Error: %s\n", e.what());
	} catch (...) {
		printf("* Error: Unknown\n");
	};

	return 1;
};
