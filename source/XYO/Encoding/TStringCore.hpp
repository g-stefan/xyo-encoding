// Encoding
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_ENCODING_TSTRINGCORE_HPP
#define XYO_ENCODING_TSTRINGCORE_HPP

#ifndef XYO_ENCODING_TMEMORYCORE_HPP
#	include <XYO/Encoding/TMemoryCore.hpp>
#endif

namespace XYO::Encoding {

	template <typename T>
	struct TStringCore {

			static const T empty[];

			static inline size_t length(const T *x) {
				size_t ln = 0;
				while (*x != 0) {
					++x;
					++ln;
				};
				return ln;
			};

			static inline void copy(T *x, const T *y) {
				while (*y != 0) {
					*x = *y;
					++x;
					++y;
				};
				*x = 0;
			};

			static inline size_t copyX(T *x, const T *y) {
				size_t ln = 0;
				while (*y != 0) {
					*x = *y;
					++x;
					++y;
					++ln;
				};
				*x = 0;
				return ln;
			};

			static inline void copyN(T *x, const T *y, size_t yLn) {
				while ((yLn != 0) && (*y != 0)) {

					*x = *y;

					++x;
					++y;
					--yLn;
				};
				*x = 0;
			};

			static inline void copyMemory(T *x, const T *y, size_t yLn) {
				TMemoryCore<T>::copyN(x, y, yLn);
				x[yLn] = 0;
			};

			static inline int compare(const T *x, const T *y) {

				while (*x != 0) {

					if (*x != *y) {
						if ((*x - *y) < 0) {
							return -1;
						};
						return 1;
					};

					++x;
					++y;
				};

				if (*y == 0) {
					return 0;
				};

				return -1;
			};

			static inline int compareN(const T *x, const T *y, size_t ln) {

				while ((ln != 0) && (*x != 0)) {

					if (*x != *y) {
						if ((*x - *y) < 0) {
							return -1;
						};
						return 1;
					};

					++x;
					++y;
					--ln;
				};

				if (ln == 0) {
					return 0;
				};

				if (*y == 0) {
					return 0;
				};

				return -1;
			};

			// ---

			static inline bool isEqual(const T *x, const T *y) {
				return (compare(x, y) == 0);
			};

			static inline bool isEqualN(const T *x, const T *y, size_t ln) {
				return (compareN(x, y, ln) == 0);
			};

			static inline bool beginWith(const T *x, const T *y) {
				return (compareN(x, y, length(y)) == 0);
			};

			static inline bool endsWith(const T *x, const T *y) {
				size_t xLn = length(x);
				size_t yLn = length(y);
				if (yLn > xLn) {
					return false;
				};
				return (compareN(x + xLn - yLn, y, yLn) == 0);
			};

			static inline void concatenate(T *x, const T *y) {
				while (*x != 0) {
					++x;
				};
				copy(x, y);
			};

			static inline void concatenateN(T *x, const T *y, size_t yLn) {
				while (*x != 0) {
					++x;
				};
				copyN(x, y, yLn);
			};

			static bool indexOf(const T *x, size_t xLn, const T *y, size_t yLn, size_t pos, size_t &index) {
				if (pos > xLn) {
					return false;
				};
				if (yLn > xLn) {
					return false;
				};
				if ((pos + yLn) > xLn) {
					return false;
				};
				for (index = pos; index <= xLn - yLn; ++index) {
					if (isEqualN(&x[index], y, yLn)) {
						return true;
					};
				};
				return false;
			};

			static bool indexOfFromEnd(const T *x, size_t xLn, const T *y, size_t yLn, size_t pos, size_t &index) {
				if (pos > xLn) {
					return false;
				}
				if (yLn > xLn) {
					return false;
				}
				if ((pos + yLn) > xLn) {
					return false;
				}
				for (index = xLn - pos - yLn + 1; index > 0; --index) {
					if (isEqualN(&x[index - 1], y, yLn)) {
						--index;
						return true;
					};
				};
				return false;
			};

			static inline bool indexOf(const T *x, const T *y, size_t &index) {
				return indexOf(x, length(x), y, length(y), 0, index);
			};

			static inline bool indexOf(const T *x, const T *y, size_t pos, size_t &index) {
				return indexOf(x, length(x), y, length(y), pos, index);
			};

			static inline bool indexOfFromEnd(const T *x, const T *y, size_t &index) {
				return indexOfFromEnd(x, length(x), y, length(y), 0, index);
			};

			static inline bool indexOfFromEnd(const T *x, const T *y, size_t pos, size_t &index) {
				return indexOfFromEnd(x, length(x), y, length(y), pos, index);
			};

			static inline T elementToLowerCaseAscii(const T &x) {
				if (x < 0x41 || x > 0x5A) {
					return x;
				};
				return x + 0x20;
			};

			static inline T elementToUpperCaseAscii(const T &x) {
				if (x < 0x61 || x > 0x7A) {
					return x;
				};
				return x - 0x20;
			};

			static int compareIgnoreCaseAscii(const T *x, const T *y) {
				T eX;
				T eY;

				while (*x != 0) {

					eX = elementToLowerCaseAscii(*x);
					eY = elementToLowerCaseAscii(*y);

					if (eX != eY) {
						if ((eX - eY) < 0) {
							return -1;
						};
						return 1;
					};

					++x;
					++y;
				};

				if (*y == 0) {
					return 0;
				};

				return -1;
			};

			static int compareIgnoreCaseNAscii(const T *x, const T *y, size_t ln) {

				T eX;
				T eY;

				while ((ln != 0) && (*x != 0)) {

					eX = elementToLowerCaseAscii(*x);
					eY = elementToLowerCaseAscii(*y);

					if (eX != eY) {
						if ((eX - eY) < 0) {
							return -1;
						};
						return 1;
					};

					++x;
					++y;
					--ln;
				};

				if (ln == 0) {
					return 0;
				};

				if (*y == 0) {
					return 0;
				};

				return -1;
			};

			static bool indexOfIgnoreCaseAscii(const T *x, size_t xLn, const T *y, size_t yLn, size_t pos, size_t &index) {
				if (pos > xLn) {
					return false;
				};
				if (yLn > xLn) {
					return false;
				};
				if ((pos + yLn) > xLn) {
					return false;
				};
				for (index = pos; index <= xLn - yLn; ++index) {
					if (compareIgnoreCaseNAscii(&x[index], y, yLn) == 0) {
						return true;
					};
				};
				return false;
			};

			static bool indexOfFromEndIgnoreCaseAscii(const T *x, size_t xLn, const T *y, size_t yLn, size_t pos, size_t &index) {
				if (pos > xLn) {
					return false;
				}
				if (yLn > xLn) {
					return false;
				}
				if ((pos + yLn) > xLn) {
					return false;
				}
				for (index = xLn - pos - yLn + 1; index > 0; --index) {
					if (compareIgnoreCaseNAscii(&x[index - 1], y, yLn)) {
						--index;
						return true;
					};
				};
				return false;
			};

			static const T *toNotInElement(const T *x, const T *y) {
				const T *m;
				bool found;
				while (*x) {
					found = false;
					for (m = y; *m; ++m) {
						if (*x == *m) {
							found = true;
						}
					};
					if (!found) {
						return x;
					}
					++x;
				};
				return x;
			};

			static const T *toNotInFromEndElement(const T *x, const T *y) {
				const T *m;
				const T *xIndex;
				const T *xIndex2;
				size_t k;
				bool found;
				k = length(x);
				xIndex = x + k;
				xIndex2 = xIndex;
				--xIndex;
				while (k > 0) {
					found = false;
					for (m = y; *m; ++m) {
						if (*xIndex == *m) {
							found = true;
						}
					};
					if (!found) {
						return xIndex;
					}
					--xIndex;
					--k;
				};
				return xIndex2;
			};

			static bool matchAscii(const T *x, size_t xLn, const T *y, size_t yLn) {
				size_t i;
				size_t j;
				size_t k;
				size_t m;

				i = 0;
				j = 0;
				for (; (i < yLn) && (j < xLn); ++i) {
					if (y[i] == '*') {
						while (x[j] && (elementToUpperCaseAscii(x[j]) != elementToUpperCaseAscii(y[i + 1]))) {
							j++;
						};
						if (x[j] == 0) {
							if (y[i + 1] == 0) {
								return true;
							};
							return false;
						};
						k = i + 1;
						m = j;

						while (x[m] && y[k] && (elementToUpperCaseAscii(x[m]) == elementToUpperCaseAscii(y[k]))) {
							++k;
							++m;
						};
						if (y[k] == 0) {
							if (x[m] == 0) {
								return true;
							};
							return false;
						};
						if (y[k] == '*') {
							i = k - 1;
							j = m;
							continue;
						} else if (y[k] == '?') {
							i = k - 1;
							j = m;
							continue;
						};
						j++;
						i--;

						continue;
					} else if ((elementToUpperCaseAscii(x[j]) == elementToUpperCaseAscii(y[i])) || (y[i] == '?')) {
						j++;
					} else {
						return false;
					}
				};
				if (y[i] == 0) {
					if (x[j] == 0) {
						return true;
					};
				};
				return false;
			};

			static inline bool matchAscii(const T *x, const T *y) {
				return matchAscii(x, length(x), y, length(y));
			};
	};

	template <typename T>
	const T TStringCore<T>::empty[] = {0};

};

#ifndef XYO_ENCODING_TSTRINGCORE_STRING_CHAR_HPP
#	include <XYO/Encoding/TStringCore-String-Char.hpp>
#endif

#endif
