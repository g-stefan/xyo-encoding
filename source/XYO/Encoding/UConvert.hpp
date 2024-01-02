// Encoding
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_ENCODING_UCONVERT_HPP
#define XYO_ENCODING_UCONVERT_HPP

#ifndef XYO_ENCODING_DEPENDENCY_HPP
#	include <XYO/Encoding/Dependency.hpp>
#endif

namespace XYO::Encoding::UConvert {

	inline uint32_t u32LeftRotate(uint32_t x, uint16_t c) {
		return (x << c) | (x >> (32 - c));
	};

	inline uint32_t u32RightRotate(uint32_t x, uint16_t c) {
		return (x >> c) | (x << (32 - c));
	};

	inline uint64_t u64LeftRotate(uint64_t x, uint16_t c) {
		return (x << c) | (x >> (64 - c));
	};

	inline uint64_t u64RightRotate(uint64_t x, uint16_t c) {
		return (x >> c) | (x << (64 - c));
	};

	inline uint32_t u32FromU8(const uint8_t *in) {
		uint32_t retV;
		retV = *(in + 3);
		retV <<= 8;
		retV |= *(in + 2);
		retV <<= 8;
		retV |= *(in + 1);
		retV <<= 8;
		retV |= *(in);
		return retV;
	};

	inline uint32_t u32FromU8Reversed(const uint8_t *in) {
		uint32_t retV;
		retV = *(in);
		retV <<= 8;
		retV |= *(in + 1);
		retV <<= 8;
		retV |= *(in + 2);
		retV <<= 8;
		retV |= *(in + 3);
		return retV;
	};

	inline uint64_t u64FromU8(const uint8_t *in) {
		uint64_t retV;
		retV = *(in + 7);
		retV <<= 8;
		retV |= *(in + 6);
		retV <<= 8;
		retV |= *(in + 5);
		retV <<= 8;
		retV |= *(in + 4);
		retV <<= 8;
		retV |= *(in + 3);
		retV <<= 8;
		retV |= *(in + 2);
		retV <<= 8;
		retV |= *(in + 1);
		retV <<= 8;
		retV |= *(in);
		return retV;
	};

	inline uint64_t u64FromU8Reversed(const uint8_t *in) {
		uint64_t retV;
		retV = *(in);
		retV <<= 8;
		retV |= *(in + 1);
		retV <<= 8;
		retV |= *(in + 2);
		retV <<= 8;
		retV |= *(in + 3);
		retV <<= 8;
		retV |= *(in + 4);
		retV <<= 8;
		retV |= *(in + 5);
		retV <<= 8;
		retV |= *(in + 6);
		retV <<= 8;
		retV |= *(in + 7);
		return retV;
	};

	inline void u32ToU8(uint32_t in, uint8_t *out) {
		out[0] = (uint8_t)((in)&0x00FF);
		in >>= 8;
		out[1] = (uint8_t)((in)&0x00FF);
		in >>= 8;
		out[2] = (uint8_t)((in)&0x00FF);
		in >>= 8;
		out[3] = (uint8_t)((in)&0x00FF);
	};

	inline void u32ToU8Reversed(uint32_t in, uint8_t *out) {
		out[3] = (uint8_t)((in)&0x00FF);
		in >>= 8;
		out[2] = (uint8_t)((in)&0x00FF);
		in >>= 8;
		out[1] = (uint8_t)((in)&0x00FF);
		in >>= 8;
		out[0] = (uint8_t)((in)&0x00FF);
	};

	inline void u64ToU8(uint64_t in, uint8_t *out) {
		out[0] = (uint8_t)((in)&0x00FF);
		in >>= 8;
		out[1] = (uint8_t)((in)&0x00FF);
		in >>= 8;
		out[2] = (uint8_t)((in)&0x00FF);
		in >>= 8;
		out[3] = (uint8_t)((in)&0x00FF);
		in >>= 8;
		out[4] = (uint8_t)((in)&0x00FF);
		in >>= 8;
		out[5] = (uint8_t)((in)&0x00FF);
		in >>= 8;
		out[6] = (uint8_t)((in)&0x00FF);
		in >>= 8;
		out[7] = (uint8_t)((in)&0x00FF);
	};

	inline void u64ToU8Reversed(uint64_t in, uint8_t *out) {
		out[7] = (uint8_t)((in)&0x00FF);
		in >>= 8;
		out[6] = (uint8_t)((in)&0x00FF);
		in >>= 8;
		out[5] = (uint8_t)((in)&0x00FF);
		in >>= 8;
		out[4] = (uint8_t)((in)&0x00FF);
		in >>= 8;
		out[3] = (uint8_t)((in)&0x00FF);
		in >>= 8;
		out[2] = (uint8_t)((in)&0x00FF);
		in >>= 8;
		out[1] = (uint8_t)((in)&0x00FF);
		in >>= 8;
		out[0] = (uint8_t)((in)&0x00FF);
	};

};

#endif
