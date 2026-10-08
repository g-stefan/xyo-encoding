# Getting started

## 1. Build and install

The library is built with [fabricare](https://github.com/g-stefan/fabricare),
the build tool used by all XYO C++ projects. `xyo-platform`,
`xyo-managed-memory` and `xyo-data-structures` must be installed to the SDK
first. From the repository root:

```bash
fabricare make       # build into output/
fabricare test       # build and run test/test.*.cpp (run make first)
fabricare install    # copy output/{bin,include,lib} to ~/.fabricare/<platform>
fabricare clean      # remove output/ and temp/
```

Two libraries are produced:

| Project                | Kind                                | Use it when                               |
|------------------------|-------------------------------------|-------------------------------------------|
| `xyo-encoding`         | DLL / shared library (`dll-or-lib`) | default, shared between several libraries |
| `xyo-encoding.static`  | static library, static CRT          | self-contained executables                |

`TString`, `TStringCore`, `THex`, `UConvert` and `TUTFConvert` are header
only. The compiled part is the UTF conversion functions, the UTF streams,
`Base16` / `Base32` / `Base64`, `NumberX` and the version / copyright /
license functions.

## 2. Depend on it from another fabricare project

In the consumer's `fabricare.json`:

```json
{
	"name": "my-library",
	"make": "dll-or-lib",
	"sourcePath": "XYO/MyLibrary",
	"dependency": [
		"xyo-encoding"
	]
}
```

For the static variant use `"xyo-encoding.static"`. It exports
`XYO_ENCODING_LIBRARY` to the consumer (`dependencyDefines`), which turns
`XYO_ENCODING_EXPORT` into nothing. `xyo-data-structures`,
`xyo-managed-memory` and `xyo-platform` come in as transitive dependencies.

## 3. Include

```cpp
#include <XYO/Encoding.hpp>
```

The umbrella header pulls in `<XYO/DataStructures.hpp>` (and through it the
managed memory and platform headers) and every public header of this
library.

Namespace `XYO::Encoding` contains `using namespace XYO::ManagedMemory;` and
`using namespace XYO::DataStructures;`, so `using namespace XYO::Encoding;`
also brings in `Object`, `TPointer`, `TMemory`, `TDynamicArray`,
`TRedBlackTree`, `IRead`, `IWrite`, ... The metadata namespaces then exist
four times (`Version`, `Copyright`, `License`): write
`XYO::Encoding::Version::version()` in full.

Libraries built on top usually re-export the names in their own
`Dependency.hpp`:

```cpp
#include <XYO/Encoding.hpp>

namespace XYO::MyLibrary {
	using namespace XYO::Encoding;
};
```

## 4. First program

Parse `key=value` lines into an ordered map, then show a value in Base64 and
as UTF-16.

```cpp
#include <XYO/Encoding.hpp>

using namespace XYO::Encoding;

int main(int, char *[]) {
	String config("name = J\xC3\xBCrgen\nrole=admin\n\ncity= Ia\xC8\x99i \n");

	TDynamicArray<String> lines;
	config.explode("\n", lines);

	TRedBlackTree<String, String> values; // String works as a key
	for (size_t k = 0; k < lines.length(); ++k) {
		String key;
		String value;
		if (lines[k].split2("=", key, value)) {
			values.set(key.trimASCII(), value.trimASCII());
		};
	};

	for (auto *node = values.begin(); node; node = node->successor()) {
		printf("%s -> %s\n", node->key.value(), node->value.value()); // city, name, role
	};

	String name;
	if (values.get("name", name)) {
		printf("base64: %s\n", Base64::encode(name).value()); // SsO8cmdlbg==

		StringUTF16 wide = UTF::utf16FromUTF8(name);
		printf("utf-16 elements: %zu, utf-8 bytes: %zu\n", wide.length(), name.length()); // 6, 7
	};

	String copy = name;  // shares the buffer
	copy += "!";         // copy on write, name is unchanged
	printf("%s / %s\n", name.value(), copy.value());

	return 0;
};
```

Things to notice, all explained in [Strings](strings.md):

- pass a `String` to `printf` with `.value()`, never the object itself;
- `explode`, `split2`, `trimASCII` return new strings, the source never
  changes;
- `String copy = name` shares memory; the first `+=` gives `copy` its own
  buffer.

As a fabricare test project:

```json
{
	"name": "test.05",
	"make": "exe",
	"category": "test",
	"SPDX-License-Identifier": "Unlicense",
	"dependency": [
		"xyo-encoding"
	]
}
```

with the source in `test/test.05.cpp`.

## 5. Threads

Everything in `xyo-managed-memory` about threads applies here:

- create threads and move data between them with **`xyo-multithreading`**;
- the main thread must use the library before any thread starts. Call
  `XYO::ManagedMemory::Registry::registryInit()` at the start of `main` when
  in doubt;
- **a `String` belongs to the thread that created it.** Its reference count
  is a plain integer and its buffer comes from that thread's pool. Do not
  copy a `String`, or release one, on another thread. To hand text to
  another thread, pass the characters (a `std::string`, or a
  `TMemorySystem` allocated object holding a buffer) and build a new
  `String` on the receiving thread.

The free functions (`UTF::`, `Base16::`, `Base32::`, `Base64::`,
`NumberX::`, `UConvert::`, `THex`, `TStringCore`) have no shared state and
can be called from any thread on that thread's own strings.

## 6. Building without fabricare

Compile the four amalgams with your sources:

1. Put `source/` of `xyo-platform`, `xyo-managed-memory`,
   `xyo-data-structures` and `xyo-encoding` on the include path.
2. Provide the configuration headers of `xyo-platform` and
   `xyo-managed-memory` (see their documentation; for the default
   configuration `XYO/ManagedMemory/Config.hpp` is a copy of
   `Config.Template.hpp`).
3. Compile `Platform.Amalgam.cpp`, `ManagedMemory.Amalgam.cpp`,
   `DataStructures.Amalgam.cpp` and `Encoding.Amalgam.cpp` together with
   your sources, and define `XYO_PLATFORM_LIBRARY`,
   `XYO_MANAGEDMEMORY_LIBRARY`, `XYO_DATASTRUCTURES_LIBRARY` and
   `XYO_ENCODING_LIBRARY` everywhere (plain static linking).
4. Link `pthread` on Linux.

Example on Linux, with the repositories side by side:

```bash
g++ -std=c++17 \
    -Ixyo-platform/source -Ixyo-managed-memory/source -Ixyo-data-structures/source -Ixyo-encoding/source \
    -DXYO_PLATFORM_LIBRARY -DXYO_MANAGEDMEMORY_LIBRARY -DXYO_DATASTRUCTURES_LIBRARY -DXYO_ENCODING_LIBRARY \
    xyo-platform/source/XYO/Platform.Amalgam.cpp \
    xyo-managed-memory/source/XYO/ManagedMemory.Amalgam.cpp \
    xyo-data-structures/source/XYO/DataStructures.Amalgam.cpp \
    xyo-encoding/source/XYO/Encoding.Amalgam.cpp \
    main.cpp -o main -pthread
```

On Windows with MSVC, also define `XYO_PLATFORM_COMPILE_STATIC`:

```bat
cl /EHsc /std:c++17 /Ixyo-platform\source /Ixyo-managed-memory\source /Ixyo-data-structures\source /Ixyo-encoding\source ^
   /DXYO_PLATFORM_COMPILE_STATIC /DXYO_PLATFORM_LIBRARY /DXYO_MANAGEDMEMORY_LIBRARY /DXYO_DATASTRUCTURES_LIBRARY /DXYO_ENCODING_LIBRARY ^
   xyo-platform\source\XYO\Platform.Amalgam.cpp ^
   xyo-managed-memory\source\XYO\ManagedMemory.Amalgam.cpp ^
   xyo-data-structures\source\XYO\DataStructures.Amalgam.cpp ^
   xyo-encoding\source\XYO\Encoding.Amalgam.cpp ^
   main.cpp
```

With an installed SDK (`~/.fabricare/<platform>`) you can instead compile
only your sources against `include/` and link the `*.static` libraries:
`xyo-encoding.static`, `xyo-data-structures.static`,
`xyo-managed-memory.static`, `xyo-platform.static` (static CRT, `/MT`).
