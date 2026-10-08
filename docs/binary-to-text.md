# Binary to text and helpers

## Base16, Base32, Base64

The three encoders have the same shape:

```cpp
String  Base64::encode(const String &toEncode);
bool    Base64::decode(const String &toDecode, String &out);
```

```cpp
String data(buffer, size);                    // any bytes, 0 included
String text = Base64::encode(data);           // "Zm9vYmFy" style

String decoded;
if (!Base64::decode(text, decoded)) {
	// invalid input, decoded is unchanged
};
// decoded.length() == size, compare with memcmp, not ==
```

- `encode` uses `length()`, so binary data with `0` bytes is fine.
- `decode` returns `false` on any invalid input and then **does not touch
  `out`**. It never reads past the end of the input.
- The decoded `String` is binary: compare it by `length()` and `memcmp`
  (`==` stops at the first 0 byte, see [Strings](strings.md#binary-data)).

| | Base16 | Base32 | Base64 |
|-|--------|--------|--------|
| Standard | hex | RFC 4648, standard alphabet `A-Z 2-7` | RFC 4648, standard alphabet `A-Z a-z 0-9 + /` |
| `encode` output | uppercase, 2 characters per byte | groups of 8, padded with `=` | groups of 4, padded with `=` |
| `decode` accepts | upper and lower case, even length | uppercase only, length a multiple of 8, correct padding | length a multiple of 4, correct padding |
| `decode` rejects | odd length, non hex characters, `0x` prefix, spaces | lowercase, missing or wrong padding, data after `=`, padding before the last group | missing padding, data after `=`, padding before the last group, whitespace / newlines, URL safe `-` `_` |

The decoders are strict on purpose. If you receive text from a lenient
source, normalize it first:

```cpp
// Base64 split into lines (MIME, PEM): remove the line breaks
String flat = pem.replace("\r", "").replace("\n", "");

// URL safe Base64 without padding (JWT): map the alphabet and pad
String b64 = token.replace("-", "+").replace("_", "/");
while (b64.length() % 4) {
	b64 += "=";
};

// lowercase Base32
String upper = text.toUpperCaseASCII();
```

## THex

Hex digits, one nibble at a time, for any element type:

```cpp
char hi = THex<char>::encodeUppercase((byte >> 4) & 0x0F);   // '0'..'9', 'A'..'F'
char lo = THex<char>::encodeLowercase(byte & 0x0F);          // '0'..'9', 'a'..'f'
THex<char>::isValid('e');                                     // true
uint8_t v = THex<char>::decode('E');                          // 14; 0 for an invalid digit, check isValid first
```

## UConvert

Bit rotation and byte order, for hashes, ciphers and binary formats:

```cpp
uint32_t r = UConvert::u32LeftRotate(x, 7);      // also u32RightRotate, u64LeftRotate, u64RightRotate
                                                  // any count, 0 and the full width included

uint8_t buffer[8];
UConvert::u32ToU8(value, buffer);                 // little endian store
uint32_t a = UConvert::u32FromU8(buffer);         // little endian load
UConvert::u32ToU8Reversed(value, buffer);         // big endian (network order) store
uint32_t b = UConvert::u32FromU8Reversed(buffer); // big endian load
// the same for 64 bits: u64ToU8, u64FromU8, u64ToU8Reversed, u64FromU8Reversed
```

The loads and stores work byte by byte, so the buffer needs no alignment and
the result does not depend on the machine's byte order.

## NumberX

```cpp
NumberX::countDigits(12345);            // 5; countDigits(0) == 1, the sign is not counted
NumberX::leftPadByDigits(7, 1000);      // "0007", padded to the digits of the second number
NumberX::leftPadByDigits(42, 9);        // "42", never truncated
```

Useful for file names that must sort correctly:

```cpp
for (int k = 0; k < count; ++k) {
	String fileName = String("frame-") + NumberX::leftPadByDigits(k, count) + ".png";
};
```
