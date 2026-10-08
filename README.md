# Encoding

C++ library
- `String`: the shared, copy on write string of the XYO libraries, for UTF-8, UTF-16 and UTF-32
(`TString<T>`), with search, split, replace, trim, wildcard match and C escaping.
- Strict UTF-8 / UTF-16 / UTF-32 conversion and UTF streams (`UTF8Read`, `UTF8Write`).
- Base16, Base32 and Base64 encoding with strict decoders; hex, byte order and bit rotation helpers.

Built on `xyo-data-structures`; used by `xyo-system`, `xyo-cryptography`,
`quantum-script` and the rest of the XYO C++ libraries.

## Documentation

- [Overview](docs/README.md) - purpose and design
- [Getting started](docs/getting-started.md) - build, depend on it, first program, threads
- [Strings](docs/strings.md) - `TString` / `String`: sharing model, operations, binary data, pitfalls
- [UTF](docs/utf.md) - `utf8` / `utf16` / `utf32`, conversion, invalid input, `wchar_t`, `UTF8Read` / `UTF8Write`
- [Binary to text and helpers](docs/binary-to-text.md) - `Base16`, `Base32`, `Base64`, `THex`, `UConvert`, `NumberX`
- [API reference](docs/reference.md)

A Claude Code skill for this library is in
[.claude/skills/xyo-encoding](.claude/skills/xyo-encoding/SKILL.md).

## License

Copyright (c) 2016-2026 Grigore Stefan
Licensed under the [MIT](LICENSE) license.
