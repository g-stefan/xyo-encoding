# API reference

Namespace `XYO::Encoding` unless noted. `T` is the element type (`char`,
`utf16`, `utf32`). "C string" means a 0 terminated `const T *`.

## Macros

| Macro | Meaning |
|-------|---------|
| `XYO_ENCODING_EXPORT` | dllexport while building the DLL (`XYO_ENCODING_INTERNAL`), dllimport for users, empty with `XYO_ENCODING_LIBRARY` |
| `XYO_ENCODING_INTERNAL` | defined by the build when compiling this library |
| `XYO_ENCODING_LIBRARY` | static linking, set for users of `xyo-encoding.static` |

## Type aliases

| Name | Type | Header |
|------|------|--------|
| `utf8` | `char` | `UTF8Core.hpp` |
| `utf16` | `uint16_t` | `UTF16Core.hpp` |
| `utf32` | `uint32_t` | `UTF32Core.hpp` |
| `String`, `StringCore`, `StringReference` | `TString<char>`, `TStringCore<char>`, `TStringReference<char>` | `String.hpp` |
| `StringUTF8`, `StringUTF8Core`, `StringUTF8Reference` | same as above (`utf8` is `char`) | `UTF.hpp` |
| `StringUTF16`, `StringUTF16Core`, `StringUTF16Reference` | `TString<utf16>`, ... | `UTF.hpp` |
| `StringUTF32`, `StringUTF32Core`, `StringUTF32Reference` | `TString<utf32>`, ... | `UTF.hpp` |

## TString&lt;T&gt; : Object

Shared, copy on write string. See [Strings](strings.md).

Construction and assignment

| Member | Description |
|--------|-------------|
| `TString()` | empty string |
| `TString(const T *value)` | copy of a C string |
| `TString(const T *value, size_t ln)` | copy of `ln` elements, `0` elements included |
| `TString(const TString &)`, `TString(TString &&)` | share / take the buffer |
| `TString(const TPointer<TStringReference<T>> &)`, `TString(TPointer<...> &&)`, `TString(TStringReference<T> *)` | wrap an existing buffer |
| `TString(size_t length)` | **empty** string with capacity `length` |
| `TString(size_t length, size_t chunk)` | empty, capacity `length`, growth chunk `chunk` |
| `operator=(const T *)` | copy of a C string, safe when it points into this string |
| `operator=(const TString &)`, `operator=(TString &&)`, `operator=(TPointer<TStringReference<T>>)`, `operator=(TStringReference<T> *)` | share the buffer |
| `void set(const T *value, size_t length)` | copy of `length` elements |
| `void set(const TString &value, size_t length)` | first `length` elements of `value` (clamped) |
| `void set(TStringReference<T> *value)` | share the buffer |
| `void empty()` | make it `""` |

Access

| Member | Description |
|--------|-------------|
| `const T *value() const`, `operator const T *() const` | the C string |
| `size_t length() const` | number of elements |
| `bool isEmpty() const` | `length() == 0` |
| `T operator[](int x) const`, `T elementAt(size_t x) const` | element by value, read only, no bounds check |
| `const T *index(size_t x) const` | pointer to element `x` |
| `const TStringReference<T> *reference() const` | the shared buffer, read only |

Changing elements (copy on write)

| Member | Description |
|--------|-------------|
| `bool setElementAt(size_t x, T element)` | in place if this string is the only user of its buffer, else on a private copy; `false` if `x >= length()` |

Appending (copy on write)

| Member | Description |
|--------|-------------|
| `operator+=(const T *)`, `operator+=(const TString &)`, `operator+=(const T &)` | append |
| `operator<<(...)` | same overloads as `+=`, returns `*this` for chaining |
| `void concatenate(const T *value, size_t length)` | append `length` elements |
| `void concatenate(const TString &value, size_t length)` | append the first `length` elements (clamped) |
| `TString operator+(const T *) const`, `operator+(const TString &) const`, `operator+(const T &) const` | new string |

Comparison (unsigned elements, stops at the first 0)

| Member | Description |
|--------|-------------|
| `==`, `!=`, `<`, `<=`, `>`, `>=` with `const T *` or `const TString &` | |
| `int compare(const T *) const`, `int compare(const TString &) const` | `< 0`, `0`, `> 0` |

Search

| Member | Description |
|--------|-------------|
| `bool indexOf(const TString &b, size_t start, size_t &index) const` | first match at or after `start` |
| `bool indexOfFromEnd(const TString &b, size_t start, size_t &index) const` | last match ending at least `start` elements before the end |
| `bool itContains(const TString &b) const` | |
| `bool beginWith(const TString &b) const`, `bool endsWith(const TString &b) const` | |
| `bool matchASCII(const TString &pattern) const` | `*` / `?` wildcards, ASCII case insensitive |

Transformation (return a new string)

| Member | Description |
|--------|-------------|
| `TString substring(size_t start, size_t length) const`, `substring(size_t start) const` | clamped, `""` if `start > length()` |
| `TString replace(const TString &x, const TString &y) const` | replace every `x` by `y`; empty `x` returns `*this` |
| `TString trimASCII() const` | remove space, tab, CR, LF at both ends |
| `TString trimWithElement(const TString &x) const` | remove the elements of `x` at both ends |
| `TString toLowerCaseASCII() const`, `toUpperCaseASCII() const` | ASCII letters only |
| `TString nilAtFirst(const TString &x) const` | part before the first `x`, or the whole string |
| `TString nilAtFirstFromEnd(const TString &x) const` | part before the last `x`, or the whole string |
| `TString encodeC() const` | C string literal with quotes |
| `TString encodeCX() const` | C string literal without quotes |
| `bool split2(const TString &sig, TString &first, TString &second) const` | split at the first `sig`; `false`: `first = *this`, `second = ""` |
| `bool split2FromEnd(const TString &sig, TString &first, TString &second) const` | split at the last `sig` |
| `bool explode(const TString &delimiter, TDynamicArray<TString> &out) const` | append the parts to `out`; `false` for an empty delimiter; a trailing empty part is dropped |
| `static TString implode(const TString &delimiter, TDynamicArray<TString> &in)` | join |

Memory hooks: `static void initMemory()`, `void activeDestructor()` (reset to
`""`). `TMemory<TString<T>>` is `TMemoryPoolActive`.
`XYO::ManagedMemory::TComparator<TString<T>>` is specialized (`isEqual`,
`isNotEqual`, `isLess`, `isGreater`, `isLessOrEqual`, `isGreaterOrEqual`,
`compare`).

## TStringReference&lt;T&gt; : Object

The shared buffer. Not copyable. `TMemory<TStringReference<T>>` is
`TMemoryPoolActive`; buffers up to 256 elements come from pools of 32, 64,
..., 256 elements, larger ones from `new[]`.

| Member | Description |
|--------|-------------|
| `T *value()`, `const T *value() const` | the buffer, writable only through a non-const reference |
| `size_t length() const`, `size_t chunk() const` | |
| `void setLength(size_t)` | set the length after writing into `value()` directly (clamped to capacity - 1) |
| `void setChunk(size_t)` | growth chunk, 0 means 32 |
| `void init()`, `init(size_t length)`, `init(size_t length, size_t chunk)` | allocate an empty buffer (with capacity) |
| `void from(const T *)`, `from(const T *, size_t)` | allocate and copy |
| `void concatenate(const TStringReference *o, const T *x)` (and `const TStringReference *x`, `const T *x, size_t xLn`, `const T &x`) | allocate `o + x` |
| `void concatenateX(const T *)`, `concatenateX(const T *, size_t)`, `concatenateX(const TStringReference *)`, `concatenateX(const T &)` | append in place (caller ensures it is not shared) |
| `static void initMemory()`, `void activeDestructor()` | memory hooks |

## TStringCore&lt;T&gt;

Static algorithms on C strings; `char` uses the libc functions.

| Member | Description |
|--------|-------------|
| `static const T empty[]` | `{0}` |
| `size_t length(const T *)` | |
| `void copy(T *x, const T *y)`, `size_t copyX(T *x, const T *y)` | copy with terminator; `copyX` returns the length |
| `void copyN(T *x, const T *y, size_t yLn)` | at most `yLn` elements, always terminated |
| `void copyMemory(T *x, const T *y, size_t yLn)` | exactly `yLn` elements, then a terminator |
| `int compare(const T *, const T *)`, `int compareN(const T *, const T *, size_t)` | unsigned element order |
| `bool isEqual(...)`, `bool isEqualN(...)` | |
| `bool beginWith(const T *x, const T *y)`, `bool endsWith(const T *x, const T *y)` | |
| `void concatenate(T *x, const T *y)`, `void concatenateN(T *x, const T *y, size_t)` | like `strcat` / `strncat` |
| `bool indexOf(const T *x, size_t xLn, const T *y, size_t yLn, size_t pos, size_t &index)` | also `(x, y, index)`, `(x, y, pos, index)` |
| `bool indexOfFromEnd(...)` | same overloads, search from the end |
| `T elementToLowerCaseASCII(const T &)`, `T elementToUpperCaseASCII(const T &)` | |
| `int compareIgnoreCaseASCII(const T *, const T *)`, `int compareIgnoreCaseNASCII(const T *, const T *, size_t)` | |
| `bool indexOfIgnoreCaseASCII(x, xLn, y, yLn, pos, index)`, `bool indexOfFromEndIgnoreCaseASCII(...)` | |
| `const T *toNotInElement(const T *x, const T *set)` | first element of `x` not in `set` (or the terminator) |
| `const T *toNotInFromEndElement(const T *x, const T *set)` | last element of `x` not in `set` |
| `bool matchASCII(const T *x, size_t xLn, const T *pattern, size_t pLn)`, `matchASCII(const T *x, const T *pattern)` | wildcards |

## TMemoryCore&lt;T&gt;

| Member | Description |
|--------|-------------|
| `static void copyN(T *x, const T *y, size_t ln)` | `memcpy` for `char` |
| `static int compareN(T *x, const T *y, size_t ln)` | `memcmp` for `char`, unsigned order |
| `static void setN(T *x, const T y, size_t ln)` | `memset` for `char` |

## THex&lt;T&gt;

| Member | Description |
|--------|-------------|
| `static T encodeUppercase(U nibble)`, `static T encodeLowercase(U nibble)` | `0..15` to a digit |
| `static bool isValid(const T x)` | `0-9 A-F a-f` |
| `static U decode(const T x)` | digit to `0..15`, 0 if invalid |

`U` is `std::make_unsigned<T>::type`.

## UConvert (namespace `XYO::Encoding::UConvert`)

| Function | Description |
|----------|-------------|
| `uint32_t u32LeftRotate(uint32_t x, uint16_t c)`, `u32RightRotate` | count taken modulo 32 |
| `uint64_t u64LeftRotate(uint64_t x, uint16_t c)`, `u64RightRotate` | count taken modulo 64 |
| `uint32_t u32FromU8(const uint8_t *)`, `uint64_t u64FromU8(const uint8_t *)` | little endian load |
| `uint32_t u32FromU8Reversed(const uint8_t *)`, `uint64_t u64FromU8Reversed(const uint8_t *)` | big endian load |
| `void u32ToU8(uint32_t, uint8_t *)`, `void u64ToU8(uint64_t, uint8_t *)` | little endian store |
| `void u32ToU8Reversed(uint32_t, uint8_t *)`, `void u64ToU8Reversed(uint64_t, uint8_t *)` | big endian store |

## UTF8Core, UTF16Core, UTF32Core (namespaces)

| Function | Description |
|----------|-------------|
| `size_t UTF8Core::elementSize(const utf8 lead)` | 1..4, 0 for an invalid lead byte |
| `bool UTF8Core::elementIsValid(const utf8 *x)` | full RFC 3629 check of one sequence |
| `bool UTF8Core::check(const utf8 x)` | continuation byte |
| `bool UTF8Core::elementIsEqual(const utf8 *x, const utf8 *y)` | same sequence |
| `size_t UTF16Core::elementSize(const utf16 x)` | 1, 2 for a high surrogate, 0 for a low surrogate or 0xFFFF |
| `bool UTF16Core::elementIsValid(const utf16 *x)` | one unit or a correct pair |
| `bool UTF32Core::elementIsValid(const utf32 x)` | not a surrogate, not 0xFFFF, at most 0x10FFFF |
| `size_t UTF32Core::elementSize(const utf32 x)` | 1 if valid, else 0 |

## UTF (namespace `XYO::Encoding::UTF`)

| Function | Description |
|----------|-------------|
| `utf8StringQuestionMark`, `utf16StringQuestionMark`, `utf32StringQuestionMark` | the default error markers, `"?"` |
| `size_t elementUTF32FromUTF8(utf32 *out, const utf8 *in)` | 1 on success, 0 if invalid |
| `size_t elementUTF32FromUTF16(utf32 *out, const utf16 *in)` | 1 on success, 0 if invalid |
| `size_t elementUTF8FromUTF32(utf8 *out, utf32 in)` | bytes written (1..4), 0 if invalid |
| `size_t elementUTF16FromUTF32(utf16 *out, utf32 in)` | units written (1..2), 0 if invalid |
| `size_t elementUTF8FromUTF32Size(utf32 in)`, `elementUTF16FromUTF32Size(utf32 in)`, `elementUTF8FromUTF16Size(const utf16 *in)` | output size of one element, 0 if invalid |
| `String utf8FromUTF16(const utf16 *in, const utf8 *err = "?")` | |
| `String utf8FromUTF32(const utf32 *in, const utf8 *err = "?")` | |
| `StringUTF16 utf16FromUTF8(const utf8 *in, const utf16 *err = "?")` | |
| `StringUTF16 utf16FromUTF32(const utf32 *in, const utf16 *err = "?")` | |
| `StringUTF32 utf32FromUTF8(const utf8 *in, const utf32 *err = "?")` | |
| `StringUTF32 utf32FromUTF16(const utf16 *in, const utf32 *err = "?")` | |
| `size_t utf8FromUTF16Length(...)`, `utf8FromUTF32Length`, `utf16FromUTF8Length`, `utf16FromUTF32Length`, `utf32FromUTF8Length`, `utf32FromUTF16Length` | same parameters, output length in elements |

Every invalid input element is replaced by `err`.

## TUTFConvert&lt;TTo, TFrom&gt;

`static TString<TTo> from(const TString<TFrom> &input)` for every pair of
`utf8`, `utf16`, `utf32`; same type returns the input. Other types:
`static_assert`.

## UTF streams

`UTFStreamMode::None` (0), `UTF8` (1), `UTF16` (2), `UTF32` (3).

`class UTF8Read : public virtual IRead`

| Member | Description |
|--------|-------------|
| `bool open(IRead *input, int mode)` | keep a reference to `input`; always `true` |
| `size_t read(void *output, size_t length)` | UTF-8 bytes produced |
| `void close()` | release `input` |
| `operator bool() const` | open |

`class UTF8Write : public virtual IWrite`

| Member | Description |
|--------|-------------|
| `bool open(IWrite *output, int mode)` | keep a reference to `output`; always `true` |
| `size_t write(const void *input, size_t length)` | UTF-8 bytes accepted |
| `void close()` | release `output` |
| `operator bool() const` | open |

## Base16, Base32, Base64 (namespaces)

| Function | Description |
|----------|-------------|
| `String encode(const String &toEncode)` | |
| `bool decode(const String &toDecode, String &out)` | `false` on invalid input, `out` unchanged |

## NumberX (namespace `XYO::Encoding::NumberX`)

| Function | Description |
|----------|-------------|
| `int countDigits(int value)` | decimal digits, sign not counted, `0` has 1 |
| `String leftPadByDigits(int index, int indexLn)` | `index` padded with `0` to `countDigits(indexLn)` digits |

## Library metadata

`Version::version()`, `Version::build()`, `Version::versionWithBuild()`,
`Version::datetime()` (`const char *`); `Copyright::copyright()`,
`publisher()`, `company()`, `contact()` (`const char *`);
`License::license()`, `License::shortLicense()` (`std::string`). Qualify
them in full (`XYO::Encoding::Version::version()`), the lower libraries have
the same names.
