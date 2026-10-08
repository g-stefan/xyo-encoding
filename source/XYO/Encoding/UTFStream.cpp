// Encoding
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/Encoding/UTFStream.hpp>

namespace XYO::Encoding {

	using namespace XYO::DataStructures;

	UTF8Read::UTF8Read() {
		input.pointerLink(this);
		mode = UTFStreamMode::None;
		index = 0;
		size = 0;
	};

	UTF8Read::~UTF8Read(){};

	bool UTF8Read::open(IRead *input_, int mode_) {
		input = input_;
		mode = mode_;
		index = 0;
		size = 0;
		return true;
	};

	size_t UTF8Read::read(void *output, size_t length) {
		switch (mode) {
		case UTFStreamMode::None:
		case UTFStreamMode::UTF8:
			return input->read(output, length);
		case UTFStreamMode::UTF16: {
			size_t readLn = 0;
			while (length > 0) {
				if (index == 0) {
					utf16 utf16Data[2];
					if (input->read(&utf16Data[0], sizeof(utf16)) != sizeof(utf16)) {
						return readLn;
					};
					size_t convertLn = 0;
					convertLn = UTF16Core::elementSize(utf16Data[0]);
					if (convertLn == 0) {
						return readLn;
					};
					if (convertLn == 2) {
						if (input->read(&utf16Data[1], sizeof(utf16)) != sizeof(utf16)) {
							return readLn;
						};
					};
					utf32 utf32Data;
					if (!UTF::elementUTF32FromUTF16(&utf32Data, &utf16Data[0])) {
						return readLn;
					};
					size = static_cast<int>(UTF::elementUTF8FromUTF32(&utf8Input[0], utf32Data));
					utf8Input[size] = 0;
				};
				((char *)output)[readLn] = utf8Input[index];
				index++;
				readLn++;
				--length;
				if (index >= size) {
					index = 0;
					size = 0;
				};
			};
			return readLn;
		};
		case UTFStreamMode::UTF32: {
			size_t readLn = 0;
			while (length > 0) {
				if (index == 0) {
					utf32 utf32Data;
					if (input->read(&utf32Data, sizeof(utf32)) != sizeof(utf32)) {
						return readLn;
					};
					size = static_cast<int>(UTF::elementUTF8FromUTF32(&utf8Input[0], utf32Data));
					if (size == 0) {
						// invalid code point, stop like the UTF16 mode does
						return readLn;
					};
					utf8Input[size] = 0;
				};
				((char *)output)[readLn] = utf8Input[index];
				index++;
				readLn++;
				--length;
				if (index >= size) {
					index = 0;
					size = 0;
				};
			};
			return readLn;
		};
		default:
			break;
		};
		return 0;
	};

	void UTF8Read::close() {
		input.deleteMemory();
	};

	// Buffer one byte of an UTF8 sequence, false if it can not continue or start one.
	// The byte is not consumed and the state is reset, so the next write starts clean.
	static inline bool utf8WriteBuffer_(char x, char *buffer, int &index, int &size) {
		if (index == 0) {
			size = static_cast<int>(UTF8Core::elementSize(x));
			if (size == 0) {
				return false;
			};
		} else if (!UTF8Core::check(x)) {
			index = 0;
			size = 0;
			return false;
		};
		buffer[index] = x;
		index++;
		return true;
	};

	UTF8Write::UTF8Write() {
		output.pointerLink(this);
		mode = UTFStreamMode::None;
		index = 0;
		size = 0;
	};

	UTF8Write::~UTF8Write(){};

	bool UTF8Write::open(IWrite *output_, int mode_) {
		output = output_;
		mode = mode_;
		index = 0;
		size = 0;
		return true;
	};

	size_t UTF8Write::write(const void *input, size_t length) {
		switch (mode) {
		case UTFStreamMode::None:
		case UTFStreamMode::UTF8:
			return output->write(input, length);
		case UTFStreamMode::UTF16: {
			size_t writeLn = 0;
			while (length > 0) {
				if (!utf8WriteBuffer_(((const char *)input)[writeLn], utf8Output, index, size)) {
					return writeLn;
				};
				writeLn++;
				--length;
				if (index >= size) {
					index = 0;
					size = 0;
					utf32 utf32Data;
					size_t convertLn = 0;
					convertLn = UTF::elementUTF32FromUTF8(&utf32Data, &utf8Output[0]);
					if (convertLn == 0) {
						return writeLn;
					};
					utf16 utf16Data[2];
					convertLn = UTF::elementUTF16FromUTF32(&utf16Data[0], utf32Data);
					if (convertLn == 0) {
						return writeLn;
					};
					if (output->write(&utf16Data[0], sizeof(utf16) * convertLn) != sizeof(utf16) * convertLn) {
						return writeLn;
					};
				};
			};
			return writeLn;
		};
		case UTFStreamMode::UTF32: {
			size_t writeLn = 0;
			while (length > 0) {
				if (!utf8WriteBuffer_(((const char *)input)[writeLn], utf8Output, index, size)) {
					return writeLn;
				};
				writeLn++;
				--length;
				if (index >= size) {
					index = 0;
					size = 0;
					utf32 utf32Data;
					size_t convertLn = 0;
					convertLn = UTF::elementUTF32FromUTF8(&utf32Data, &utf8Output[0]);
					if (convertLn == 0) {
						return writeLn;
					};
					if (output->write(&utf32Data, sizeof(utf32Data) * convertLn) != sizeof(utf32Data) * convertLn) {
						return writeLn;
					};
				};
			};
			return writeLn;
		};
		default:
			break;
		};
		return 0;
	};

	void UTF8Write::close() {
		output.deleteMemory();
	};

};
