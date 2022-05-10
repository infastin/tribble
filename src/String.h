#ifndef STRING_H_WW4E6R5D
#define STRING_H_WW4E6R5D

#include "Types.h"

#include <stdarg.h>

/**
 * SECTION: String
 * @title: String
 *
 * This example shows how to init and print the string to stdout:
 * ```c
 * String example_string;
 * string_init(&example_string, "I am an example!");
 *
 * printf("%s\n", example_string.data);
 * ```
 **/

typedef struct _String String;

struct _String {
	char *data;
	usize len;
	usize capacity;
};

/**
 * string_init0:
 * @string: The pointer to the string to be initialized (can be `NULL`).
 *
 * Creates a new #String.
 *
 * Returns: A new #String.
 **/
String *string_init0(String *string);

String *string_init(String *string, const char *c_str);

/**
 * string_init_len:
 * @string: The pointer to the string to be initialized (can be `NULL`).
 * @str: The initial contents of the string.
 * @len: The number of bytes in the @str to use.
 *
 * Creates a new #String with @len bytes of the @str.
 *
 * Returns: A new #String.
 **/
String *string_init_len(String *string, const char *str, usize len);

/**
 * string_init_sized:
 * @string: The pointer to the string to be initialized (can be `NULL`).
 * @cap: The initial capacity of the string.
 *
 * Creates a new #String with capacity of @cap bytes.
 *
 * Returns: A new #String.
 **/
String *string_init_sized(String *string, usize cap);

/**
 * string_push_front:
 * @string: The string where to add the other string.
 * @c_str: The string to be added.
 *
 * Adds the @str to the end of the @string.
 *
 * Returns: `TRUE` on success.
 **/
bool string_push_back(String *string, const char *c_str);

/**
 * string_push_back_len:
 * @string: The string where to add the other string.
 * @str: The string to be added (can be `NULL`).
 * @len: The number of bytes in the @str to use.
 *
 * Adds @len bytes of the @str to the end of the @string.
 * If @str is `NULL`, adds @len zeros to the end of the @string.
 *
 * Returns: `TRUE` on success.
 **/
bool string_push_back_len(String *string, const char *str, usize len);

/**
 * string_push_back_c:
 * @string: The string where to add the character.
 * @c: The character to be added.
 *
 * Adds the character to the end of the string.
 *
 * Returns: `TRUE` on success.
 **/
bool string_push_back_c(String *string, char c);

/**
 * string_push_back_fmt:
 * @string: The string where to add the formatted string
 * @fmt: The format of the string to be added.
 * @...: Arguments.
 *
 * Adds the formatted string to the end of the @string.
 *
 * Returns: `TRUE` on success.
 **/
bool string_push_back_fmt(String *string, const char *fmt, ...);

/**
 * string_push_back_vfmt:
 * @string: The string where to add the formatted string
 * @fmt: The format of the string to be added.
 * @args The argument list.
 *
 * Adds the formatted string to the end of the @string.
 *
 * Returns: `TRUE` on success.
 **/
bool string_push_back_vfmt(String *string, const char *fmt, va_list args);

/**
 * string_push_front:
 * @string: The string where to add the other string.
 * @c_str: The string to be added.
 *
 * Adds the @str to the beginning of the @string.
 *
 * Returns: `TRUE` on success.
 **/
bool string_push_front(String *string, const char *c_str);

/**
 * string_push_front_len:
 * @string: The string where to add the other string.
 * @str: The string to be added (can be `NULL`).
 * @len: The number of bytes in @str to use.
 *
 * Adds @len bytes of the @str to the beginning of the @string.
 * If @str is `NULL`, adds @len zeros to the beginning of the @string.
 *
 * Returns: `TRUE` on success.
 **/
bool string_push_front_len(String *string, const char *str, usize len);

/**
 * string_push_front_c:
 * @string: The string where to add the character.
 * @c: The character to be added.
 *
 * Adds the character to the beginning of the string.
 *
 * Returns: `TRUE` on success.
 **/
bool string_push_front_c(String *string, char c);

/**
 * string_push_front_fmt:
 * @string: The string where to add the formatted string
 * @fmt: The format of the string to be added.
 * @...: Arguments.
 *
 * Adds the formatted string to the beginning of the @string.
 *
 * Returns: `TRUE` on success.
 **/
bool string_push_front_fmt(String *string, const char *fmt, ...);

/**
 * string_push_front_vfmt:
 * @string: The string where to add the formatted string
 * @fmt: The format of the string to be added.
 * @args: The argument list.
 *
 * Adds the formatted string to the beginning of the @string.
 *
 * Returns: `TRUE` on success.
 **/
bool string_push_front_vfmt(String *string, const char *fmt, va_list args);

/**
 * string_insert:
 * @string: The string where to insert the other string.
 * @index: The position in the @string where the @str should be inserted.
 * @c_str: The string to be inserted.
 *
 * Inserts the @str into the @string at the given index.
 *
 * Returns: `TRUE` on success.
 **/
bool string_insert(String *string, usize index, const char *c_str);

/**
 * string_insert_len:
 * @string: The string where to insert the other string.
 * @index: The position in the @string where the @str should be inserted.
 * @str: The string to be inserted (can be `NULL`).
 * @len: The number of bytes in the @str to use.
 *
 * Inserts @len bytes of the @str into the @string at the given index.
 * If @str is `NULL`, inserts @len zeros into the @string.
 *
 * Returns: `TRUE` on success.
 **/
bool string_insert_len(String *string, usize index, const char *str, usize len);

/**
 * string_insert_c:
 * @string: The string where to insert the character.
 * @index: The position in the @string where the @c should be inserted.
 * @c: The character to be inserted.
 *
 * Inserts the character into the string at the given index.
 *
 * Returns: `TRUE` on success.
 **/
bool string_insert_c(String *string, usize index, char c);

/**
 * string_insert_fmt:
 * @string: The string where to insert the formatted string.
 * @index: The position in the @string where the formatted string should be inserted.
 * @fmt: The format of the string to be inserted.
 * @...: Arguments.
 *
 * Inserts the formatted string into the @string at the given index.
 *
 * Returns: `TRUE` on success.
 **/
bool string_insert_fmt(String *string, usize index, const char *fmt, ...);

/**
 * string_insert_vfmt:
 * @string: The string where to add the formatted string
 * @index: The position in the @string where the formatted string should be inserted.
 * @fmt: The format of the string to be inserted.
 * @args: The argument list.
 *
 * Inserts the formatted string into the @string at the given index.
 *
 * Returns: `TRUE` on success.
 **/
bool string_insert_vfmt(String *string, usize index, const char *fmt, va_list args);

/**
 * string_get:
 * @string: The string where to get.
 * @index: The index of the first byte to be got.
 * @len: The number of bytes to be got.
 * @ret: The pointer to retrieve the data (can be `NULL`).
 *
 * Gets @len bytes in the string starting from @index.
 *
 * Returns: `TRUE` on success.
 **/
bool string_get(String *string, usize index, usize len, char *ret);

/**
 * string_get_c:
 * @string: The string where to get.
 * @index: The position of the character.
 * @ret: The pointer to retrieve the character.
 *
 * Gets the character from the string.
 *
 * Returns: `TRUE` on success.
 **/
bool string_get_c(String *string, usize index, char *ret);

/**
 * string_overwrite:
 * @string: The string where to overwrite.
 * @index: The index of the first byte to be overwriten.
 * @c_str: The string that should overwrite some part of the @string.
 *
 * Overwrites the @string at the given index with the @str.
 *
 * Returns: `TRUE` on success.
 **/
bool string_overwrite(String *string, usize index, const char *c_str);

/**
 * string_overwrite_len:
 * @string: The string where to overwrite.
 * @index: The index of the first byte to be overwriten.
 * @str: The string that should overwrite some part of the @string (can be `NULL`).
 * @len: The number of bytes in the @str to use.
 *
 * Overwrites @len bytes in the @string at the given index with the @str.
 * If @str is `NULL`, overwrites @len bytes in the @string with zeroes.
 *
 * Returns: `TRUE` on success.
 **/
bool string_overwrite_len(String *string, usize index, const char *str, usize len);

/**
 * string_overwrite_c:
 * @string: The string where to overwrite.
 * @index: The index of the character to be overwriten.
 * @c: The character the should overwrite some character in the @string.
 *
 * Overwrites the character in the @string at the given index with @c.
 *
 * Returns: `TRUE` on success.
 **/
bool string_overwrite_c(String *string, usize index, char c);

/**
 * string_overwrite_fmt:
 * @string: The string where to overwrite.
 * @index: The index of the first byte to be overwriten.
 * @fmt: The format of the string that should overwrite some part of the @string.
 * @...: Arguments.
 *
 * Overwrites the @string at the given index with the formatted string.
 *
 * Returns: `TRUE` on success.
 **/
bool string_overwrite_fmt(String *string, usize index, const char *fmt, ...);

/**
 * string_overwrite_vfmt:
 * @string: The string where to overwrite.
 * @index: The index of the first byte to be overwriten.
 * @fmt: The format of the string that should overwrite some part of the @string.
 * @args: The argument list.
 *
 * Overwrites the @string at the given index with the formatted string.
 *
 * Returns: `TRUE` on success.
 **/
bool string_overwrite_vfmt(String *string, usize index, const char *fmt, va_list args);

/**
 * string_assign_len:
 * @string: The string which contents are to be replaced.
 * @c_str: The string that should replace contents of the @string.
 *
 * Replaces contents of the @string with the @str.
 *
 * Returns: `TRUE` on success.
 **/
bool string_assign(String *string, const char *str);

/**
 * string_assign_len:
 * @string: The string which contents are to be replaced.
 * @str: The string that should replace contents of the @string.
 * @len: The number of bytes in the @str to use.
 *
 * Replaces contents of the @string with @len bytes of the @str.
 *
 * Returns: `TRUE` on success.
 **/
bool string_assign_len(String *string, const char *str, usize len);

/**
 * string_erase:
 * @string: The string where to erase.
 * @index: The index of the first byte to be removed.
 * @len: The number of bytes to be removed.
 * @ret: The pointer to retrieve removed data (can be `NULL`).
 *
 * Erases @len bytes in the string starting from @index.
 *
 * Returns: `TRUE` on success.
 **/
bool string_erase(String *string, usize index, usize len, char *ret);

/**
 * string_erase_c:
 * @string: The string where to erase.
 * @index: The index of the character to be removed.
 * @ret: The pointer to retrieve removed character (can be `NULL`).
 *
 * Erases character in the string at the given index.
 *
 * Returns: `TRUE` on success.
 **/
bool string_erase_c(String *string, usize index, char *ret);

/**
 * string_cmp:
 * @a: The first string to be compared.
 * @b: The second string to be compared.
 *
 * Compares the first and second strings.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
i32 string_cmp(const String *a, const String *b);

/**
 * string_fmt:
 * @string: The string which contents are to be replaced.
 * @fmt: The format of the string that should replace contents of the @string.
 * @...: Arguments.
 *
 * Replaces contents of the @string with the formatted string.
 *
 * Returns: `TRUE` on success.
 **/
bool string_fmt(String *string, const char *fmt, ...);

/**
 * string_vfmt:
 * @string: The string which contents are to be replaced.
 * @fmt: The format of the string that should replace contents of the @string.
 * @args: The argument list.
 *
 * Replaces contents of the @string with the formatted string.
 *
 * Returns: `TRUE` on success.
 **/
bool string_vfmt(String *string, const char *fmt, va_list args);

/**
 * string_steal0:
 * @string: The string where to steal the buffer.
 * @ret: The pointer to retrieve the buffer.
 * @len: The pointer to retrieve the length of the string (can be `NULL`).
 * @to_copy: if `TRUE`, the string buffer will be copied to ret,
 *           otherwise pointer to the buffer will be returned.
 *
 * Steals the string buffer.
 * String's buffer becomes `NULL`.
 *
 * Returns: `TRUE` on success.
 **/
bool string_steal0(String *string, char **ret, usize *len, bool to_copy);

/**
 * string_steal:
 * @string: The string where to steal the buffer.
 * @ret: The pointer to retrieve the buffer.
 * @len: The pointer to retrieve the length of the string (can be `NULL`).
 * @to_copy: if `TRUE`, the string buffer will be copied to ret and then recreated,
 *           otherwise pointer to the buffer will be returned.
 *
 * Steals the string buffer.
 * String creates a new buffer.
 *
 * Returns: `TRUE` on success.
 **/
bool string_steal(String *string, char **ret, usize *len, bool to_copy);

/**
 * string_purge:
 * @string: The string which buffer is to be freed.
 *
 * Frees the string buffer.
 **/
void string_purge(String *string);

/**
 * string_free:
 * @string: The string to be freed.
 *
 * Frees the string completely.
 **/
void string_free(String *string);

#endif /* end of include guard: STRING_H_WW4E6R5D */
