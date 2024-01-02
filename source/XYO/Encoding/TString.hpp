// Encoding
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
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
			};

			// ---

			inline TString trimWithElement(const TString &x) const {
				const T *x1;
				const T *x2;
				x1 = TStringCore<T>::toNotInElement(*this, x);
				x2 = TStringCore<T>::toNotInFromEndElement(*this, x);

				if (*x1 == 0) {
					return x1;
				};

				if (*x2 == 0) {
					return x2;
				};

				return TString(x1, x2 - x1 + 1);
			};

			inline TString trimAscii() const {
				const T *x1;
				const T *x2;
				const T x[5] = {0x20, 0x09, 0x0D, 0x0A, 0x00};
				x1 = TStringCore<T>::toNotInElement(*this, x);
				x2 = TStringCore<T>::toNotInFromEndElement(*this, x);

				if (*x1 == 0) {
					return x1;
				};

				if (*x2 == 0) {
					return x2;
				};

				return TString(x1, x2 - x1 + 1);
			};

			inline TString replace(const TString &x, const TString &y) const {
				TStringReference<T> *retV;
				size_t k, ln;
				size_t xLn;

				xLn = x.length();
				if (length() < xLn) {
					return *this;
				};

				retV = TMemory<TStringReference<T>>::newMemory();
				retV->init();

				ln = length() - xLn;

				k = 0;
				while (k < ln) {
					if (TStringCore<T>::compareN(index(k), x, xLn) == 0) {
						retV->concatenateX(y.reference());
						k += xLn;
						continue;
					} else {
						retV->concatenateX(elementAt(k));
						k++;
					};
				};

				if (TStringCore<T>::compareN(index(k), x, xLn) == 0) {
					retV->concatenateX(y.reference());
				} else {
					retV->concatenateX(index(k));
				};

				return retV;
			};

			inline TString nilAtFirst(const TString &x) const {
				size_t idx;
				if (TStringCore<T>::indexOf(*this, x, idx)) {
					return TString(*this, idx);
				};
				return *this;
			};

			inline TString nilAtFirstFromEnd(const TString &x) const {
				size_t idx;
				if (TStringCore<T>::indexOfFromEnd(*this, x, idx)) {
					return TString(*this, idx);
				};
				return *this;
			};

			inline TString substring(size_t start, size_t length_) const {
				if (length_ == 0) {
					return TString();
				};
				if (start > length()) {
					return TString();
				};
				if (start + length_ > length()) {
					length_ = length() - start;
				};
				return TString(index(start), length_);
			};

			inline TString substring(size_t start) const {
				if (start > length()) {
					return TString();
				};
				return TString(index(start), length() - start);
			};

			inline TString toLowerCaseAscii() const {
				TStringReference<T> *retV = TMemory<TStringReference<T>>::newMemory();

				retV->init(length());

				size_t totalLn;
				const T *inx;
				T *outx;

				totalLn = 0;
				inx = value();
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

			inline TString toUpperCaseAscii() const {
				TStringReference<T> *retV = TMemory<TStringReference<T>>::newMemory();

				retV->init(length());

				size_t totalLn;
				const T *inx;
				T *outx;

				totalLn = 0;
				inx = value();
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

			inline bool matchAscii(const TString &sig) const {
				return TStringCore<T>::matchAscii(*this, length(), sig, sig.length());
			};

			inline bool split2(const TString &sig, TString &firstPart, TString &secondPart) const {
				size_t index;
				if (TStringCore<T>::indexOf(*this, length(), sig, sig.length(), 0, index)) {
					firstPart = substring(0, index);
					secondPart = substring(index + sig.length());
					return true;
				};
				firstPart = *this;
				secondPart = TStringCore<T>::empty;
				return false;
			};

			inline bool split2FromEnd(const TString &sig, TString &firstPart, TString &secondPart) const {
				size_t index;
				if (TStringCore<T>::indexOfFromEnd(*this, length(), sig, sig.length(), 0, index)) {
					firstPart = substring(0, index);
					secondPart = substring(index + sig.length());
					return true;
				};
				firstPart = *this;
				secondPart = TStringCore<T>::empty;
				return false;
			};

			inline void encodeC_(TStringReference<T> *retV) const {
				size_t k;
				const T *scan;
				scan = value();
				for (k = 0; k < length(); ++k, ++scan) {
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

			inline TString encodeC() const {
				TStringReference<T> *retV = TMemory<TStringReference<T>>::newMemory();
				retV->init();
				retV->concatenateX('"');
				encodeC_(retV);
				retV->concatenateX('"');
				return retV;
			};

			inline TString encodeCX() const {
				TStringReference<T> *retV = TMemory<TStringReference<T>>::newMemory();
				retV->init();
				encodeC_(retV);
				return retV;
			};

			inline bool indexOf(const TString &b, size_t start, size_t &index) const {
				return TStringCore<T>::indexOf(*this, length(), b, b.length(), start, index);
			};

			inline bool indexOfFromEnd(const TString &b, size_t start, size_t &index) const {
				return TStringCore<T>::indexOfFromEnd(*this, length(), b, b.length(), start, index);
			};

			inline bool itContains(const TString &b) const {
				size_t index = 0;
				return TStringCore<T>::indexOf(*this, length(), b, b.length(), 0, index);
			};

			inline bool beginWith(const TString &b) const {
				return (TStringCore<T>::compareN(*this, b, b.length()) == 0);
			};

			inline bool endsWith(const TString &b) const {
				if (b.length() > length()) {
					return false;
				};
				return (TStringCore<T>::compareN(index(length() - b.length()), b, b.length()) == 0);
			};

			inline bool explode(const TString &delimiter, TDynamicArray<TString> &out) const {
				size_t index = 0;
				size_t indexNext = 0;
				if (delimiter.isEmpty()) {
					return false;
				};
				for (;;) {
					if (indexOf(delimiter, index, indexNext)) {
						out.push(substring(index, indexNext - index));
						index = indexNext + delimiter.length();
						continue;
					};
					break;
				};
				if (index < length()) {
					out.push(substring(index, length() - index));
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
	template <typename T>
	struct TMemory<XYO::Encoding::TString<T>> : TMemoryPoolActive<XYO::Encoding::TString<T>> {
	};
};

#ifndef XYO_ENCODING_TCOMPARATOR_TSTRING_HPP
#	include <XYO/Encoding/TComparator-TString.hpp>
#endif

#endif
