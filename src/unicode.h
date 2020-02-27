/**
	@file unicode.h
	@author s22h
	@date 27 Feb 2020
	@brief Collection of unicode handling functions
	@version 0.2.0

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
size_t utf8_codepoint_size(const uint8_t byte);

/**
	@brief Finds the string length of a given, NUL-terminated UTF8 string, using
		up to `max_bytes` bytes.

	@param text A NUL-terminated UTF8 string.
	@param valid_bytes A pointer to a size_t variable that will be set with the
		number of valid bytes in the string. Can be NULL.
	@param max_bytes The maximum amount of bytes to read.
	@return The number of valid characters in the string.
*/
size_t utf8_strnlen(const uint8_t* text, size_t* valid_bytes, size_t max_bytes);

/**
	@brief Finds the string length of a given, NUL-terminated UTF8 string.

	@param text A NUL-terminated UTF8 string.
	@param valid_bytes A pointer to a size_t variable that will be set with the
		number of valid bytes in the string. Can be NULL.
	@return The number of valid characters in the string.
*/
size_t utf8_strlen(const uint8_t* text, size_t* valid_bytes);

/**
	@brief Finds the string length of a given, NUL-terminated UTF32 string,
		using up to `max_bytes` bytes.

	@param text A NUL-terminated UTF32 string.
	@param valid_bytes A pointer to a size_t variable that will be set with the
		number of valid bytes in the string. Can be NULL.
	@param max_bytes The maximum amount of bytes to read.
	@return The number of valid characters in the string.
*/
size_t utf32_strnlen(const uint32_t* text, size_t* valid_bytes, size_t max_bytes);

/**
	@brief Finds the string length of a given, NUL-terminated UTF32 string.

	@param text A NUL-terminated UTF32 string.
	@param valid_bytes A pointer to a size_t variable that will be set with the
		number of valid bytes in the string. Can be NULL.
	@return The number of valid characters in the string.
*/
size_t utf32_strlen(const uint32_t* text, size_t* valid_bytes);

/**
	@brief Converts a UTF8 string to UTF32.

	@param text A NUL-terminated UTF8 string.
	@param buffer The buffer that the converted string is written to.
	@param buffer_size The size of `buffer`.
	@return The number of converted characters or 0 on error.
*/
size_t utf8_to_utf32(const uint8_t* text, uint32_t* buffer, size_t buffer_size);

#endif

