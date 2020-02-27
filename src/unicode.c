/**
	@file unicode.c
	@author s22h
	@date 27 Feb 2020
	@version 0.2.0
 */

#include "unicode.h"

#include <stdio.h>

size_t utf8_codepoint_size(const uint8_t byte) {
	if ((byte & UTF8_ONE_BYTE_MASK) == UTF8_ONE_BYTE_BITS) {
		return 1;
	}

	if ((byte & UTF8_TWO_BYTES_MASK) == UTF8_TWO_BYTES_BITS) {
		return 2;
	}

	if ((byte & UTF8_THREE_BYTES_MASK) == UTF8_THREE_BYTES_BITS) {
		return 3;
	}

	if ((byte & UTF8_FOUR_BYTES_MASK) == UTF8_FOUR_BYTES_BITS) {
		return 4;
	}

	return 0;
}

size_t utf8_strnlen(const uint8_t* text, size_t* valid_bytes, size_t max_bytes) {
	size_t i = 0;
	size_t cp_size;
	size_t num_chars = 0;
	int should_break = 0;

	while (text[i] != 0) {
		cp_size = utf8_codepoint_size(text[i]);

		if (cp_size == 0) {
			// Invalid code point
			fprintf(stderr, "utf8_strnlen: invalid codepoint at byte %lu.\n", i);
			break;
		}

		if (max_bytes > 0 && i + cp_size > max_bytes) {
			// Codepoint would be out of bounds
			fprintf(stderr, "utf8_strnlen: codepoint out of bounds at bytes %lu-%lu.\n", i, i + cp_size);
			break;
		}

		if (cp_size > 1) {
			for (unsigned int n = 1; n < cp_size; n++) {
				if (text[i + n] == 0) {
					// String is NUL-terminated in the middle of the codepoint
					fprintf(stderr, "utf8_strnlen: string is NUL-terminated in the middle of a codepoint.\n");
					should_break = 1;
					break;
				} else if ((text[i + n] & UTF8_CONTINUATION_MASK) != UTF8_CONTINUATION_BITS) {
					// Invalid byte in codepoint
					fprintf(stderr, "utf8_strnlen: invalid byte in codepoint.\n");
					should_break = 1;
					break;
				}
			}
		}

		if (should_break) {
			break;
		}

		i += cp_size;
		num_chars++;

		if (max_bytes > 0 && i > max_bytes) {
			// Next codepoint would be out of bounds
			fprintf(stderr, "utf8_strnlen: next codepoint out of bounds at byte %lu.\n", i);
			break;
		}
	}

	if (valid_bytes) {
		*valid_bytes = i;
	}

	return num_chars;
}

size_t utf8_strlen(const uint8_t* text, size_t* valid_bytes) {
	return utf8_strnlen(text, valid_bytes, 0);
}

size_t utf32_strnlen(const uint32_t* text, size_t* valid_bytes, size_t max_bytes) {
	size_t i = 0;

	while (text[i] != 0) {
		if (max_bytes > 0 && i >= max_bytes) {
			break;
		}

		++i;
	}

	if (valid_bytes) {
		*valid_bytes = i;
	}

	return i;
}

size_t utf32_strlen(const uint32_t* text, size_t* valid_bytes) {
	return utf32_strnlen(text, valid_bytes, 0);
}

size_t utf8_to_utf32(const uint8_t* text, uint32_t* buffer, size_t buffer_size) {
	size_t num_chars = utf8_strlen(text, NULL);
	size_t i = 0;
	size_t cp_size;

	if (!buffer || buffer_size < num_chars + 1) {
		fprintf(stderr, "utf8_to_utf32: buffer is NULL or too small.\n");
		return 0;
	}

	for (size_t n = 0; n < num_chars; n++) {
		cp_size = utf8_codepoint_size(text[i]);

		switch (cp_size) {
		case 1:
			buffer[n] = ((uint32_t) text[i] & ~UTF8_ONE_BYTE_MASK);
			break;
		case 2:
			buffer[n] =
				((uint32_t) text[i] & ~UTF8_TWO_BYTES_MASK) << 6 |
				((uint32_t) text[i + 1] & ~UTF8_CONTINUATION_MASK)
			;
			break;
		case 3:
			buffer[n] =
				((uint32_t) text[i] & ~UTF8_THREE_BYTES_MASK) << 12 |
				((uint32_t) text[i + 1] & ~UTF8_CONTINUATION_MASK) << 6 |
				((uint32_t) text[i + 2] & ~UTF8_CONTINUATION_MASK)
			;

			break;
		case 4:
			buffer[n] =
				((uint32_t) text[i] & ~UTF8_FOUR_BYTES_MASK) << 18 |
				((uint32_t) text[i + 1] & ~UTF8_CONTINUATION_MASK) << 12 |
				((uint32_t) text[i + 2] & ~UTF8_CONTINUATION_MASK) << 6 |
				((uint32_t) text[i + 3] & ~UTF8_CONTINUATION_MASK)
			;
			break;
		default:
			// this should never happen, since we check validity of the string
			fprintf(stderr, "utf8_to_utf32: invalid byte in UTF8 string.\n");
			return 0;
		}

		i += cp_size;
	}

	buffer[num_chars] = '\0';
	return num_chars;
}

