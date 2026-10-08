---
name: xyo-encoding
description: >-
  How to use the xyo-encoding C++ library (namespace XYO::Encoding), the
  text layer of the XYO C++ stack on top of xyo-data-structures: the shared
  copy on write string TString<T> / String / StringUTF8 / StringUTF16 /
  StringUTF32 (append, compare, search, substring, replace, trimASCII,
  split2, explode / implode, matchASCII, encodeC), TStringCore /
  TStringReference / TMemoryCore, the utf8 / utf16 / utf32 types and strict
  UTF conversion (UTF::utf8FromUTF16, utf16FromUTF8, utf32FromUTF8, ...,
  TUTFConvert, UTF8Core / UTF16Core / UTF32Core), the UTF8Read / UTF8Write
  streams, Base16 / Base32 / Base64 encode / decode, THex, UConvert (rotate,
  little / big endian), NumberX. Use when writing or reviewing code that
  includes <XYO/Encoding.hpp>, depends on "xyo-encoding" in fabricare.json,
  uses any of these names, or when working inside the xyo-encoding
  repository or libraries built on it (xyo-system, xyo-cryptography,
  xyo-networking, quantum-script).
---

# xyo-encoding

Text layer of the XYO C++ libraries, on top of `xyo-data-structures` (see
the `xyo-data-structures` and `xyo-managed-memory` skills; their rules
apply). Purpose: **the one string type of the XYO stack** (`String`, shared
and copy on write, pooled buffers) plus UTF-8 / 16 / 32 conversion, UTF
streams, Base16 / 32 / 64 and small byte helpers.

Full documentation: `docs/` in the xyo-encoding repository
(`X:\Storage\XYO\Gitea\CPP\xyo-encoding\docs` on this machine): README,
getting-started, **strings** (model, every operation, pitfalls), utf,
binary-to-text, reference. Read the matching page when you need more than
this summary. When in doubt read the header in `source/XYO/Encoding/`.

## Types

| Name | Is |
|------|----|
| `String` = `StringUTF8` | `TString<char>`, UTF-8 by convention |
| `StringUTF16`, `StringUTF32` | `TString<utf16>`, `TString<utf32>` |
| `utf8`, `utf16`, `utf32` | `char`, `uint16_t`, `uint32_t` (not `char16_t` / `wchar_t` / `char32_t`: `reinterpret_cast` pointers) |
| `StringCore`, `StringUTF16Core`, ... | `TStringCore<T>`: static C string algorithms |
| `StringReference`, ... | `TStringReference<T>`: the shared buffer |

## String model

- `TString` holds a `TPointer<TStringReference<T>>`. **Copy / assign /
  pass / return by value shares the buffer** (refcount increment).
- **Every change is copy on write**: `+=`, `<<`, `concatenate`,
  `setElementAt` work **in place only if this string is the single user**
  (`hasOneReference()`), else on a new buffer; other copies never change.
  All other operations return a new string. Reads never copy.
- Buffer always 0 terminated; `value()` is a C string. Pools for buffers up
  to 256 elements, then `new[]`; growth by chunks of 32, then geometric.
- `String` is an `Object` and a **leaf**: fine as a local, a by-value class
  member (no `TPointerX` / `pointerLink` needed), parameter, return value,
  container element or key (`TComparator<TString<T>>` is specialized:
  `TRedBlackTree<String, V>`, `TRedBlackTreeOne<String>`,
  `TAssociativeArray<String, V>`, `TDynamicArray<String>`).

## Hard rules

1. **`operator[]` / `elementAt` are read only and return by value**
   (`s[0] = 'x'` does not compile). Change an element with
   `s.setElementAt(i, c)` (copy on write, `false` if `i >= length()`).
   `reference()` returns `const TStringReference<T> *`; never `const_cast`
   `value()` / `index()` / `reference()` to write.
2. **`String s(5)` is an empty string with capacity 5**, not `"5"`. No
   number constructor; format with `snprintf` / `std::to_string(...).c_str()`
   / `NumberX`.
3. **Pass `s.value()` to `printf`** and other variadic functions, never the
   object.
4. **Binary data**: build with `String(ptr, len)`, append with `+= String`
   or `concatenate(ptr, len)`. Comparisons (`==`, `<`, `compare`), `+= const
   T *`, `replace`, `indexOf`, `trim*`, `to*CaseASCII`, `matchASCII`,
   `encodeC` **stop at the first 0 element**; compare binary with `length()`
   + `memcmp`.
5. **No bounds checks** on `[]` (takes `int`), `elementAt`, `index`
   (`setElementAt` checks).
   `substring`, `set(TString, n)`, `concatenate(TString, n)` clamp.
6. **One thread per string**: plain refcount and per-thread pools. Never
   copy or release a `String` on another thread; pass characters
   (`std::string`, `TMemorySystem` object) and build a new `String` on the
   receiving thread. Call `XYO::ManagedMemory::Registry::registryInit()`
   first in `main` when threads are used.
7. `length()` counts elements (bytes for UTF-8, 16-bit units for UTF-16),
   not code points; use `UTF::utf32FromUTF8Length(s)` for code points.
8. Case functions are ASCII only (`toUpperCaseASCII`,
   `compareIgnoreCaseASCII`, `matchASCII`, `indexOfIgnoreCaseASCII`).
9. `Version`, `Copyright`, `License` exist in every XYO library and
   `XYO::Encoding` pulls in `XYO::ManagedMemory` and `XYO::DataStructures`:
   qualify them (`XYO::Encoding::Version::version()`).

## String API (all on `TString<T>`)

```cpp
String s("abc"); String p(ptr, len); String r(reserve); String r2(reserve, chunk);
s = "x"; s.set(ptr, len); s.set(other, n); s.empty();
s.value(); s.length(); s.isEmpty(); s[i]; s.elementAt(i); s.index(i);  // read only, by value
s.setElementAt(i, c);                                        // copy on write, bool
s += "x"; s += other; s += 'c'; s << a << b; s.concatenate(ptr, len); s + "x";
s == "x"; s < other; s.compare(other);                       // unsigned element order
s.indexOf(x, start, idx); s.indexOfFromEnd(x, startFromEnd, idx);
s.itContains(x); s.beginWith(x); s.endsWith(x); s.matchASCII("*.TXT");  // * ?, ignore ASCII case
s.substring(start, len); s.substring(start);                 // clamped
s.replace(x, y);                                             // all; empty x -> unchanged
s.trimASCII(); s.trimWithElement(" /");                      // space tab CR LF / given set
s.toLowerCaseASCII(); s.toUpperCaseASCII();
s.nilAtFirst("/");         // "a/b/c" -> "a"
s.nilAtFirstFromEnd("/");  // "a/b/c" -> "a/b"
s.encodeC(); s.encodeCX();                                   // C literal with / without quotes
s.split2("=", first, second);        // at first; false -> first = s, second = ""
s.split2FromEnd(".", first, second); // at last
s.explode(",", array);               // APPENDS to TDynamicArray<String>; false if delimiter empty;
                                     // trailing empty part dropped ("a,b," -> 2), leading kept
String::implode(",", array);
```

Static C string helpers: `StringCore::length / copyN (always terminates) /
compare / compareIgnoreCaseASCII / indexOf / indexOfIgnoreCaseASCII /
beginWith / endsWith / matchASCII / toNotInElement`.
`TMemoryCore<T>::copyN / compareN / setN`.

Building a result without copies (library style):

```cpp
TPointer<StringReference> retV(TMemory<StringReference>::newMemory());
retV->init(expectedLength);
retV->concatenateX("abc");   // in place: only on a reference you created and have not shared
return retV;                 // converts to String
```

## UTF

```cpp
StringUTF16 w = UTF::utf16FromUTF8(s);          // all 6 directions: utf{8,16,32}FromUTF{8,16,32}
String back  = UTF::utf8FromUTF16(w);
StringUTF32 c = TUTFConvert<utf32, utf8>::from(s);
size_t cps   = UTF::utf32FromUTF8Length(s);     // ...Length twins: output length, no allocation
UTF::utf8FromUTF16(bad, "[bad]");               // custom error marker (default "?", "" drops)
```

- Inputs are 0 terminated pointers (`TString` converts implicitly).
- **Strict**: overlong UTF-8, surrogates, > U+10FFFF, U+FFFF, lone /
  missing continuation bytes, unpaired UTF-16 surrogates are invalid. Each
  invalid **input element** becomes the marker (`"A\xE2\x82"` -> `"A??"`);
  following valid data is kept.
- Validity check: `utf32FromUTF8Length(s, StringUTF32Core::empty) ==
  utf32FromUTF8Length(s)`.
- Per element: `UTF8Core::elementSize(lead)` (0 = invalid lead),
  `UTF8Core::elementIsValid(p)`, `UTF::elementUTF32FromUTF8(&cp, p)` (1 / 0),
  `UTF::elementUTF8FromUTF32(out, cp)` (bytes, 0 = invalid),
  `UTF::elementUTF16FromUTF32(out, cp)`, `UTF::elementUTF32FromUTF16(&cp,
  p16)`, `UTF16Core::elementSize(u)` (2 = high surrogate),
  `UTF32Core::elementIsValid(cp)`.
- UTF-16 / 32 in native (little endian) order, no BOM handling.
- Windows: `UTF::utf8FromUTF16(reinterpret_cast<const utf16 *>(L"..."))`,
  `reinterpret_cast<const wchar_t *>(UTF::utf16FromUTF8(s).value())` for W
  APIs. Linux `wchar_t` is 32 bit: use utf32.

Streams (`IRead` / `IWrite` from xyo-data-structures):

```cpp
UTF8Read reader;  reader.open(&utf16Input, UTFStreamMode::UTF16);  reader.read(buf, n);  reader.close();
UTF8Write writer; writer.open(&utf32Output, UTFStreamMode::UTF32); writer.write(s.value(), s.length()); writer.close();
```

Modes `None` / `UTF8` pass through. `read` returns UTF-8 bytes, keeps a
split code point for the next call, stops early (possibly 0) on an invalid
element, continuing after it next call. `write` buffers sequences split
across calls; a byte that cannot start / continue a sequence is refused
(return count before it, state reset). `open` keeps a reference, `close`
releases it; neither closes the inner stream.

## Base16 / Base32 / Base64

```cpp
String e = Base64::encode(data);                 // length() based, binary safe
String d;
if (!Base64::decode(e, d)) { /* invalid, d unchanged */ }
```

Strict decoders: Base16 accepts both cases, even length; Base32 RFC 4648
uppercase only, length % 8 == 0, correct `=` padding; Base64 standard
`+/` alphabet, length % 4 == 0, padding required, **no whitespace / newlines,
no URL safe `-_`**. Normalize first (`replace("\n", "")`, map `-_` to `+/`
and pad, `toUpperCaseASCII()` for Base32). Encoders: Base16 uppercase,
Base32 / 64 padded.

## Helpers

- `THex<T>::encodeUppercase(n)` / `encodeLowercase(n)` (nibble to digit),
  `isValid(c)`, `decode(c)` (0 if invalid).
- `UConvert::u32LeftRotate / u32RightRotate / u64LeftRotate /
  u64RightRotate` (any count), `u32FromU8 / u64FromU8 / u32ToU8 / u64ToU8`
  (little endian), `...Reversed` (big endian / network order). Byte by
  byte, no alignment needed.
- `NumberX::countDigits(n)`, `NumberX::leftPadByDigits(7, 1000)` ->
  `"0007"`.

## Using it

- `#include <XYO/Encoding.hpp>` (umbrella). C++17.
- fabricare consumer: `"dependency": ["xyo-encoding"]` (DLL) or
  `["xyo-encoding.static"]` (exports `XYO_ENCODING_LIBRARY`). Install
  `xyo-platform`, `xyo-managed-memory`, `xyo-data-structures`, then this
  library to the SDK before building dependents (see the `fabricare`
  skill).
- Without fabricare: compile `Platform.Amalgam.cpp`,
  `ManagedMemory.Amalgam.cpp`, `DataStructures.Amalgam.cpp`,
  `Encoding.Amalgam.cpp` with `-DXYO_PLATFORM_LIBRARY
  -DXYO_MANAGEDMEMORY_LIBRARY -DXYO_DATASTRUCTURES_LIBRARY
  -DXYO_ENCODING_LIBRARY` (MSVC: also `/DXYO_PLATFORM_COMPILE_STATIC`), the
  four `source/` dirs on the include path, `-pthread` on Linux. Or link the
  SDK `*.static.lib` files with `/MT`.

## Code style (match the repository)

- Tabs (width 8), `.clang-format` in the repo, CRLF line endings; statements
  and blocks end with `};`.
- camelCase methods, `retV` for return values, trailing `_` for members
  (`value_`, `length_`) and internal helpers (`encodeC_`, `deleteMemory_`).
- Headers: include guard `XYO_ENCODING_<NAME>_HPP`, guarded includes
  (`#ifndef XYO_ENCODING_DEPENDENCY_HPP #include ...`). Add new headers to
  `source/XYO/Encoding.hpp` and new `.cpp` files to
  `source/XYO/Encoding.Amalgam.cpp`. Exported functions use
  `XYO_ENCODING_EXPORT`. Free functions live in a sub-namespace
  (`XYO::Encoding::Base64`).
- Build results through `TPointer<StringReference>` + `init(capacity)` +
  `concatenateX`; decoders return `bool` and assign `out` only on success.
- SPDX header: MIT for `source/`, Unlicense for `test/`.
- Tests: `test/test.NN.cpp` with the `CHECK` macro pattern, plus a
  `"category": "test"` project in `fabricare.json`; run `fabricare make`
  then `fabricare test`.
