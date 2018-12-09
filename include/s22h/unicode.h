/**
	@file s22h/unicode.h
	@author s22h
	@date 30 Jun 2018
	@brief A header-only library for Unicode handling
	@version 0.1.1

	Code review of initial version provided by StackExchange members:

	https://codereview.stackexchange.com/questions/197548/convert-utf8-string-to-utf32-string-in-c

	chux: https://codereview.stackexchange.com/users/29485/chux
	hoffmale: https://codereview.stackexchange.com/users/94458/hoffmale
	Martin R: https://codereview.stackexchange.com/users/35991/martin-r
	Raziel: https://codereview.stackexchange.com/users/172927/raziel
 */

#ifndef S22H_UNICODE_H_
#define S22H_UNICODE_H_

#include <stdlib.h>
#include <stdint.h>

static const uint32_t UTF8_ONE_BYTE_MASK = 0x80;
static const uint32_t UTF8_ONE_BYTE_BITS = 0;
static const uint32_t UTF8_TWO_BYTES_MASK = 0xE0;
static const uint32_t UTF8_TWO_BYTES_BITS = 0xC0;
static const uint32_t UTF8_THREE_BYTES_MASK = 0xF0;
static const uint32_t UTF8_THREE_BYTES_BITS = 0xE0;
static const uint32_t UTF8_FOUR_BYTES_MASK = 0xF8;
static const uint32_t UTF8_FOUR_BYTES_BITS = 0xF0;
static const uint32_t UTF8_CONTINUATION_MASK = 0xC0;
static const uint32_t UTF8_CONTINUATION_BITS = 0x80;

/**
	@brief Finds the codepoint size of a given starting UTF8 byte.

	@param byte The byte that is the starting byte of a UTF8 codepoint.
	@return The number of bytes of the codepoint or 0 if not a valid code point.
*/
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

/**
	@brief Finds the string length of a given, NUL-terminated UTF8 string, using
		up to `max_bytes` bytes.

	@param text A NUL-terminated UTF8 string.
	@param valid_bytes A pointer to a size_t variable that will be set with the
		number of valid bytes in the string. Can be NULL.
	@param max_bytes The maximal amount of bytes to read.
	@return The number of valid characters in the string.
*/
size_t utf8_strnlen(const uint8_t* text, size_t* valid_bytes, size_t max_bytes) {
	size_t i = 0;
	size_t cp_size;
	size_t num_chars = 0;
	int should_break = 0;

	while (text[i] != 0) {
		cp_size = utf8_codepoint_size(text[i]);

		if (cp_size == 0) {
			// Invalid code point
			break;
		}

		if (max_bytes > 0 && i + cp_size >= max_bytes) {
			// Codepoint would be out of bounds
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

		if (max_bytes > 0 && i >= max_bytes) {
			// Next codepoint would be out of bounds
			break;
		}
	}

	if (valid_bytes) {
		*valid_bytes = i;
	}

	return num_chars;
}

/**
	@brief Finds the string length of a given, NUL-terminated UTF8 string.

	@param text A NUL-terminated UTF8 string.
	@param valid_bytes A pointer to a size_t variable that will be set with the
		number of valid bytes in the string. Can be NULL.
	@return The number of valid characters in the string.
*/
size_t utf8_strlen(const uint8_t* text, size_t* valid_bytes) {
	return utf8_strnlen(text, valid_bytes, 0);
}

/**
	@brief Converts a UTF8 string to UTF32.

	@param text A NUL-terminated UTF8 string.
	@param buffer The buffer that the converted string is written to.
	@param buffer_size The size of `buffer`.
	@return The number of converted characters or 0 on error.
*/
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

#endif /* S22H_UNICODE_H_ */
