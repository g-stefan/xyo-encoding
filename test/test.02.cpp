// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// TString / TStringReference / TStringCore: self aliasing, ordering, copyN, replace, encodeC

#include <XYO/Encoding.hpp>
#include <cstdio>
#include <cstring>
#include <type_traits>
#include <utility>

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

static bool isDigitPattern(const String &s) {
	for (size_t k = 0; k < s.length(); ++k) {
		if (s.elementAt(k) != (char)('0' + (k % 10))) {
			return false;
		};
	};
	return true;
};

// s += s, s << s on a single reference string used to write past the buffer
static void selfAppend() {
	String s("0123456789");
	size_t expect = 10;
	for (int k = 0; k < 8; ++k) {
		s += s;
		expect *= 2;
		CHECK(s.length() == expect, "self += length %zu expected %zu", s.length(), expect);
		CHECK(isDigitPattern(s), "self += content at step %d", k);
	};

	String t("0123456789");
	t << t << t;
	CHECK(t.length() == 40, "self << length %zu", t.length());
	CHECK(isDigitPattern(t), "self << content");
};

// appending a pointer or element that lives inside the destination buffer
static void appendOwnMemory() {
	String s("0123456789012345678901234567890");
	CHECK(s.length() == 31, "setup length");
	// grows past the 32 element bucket while the source is the old buffer
	s += s.index(20);
	CHECK(s == "0123456789012345678901234567890"
	           "01234567890",
	      "append own tail '%s'", s.value());

	String big;
	for (int k = 0; k < 300; ++k) {
		big += (char)('a' + (k % 26));
	};
	String bigCopy = big.substring(0);
	big += big.index(0);
	CHECK(big.length() == 600, "append own head length %zu", big.length());
	CHECK(big.substring(300) == bigCopy, "append own head content");

	String e("x");
	for (int k = 0; k < 400; ++k) {
		e += e.elementAt(0);
	};
	CHECK(e.length() == 401, "append own element length %zu", e.length());
	bool allX = true;
	for (size_t k = 0; k < e.length(); ++k) {
		allX = allX && (e.elementAt(k) == 'x');
	};
	CHECK(allX, "append own element content");

	String c("abcdefghijklmnopqrstuvwxyz012345");
	c.concatenate(c.index(26), 6);
	CHECK(c == "abcdefghijklmnopqrstuvwxyz012345012345", "concatenate own '%s'", c.value());
};

// s = s.index(k) used to read the old buffer after releasing it
static void assignOwnPointer() {
	String s;
	for (int k = 0; k < 400; ++k) {
		s += (char)('A' + (k % 26));
	};
	String expect = s.substring(100);
	s = s.index(100);
	CHECK(s == expect, "assign own pointer");
	CHECK(s.length() == 300, "assign own pointer length %zu", s.length());

	String small("hello world");
	small = small.index(6);
	CHECK(small == "world", "assign own pointer small '%s'", small.value());
};

template <typename T>
static bool orderIsSane(const T &a, const T &b) {
	return (a < b) && !(b < a) && (b > a) && (a.compare(b) < 0) && (b.compare(a) > 0) && (a.compare(a) == 0);
};

// (x - y) < 0 is never true for uint32_t
static void ordering() {
	const utf32 a32[] = {'a', 0};
	const utf32 b32[] = {'b', 0};
	const utf32 ab32[] = {'a', 'b', 0};
	const utf32 big32[] = {0x10000, 0};
	CHECK(orderIsSane(StringUTF32(a32), StringUTF32(b32)), "utf32 a < b");
	CHECK(orderIsSane(StringUTF32(a32), StringUTF32(ab32)), "utf32 prefix");
	CHECK(orderIsSane(StringUTF32(b32), StringUTF32(big32)), "utf32 large element");
	CHECK(StringUTF32Core::compareN(a32, b32, 1) < 0, "utf32 compareN");
	CHECK(StringUTF32Core::compareN(b32, a32, 1) > 0, "utf32 compareN reverse");
	CHECK(StringUTF32Core::compareIgnoreCaseASCII(a32, b32) < 0, "utf32 compareIgnoreCase");
	CHECK(StringUTF32Core::compareIgnoreCaseNASCII(b32, a32, 1) > 0, "utf32 compareIgnoreCaseN");
	CHECK(TMemoryCore<utf32>::compareN(const_cast<utf32 *>(a32), b32, 1) < 0, "utf32 memory compareN");

	const utf16 a16[] = {'a', 0};
	const utf16 hi16[] = {0xFF00, 0};
	CHECK(orderIsSane(StringUTF16(a16), StringUTF16(hi16)), "utf16 order");

	// bytes compare as unsigned, like strcmp
	CHECK(orderIsSane(String("a"), String("\xC3\xA9")), "char high byte order");
	CHECK(StringCore::compareIgnoreCaseASCII("a", "\xC3\xA9") < 0, "char compareIgnoreCase high byte");
	CHECK(StringCore::compareIgnoreCaseASCII("ABC", "abc") == 0, "char compareIgnoreCase equal");
};

// strncpy based copyN did not terminate
static void copyN() {
	char buf[8];
	memset(buf, 'X', sizeof(buf));
	StringCore::copyN(buf, "hello", 3);
	CHECK(strcmp(buf, "hel") == 0, "char copyN truncating");
	memset(buf, 'X', sizeof(buf));
	StringCore::copyN(buf, "hi", 5);
	CHECK(strcmp(buf, "hi") == 0, "char copyN short source");

	utf16 buf16[8];
	const utf16 hello16[] = {'h', 'e', 'l', 'l', 'o', 0};
	for (int k = 0; k < 8; ++k) {
		buf16[k] = 'X';
	};
	StringUTF16Core::copyN(buf16, hello16, 3);
	CHECK(buf16[3] == 0 && buf16[2] == 'l', "utf16 copyN");

	char cat[16] = "ab";
	StringCore::concatenateN(cat, "cdef", 2);
	CHECK(strcmp(cat, "abcd") == 0, "concatenateN '%s'", cat);
};

static void replace() {
	CHECK(String("abc").replace("", "x") == "abc", "replace empty pattern");
	CHECK(String("aXbXc").replace("X", "--") == "a--b--c", "replace middle");
	CHECK(String("abX").replace("X", "--") == "ab--", "replace end");
	CHECK(String("Xab").replace("X", "") == "ab", "replace start");
	CHECK(String("XX").replace("XX", "y") == "y", "replace whole");
	CHECK(String("ab").replace("abc", "y") == "ab", "replace longer pattern");
};

// length larger than the source used to over read
static void clampedLength() {
	String a("abc");
	String b;
	b.set(a, 100);
	CHECK(b == "abc" && b.length() == 3, "set clamps '%s'", b.value());
	b.set(a, 2);
	CHECK(b == "ab", "set prefix");
	String c("x");
	c.concatenate(a, 100);
	CHECK(c == "xabc" && c.length() == 4, "concatenate clamps '%s'", c.value());
};

static void encodeC() {
	CHECK(String("a\"b\\c\n\r\t'").encodeC() == "\"a\\\"b\\\\c\\n\\r\\t'\"", "encodeC escapes '%s'", String("a\"b\\c\n\r\t'").encodeC().value());
	// a hex digit right after \xHH must not extend the escape
	CHECK(String("\x01"
	             "A")
	              .encodeCX() == "\\x01\\x41",
	      "encodeC greedy hex '%s'", String("\x01"
	                                        "A")
	                                     .encodeCX()
	                                     .value());
	CHECK(String("\x01"
	             "a9")
	              .encodeCX() == "\\x01\\x61\\x39",
	      "encodeC greedy hex chain");
	CHECK(String("\x01"
	             "G")
	              .encodeCX() == "\\x01G",
	      "encodeC non hex after escape");
	CHECK(String("\xC3\xA9").encodeCX() == "\\xC3\\xA9", "encodeC high bytes");
	CHECK(String("A1").encodeCX() == "A1", "encodeC plain");

	// wide elements keep all their bits
	const utf16 w16[] = {0x4E2D, 'Z', 0};
	const utf16 e16[] = {'\\', 'x', '4', 'E', '2', 'D', 'Z', 0};
	CHECK(StringUTF16(w16).encodeCX() == StringUTF16(e16), "encodeC utf16");
	const utf32 w32[] = {0x1F600, 0};
	const utf32 e32[] = {'\\', 'x', '0', '0', '0', '1', 'F', '6', '0', '0', 0};
	CHECK(StringUTF32(w32).encodeCX() == StringUTF32(e32), "encodeC utf32");
};

static void trimAndSplit() {
	CHECK(String("").trimASCII() == "", "trim empty");
	CHECK(String("   ").trimASCII() == "", "trim blank");
	CHECK(String(" \t a b \r\n").trimASCII() == "a b", "trim both");
	CHECK(String("xxaxx").trimWithElement("x") == "a", "trimWithElement");
	CHECK(String("").trimWithElement("x") == "", "trimWithElement empty");

	String first;
	String second;
	CHECK(String("key=value").split2("=", first, second) && first == "key" && second == "value", "split2");
	CHECK(String("a.b.c").split2FromEnd(".", first, second) && first == "a.b" && second == "c", "split2FromEnd");

	TDynamicArray<String> parts;
	CHECK(String("a,b,,c").explode(",", parts), "explode");
	CHECK(parts.length() == 4 && parts[0] == "a" && parts[2] == "" && parts[3] == "c", "explode parts");
	CHECK(String::implode(",", parts) == "a,b,,c", "implode");
};

// operator[] and elementAt are read only, writing through them used to change every copy
static_assert(!std::is_assignable<decltype(std::declval<String &>()[0]), char>::value, "operator[] must not be writable");
static_assert(!std::is_assignable<decltype(std::declval<String &>().elementAt(0)), char>::value, "elementAt must not be writable");

// setElementAt is copy on write, like +=
static void copyOnWrite() {
	String a("xyz");
	String b = a;
	CHECK(b.setElementAt(0, 'Q'), "setElementAt shared");
	CHECK(a == "xyz", "shared source unchanged '%s'", a.value());
	CHECK(b == "Qyz", "shared copy changed '%s'", b.value());
	CHECK(a.reference() != b.reference(), "shared copy has its own buffer");

	// single user, in place
	String c("abc");
	const char *before = c.value();
	CHECK(c.setElementAt(1, 'X'), "setElementAt single");
	CHECK(c == "aXc", "single changed '%s'", c.value());
	CHECK(c.value() == before, "single changed in place");

	// a copy taken after an in place write keeps its value
	String d = c;
	c.setElementAt(0, 'Z');
	CHECK(d == "aXc" && c == "ZXc", "copy after write '%s' '%s'", d.value(), c.value());

	// out of range, nothing changes
	CHECK(!c.setElementAt(3, '!'), "setElementAt past the end");
	CHECK(!String().setElementAt(0, '!'), "setElementAt empty");
	CHECK(c == "ZXc" && c.length() == 3, "out of range unchanged '%s'", c.value());

	// reading does not copy
	String e = a;
	CHECK(e[0] == 'x' && e.elementAt(2) == 'z', "read");
	CHECK(e.reference() == a.reference(), "read keeps the shared buffer");

	// the private copy keeps binary data and the growth chunk
	String bin("a\0b", 3);
	String bin2 = bin;
	CHECK(bin2.setElementAt(0, 'z'), "setElementAt binary");
	CHECK(bin2.length() == 3 && bin2.elementAt(1) == 0 && bin2.elementAt(2) == 'b', "binary copy");
	CHECK(bin.elementAt(0) == 'a', "binary source unchanged");

	String g(10, 100);
	g += "abc";
	String h = g;
	h.setElementAt(0, 'X');
	CHECK(h.reference()->chunk() == 100, "chunk kept %zu", h.reference()->chunk());

	// wide strings
	const utf16 w[] = {'a', 'b', 0};
	StringUTF16 w1(w);
	StringUTF16 w2 = w1;
	w2.setElementAt(1, 0x4E2D);
	CHECK(w1[1] == 'b' && w2[1] == 0x4E2D, "utf16 copy on write");
};

int main(int cmdN, char *cmdS[]) {
	(void)cmdN;
	(void)cmdS;
	try {

		selfAppend();
		appendOwnMemory();
		assignOwnPointer();
		ordering();
		copyN();
		replace();
		clampedLength();
		encodeC();
		trimAndSplit();
		copyOnWrite();

		printf(failures ? "* FAILED: %d\n" : "test.02: all passed\n", failures);
		return failures ? 1 : 0;

	} catch (const std::exception &e) {
		printf("* Error: %s\n", e.what());
	} catch (...) {
		printf("* Error: Unknown\n");
	};

	return 1;
};
