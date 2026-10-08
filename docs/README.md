# XYO Encoding — Documentation

`xyo-encoding` is the text layer of the XYO C++ library stack. It sits on top
of `xyo-data-structures` and gives every library above it (`xyo-system`,
`xyo-cryptography`, `xyo-networking`, `quantum-script`, ...) **one string type
and the encodings around it**:

- **A managed, shared string.** `TString<T>` (`String` for `char`) is a small
  value type that points to a reference counted, pooled buffer. Copies share
  the buffer; appending copies it only when it is shared (copy on write).
  Small buffers (up to 256 elements) come from per-thread pools, so building
  strings does not hit the system allocator.
- **Three Unicode encodings, one template.** `utf8` (`char`), `utf16`
  (`uint16_t`) and `utf32` (`uint32_t`) strings share the same `TString` code
  (`StringUTF8` / `StringUTF16` / `StringUTF32`).
- **Strict UTF conversion.** `UTF::utf8FromUTF16`, `utf16FromUTF8`, ... follow
  RFC 3629: overlong forms, surrogates and code points above U+10FFFF are
  rejected and replaced by an error marker (`"?"` by default), never decoded.
- **UTF streams.** `UTF8Read` / `UTF8Write` sit on any `IRead` / `IWrite` and
  turn UTF-16 or UTF-32 data into UTF-8 on the fly, and back.
- **Binary to text.** `Base16` (hex), `Base32` and `Base64` (RFC 4648), with
  strict decoders that never read past the input and leave the output
  untouched on error.
- **Small helpers.** `THex` (hex digits), `UConvert` (bit rotation,
  little / big endian loads and stores), `NumberX` (zero padded numbers),
  `TStringCore` / `TMemoryCore` (C string and memory algorithms for any
  element type).

```
quantum-script, applications ...
xyo-system, xyo-cryptography, xyo-networking, ...
xyo-encoding          <-- this library
xyo-data-structures   (containers, IRead / IWrite, DynamicObject)
xyo-managed-memory    (Object, TPointer, TPointerX, pools)
xyo-platform          (macros, TAtomic, CriticalSection, Thread)
```

## Why use it

- **It is the string of the XYO stack.** Every library above takes and
  returns `String`; file names, JSON, HTTP headers, script values are all
  `String`.
- **Cheap to pass around.** Copying a `String` is a reference count
  increment. Returning one by value costs nothing more.
- **Works with the managed memory model.** `String` is an `Object`, has an
  active pool, and is a leaf (it never points to other objects), so it can be
  a local, a class member or a container element without creating cycles.
- **Ready for containers.** `TComparator<TString<T>>` is specialized, so
  `TRedBlackTree<String, V>`, `TRedBlackTreeOne<String>`,
  `TAssociativeArray<String, V>` and `TDynamicArray<String>` work as they are.
- **Safe on bad input.** UTF and Base decoders validate everything and report
  errors through the result (`bool`, or a replacement marker) instead of
  exceptions or undefined behavior.

## Concepts at a glance

| Need | Use | Notes |
|------|-----|-------|
| A text string | `String` (`TString<char>`) | UTF-8 by convention, copy on write |
| UTF-16 / UTF-32 text | `StringUTF16`, `StringUTF32` | `utf16` = `uint16_t`, `utf32` = `uint32_t`, native (little endian) order |
| Convert between encodings | `UTF::utf8FromUTF16`, `UTF::utf16FromUTF8`, ... or `TUTFConvert<To, From>::from` | invalid input becomes `"?"` (or your marker) |
| Read UTF-16 / UTF-32 data as UTF-8 | `UTF8Read` over an `IRead` | `UTFStreamMode::UTF16` / `UTF32` |
| Write UTF-8 as UTF-16 / UTF-32 | `UTF8Write` over an `IWrite` | partial sequences are buffered between writes |
| Hex / Base32 / Base64 | `Base16::`, `Base32::`, `Base64::` `encode` / `decode` | `decode` returns `false` on invalid input |
| Byte order, rotations | `UConvert::u32FromU8`, `u32ToU8Reversed`, `u32LeftRotate`, ... | `Reversed` = big endian |
| Hex digit encode / decode | `THex<T>` | per nibble |
| `"007"` style numbering | `NumberX::leftPadByDigits(7, 100)` | |
| C string algorithms for any element type | `TStringCore<T>` (`StringCore`, `StringUTF16Core`, ...) | static functions on `const T *` |

## Contents

| Document | What it covers |
|----------|----------------|
| [Getting started](getting-started.md) | Build it, depend on it (DLL or static), first program, threads, building without fabricare |
| [Strings](strings.md) | `TString` / `String`: the sharing model, every operation, binary data, pitfalls, `TStringCore` |
| [UTF](utf.md) | UTF-8 / 16 / 32 types and strings, conversion, error handling, `wchar_t`, `UTF8Read` / `UTF8Write` |
| [Binary to text and helpers](binary-to-text.md) | `Base16`, `Base32`, `Base64`, `THex`, `UConvert`, `NumberX` |
| [API reference](reference.md) | Every public symbol on one page |

The memory model (`Object`, `TPointer`, pools, threads) is documented in the
`xyo-managed-memory` repository, `docs/`; the containers and the `IRead` /
`IWrite` interfaces in the `xyo-data-structures` repository, `docs/`.

## Source map

```
source/XYO/Encoding.hpp                  umbrella header, include this
source/XYO/Encoding.Amalgam.cpp          compiled part of the library in one translation unit
source/XYO/Encoding/
    Dependency.hpp                       xyo-data-structures, export macros
    TMemoryCore[-Char].hpp               copyN / compareN / setN on element arrays (memcpy for char)
    TStringCore[-String-Char].hpp        C string algorithms (libc for char)
    TStringReference.hpp                 the shared, pooled buffer behind a TString
    TString.hpp                          the string class
    TComparator-TString.hpp              TComparator specialization, strings as container keys
    String.hpp                           String, StringCore, StringReference (char)
    THex.hpp                             hex digit encode / decode
    UConvert.hpp                         rotations, little / big endian byte conversion
    UTF8Core / UTF16Core / UTF32Core     element size and validation per encoding
    UTF[.cpp]                            utf8 / utf16 / utf32 string types and conversion
    TUTFConvert.hpp                      template front end for UTF conversion
    UTFStream[.cpp]                      UTF8Read / UTF8Write
    Base16 / Base32 / Base64[.cpp]       binary to text
    NumberX[.cpp]                        countDigits, leftPadByDigits
    Copyright / License / Version        library metadata
test/test.01.cpp                         Base16 / 32 / 64 of a string, smoke test
test/test.02.cpp                         TString: self append, ordering, replace, encodeC, trim, split, explode
test/test.03.cpp                         UTF: round trips, strict UTF-8, invalid input, streams
test/test.04.cpp                         Base16 / 32 / 64 vectors and rejection, UConvert, NumberX
```

## AI assistant skill

A Claude Code skill describing how to use this library lives in
[`.claude/skills/xyo-encoding/`](../.claude/skills/xyo-encoding/SKILL.md).
It is picked up automatically inside this repository; copy the folder to
`~/.claude/skills/` to have it available in the projects that depend on
`xyo-encoding`.
