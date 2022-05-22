#ifndef STRING_H_WW4E6R5D
#define STRING_H_WW4E6R5D

#include "Macros.h"
#include "Types.h"

#include <stdarg.h>

typedef struct _TrbString TrbString;

/**
 * TrbString:
 * @data: The string buffer.
 * @len: The string length.
 * @capacity: The capacity of the string buffer.
 *
 * A dynamic size string.
 *
 * This example shows how to init and print the string to stdout:
 * ```c
 * TrbString example_string;
 * trb_string_init(&example_string, "I am an example!");
 *
 * printf("%s\n", example_string.data);
 *
 * trb_string_destroy(&example_string);
 * ```
 *
 * You should get an output similar to the following:
 * ```
 * I am an example!
 * ```
 **/
struct _TrbString {
	char *data;
	usize len;
	usize capacity;
};

/**
 * trb_string_init0:
 * @self: (nullable): The pointer to the string to be initialized.
 *
 * Creates a new #TrbString.
 *
 * Returns: (nullable): A new #TrbString.
 * Can return %NULL if an alloction error occurs.
 **/
TrbString *trb_string_init0(TrbString *self);

/**
 * trb_string_init:
 * @self: (nullable): The pointer to the string to be initialized.
 * @c_str: The inital contents of the #TrbString.
 *
 * Creates a new #TrbString from @str.
 *
 * Returns: (nullable): A new #TrbString.
 * Can return %NULL if an alloction error occurs.
 **/
TrbString *trb_string_init(TrbString *self, const char *c_str);

/**
 * trb_string_init_len:
 * @self: (nullable): The pointer to the string to be initialized.
 * @str: The initial contents of the #TrbString.
 * @len: The number of bytes in the @str to use.
 *
 * Creates a new #TrbString with @len bytes of the @str.
 *
 * Returns: (nullable): A new #TrbString.
 * Can return %NULL if an alloction error occurs.
 **/
TrbString *trb_string_init_len(TrbString *self, const char *str, usize len);

/**
 * trb_string_init_sized:
 * @self: (nullable): The pointer to the string to be initialized.
 * @cap: The initial capacity of the string.
 *
 * Creates a new #TrbString with capacity of @cap bytes.
 *
 * Returns: (nullable): A new #TrbString.
 * Can return %NULL if an alloction error occurs.
 **/
TrbString *trb_string_init_sized(TrbString *self, usize cap);

/**
 * trb_string_init_fmt:
 * @self: (nullable): The pointer to the string to be initialized.
 * @fmt: The format of the string that wiil be used as the initial contents of the #TrbString.
 * @...: Arguments.
 *
 * Creates a new #TrbString from the formatted string.
 *
 * Returns: (nullable): A new #TrbString
 * Can return %NULL if an allocate error occurs.
 **/
TrbString *trb_string_init_fmt(TrbString *self, const char *fmt, ...) TRB_FORMAT(printf, 2, 3);

/**
 * trb_string_init_vfmt:
 * @self: (nullable): The pointer to the string to be initialized.
 * @fmt: The format of the string that will be used as the initial contents of the #TrbString.
 * @args: The argument list.
 *
 * Creates a new #TrbString from the formatted string.
 *
 * Returns: (nullable): A new #TrbString.
 * Can return %NULL if an alloction error occurs.
 **/
TrbString *trb_string_init_vfmt(TrbString *self, const char *fmt, va_list args) TRB_FORMAT(printf, 2, 0);

/**
 * trb_string_push_back:
 * @self: The string where to add the other string.
 * @c_str: The string to be added.
 *
 * Adds the @str to the end of the @self.
 *
 * Returns: %TRUE on success.
 **/
bool trb_string_push_back(TrbString *self, const char *c_str);

/**
 * trb_string_push_back_len:
 * @self: The string where to add the other string.
 * @str: The string to be added.
 * @len: The number of bytes in the @str to use.
 *
 * Adds @len bytes of the @str to the end of the @self.
 * If @str is %NULL, adds @len zeros to the end of the @self.
 *
 * Returns: %TRUE on success.
 **/
bool trb_string_push_back_len(TrbString *self, const char *str, usize len);

/**
 * trb_string_push_back_c:
 * @self: The string where to add the character.
 * @c: The character to be added.
 *
 * Adds the character to the end of the string.
 *
 * Returns: %TRUE on success.
 **/
bool trb_string_push_back_c(TrbString *self, char c);

/**
 * trb_string_push_back_fmt:
 * @self: The string where to add the formatted string
 * @fmt: The format of the string to be added.
 * @...: Arguments.
 *
 * Adds the formatted string to the end of the @self.
 *
 * Returns: %TRUE on success.
 **/
bool trb_string_push_back_fmt(TrbString *self, const char *fmt, ...) TRB_FORMAT(printf, 2, 3);

/**
 * trb_string_push_back_vfmt:
 * @self: The string where to add the formatted string
 * @fmt: The format of the string to be added.
 * @args: The argument list.
 *
 * Adds the formatted string to the end of the @self.
 *
 * Returns: %TRUE on success.
 **/
bool trb_string_push_back_vfmt(TrbString *self, const char *fmt, va_list args) TRB_FORMAT(printf, 2, 0);

/**
 * trb_string_push_front:
 * @self: The string where to add the other string.
 * @c_str: The string to be added.
 *
 * Adds the @str to the beginning of the @self.
 *
 * Returns: %TRUE on success.
 **/
bool trb_string_push_front(TrbString *self, const char *c_str);

/**
 * trb_string_push_front_len:
 * @self: The string where to add the other string.
 * @str: The string to be added.
 * @len: The number of bytes in @str to use.
 *
 * Adds @len bytes of the @str to the beginning of the @self.
 * If @str is %NULL, adds @len zeros to the beginning of the @self.
 *
 * Returns: %TRUE on success.
 **/
bool trb_string_push_front_len(TrbString *self, const char *str, usize len);

/**
 * trb_string_push_front_c:
 * @self: The string where to add the character.
 * @c: The character to be added.
 *
 * Adds the character to the beginning of the string.
 *
 * Returns: %TRUE on success.
 **/
bool trb_string_push_front_c(TrbString *self, char c);

/**
 * trb_string_push_front_fmt:
 * @self: The string where to add the formatted string
 * @fmt: The format of the string to be added.
 * @...: Arguments.
 *
 * Adds the formatted string to the beginning of the @self.
 *
 * Returns: %TRUE on success.
 **/
bool trb_string_push_front_fmt(TrbString *self, const char *fmt, ...) TRB_FORMAT(printf, 2, 3);

/**
 * trb_string_push_front_vfmt:
 * @self: The string where to add the formatted string
 * @fmt: The format of the string to be added.
 * @args: The argument list.
 *
 * Adds the formatted string to the beginning of the @self.
 *
 * Returns: %TRUE on success.
 **/
bool trb_string_push_front_vfmt(TrbString *self, const char *fmt, va_list args) TRB_FORMAT(printf, 2, 0);

/**
 * trb_string_insert:
 * @self: The string where to insert the other string.
 * @index: The position in the @self where the @str should be inserted.
 * @c_str: The string to be inserted.
 *
 * Inserts the @str into the @self at the given index.
 *
 * Returns: %TRUE on success.
 **/
bool trb_string_insert(TrbString *self, usize index, const char *c_str);

/**
 * trb_string_insert_len:
 * @self: The string where to insert the other string.
 * @index: The position in the @self where the @str should be inserted.
 * @str: The string to be inserted.
 * @len: The number of bytes in the @str to use.
 *
 * Inserts @len bytes of the @str into the @self at the given index.
 * If @str is %NULL, inserts @len zeros into the @self.
 *
 * Returns: %TRUE on success.
 **/
bool trb_string_insert_len(TrbString *self, usize index, const char *str, usize len);

/**
 * trb_string_insert_c:
 * @self: The string where to insert the character.
 * @index: The position in the @self where the @c should be inserted.
 * @c: The character to be inserted.
 *
 * Inserts the character into the string at the given index.
 *
 * Returns: %TRUE on success.
 **/
bool trb_string_insert_c(TrbString *self, usize index, char c);

/**
 * trb_string_insert_fmt:
 * @self: The string where to insert the formatted string.
 * @index: The position in the @self where the formatted string should be inserted.
 * @fmt: The format of the string to be inserted.
 * @...: Arguments.
 *
 * Inserts the formatted string into the @self at the given index.
 *
 * Returns: %TRUE on success.
 **/
bool trb_string_insert_fmt(TrbString *self, usize index, const char *fmt, ...) TRB_FORMAT(printf, 3, 4);

/**
 * trb_string_insert_vfmt:
 * @self: The string where to add the formatted string
 * @index: The position in the @self where the formatted string should be inserted.
 * @fmt: The format of the string to be inserted.
 * @args: The argument list.
 *
 * Inserts the formatted string into the @self at the given index.
 *
 * Returns: %TRUE on success.
 **/
bool trb_string_insert_vfmt(TrbString *self, usize index, const char *fmt, va_list args) TRB_FORMAT(printf, 3, 0);

/**
 * trb_string_get:
 * @self: The string where to get.
 * @index: The index of the first byte to be got.
 * @len: The number of bytes to be got.
 * @ret: (optional) (out): The pointer to retrieve the data.
 *
 * Gets @len bytes in the string starting from @index.
 *
 * Returns: %TRUE on success.
 **/
bool trb_string_get(TrbString *self, usize index, usize len, char *ret);

/**
 * trb_string_get_c:
 * @self: The string where to get.
 * @index: The position of the character.
 * @ret: (out): The pointer to retrieve the character.
 *
 * Gets the character from the string.
 *
 * Returns: %TRUE on success.
 **/
bool trb_string_get_c(TrbString *self, usize index, char *ret);

/**
 * trb_string_overwrite:
 * @self: The string where to overwrite.
 * @index: The index of the first byte to be overwriten.
 * @c_str: The string that should overwrite some part of the @self.
 *
 * Overwrites the @self at the given index with the @str.
 *
 * Returns: %TRUE on success.
 **/
bool trb_string_overwrite(TrbString *self, usize index, const char *c_str);

/**
 * trb_string_overwrite_len:
 * @self: The string where to overwrite.
 * @index: The index of the first byte to be overwriten.
 * @str: The string that should overwrite some part of the @self.
 * @len: The number of bytes in the @str to use.
 *
 * Overwrites @len bytes in the @self at the given index with the @str.
 * If @str is %NULL, overwrites @len bytes in the @self with zeroes.
 *
 * Returns: %TRUE on success.
 **/
bool trb_string_overwrite_len(TrbString *self, usize index, const char *str, usize len);

/**
 * trb_string_overwrite_c:
 * @self: The string where to overwrite.
 * @index: The index of the character to be overwriten.
 * @c: The character the should overwrite some character in the @self.
 *
 * Overwrites the character in the @self at the given index with @c.
 *
 * Returns: %TRUE on success.
 **/
bool trb_string_overwrite_c(TrbString *self, usize index, char c);

/**
 * trb_string_overwrite_fmt:
 * @self: The string where to overwrite.
 * @index: The index of the first byte to be overwriten.
 * @fmt: The format of the string that should overwrite some part of the @self.
 * @...: Arguments.
 *
 * Overwrites the @self at the given index with the formatted string.
 *
 * Returns: %TRUE on success.
 **/
bool trb_string_overwrite_fmt(TrbString *self, usize index, const char *fmt, ...) TRB_FORMAT(printf, 3, 4);

/**
 * trb_string_overwrite_vfmt:
 * @self: The string where to overwrite.
 * @index: The index of the first byte to be overwriten.
 * @fmt: The format of the string that should overwrite some part of the @self.
 * @args: The argument list.
 *
 * Overwrites the @self at the given index with the formatted string.
 *
 * Returns: %TRUE on success.
 **/
bool trb_string_overwrite_vfmt(TrbString *self, usize index, const char *fmt, va_list args) TRB_FORMAT(printf, 3, 0);

/**
 * trb_string_assign:
 * @self: The string which contents are to be replaced.
 * @c_str: The string that should replace contents of the @self.
 *
 * Replaces contents of the @self with the @str.
 *
 * Returns: %TRUE on success.
 **/
bool trb_string_assign(TrbString *self, const char *c_str);

/**
 * trb_string_assign_len:
 * @self: The string which contents are to be replaced.
 * @str: The string that should replace contents of the @self.
 * @len: The number of bytes in the @str to use.
 *
 * Replaces contents of the @self with @len bytes of the @str.
 *
 * Returns: %TRUE on success.
 **/
bool trb_string_assign_len(TrbString *self, const char *str, usize len);

/**
 * trb_string_erase:
 * @self: The string where to erase.
 * @index: The index of the first byte to be removed.
 * @len: The number of bytes to be removed.
 * @ret: (optional) (out): The pointer to retrieve removed data.
 *
 * Erases @len bytes in the string starting from @index.
 *
 * Returns: %TRUE on success.
 **/
bool trb_string_erase(TrbString *self, usize index, usize len, char *ret);

/**
 * trb_string_erase_c:
 * @self: The string where to erase.
 * @index: The index of the character to be removed.
 * @ret: (optional) (out): The pointer to retrieve removed character.
 *
 * Erases character in the string at the given index.
 *
 * Returns: %TRUE on success.
 **/
bool trb_string_erase_c(TrbString *self, usize index, char *ret);

/**
 * trb_string_cmp:
 * @a: The first string to be compared.
 * @b: The second string to be compared.
 *
 * Compares the first and second strings.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
i32 trb_string_cmp(const TrbString *a, const TrbString *b);

/**
 * trb_string_assign_fmt:
 * @self: The string which contents are to be replaced.
 * @fmt: The format of the string that should replace contents of the @self.
 * @...: Arguments.
 *
 * Replaces contents of the @self with the formatted string.
 *
 * Returns: %TRUE on success.
 **/
bool trb_string_assign_fmt(TrbString *self, const char *fmt, ...) TRB_FORMAT(printf, 2, 3);

/**
 * trb_string_assign_vfmt:
 * @self: The string which contents are to be replaced.
 * @fmt: The format of the string that should replace contents of the @self.
 * @args: The argument list.
 *
 * Replaces contents of the @self with the formatted string.
 *
 * Returns: %TRUE on success.
 **/
bool trb_string_assign_vfmt(TrbString *self, const char *fmt, va_list args) TRB_FORMAT(printf, 2, 0);

/**
 * trb_string_steal0:
 * @self: The string where to steal the buffer.
 * @len: (optional) (out): The pointer to retrieve the length of the string.
 *
 * Steals the string buffer.
 * TrbString's buffer becomes %NULL.
 *
 * Returns: (transfer full) (nullable): The buffer on success.
 * Can return %NULL on failure.
 **/
char *trb_string_steal0(TrbString *self, usize *len);

/**
 * trb_string_steal:
 * @self: The string where to steal the buffer.
 * @len: (optional) (out): The pointer to retrieve the length of the string.
 *
 * Steals the string buffer.
 * TrbString creates a new buffer.
 *
 * Returns: (transfer full) (nullable): The buffer on success.
 * Can return %NULL on failure.
 **/
char *trb_string_steal(TrbString *self, usize *len);

/**
 * trb_string_destroy:
 * @self: The string which buffer is to be freed.
 *
 * Frees the string buffer.
 **/
void trb_string_destroy(TrbString *self);

/**
 * trb_string_free:
 * @self: The string to be freed.
 *
 * Frees the string completely.
 **/
void trb_string_free(TrbString *self);

/**
 * trb_string_copy:
 * @dst: The pointer to the destination string.
 * @src: The string to be copied.
 *
 * Creates a copy of the string.
 *
 * Returns: (nullable): A copy of the array.
 * Can return %NULL if an allocation error occurs.
 **/
TrbString *trb_string_copy(TrbString *dst, const TrbString *src);

#endif /* end of include guard: STRING_H_WW4E6R5D */
