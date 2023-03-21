// Encoding
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_ENCODING_TSTRINGX_HPP
#define XYO_ENCODING_TSTRINGX_HPP

#ifndef XYO_ENCODING_TSTRING_HPP
#	include <XYO/Encoding/TString.hpp>
#endif

#ifndef XYO_ENCODING_THEX_HPP
#	include <XYO/Encoding/THex.hpp>
#endif

namespace XYO::Encoding {

	using namespace XYO::DataStructures;

	template <typename T>
	struct TStringX {

			static inline TString<T> trimWithElement(const TString<T> &o, const TString<T> &x) {
				const T *x1;
				const T *x2;
				x1 = TStringCore<T>::toNotInElement(o, x);
				x2 = TStringCore<T>::toNotInFromEndElement(o, x);

				if (*x1 == 0) {
					return x1;
				};

				if (*x2 == 0) {
					return x2;
				};

				return TString<T>(x1, x2 - x1 + 1);
			};

			static inline TString<T> trimAscii(const TString<T> &o) {
				const T *x1;
				const T *x2;
				const T x[5] = {0x20, 0x09, 0x0D, 0x0A, 0x00};
				x1 = TStringCore<T>::toNotInElement(o, x);
				x2 = TStringCore<T>::toNotInFromEndElement(o, x);

				if (*x1 == 0) {
					return x1;
				};

				if (*x2 == 0) {
					return x2;
				};

				return TString<T>(x1, x2 - x1 + 1);
			};

			static inline TString<T> replace(const TString<T> &o, const TString<T> &x, const TString<T> &y) {
				TStringReference<T> *retV;
				size_t k, ln;
				size_t xLn;

				xLn = x.length();
				if (o.length() < xLn) {
					return const_cast<TString<T> &>(o);
				};

				retV = TMemory<TStringReference<T>>::newMemory();
				retV->init();

				ln = o.length() - xLn;

				k = 0;
				while (k < ln) {
					if (TStringCore<T>::compareN(o.index(k), x, xLn) == 0) {
						retV->concatenateX(y.reference());
						k += xLn;
						continue;
					} else {
						retV->concatenateX(o.elementAt(k));
						k++;
					};
				};

				if (TStringCore<T>::compareN(o.index(k), x, xLn) == 0) {
					retV->concatenateX(y.reference());
				} else {
					retV->concatenateX(o.index(k));
				};

				return retV;
			};

			static inline TString<T> nilAtFirst(const TString<T> &o, const TString<T> &x) {
				size_t idx;
				if (TStringCore<T>::indexOf(o, x, idx)) {
					return TString<T>(o.value(), idx);
				};
				return o;
			};

			static inline TString<T> nilAtFirstFromEnd(const TString<T> &o, const TString<T> &x) {
				size_t idx;
				if (TStringCore<T>::indexOfFromEnd(o, x, idx)) {
					return TString<T>(o.value(), idx);
				};
				return o;
			};

			static inline TString<T> substring(const TString<T> &o, size_t start, size_t length) {
				if (length == 0) {
					return TString<T>();
				};
				if (start > o.length()) {
					return TString<T>();
				};
				if (start + length > o.length()) {
					length = o.length() - start;
				};
				return TString<T>(o.index(start), length);
			};

			static inline TString<T> substring(const TString<T> &o, size_t start) {
				if (start > o.length()) {
					return TString<T>();
				};
				return TString<T>(o.index(start), o.length() - start);
			};

			static inline TString<T> toLowerCaseAscii(const TString<T> &value) {
				TStringReference<T> *retV = TMemory<TStringReference<T>>::newMemory();

				retV->init(value.length());

				size_t totalLn;
				const T *inx;
				T *outx;

				totalLn = 0;
				inx = value.value();
				outx = retV->value();
				while (*inx) {
					*outx = TStringCore<T>::elementToLowerCaseAscii(*inx);
					++outx;
					++inx;
					++totalLn;
				};
				*outx = 0;
				retV->setLength(totalLn);
				return retV;
			};

			static inline TString<T> toUpperCaseAscii(const TString<T> &value) {
				TStringReference<T> *retV = TMemory<TStringReference<T>>::newMemory();

				retV->init(value.length());

				size_t totalLn;
				const T *inx;
				T *outx;

				totalLn = 0;
				inx = value.value();
				outx = retV->value();
				while (*inx) {
					*outx = TStringCore<T>::elementToUpperCaseAscii(*inx);
					++outx;
					++inx;
					++totalLn;
				};
				*outx = 0;

				retV->setLength(totalLn);
				return retV;
			};

			static inline bool matchAscii(const TString<T> &text, const TString<T> &sig) {
				return TStringCore<T>::matchAscii(text, text.length(), sig, sig.length());
			};

			static inline bool split2(const TString<T> &text, const TString<T> &sig, TString<T> &firstPart, TString<T> &secondPart) {
				size_t index;
				if (TStringCore<T>::indexOf(text, text.length(), sig, sig.length(), 0, index)) {
					firstPart = substring(text, 0, index);
					secondPart = substring(text, index + sig.length());
					return true;
				};
				firstPart = text;
				secondPart = TStringCore<T>::empty;
				return false;
			};

			static inline bool split2FromEnd(const TString<T> &text, const TString<T> &sig, TString<T> &firstPart, TString<T> &secondPart) {
				size_t index;
				if (TStringCore<T>::indexOfFromEnd(text, text.length(), sig, sig.length(), 0, index)) {
					firstPart = substring(text, 0, index);
					secondPart = substring(text, index + sig.length());
					return true;
				};
				firstPart = text;
				secondPart = TStringCore<T>::empty;
				return false;
			};

			static void encodeC_(TStringReference<T> *retV, const TString<T> &in) {
				size_t k;
				const T *scan;
				scan = in.value();
				for (k = 0; k < in.length(); ++k, ++scan) {
					if (*scan == '\\') {
						retV->concatenateX('\\');
						retV->concatenateX('\\');
						continue;
					};
					if (*scan == '"') {
						retV->concatenateX('\\');
						retV->concatenateX('"');
						continue;
					};
					if (*scan == '\'') {
						retV->concatenateX('\'');
						continue;
					};
					if (*scan == '\n') {
						retV->concatenateX('\\');
						retV->concatenateX('n');
						continue;
					};
					if (*scan == '\r') {
						retV->concatenateX('\\');
						retV->concatenateX('r');
						continue;
					};
					if (*scan == '\t') {
						retV->concatenateX('\\');
						retV->concatenateX('t');
						continue;
					};
					if (*scan >= 0x20 && *scan <= 0x7E) {
						retV->concatenateX(*scan);
						continue;
					};
					retV->concatenateX('\\');
					retV->concatenateX('x');
					retV->concatenateX(THex<T>::encodeUppercase((*scan >> 4) & 0x0F));
					retV->concatenateX(THex<T>::encodeUppercase((*scan) & 0x0F));
				};
			};

			static inline TString<T> encodeC(const TString<T> &in) {
				TStringReference<T> *retV = TMemory<TStringReference<T>>::newMemory();
				retV->init();
				retV->concatenateX('"');
				encodeC_(retV, in);
				retV->concatenateX('"');
				return retV;
			};

			static inline TString<T> encodeCX(const TString<T> &in) {
				TStringReference<T> *retV = TMemory<TStringReference<T>>::newMemory();
				retV->init();
				encodeC_(retV, in);
				return retV;
			};

			static inline bool indexOf(const TString<T> &a, const TString<T> &b, size_t start, size_t &index) {
				return TStringCore<T>::indexOf(a, a.length(), b, b.length(), start, index);
			};

			static inline bool indexOfFromEnd(const TString<T> &a, const TString<T> &b, size_t start, size_t &index) {
				return TStringCore<T>::indexOfFromEnd(a, a.length(), b, b.length(), start, index);
			};

			static inline bool itContains(const TString<T> &a, const TString<T> &b) {
				size_t index = 0;
				return TStringCore<T>::indexOf(a, a.length(), b, b.length(), 0, index);
			};

			static inline bool beginWith(const TString<T> &a, const TString<T> &b) {
				return (TStringCore<T>::compareN(a, b, b.length()) == 0);
			};

			static inline bool endsWith(const TString<T> &a, const TString<T> &b) {
				if (b.length() > a.length()) {
					return false;
				};
				return (TStringCore<T>::compareN(a.index(a.length() - b.length()), b, b.length()) == 0);
			};

			static inline bool explode(const TString<T> &delimiter, const TString<T> &strToExplode, TDynamicArray<TString<T>> &out) {
				size_t index = 0;
				size_t indexNext = 0;
				if (delimiter.isEmpty()) {
					return false;
				};
				for (;;) {
					if (indexOf(strToExplode, delimiter, index, indexNext)) {
						out.push(substring(strToExplode, index, indexNext - index));
						index = indexNext + delimiter.length();
						continue;
					};
					break;
				};
				if (index < strToExplode.length()) {
					out.push(substring(strToExplode, index, strToExplode.length() - index));
				};
				return true;
			};

			static inline TString<T> implode(const TString<T> &delimiter, TDynamicArray<TString<T>> &in) {
				TString<T> retV;
				size_t k;
				if (in.length() < 1) {
					return retV;
				};
				retV = in[0];
				for (k = 1; k < in.length(); ++k) {
					retV += delimiter;
					retV += in[k];
				};
				return retV;
			};
	};
};

#endif
