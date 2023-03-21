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
