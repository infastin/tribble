#ifndef STRING_H_WW4E6R5D
#define STRING_H_WW4E6R5D

#include "Types.h"

typedef struct _String String;

struct _String {
	char *data;
	usize len;
	usize capacity;
};

/**
 * string_init:
 * @string: The pointer to the string to be initialized (can be `NULL`).
 * @str: The initial contents of the string.
 * @len: The length of the @str to use.
 *
 * Creates a new #String with @len bytes of the @str.
 *
 * Returns: A new #String.
 **/
String *string_init(String *string, const char *str, usize len);

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
 * string_push_back:
 * @string: The string where to add the other string.
 * @str: The string to be added (can be `NULL`).
 * @len: The length of the @str to use.
 *
 * Adds @len bytes of the @str to the end of the @string.
 *
 * Returns: `TRUE` on success.
 **/
bool string_push_back(String *string, const char *str, usize len);

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
 * string_push_front:
 * @string: The string where to add the other string.
 * @str: The string to be added (can be `NULL`).
 * @len: The length of the @str to use.
 *
 * Adds @len bytes of the @str to the beginning of the @string.
 *
 * Returns: `TRUE` on success.
 **/
bool string_push_front(String *string, const char *str, usize len);

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
 * string_insert:
 * @string: The string where to insert the other string.
 * @str: The string to be inserted (can be `NULL`).
 * @len: The length of the @str to use.
 *
 * Inserts @len bytes of the @str to the @string at the given index.
 *
 * Returns: `TRUE` on success.
 **/
bool string_insert(String *string, usize index, const char *str, usize len);

/**
 * string_insert_c:
 * @string: The string where to insert the character.
 * @c: The character to be inserted.
 *
 * Inserts the character to the string at the given index.
 *
 * Returns: `TRUE` on success.
 **/
bool string_insert_c(String *string, usize index, char c);

bool string_overwrite(String *string, usize index, const char *str, usize len);

bool string_overwrite_c(String *string, usize index, char c);

bool string_assign(String *string, const char *str, usize len);

bool string_remove(String *string, usize index, usize len, char *ret);

bool string_remove_c(String *string, usize index, char *ret);

i32 string_cmp(const String *a, const String *b);

#endif /* end of include guard: STRING_H_WW4E6R5D */
