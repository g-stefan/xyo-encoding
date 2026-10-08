# UTF

## Element types and strings

| Encoding | Element type | String | C string algorithms |
|----------|--------------|--------|---------------------|
| UTF-8    | `utf8` = `char`       | `String` / `StringUTF8` | `StringCore` / `StringUTF8Core` |
| UTF-16   | `utf16` = `uint16_t`  | `StringUTF16`           | `StringUTF16Core` |
| UTF-32   | `utf32` = `uint32_t`  | `StringUTF32`           | `StringUTF32Core` |

- `String` is UTF-8 by convention. The XYO libraries above treat every
  `String` as UTF-8 (file names, console output, JSON, network).
- UTF-16 and UTF-32 elements are in the machine's native byte order (little
  endian on every supported target). There is no byte order mark handling.
- `utf16` is **not** `char16_t` or `wchar_t`, and `utf32` is not `char32_t`.
  Cast pointers when you cross that boundary (below).
- All strings and conversion inputs are **0 terminated**; the converters stop
  at the first 0 element.
- A `StringUTF16` / `StringUTF32` literal is an array:

  ```cpp
  const utf16 hello16[] = {'h', 'e', 'l', 'l', 'o', 0};
  StringUTF16 hello(hello16);
  ```

## Converting

```cpp
String      s8  = "A\xC3\xA9\xE2\x82\xAC\xF0\x9F\x98\x80";   // A, e acute, euro, U+1F600
StringUTF16 s16 = UTF::utf16FromUTF8(s8);    // 5 elements (the last code point is a surrogate pair)
StringUTF32 s32 = UTF::utf32FromUTF8(s8);    // 4 elements, one per code point
String      back = UTF::utf8FromUTF16(s16);  // == s8

// the same through a template, when the types are parameters
StringUTF32 x = TUTFConvert<utf32, utf8>::from(s8);
```

All six directions exist:

| From \ To | UTF-8 | UTF-16 | UTF-32 |
|-----------|-------|--------|--------|
| UTF-8     | -     | `utf16FromUTF8` | `utf32FromUTF8` |
| UTF-16    | `utf8FromUTF16` | - | `utf32FromUTF16` |
| UTF-32    | `utf8FromUTF32` | `utf16FromUTF32` | - |

Each takes a `const` element pointer (a `TString` converts implicitly) and
an optional error marker, and returns a new string. `TUTFConvert<To, From>`
also covers `To == From` (returns the input); other types fail to compile.

Each converter has a `...Length` twin returning the number of output
elements without building the string, e.g.
`UTF::utf32FromUTF8Length(s8)` counts code points (invalid elements count
as the marker length).

## Invalid input

Decoding is strict (RFC 3629 for UTF-8):

- UTF-8: lead bytes `C0`, `C1`, `F5`..`FF`, overlong forms, encoded
  surrogates (`ED A0`..`ED BF`), code points above U+10FFFF, lone or missing
  continuation bytes are invalid;
- UTF-16: unpaired surrogates are invalid;
- UTF-32: surrogates (U+D800..U+DFFF) and values above U+10FFFF are invalid;
- U+FFFF is treated as invalid in all three.

Each invalid **input element** is replaced by the error marker, `"?"` by
default, and decoding continues with the next element. A broken sequence
never swallows the valid data after it, and never skips the terminator:

```cpp
UTF::utf32FromUTF8("a\xFF" "b");                // 'a', '?', 'b'
UTF::utf32FromUTF8("A\xE2\x82");                // 'A', '?', '?'  (truncated 3 byte sequence)
UTF::utf8FromUTF16(loneHighSurrogateThenA);     // "?A"

UTF::utf8FromUTF16(bad16, "[bad]");             // custom marker, any length, may be "" to drop
const utf32 replacement[] = {0xFFFD, 0};
UTF::utf32FromUTF8(input, replacement);         // U+FFFD REPLACEMENT CHARACTER
```

To **check** input instead of repairing it, decode element by element, or
compare with an empty marker:

```cpp
bool isValidUTF8(const String &s) {
	return UTF::utf32FromUTF8Length(s, StringUTF32Core::empty) == UTF::utf32FromUTF8Length(s);
};
```

## Element functions

For code that walks buffers one code point at a time:

```cpp
utf32 cp;
size_t used = UTF8Core::elementSize(*p);          // 1..4 from the lead byte, 0 if invalid lead
if (UTF::elementUTF32FromUTF8(&cp, p)) {          // 1 on success, 0 if invalid
	p += used;
};

utf8 out[4];
size_t n = UTF::elementUTF8FromUTF32(out, cp);    // bytes written, 0 if cp is invalid
utf16 out16[2];
size_t m = UTF::elementUTF16FromUTF32(out16, cp); // 1 or 2 units, 0 if invalid
UTF::elementUTF32FromUTF16(&cp, p16);             // 1 on success; UTF16Core::elementSize(*p16) units used
```

- `UTF8Core::elementIsValid(p)`, `UTF16Core::elementIsValid(p)`,
  `UTF32Core::elementIsValid(cp)`: validate one element.
- `UTF8Core::check(byte)`: is it a continuation byte (`10xxxxxx`).
- `elementUTF8FromUTF32Size(cp)`, `elementUTF16FromUTF32Size(cp)`,
  `elementUTF8FromUTF16Size(p16)`: output size for one element, 0 if
  invalid.

## Windows wide strings

On Windows `wchar_t` is 16 bits and holds UTF-16, so the bits are the same as
`utf16`; only the type differs:

```cpp
// wide -> UTF-8
String name = UTF::utf8FromUTF16(reinterpret_cast<const utf16 *>(L"résumé.txt"));

// UTF-8 -> wide, for a W API
StringUTF16 wide = UTF::utf16FromUTF8(name);
CreateFileW(reinterpret_cast<const wchar_t *>(wide.value()), ...);
```

On Linux `wchar_t` is 32 bits: use `utf32` and `utf32FromUTF8` /
`utf8FromUTF32` instead. `char16_t` / `char32_t` strings need the same
`reinterpret_cast`.

## UTF streams

`UTF8Read` and `UTF8Write` adapt a stream of UTF-16 or UTF-32 data so that
your code reads and writes UTF-8. They implement `IRead` / `IWrite` from
`xyo-data-structures`, so they plug into anything that takes those
interfaces.

```cpp
// file / socket / buffer that contains UTF-16 data
UTF8Read reader;
reader.open(&input, UTFStreamMode::UTF16);      // input is an IRead
char buffer[4096];
size_t n;
while ((n = reader.read(buffer, sizeof(buffer))) > 0) {
	// buffer[0..n) is UTF-8
};
reader.close();

// produce UTF-32 data from UTF-8
UTF8Write writer;
writer.open(&output, UTFStreamMode::UTF32);     // output is an IWrite
writer.write(text.value(), text.length());
writer.close();
```

Modes (`UTFStreamMode::`): `None` and `UTF8` pass the bytes through
unchanged, `UTF16` and `UTF32` convert.

Behavior:

- `UTF8Read::read` may return fewer bytes than asked. A code point is never
  lost between calls: if the buffer ends in the middle of a multi byte
  sequence the rest is returned by the next `read`.
- `UTF8Read::read` stops at the first invalid element of the input (returns
  what it produced so far, possibly 0). The invalid element is consumed (for
  an unpaired high surrogate, also the unit after it) and the next call
  continues after it. A short `read` therefore means end of data **or**
  invalid data.
- `UTF8Write::write` returns the number of UTF-8 bytes accepted. A multi
  byte sequence split across two `write` calls is buffered and completed by
  the next call. A byte that cannot start or continue a sequence is refused:
  `write` returns the count before it, the partial sequence is dropped, and
  the next `write` starts clean. A complete sequence that decodes to an
  invalid code point (overlong, surrogate) is counted but not written, and
  `write` stops after it.
- `open` keeps a reference to the inner stream (`TPointerX`), `close`
  releases it. Neither closes the inner stream itself.
- Not copyable or movable; one thread at a time, like every object.

The inner stream can be any `IRead` / `IWrite` object, for example a file of
`xyo-system`, or your own class:

```cpp
class MemoryIO : public virtual IRead, public virtual IWrite {
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
```
