// Encoding
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_ENCODING_TSTRING_HPP
#define XYO_ENCODING_TSTRING_HPP

#ifndef XYO_ENCODING_TSTRINGREFERENCE_HPP
#	include <XYO/Encoding/TStringReference.hpp>
#endif

#ifndef XYO_ENCODING_THEX_HPP
#	include <XYO/Encoding/THex.hpp>
#endif

namespace XYO::Encoding {

	using namespace XYO::DataStructures;

	template <typename T>
	class TString : public Object {
		protected:
			TPointer<TStringReference<T>> value_;

		public:
			inline TString() {
				value_.newMemory();
				value_->init();
			};

			inline ~TString(){};

			inline TString(const T *value) {
				value_.newMemory();
				value_->from(value);
			};

			inline TString(const T *value, size_t ln) {
				value_.newMemory();
				value_->from(value, ln);
			};

			inline TString(const TString &value) {
				value_ = (const_cast<TString &>(value)).value_;
			};

			inline TString(TString &&value) {
				value_ = std::move(value.value_);
			};

			inline TString(const TPointer<TStringReference<T>> &value) {
				value_ = value;
			};

			inline TString(TPointer<TStringReference<T>> &&value) {
				value_ = std::move(value);
			};

			inline TString(TStringReference<T> *value) {
				value_ = value;
			};

			inline TString(size_t length_) {
				value_.newMemory();
				value_->init(length_);
			};

			inline TString(size_t length_, size_t chunk_) {
				value_.newMemory();
				value_->init(length_, chunk_);
			};

			inline TString &operator=(const T *value) {
				value_.newMemory();
				value_->from(value);
				return *this;
			};

			inline TString &operator=(const TString &value) {
				value_ = (const_cast<TString &>(value)).value_;
				return *this;
			};

			inline TString &operator=(TString &&value) {
				value_ = std::move(value.value_);
				return *this;
			};

			inline TString &operator=(const TPointer<TStringReference<T>> &value) {
				value_ = value;
				return *this;
			};

			inline TString &operator=(TPointer<TStringReference<T>> &&value) {
				value_ = std::move(value);
				return *this;
			};

			inline TString &operator=(TStringReference<T> *value) {
				value_ = value;
				return *this;
			};

			inline TString &operator+=(const T *value) {
				if (value_->hasOneReference()) {
					value_->concatenateX(value);
				} else {
					TStringReference<T> *newString = TMemory<TStringReference<T>>::newMemory();
					newString->concatenate(value_, value);
					value_ = newString;
				};
				return *this;
			};

			inline TString &operator+=(const TString &value) {
				if (value_->hasOneReference()) {
					value_->concatenateX(const_cast<TString &>(value).value_.value());
				} else {
					TStringReference<T> *newString = TMemory<TStringReference<T>>::newMemory();
					newString->concatenate(value_, const_cast<TString &>(value).value_.value());
					value_ = newString;
				};
				return *this;
			};

			inline TString &operator+=(const T &value) {
				if (value_->hasOneReference()) {
					value_->concatenateX(value);
				} else {
					TStringReference<T> *newString = TMemory<TStringReference<T>>::newMemory();
					newString->concatenate(value_, value);
					value_ = newString;
				};
				return *this;
			};

			inline TString &operator<<(const TString &value) {
				if (value_->hasOneReference()) {
					value_->concatenateX(const_cast<TString &>(value).value_.value());
				} else {
					TStringReference<T> *newString = TMemory<TStringReference<T>>::newMemory();
					newString->concatenate(value_, const_cast<TString &>(value).value_.value());
					value_ = newString;
				};
				return *this;
			};

			inline TString &operator<<(const T *value) {
				if (value_->hasOneReference()) {
					value_->concatenateX(value);
				} else {
					TStringReference<T> *newString = TMemory<TStringReference<T>>::newMemory();
					newString->concatenate(value_, value);
					value_ = newString;
				};
				return *this;
			};

			inline TString &operator<<(const T &value) {
				if (value_->hasOneReference()) {
					value_->concatenateX(value);
				} else {
					TStringReference<T> *newString = TMemory<TStringReference<T>>::newMemory();
					newString->concatenate(value_, value);
					value_ = newString;
				};
				return *this;
			};

			inline operator const T *() const {
				return value_->value();
			};

			inline const T *value() const {
				return value_->value();
			};

			inline const T *index(size_t x) const {
				return &(value_->value())[x];
			};

			inline T &elementAt(size_t x) const {
				return (value_->value())[x];
			};

			inline T &operator[](int x) const {
				return (value_->value())[x];
			};

			inline size_t length() const {
				return value_->length();
			};

			inline void set(TStringReference<T> *value) {
				value_.set(value);
			};

			inline void set(const T *value, size_t length) {
				TStringReference<T> *newString = TMemory<TStringReference<T>>::newMemory();
				newString->from(value, length);
				value_ = newString;
			};

			inline void set(const TString &value, size_t length) {
				TStringReference<T> *newString = TMemory<TStringReference<T>>::newMemory();
				newString->from(const_cast<TString &>(value), length);
				value_ = newString;
			};

			inline void concatenate(const T *value, size_t length) {
				if (value_->hasOneReference()) {
					value_->concatenateX(value, length);
				} else {
					TStringReference<T> *newString = TMemory<TStringReference<T>>::newMemory();
					newString->concatenate(value_, value, length);
					value_ = newString;
				};
			};

			inline void concatenate(const TString &value, size_t length) {
				if (value_->hasOneReference()) {
					value_->concatenateX(const_cast<TString &>(value), length);
				} else {
					TStringReference<T> *newString = TMemory<TStringReference<T>>::newMemory();
					newString->concatenate(value_, const_cast<TString &>(value), length);
					value_ = newString;
				};
			};

			inline bool operator==(const T *x) const {
				return (TStringCore<T>::compare((const_cast<TPointer<TStringReference<T>> &>(value_))->value(), x) == 0);
			};

			inline bool operator==(const TString &x) const {
				return (TStringCore<T>::compare((const_cast<TPointer<TStringReference<T>> &>(value_))->value(), x.value()) == 0);
			};

			inline bool operator!=(const T *x) const {
				return (TStringCore<T>::compare((const_cast<TPointer<TStringReference<T>> &>(value_))->value(), x) != 0);
			};

			inline bool operator!=(const TString &x) const {
				return (TStringCore<T>::compare((const_cast<TPointer<TStringReference<T>> &>(value_))->value(), x.value()) != 0);
			};

			inline bool operator<(const T *x) const {
				return (TStringCore<T>::compare((const_cast<TPointer<TStringReference<T>> &>(value_))->value(), x) < 0);
			};

			inline bool operator<(const TString &x) const {
				return (TStringCore<T>::compare((const_cast<TPointer<TStringReference<T>> &>(value_))->value(), x.value()) < 0);
			};

			inline bool operator<=(const T *x) const {
				return (TStringCore<T>::compare((const_cast<TPointer<TStringReference<T>> &>(value_))->value(), x) <= 0);
			};

			inline bool operator<=(const TString &x) const {
				return (TStringCore<T>::compare((const_cast<TPointer<TStringReference<T>> &>(value_))->value(), x.value()) <= 0);
			};

			inline bool operator>(const T *x) const {
				return (TStringCore<T>::compare((const_cast<TPointer<TStringReference<T>> &>(value_))->value(), x) > 0);
			};

			inline bool operator>(const TString &x) const {
				return (TStringCore<T>::compare((const_cast<TPointer<TStringReference<T>> &>(value_))->value(), x.value()) > 0);
			};

			inline bool operator>=(const T *x) const {
				return (TStringCore<T>::compare((const_cast<TPointer<TStringReference<T>> &>(value_))->value(), x) >= 0);
			};

			inline bool operator>=(const TString &x) const {
				return (TStringCore<T>::compare((const_cast<TPointer<TStringReference<T>> &>(value_))->value(), x.value()) >= 0);
			};

			inline int compare(const T *x) const {
				return TStringCore<T>::compare((const_cast<TPointer<TStringReference<T>> &>(value_))->value(), x);
			};

			inline int compare(const TString &value) const {
				return TStringCore<T>::compare((const_cast<TPointer<TStringReference<T>> &>(value_))->value(), value.value());
			};

			inline void activeDestructor() {
				value_.newMemory();
				value_->init();
			};

			inline void empty() {
				value_.newMemory();
				value_->init();
			};

			static inline void initMemory() {
				TMemory<TStringReference<T>>::initMemory();
			};

			inline TStringReference<T> *reference() const {
				return value_;
			};

			inline TString operator+(const T *value) const {
				TString retV(*this);
				retV += value;
				return retV;
			};

			inline TString operator+(const TString &value) const {
				TString retV(*this);
				retV += value;
				return retV;
			};

			inline TString operator+(const T &value) const {
				TString retV(*this);
				retV += value;
				return retV;
			};

			inline bool isEmpty() const {
				return (length() == 0);
			}

			// ---

			static inline TString trimWithElement(const TString &o, const TString &x) {
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

				return TString(x1, x2 - x1 + 1);
			};

			static inline TString trimAscii(const TString &o) {
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

				return TString(x1, x2 - x1 + 1);
			};

			static inline TString replace(const TString &o, const TString &x, const TString &y) {
				TStringReference<T> *retV;
				size_t k, ln;
				size_t xLn;

				xLn = x.length();
				if (o.length() < xLn) {
					return const_cast<TString &>(o);
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

			static inline TString nilAtFirst(const TString &o, const TString &x) {
				size_t idx;
				if (TStringCore<T>::indexOf(o, x, idx)) {
					return TString(o.value(), idx);
				};
				return o;
			};

			static inline TString nilAtFirstFromEnd(const TString &o, const TString &x) {
				size_t idx;
				if (TStringCore<T>::indexOfFromEnd(o, x, idx)) {
					return TString(o.value(), idx);
				};
				return o;
			};

			static inline TString substring(const TString &o, size_t start, size_t length) {
				if (length == 0) {
					return TString();
				};
				if (start > o.length()) {
					return TString();
				};
				if (start + length > o.length()) {
					length = o.length() - start;
				};
				return TString(o.index(start), length);
			};

			static inline TString substring(const TString &o, size_t start) {
				if (start > o.length()) {
					return TString();
				};
				return TString(o.index(start), o.length() - start);
			};

			static inline TString toLowerCaseAscii(const TString &value) {
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

			static inline TString toUpperCaseAscii(const TString &value) {
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

			static inline bool matchAscii(const TString &text, const TString &sig) {
				return TStringCore<T>::matchAscii(text, text.length(), sig, sig.length());
			};

			static inline bool split2(const TString &text, const TString &sig, TString &firstPart, TString &secondPart) {
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

			static inline bool split2FromEnd(const TString &text, const TString &sig, TString &firstPart, TString &secondPart) {
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

			static void encodeC_(TStringReference<T> *retV, const TString &in) {
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

			static inline TString encodeC(const TString &in) {
				TStringReference<T> *retV = TMemory<TStringReference<T>>::newMemory();
				retV->init();
				retV->concatenateX('"');
				encodeC_(retV, in);
				retV->concatenateX('"');
				return retV;
			};

			static inline TString encodeCX(const TString &in) {
				TStringReference<T> *retV = TMemory<TStringReference<T>>::newMemory();
				retV->init();
				encodeC_(retV, in);
				return retV;
			};

			static inline bool indexOf(const TString &a, const TString &b, size_t start, size_t &index) {
				return TStringCore<T>::indexOf(a, a.length(), b, b.length(), start, index);
			};

			static inline bool indexOfFromEnd(const TString &a, const TString &b, size_t start, size_t &index) {
				return TStringCore<T>::indexOfFromEnd(a, a.length(), b, b.length(), start, index);
			};

			static inline bool itContains(const TString &a, const TString &b) {
				size_t index = 0;
				return TStringCore<T>::indexOf(a, a.length(), b, b.length(), 0, index);
			};

			static inline bool beginWith(const TString &a, const TString &b) {
				return (TStringCore<T>::compareN(a, b, b.length()) == 0);
			};

			static inline bool endsWith(const TString &a, const TString &b) {
				if (b.length() > a.length()) {
					return false;
				};
				return (TStringCore<T>::compareN(a.index(a.length() - b.length()), b, b.length()) == 0);
			};

			static inline bool explode(const TString &delimiter, const TString &strToExplode, TDynamicArray<TString> &out) {
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

			static inline TString implode(const TString &delimiter, TDynamicArray<TString> &in) {
				TString retV;
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

namespace XYO::ManagedMemory {
	template <typename U>
	struct TComparator<XYO::Encoding::TString<U>> {
			typedef XYO::Encoding::TString<U> T;

			static inline bool isEqual(const T &x, const T &y) {
				return const_cast<T &>(x) == const_cast<T &>(y);
			};

			static inline bool isNotEqual(const T &x, const T &y) {
				return const_cast<T &>(x) != const_cast<T &>(y);
			};

			static inline bool isLess(const T &x, const T &y) {
				return const_cast<T &>(x) < const_cast<T &>(y);
			};

			static inline bool isGreater(const T &x, const T &y) {
				return const_cast<T &>(x) > const_cast<T &>(y);
			};

			static inline bool isLessOrEqual(const T &x, const T &y) {
				return const_cast<T &>(x) <= const_cast<T &>(y);
			};

			static inline bool isGreaterOrEqual(const T &x, const T &y) {
				return const_cast<T &>(x) >= const_cast<T &>(y);
			};

			static inline int compare(const T &x, const T &y) {
				return (const_cast<T &>(x)).compare(const_cast<T &>(y));
			};
	};

	template <typename T>
	struct TMemory<XYO::Encoding::TString<T>> : TMemoryPoolActive<XYO::Encoding::TString<T>> {
	};
};

#endif
