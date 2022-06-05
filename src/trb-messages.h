#ifndef MESSAGES_H_V1NHZAPH
#define MESSAGES_H_V1NHZAPH

#include "trb-macros.h"
#include "trb-types.h"

/**
 * SECTION: Messages
 *
 * Message levels:
 * * ERROR - Something "serious" has gone wrong. But the program can recover from it.
 * * CRITICAL - Same as "ERROR", but program can't recover from it.
 * * WARN - Something has not worked as it should.
 * * INFO - The text is of a purely informative nature.
 * * PRINT - Print some message.
 * * DEBUG - Debug.
 **/

typedef enum {
	TRB_MESSAGE_ERROR,
	TRB_MESSAGE_CRITICAL,
	TRB_MESSAGE_WARNING,
	TRB_MESSAGE_PRINT,
	TRB_MESSAGE_INFO,
	TRB_MESSAGE_DEBUG
} TrbMessageFlags;

/**
 * trb_message:
 * @flag: The type of message.
 * @msg: The format string.
 * @...: Arguments.
 *
 * Prints a message.
 **/
void trb_message(TrbMessageFlags flag, const char *msg, ...) TRB_FORMAT(printf, 2, 3);

void trb_message_func(TrbMessageFlags flag, const char *file, usize line, const char *func, const char *msg, ...) TRB_FORMAT(printf, 5, 6);

void trb_return_if_fail_warning(const char *file, usize line, const char *func, const char *expr);

void trb_exit_if_fail_critical(const char *file, usize line, const char *func, const char *expr);

/**
 * trb_msg_error:
 * @fmt: The format string.
 * @...: Arguments.
 *
 * Prints an error message. The message is printed out to stderr.
 **/
#define trb_msg_error(fmt...) (trb_message_func(TRB_MESSAGE_ERROR, TRB_STRFILE, TRB_USIZELINE, TRB_STRFUNC, fmt))

/**
 * trb_msg_warn:
 * @fmt: The format string.
 * @...: Arguments.
 *
 * Prints a warning message. The message is printed out to stderr.
 **/
#define trb_msg_warn(fmt...) (trb_message_func(TRB_MESSAGE_WARNING, TRB_STRFILE, TRB_USIZELINE, TRB_STRFUNC, fmt))

/**
 * trb_msg_critical:
 * @fmt: The format string.
 * @...: Arguments.
 *
 * Prints a critical message. The message is printed out to stderr.
 **/
#define trb_msg_critical(fmt...) (trb_message_func(TRB_MESSAGE_CRITICAL, TRB_STRFILE, TRB_USIZELINE, TRB_STRFUNC, fmt))

/**
 * trb_msg_info:
 * @fmt: The format string.
 * @...: Arguments.
 *
 * Prints an infomation message. The message is printed out to stdout.
 **/
#define trb_msg_info(fmt...) (trb_message_func(TRB_MESSAGE_INFO, TRB_STRFILE, TRB_USIZELINE, TRB_STRFUNC, fmt))

/**
 * trb_msg_debug:
 * @fmt: The format string.
 * @...: Arguments.
 *
 * Prints a debug message. The message is printed out to stdout.
 **/
#define trb_msg_debug(fmt...) (trb_message_func(TRB_MESSAGE_DEBUG, TRB_STRFILE, TRB_USIZELINE, TRB_STRFUNC, fmt))

/**
 * trb_msg_print:
 * @fmt: The format string.
 * @...: Arguments.
 *
 * Prints a message. The message is printed out to stdout.
 **/
#define trb_msg_print(fmt...) (trb_message_func(TRB_MESSAGE_PRINT, TRB_STRFILE, TRB_USIZELINE, TRB_STRFUNC, fmt))

#define trb_return_if_fail(expr)                                                    \
	if (!(expr)) {                                                                  \
		trb_return_if_fail_warning(TRB_STRFILE, TRB_USIZELINE, TRB_STRFUNC, #expr); \
		return;                                                                     \
	}

#define trb_return_val_if_fail(expr, val)                                           \
	if (!(expr)) {                                                                  \
		trb_return_if_fail_warning(TRB_STRFILE, TRB_USIZELINE, TRB_STRFUNC, #expr); \
		return (val);                                                               \
	}

#define trb_do_if_fail(expr) \
	if (!(expr) && (trb_return_if_fail_warning(TRB_STRFILE, TRB_USIZELINE, TRB_STRFUNC, #expr), 1))

#define trb_exit_if_fail(expr)                                                     \
	if (!(expr)) {                                                                 \
		trb_exit_if_fail_critical(TRB_STRFILE, TRB_USIZELINE, TRB_STRFUNC, #expr); \
		exit(EXIT_FAILURE);                                                        \
	}

#define trb_exit_with_code_if_fail(expr, code)                                     \
	if (!(expr)) {                                                                 \
		trb_exit_if_fail_critical(TRB_STRFILE, TRB_USIZELINE, TRB_STRFUNC, #expr); \
		exit(code);                                                                \
	}

#endif /* end of include guard: MESSAGES_H_V1NHZAPH */
