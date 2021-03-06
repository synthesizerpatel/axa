/*
 * Operate on bits in words
 *
 *  Copyright (c) 2014-2015 by Farsight Security, Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <axa/bits.h>

/*
 * Find the index of the first bit set in an array of 64-bit words.
 *	Answer with len*64 if no bit is set.
 */
uint
axa_find_bitwords(axa_word_t *wp, uint bits_len)
{
	uint n;
	axa_word_t w;

	n = 0;
	for (;;) {
		if (n >= bits_len)
			return (bits_len);
		w = *wp;
		if (w != 0)
			return (n + axa_fls_word(w));
		++w;
		n += AXA_WORD_BITS;
	}
}
