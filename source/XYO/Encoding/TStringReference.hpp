// Encoding
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_ENCODING_TSTRINGREFERENCE_HPP
#define XYO_ENCODING_TSTRINGREFERENCE_HPP

#ifndef XYO_ENCODING_DEPENDENCY_HPP
#	include <XYO/Encoding/Dependency.hpp>
#endif

#ifndef XYO_ENCODING_TSTRINGCORE_HPP
#	include <XYO/Encoding/TStringCore.hpp>
#endif

namespace XYO::Encoding {

	template <typename T>
	class TStringReference : public Object {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(TStringReference);

		protected:
			T *value_;
			size_t length_;
			size_t size_;
			bool mode_;
			size_t chunk_;

			template <typename U, size_t sizeOfMemory>
			struct TStringCoreReferenceMemory {
					U memory[sizeOfMemory];
			};

			inline void deleteMemory_() {
				if (value_ != nullptr) {
					if (mode_) {
						if (size_ <= 32) {
							TMemory<TStringCoreReferenceMemory<T, 32>>::deleteMemory(reinterpret_cast<TStringCoreReferenceMemory<T, 32> *>(value_));
							mode_ = false;
							return;
						};
						if (size_ <= 64) {
							TMemory<TStringCoreReferenceMemory<T, 64>>::deleteMemory(reinterpret_cast<TStringCoreReferenceMemory<T, 64> *>(value_));
							mode_ = false;
							return;
						};
						if (size_ <= 96) {
							TMemory<TStringCoreReferenceMemory<T, 96>>::deleteMemory(reinterpret_cast<TStringCoreReferenceMemory<T, 96> *>(value_));
							mode_ = false;
							return;
						};
						if (size_ <= 128) {
							TMemory<TStringCoreReferenceMemory<T, 128>>::deleteMemory(reinterpret_cast<TStringCoreReferenceMemory<T, 128> *>(value_));
							mode_ = false;
							return;
						};
						if (size_ <= 160) {
							TMemory<TStringCoreReferenceMemory<T, 160>>::deleteMemory(reinterpret_cast<TStringCoreReferenceMemory<T, 160> *>(value_));
							mode_ = false;
							return;
						};
						if (size_ <= 192) {
							TMemory<TStringCoreReferenceMemory<T, 192>>::deleteMemory(reinterpret_cast<TStringCoreReferenceMemory<T, 192> *>(value_));
							mode_ = false;
							return;
						};
						if (size_ <= 224) {
							TMemory<TStringCoreReferenceMemory<T, 224>>::deleteMemory(reinterpret_cast<TStringCoreReferenceMemory<T, 224> *>(value_));
							mode_ = false;
							return;
						};
						if (size_ <= 256) {
							TMemory<TStringCoreReferenceMemory<T, 256>>::deleteMemory(reinterpret_cast<TStringCoreReferenceMemory<T, 256> *>(value_));
							mode_ = false;
							return;
						};
					} else {
						delete[] value_;
					};
				};
				mode_ = false;
			};

			inline void newMemory_(size_t size) {
				size_ = size;
				if (size_ <= 32) {
					value_ = reinterpret_cast<T *>(TMemory<TStringCoreReferenceMemory<T, 32>>::newMemory());
					mode_ = true;
					return;
				};
				if (size_ <= 64) {
					value_ = reinterpret_cast<T *>(TMemory<TStringCoreReferenceMemory<T, 64>>::newMemory());
					mode_ = true;
					return;
				};
				if (size_ <= 96) {
					value_ = reinterpret_cast<T *>(TMemory<TStringCoreReferenceMemory<T, 96>>::newMemory());
					mode_ = true;
					return;
				};
				if (size_ <= 128) {
					value_ = reinterpret_cast<T *>(TMemory<TStringCoreReferenceMemory<T, 128>>::newMemory());
					mode_ = true;
					return;
				};
				if (size_ <= 160) {
					value_ = reinterpret_cast<T *>(TMemory<TStringCoreReferenceMemory<T, 160>>::newMemory());
					mode_ = true;
					return;
				};
				if (size_ <= 192) {
					value_ = reinterpret_cast<T *>(TMemory<TStringCoreReferenceMemory<T, 192>>::newMemory());
					mode_ = true;
					return;
				};
				if (size_ <= 224) {
					value_ = reinterpret_cast<T *>(TMemory<TStringCoreReferenceMemory<T, 224>>::newMemory());
					mode_ = true;
					return;
				};
				if (size_ <= 256) {
					value_ = reinterpret_cast<T *>(TMemory<TStringCoreReferenceMemory<T, 256>>::newMemory());
					mode_ = true;
					return;
				};

				value_ = new T[size];
				mode_ = false;
			};

			inline void memoryResize_(size_t size) {
				T *newValue_;
				bool newMode_;
				newMode_ = false;
				if (size <= 32) {
					newValue_ = reinterpret_cast<T *>(TMemory<TStringCoreReferenceMemory<T, 32>>::newMemory());
					newMode_ = true;
					TMemoryCore<T>::copyN(newValue_, value_, length_);
					deleteMemory_();
					value_ = newValue_;
					mode_ = newMode_;
					size_ = size;
					return;
				};
				if (size <= 64) {
					newValue_ = reinterpret_cast<T *>(TMemory<TStringCoreReferenceMemory<T, 64>>::newMemory());
					newMode_ = true;
					TMemoryCore<T>::copyN(newValue_, value_, length_);
					deleteMemory_();
					value_ = newValue_;
					mode_ = newMode_;
					size_ = size;
					return;
				};
				if (size <= 96) {
					newValue_ = reinterpret_cast<T *>(TMemory<TStringCoreReferenceMemory<T, 96>>::newMemory());
					newMode_ = true;
					TMemoryCore<T>::copyN(newValue_, value_, length_);
					deleteMemory_();
					value_ = newValue_;
					mode_ = newMode_;
					size_ = size;
					return;
				};
				if (size <= 128) {
					newValue_ = reinterpret_cast<T *>(TMemory<TStringCoreReferenceMemory<T, 128>>::newMemory());
					newMode_ = true;
					TMemoryCore<T>::copyN(newValue_, value_, length_);
					deleteMemory_();
					value_ = newValue_;
					mode_ = newMode_;
					size_ = size;
					return;
				};
				if (size <= 160) {
					newValue_ = reinterpret_cast<T *>(TMemory<TStringCoreReferenceMemory<T, 160>>::newMemory());
					newMode_ = true;
					TMemoryCore<T>::copyN(newValue_, value_, length_);
					deleteMemory_();
					value_ = newValue_;
					mode_ = newMode_;
					size_ = size;
					return;
				};
				if (size <= 192) {
					newValue_ = reinterpret_cast<T *>(TMemory<TStringCoreReferenceMemory<T, 192>>::newMemory());
					newMode_ = true;
					TMemoryCore<T>::copyN(newValue_, value_, length_);
					deleteMemory_();
					value_ = newValue_;
					mode_ = newMode_;
					size_ = size;
					return;
				};
				if (size <= 224) {
					newValue_ = reinterpret_cast<T *>(TMemory<TStringCoreReferenceMemory<T, 224>>::newMemory());
					newMode_ = true;
					TMemoryCore<T>::copyN(newValue_, value_, length_);
					deleteMemory_();
					value_ = newValue_;
					mode_ = newMode_;
					size_ = size;
					return;
				};
				if (size <= 256) {
					newValue_ = reinterpret_cast<T *>(TMemory<TStringCoreReferenceMemory<T, 256>>::newMemory());
					newMode_ = true;
					TMemoryCore<T>::copyN(newValue_, value_, length_);
					deleteMemory_();
					value_ = newValue_;
					mode_ = newMode_;
					size_ = size;
					return;
				};

				newValue_ = new T[size];
				TMemoryCore<T>::copyN(newValue_, value_, length_);
				deleteMemory_();
				value_ = newValue_;
				mode_ = newMode_;
				size_ = size;
			};

			inline size_t memoryResizeCheck_(size_t newLength_) {
				if (newLength_ < size_) {
					return newLength_;
				};
				memoryResize_(((newLength_ / chunk_) + 1) * chunk_);
				return newLength_;
			};

			inline size_t memoryNewCheck_(size_t newLength_) {
				if (chunk_ == 0) {
					chunk_ = 32;
				};
				if (newLength_ >= chunk_) {
					newMemory_(((newLength_ / chunk_) + 1) * chunk_);
				} else {
					newMemory_(chunk_);
				};
				return newLength_;
			};

		public:
			inline TStringReference() {
				value_ = nullptr;
				mode_ = false;
				chunk_ = 32;
			};

			inline ~TStringReference() {
				deleteMemory_();
			};

			static inline void initMemory() {
				TMemory<TStringCoreReferenceMemory<T, 32>>::initMemory();
				TMemory<TStringCoreReferenceMemory<T, 64>>::initMemory();
				TMemory<TStringCoreReferenceMemory<T, 96>>::initMemory();
				TMemory<TStringCoreReferenceMemory<T, 128>>::initMemory();
				TMemory<TStringCoreReferenceMemory<T, 160>>::initMemory();
				TMemory<TStringCoreReferenceMemory<T, 192>>::initMemory();
				TMemory<TStringCoreReferenceMemory<T, 224>>::initMemory();
				TMemory<TStringCoreReferenceMemory<T, 256>>::initMemory();
			};

			inline T *value() const {
				return value_;
			};

			inline size_t length() const {
				return length_;
			};

			inline size_t chunk() const {
				return chunk_;
			};

			inline void setLength(size_t length) {
				if (length >= size_) {
					length = size_ - 1;
				};
				length_ = length;
			};

			inline void setChunk(size_t chunk) {
				if (chunk == 0) {
					chunk = 32;
				};
				chunk_ = chunk;
			};

			inline void activeDestructor() {
				deleteMemory_();
				value_ = nullptr;
				mode_ = false;
				chunk_ = 32;
			};

			inline void init() {
				newMemory_(chunk_);
				value_[0] = TStringCore<T>::empty[0];
				length_ = 0;
			};

			inline void init(size_t length) {
				memoryNewCheck_(length);
				value_[0] = TStringCore<T>::empty[0];
				length_ = 0;
			};

			inline void init(size_t length, size_t chunk) {
				if (chunk == 0) {
					chunk = 32;
				};
				chunk_ = chunk;
				memoryNewCheck_(length);
				value_[0] = TStringCore<T>::empty[0];
				length_ = 0;
			};

			inline void from(const T *o) {
				length_ = memoryNewCheck_(TStringCore<T>::length(o));
				TStringCore<T>::copy(value_, o);
			};

			inline void from(const T *o, size_t oLn) {
				length_ = memoryNewCheck_(oLn);
				TStringCore<T>::copyMemory(value_, o, oLn);
			};

			inline void concatenate(TStringReference *o, const T *x) {
				length_ = memoryNewCheck_(o->length_ + TStringCore<T>::length(x));
				TMemoryCore<T>::copyN(value_, o->value_, o->length_);
				TStringCore<T>::copy(&value_[o->length_], x);
			};

			inline void concatenate(TStringReference *o, TStringReference *x) {
				length_ = memoryNewCheck_(o->length_ + x->length_);
				TMemoryCore<T>::copyN(value_, o->value_, o->length_);
				TMemoryCore<T>::copyN(&value_[o->length_], x->value_, x->length_ + 1);
			};

			inline void concatenate(TStringReference *o, const T *x, size_t xLn) {
				length_ = memoryNewCheck_(o->length_ + xLn);
				TMemoryCore<T>::copyN(value_, o->value_, o->length_);
				TStringCore<T>::copyMemory(&value_[o->length_], x, xLn);
			};

			inline void concatenate(TStringReference *o, const T &x) {
				length_ = memoryNewCheck_(o->length_ + 1);
				TMemoryCore<T>::copyN(value_, o->value_, o->length_);
				value_[o->length_] = x;
				value_[o->length_ + 1] = 0;
			};

			inline void concatenateX(const T *x) {
				size_t index = length_;
				length_ = memoryResizeCheck_(length_ + TStringCore<T>::length(x));
				TStringCore<T>::copy(&value_[index], x);
			};

			inline void concatenateX(TStringReference *x) {
				size_t index = length_;
				length_ = memoryResizeCheck_(length_ + x->length_);
				TMemoryCore<T>::copyN(&value_[index], x->value_, x->length_ + 1);
			};

			inline void concatenateX(const T *x, size_t xLn) {
				size_t index = length_;
				length_ = memoryResizeCheck_(length_ + xLn);
				TStringCore<T>::copyMemory(&value_[index], x, xLn);
			};

			inline void concatenateX(const T &x) {
				size_t index = length_;
				length_ = memoryResizeCheck_(length_ + 1);
				value_[index] = x;
				value_[index + 1] = 0;
			};
	};

};

namespace XYO::ManagedMemory {
	template <typename T>
	struct TMemory<Encoding::TStringReference<T>> : TMemoryPoolActive<Encoding::TStringReference<T>> {
	};
};

#endif
