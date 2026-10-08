# Strings

`TString<T>` is the string class of the XYO stack. `T` is the element type:
`char` (`String`, also named `StringUTF8`), `utf16` (`StringUTF16`) or
`utf32` (`StringUTF32`). Everything on this page applies to all three.

```cpp
typedef TString<char>  String;        // String.hpp
typedef TString<utf8>  StringUTF8;    // same type, utf8 is char
typedef TString<utf16> StringUTF16;   // UTF.hpp
typedef TString<utf32> StringUTF32;
```

## How it works

```
String a ──┐
String b ──┼──> TStringReference<char>  (Object, reference counted, pooled)
String c ──┘       value_  ──> "hello\0"   buffer: pool bucket 32..256 elements, or new[]
                   length_ = 5
```

- A `TString` holds one `TPointer<TStringReference<T>>`. The reference owns
  the buffer, its `length()` and its capacity.
- **Copying a `TString` shares the reference** (copy constructor,
  assignment, pass / return by value): one reference count increment, no
  character copy.
- **Every change is copy on write.** `+=`, `<<`, `concatenate` and
  `setElementAt` modify the buffer in place if this string is its only user
  (`hasOneReference()`); otherwise they give this string a new buffer and
  leave the other strings untouched. No public member can write into a
  buffer that another string still uses.
- Every other operation (`substring`, `replace`, `trimASCII`,
  `toUpperCaseASCII`, ...) **returns a new string** and never changes the
  source.
- Reading never copies: `operator[]`, `elementAt`, `value()`, `index()` are
  read only and keep the buffer shared.
- Buffers grow in chunks of 32 elements while they fit the pooled buckets
  (32, 64, ..., 256 elements), then geometrically (at least double), so
  repeated appends are amortized O(1).
- The buffer is always terminated by a `0` element, so `value()` is a valid
  C string.
- `TMemory<TString<T>>` and `TMemory<TStringReference<T>>` are active pools:
  a released string is recycled as an empty string.

A `TString` is an `Object` and a leaf: it never points to other objects, so
it can be a local, a by-value class member, a function parameter, a return
value or a container element without any `TPointerX` / `pointerLink` work,
and it can never be part of a cycle.

```cpp
class User : public Object {
		XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(User);

	public:
		String name;     // fine as a plain member
		String email;
};
```

## Creating strings

```cpp
String a;                         // ""
String b("hello");                // from a C string
String c("hello", 3);             // "hel", from a pointer and a length (may contain 0 bytes)
String d(b);                      // shares b's buffer
String e = b + " world";          // new string
String f(1024);                   // EMPTY string with room for 1024 elements, not "1024"
String g(1024, 4096);             // empty, room for 1024, grows in chunks of 4096

a = "text";                       // safe even if the pointer is inside a's own buffer
a.set("abcdef", 3);               // "abc"
a.set(b, 2);                      // first 2 elements of b, the length is clamped to b.length()
a.empty();                        // back to ""
```

There is no constructor from a number: `String s(5)` reserves capacity. Use
`snprintf` into a buffer, `std::to_string(n).c_str()` or `NumberX` to format
numbers.

## Reading

```cpp
const char *p = s.value();       // C string, valid while s (or a copy) keeps the buffer
const char *q = s;                // same, implicit conversion
size_t n = s.length();            // elements, not code points
bool z = s.isEmpty();
char c = s[0];                    // operator[](int), by value, no bounds check
char d = s.elementAt(2);          // by value, no bounds check
const char *tail = s.index(2);    // pointer to element 2, read only
```

`operator[]` and `elementAt` return the element **by value**, they are read
only: `s[0] = 'x'` does not compile. Use `setElementAt` to change an element.

Pass `s.value()` to `printf` and other variadic functions; passing the
object itself is undefined behavior.

`length()` counts elements: bytes for `String`, 16-bit units for
`StringUTF16`. To count code points convert to `StringUTF32` and take its
length, or use `UTF::utf32FromUTF8Length`.

## Changing elements

```cpp
String a("xyz");
String b = a;                     // shared
b.setElementAt(0, 'Q');           // b gets its own buffer: a == "xyz", b == "Qyz"
b.setElementAt(1, 'R');           // b is the only user now, changed in place: "QRz"
b.setElementAt(9, '!');           // false, index >= length(), nothing changes
```

`bool setElementAt(size_t index, T element)` is copy on write like `+=`:
in place when the string is the only user of its buffer, otherwise it first
copies the buffer (binary data and growth chunk included). It returns
`false` and changes nothing when `index >= length()`; it never changes the
length.

Changing many elements of a shared string costs one copy, on the first
call; the next calls are in place. To build a new text element by element,
prefer appending to a reserved string (`String out(n); out += c;`).

## Appending

```cpp
String s("a");
s += "bc";                        // const T *
s += other;                       // TString, binary safe
s += 'd';                         // one element
s << "e" << other << 'f';         // chained append, same rules as +=
s.concatenate("xyz", 2);          // pointer and length
s.concatenate(other, 4);          // first 4 elements of other (clamped)
String t = s + "!";               // new string, s unchanged
```

Appending a string to itself (`s += s`, `s << s`) or a pointer into its own
buffer (`s += s.index(3)`) is handled.

For many appends to one string, reserve once: `String out(expectedLength);`
then `+=` in a loop. While `out` is not shared, every append is in place.

## Comparing

```cpp
s == "abc";   s != other;   s < other;   s >= "m";   // all operators, against TString or const T *
int r = s.compare(other);                           // < 0, 0, > 0
StringCore::compareIgnoreCaseASCII(s, "ABC") == 0;  // case insensitive, ASCII letters only
```

Elements are compared as **unsigned** values, like `strcmp` (`"\xC3"` is
greater than `"a"`). For UTF-8 this is the same as code point order.

Strings work as keys in the XYO containers, `TComparator<TString<T>>` is
specialized:

```cpp
TRedBlackTree<String, int> count;
count.set("apple", 1);
TRedBlackTreeOne<String> seen;
seen.set("apple");
TAssociativeArray<String, String> headers;
headers.set("Content-Type", "text/html");
```

## Searching

```cpp
size_t index;
if (s.indexOf("needle", 0, index)) { ... }       // first match at or after position 0
if (s.indexOfFromEnd(".", 0, index)) { ... }      // last match
s.itContains("needle");
s.beginWith("http://");
s.endsWith(".txt");
s.matchASCII("*.TXT");                            // wildcards, case insensitive
```

- `indexOf(x, start, index)`: first match at a position `>= start`.
- `indexOfFromEnd(x, start, index)`: last match that ends at least `start`
  elements before the end (`"a.b.c".indexOfFromEnd(".", 0, i)` gives 3,
  with `start = 2` it gives 1).
- `matchASCII(pattern)`: `*` matches any run of elements, `?` matches one
  element, other elements match ignoring ASCII case. `"Hello.txt"` matches
  `"*.TXT"`, `"hello world"` matches `"h?llo*"`.

For case insensitive search use the static
`TStringCore<T>::indexOfIgnoreCaseASCII(x, xLn, y, yLn, start, index)`.

## Transforming

All of these return a new string.

```cpp
s.substring(start, length);       // clamped to the string; "" if start is past the end
s.substring(start);               // to the end
s.replace("-", "+");              // every occurrence; an empty pattern returns s
s.trimASCII();                    // remove space, tab, CR, LF at both ends
s.trimWithElement("/\\");         // remove any of these elements at both ends
s.toLowerCaseASCII();             // A-Z only, other bytes unchanged
s.toUpperCaseASCII();
s.nilAtFirst("/");                // "path/to/file" -> "path"     (part before the first "/")
s.nilAtFirstFromEnd("/");         // "path/to/file" -> "path/to"  (part before the last "/")
s.encodeC();                      // C string literal, with quotes: "say \"hi\"\n" style
s.encodeCX();                     // the same, without the surrounding quotes
```

`encodeC` escapes `\`, `"`, newline, CR, tab, and writes every other
element outside 0x20..0x7E as `\xHH` (`\xHHHH` / `\xHHHHHHHH` for wide
elements). A hex digit right after a `\x` escape is escaped too, so the
result reads back exactly. `'` is not escaped.

## Splitting and joining

```cpp
String key, value;
if (line.split2("=", key, value)) { ... }          // at the first "=", false if not found
path.split2FromEnd(".", baseName, extension);      // at the last "."

TDynamicArray<String> parts;
csv.explode(",", parts);                           // appends to parts
String joined = String::implode(", ", parts);
```

- `split2` / `split2FromEnd` return `false` when the separator is not found;
  then `first` is the whole string and `second` is empty.
- `explode` **appends** to the array (clear it first to reuse it), returns
  `false` for an empty delimiter. Empty parts are kept, except a trailing
  one: `"a,,b"` gives 3 parts, `",a"` gives `""` and `"a"`, `"a,b,"` gives 2
  parts, `""` gives none.

## Binary data

A `String` can hold any bytes, `0` included, when it is built with a length
and appended to with other strings or lengths:

```cpp
String data(buffer, size);        // copies size bytes
data += other;                    // uses other.length()
data.concatenate(buffer, size);
Base64::encode(data);             // Base16 / 32 / 64 use length()
```

Everything that works on C strings **stops at the first 0 element**: the
comparison operators and `compare`, `+= const T *`, `replace`, `indexOf`,
`trim*`, `to*CaseASCII`, `matchASCII`, `encodeC`. Two binary strings that
differ after a `0` byte compare as equal. Compare binary data with
`length()` and `memcmp(a.value(), b.value(), a.length())`.

## Pitfalls

- **`String s(5)` is an empty string**, not `"5"`.
- **No bounds checks** on `[]`, `elementAt`, `index`. `substring`, `set`
  and `concatenate` with a `TString` clamp; the pointer versions trust the
  length you give. `setElementAt` checks the index.
- **`value()` points into a shared buffer.** It stays valid while any string
  holding that buffer is alive and nobody changes it in place (`+=`,
  `setElementAt` on its only user). Do not keep it after the string is
  released or modified.
- **Do not cast away `const`.** `value()`, `index()` and `reference()` are
  read only views of a buffer that may be shared; writing through them with
  `const_cast` changes every string that shares it.
- **One thread per string.** See
  [Getting started](getting-started.md#5-threads).
- **ASCII only case functions.** `toUpperCaseASCII`, `compareIgnoreCaseASCII`,
  `matchASCII` do not know Unicode case rules; non ASCII bytes pass through
  unchanged.

## TStringCore and TMemoryCore

The algorithms behind `TString` are available as static functions on plain
`const T *` C strings, for code that works on buffers it owns:

```cpp
size_t n = StringCore::length("abc");                 // strlen for char
StringCore::copyN(buffer, source, 10);                 // always terminates (strncpy does not)
StringCore::compareIgnoreCaseASCII(a, b);
StringCore::indexOf(text, "needle", index);
StringCore::endsWith(fileName, ".txt");
StringCore::matchASCII(fileName, "*.txt");
const char *p = StringCore::toNotInElement(" \tabc", " \t");   // first element not in the set

StringUTF16Core::length(wide);                         // the same for utf16 / utf32
```

`TMemoryCore<T>` has `copyN`, `compareN` and `setN` on element arrays
(`memcpy` / `memcmp` / `memset` for `char`). `TStringCore<T>::empty` is a
static empty C string of each element type.

`TStringReference<T>` is the shared buffer itself. You normally do not use
it; the library uses it to build results without extra copies:

```cpp
TPointer<StringReference> retV(TMemory<StringReference>::newMemory());
retV->init(expectedLength);       // empty, with capacity
retV->concatenateX("abc");        // in place append, X = no copy on write check
return retV;                      // converts to String
```

Only write to a `TStringReference` you just created and have not shared
yet. `TString::reference()` returns a `const TStringReference<T> *`: the
buffer of an existing string can be read through it (`value()`, `length()`,
`chunk()`), not changed.
