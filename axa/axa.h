/*
 * Advanced Exchange Access (AXA) definitions
 *
 *  Copyright (c) 2014 by Farsight Security, Inc.
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

#ifndef AXA_AXA_H
#define AXA_AXA_H

/*! \file axa.h
 *  \brief Top-level interface specification for libaxa
 *
 *  This file contains AXA macros, datatype definitions and function
 *  declarations.
 */

#include <inttypes.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>


/**
 *  Return the number of elements in an array
 *
 *  \param[in] _a the array to size up
 *
 *  \return the size of the array as an int instead of size_t
 */
#define AXA_DIM(_a)    ((int)((sizeof(_a) / sizeof((_a)[0]))))

/**
 *  Return a pointer to the last item of an array
 *
 *  \param[in] _a the array containing the item
 *
 *  \return a pointer to the last item
 */
#define AXA_LAST(_a)    (&(_a)[AXA_DIM(_a)-1])

/** @cond */
/* Produce a pointer to a byte in a buffer that corresponds to a structure
 * tag.  This works where a cast to the structure would not work or
 * would give the wrong answer because of word mis-alignment of the buffer
 * or other reasons. */
#define AXA_OFFSET(_p,_s,_t)  ((uint8_t *)(_p)				\
			       + ((uint8_t *)&((_s *)0)->_t  - (uint8_t *)0))

/** @endcond */

/**
 *  Test a char to see if it's whitespace. AXA ignores locales to get
 *  consistent results on all systems, and because the AXA control files are
 *  ASCII.
 *
 *  \param[in] c char to test
 *
 *  \retval 1 if whitespace
 *  \retval 0 if not
 */
#define AXA_IS_WHITE(c) ({char _c = (c); _c == ' ' || _c == '\t'	\
				      || _c == '\r' || _c == '\n';})
/** @cond */
#define AXA_WHITESPACE	" \t\n\r"
/** @endcond */

/**
 *  test if char is uppercase
 *
 *  \param[in] c char to test
 *
 *  \retval 1 if uppercase char
 *  \retval 0 if not
 */
#define AXA_IS_UPPER(c) ({char _c = (c); _c >= 'A' && _c <= 'Z';})

/**
 *  test if char is lowercase
 *
 *  \param[in] c char to test
 *
 *  \retval 1 if lowercase
 *  \retval 0 if not
 */
#define AXA_IS_LOWER(c) ({char _c = (c); _c >= 'a' && _c <= 'z';})

/**
 *  test if char is base 10 digit
 *
 *  \param[in] c char to test
 *
 *  \retval 1 if base 10 digit
 *  \retval 0 if not
 */
#define AXA_IS_DIGIT(c) ({char _c = (c); _c >= '0' && _c <= '9';})

/**
 *  convert char to lowercase
 *
 *  \param[in] c char to convert
 *
 *  \return the converted char if it was previously uppercase, c if not
 */
#define AXA_TO_LOWER(c) ({char _l = (c); AXA_IS_UPPER(_l) ? (_l+'a'-'A') : _l;})

/**
 *  convert char to uppercase
 *
 *  \param[in] c char to convert
 *
 *  \return the converted char if it was previously lowercase, c if not
 */
#define AXA_TO_UPPER(c) ({char _u = (c); AXA_IS_LOWER(_u) ? (_u-'a'-'A') : _u;})

/**
 * Tell the compiler not to complain about an unused parameter.
 */
#define AXA_UNUSED	__attribute__((unused))

/**
 * Tell the compiler to check an actual format string against  the other
 * actual parameters.
 *
 * \param[in] f the number of the "format string" parameter
 * \param[in] l the number of the first variadic parameter
 */
#define AXA_PF(f,l)	__attribute__((format(printf,f,l)))

/** Tell the compiler that this function will never return */
#define	AXA_NORETURN    __attribute__((__noreturn__))


/** @cond */
/* Use local definitions of min() and max() because some UNIX-like systems
 * have less useful or hard to find definitions. */
#undef min
#define min(a,b) ({typeof(a) _a = (a); typeof(b)_b = (b); _a < _b ? _a : _b; })
#undef max
#define max(a,b) ({typeof(a) _a = (a); typeof(b) _b = (b); _a > _b ? _a : _b; })
/** @endcond */

/**
 *  Return the larger of two scalar values. This macro is for declarations
 *  where ({}) is not allowed and side effects can't happen.
 *
 *  \param[in] a first value to compare
 *  \param[in] b second value to compare
 *
 *  \return the larger of the two values, if they are equal, return a
 */
#define dcl_max(a,b) ((a) >= (b) ? (a) : (b))

/**
 *  Case compare two NULL terminated strings, comparing at most sizeof(_s - 1)
 *  characters.
 *
 *  \param[in] _b const char * first, not necessarily null terminated string
 *	to compare
 *  \param[in] _s const char * null terminated second string to compare
 *
 *  \return same semantics as strncasecmp()
 */
#define AXA_CLITCMP(_b,_s)  (strncasecmp((_b), (_s), sizeof(_s)-1) == 0)

/** for reference counting */
typedef int32_t		axa_ref_cnt_t;

/**
 *  Wrapper for compiler builtin __sync_and_add_fetch(c,1). This function
 *  atomically adds 1 to the variable that c points to. If the variable would
 *  be less than 0, AXA_ASSERT() will be called and the program will exit.
 *
 *  \param[in] c pointer to the variable to be incremented
 *
 *  \return the new value of what c points to
 */
#define AXA_INC_REF(c)	AXA_ASSERT(__sync_add_and_fetch(&(c), 1) > 0)

/**
 *  Wrapper for compiler builtin __sync_and_sub_fetch(c,1). This function
 *  atomically subtracts 1 from the variable that c points to. If the variable
 *  would be less than 0, AXA_ASSERT() will be called and the program will exit.
 *
 *  \param[in] c pointer to the variable to be decremented
 *
 *  \return the new value of what c points to
 */
#define AXA_DEC_REF(c)	AXA_ASSERT(__sync_sub_and_fetch(&(c), 1) >= 0)


/* domain_to_str.c */
/**
 *  Convert a domain name to a canonical string. Sane wrapper for
 *  wdns_domain_to_str(). dst_len must be >=NS_MAXDNAME because
 *  wdns_domain_to_str() does not check.
 *
 *  \param[in] src domain name in wire format
 *  \param[in] src_len length of domain name in bytes
 *  \param[out] dst caller allocated string buffer that should be of
 *	size NS_MAXDNAME
 *  \param[in] dst_len size of the dst buffer
 *
 *  \return the value of dst
 */
extern const char *axa_domain_to_str(const uint8_t *src, size_t src_len,
				     char *dst, size_t dst_len);


/* emsg.c */
/**
 *  A calloc() wrapper that crashes immediately (via AXA_ASSERT()) on malloc
 *  failures. The memory region will be zero-filled.
 *
 *  \param[in] s size of memory to allocate
 *
 *  \return pointer to the allocated memory
 */
extern void *axa_zalloc(size_t s);

/**
 *  A axa_zalloc() wrapper that returns zero-filled memory of size sizeof (t)
 *  that is cast to (t *).
 *
 *  \param[in] t object to allocate memory for
 *
 *  \return pointer to the allocated memory
 */
#define AXA_SALLOC(t) ((t *)axa_zalloc(sizeof(t)))

/**
 *  A strdup() wrapper that crashes immediately (via AXA_ASSERT()) on a strdup()
 *  failure (which should be ENOMEM). You should free the memory referenced by
 *  the string this function returns.
 *
 *  \param[in] s the string to duplicate
 *
 *  \return pointer to the duplicated string
 */
extern char *axa_strdup(const char *s);

/**
 *  A vasprintf() wrapper that crashes immediately (via AXA_ASSERT()) on
 *  vasprintf failures. When you're done with it, bufp should be subsequently
 *  freed.
 *
 *  \param[out] bufp a pointer to the newly minted and formated string
 *  \param[in] p the format string
 *  \param[in] args a var args list
 */
extern void axa_vasprintf(char **bufp, const char *p, va_list args);

/**
 *  An asprintf() wrapper that crashes immediately (via AXA_ASSERT()) on
 *  asprintf failures. When you're done with it, bufp should be subsequently
 *  freed.
 *
 *  \param[out] bufp a pointer to the newly minted and formated string
 *  \param[in] p the format string
 *  \param[in] ... a var args list
 */
extern void axa_asprintf(char **bufp, const char *p, ...) AXA_PF(2,3);

/** Try to enable core files. Wraps getrlimit() and setrlimit(). */
extern void axa_set_core(void);

/** AXA error message datatype */
typedef struct axa_emsg {
	char	c[120];			/**< error strings go here */
} axa_emsg_t;

/** AXA debug control.  0 is off.  More positive values generate more messages.
 * See #AXA_DEBUG_WATCH, #AXA_DEBUG_TRACE, and so forth */
extern uint axa_debug;

/** value for axa_debug to generate messages related to watches, anomaly
 *  modules, and channels */
#define AXA_DEBUG_WATCH	2
/** value for axa_debug to also generate messages about
 * AXA message sending and receiving */
#define AXA_DEBUG_TRACE	3
/** value for axa_debug to see nmsg related debugging messages */
#define AXA_DEBUG_NMSG	4

/** convert AXA debug level to nmsg debug level */
#define AXA_DEBUG_TO_NMSG() nmsg_set_debug(axa_debug <= AXA_DEBUG_NMSG	\
					   ? 0 : axa_debug - AXA_DEBUG_NMSG)

/** maximum value for axa_debug */
#define AXA_DEBUG_MAX	10

/**
 *  Set the global program name, the syslog logging stuff needs this to be
 *  called first.
 *  \param[in] me argv[0]
 */
extern void axa_set_me(const char *me);

/** AXA program name (should be set via axa_set_me()) */
extern char axa_prog_name[];

/**
 *  Parse log options string
 *  \param[in] arg CSV string with the following options:
 *  {trace|error|acct},{off|FACILITY.LEVEL}[,{none,stderr,stdout}]
 *  "trace", "error" and "acct" correspond to the members of axa_syslog_type_t.
 *  The following settings for "trace", "error" and "acct" are assumed:
 *	trace,LOG_DEBUG.LOG_DAEMON
 *	error.LOG_ERR,LOG_DAEMON
 *	acct,LOG_NOTICE.LOG_AUTH,none
 *
 *  \retval true if string is valid
 *  \retval false if not
 */
extern bool axa_parse_log_opt(const char *arg);

/** initialize the AXA syslog interface.
 *  Call this function after calling axa_parse_log_opt() and axa_set_me(),
 *  and before calling any AXA logging, accouting, or tracing function. */
extern void axa_syslog_init(void);

/**
 *  Add text to an error or other message buffer.
 *  If we run out of room, add "...".
 *
 *  \param[in,out] bufp in: the orignal string, out: the concatenated strings
 *  \param[in,out] buf_lenp in: the length of bufp string, out: new length
 *  \param[in] p the format string to copy over
 *  \param[in] ... va_args business
 */
extern void axa_buf_print(char **bufp, size_t *buf_lenp,
			  const char *p, ...) AXA_PF(3,4);

/** prevent surprises via use of stdio FDs by ensuring that the FDs are open
 * to at least /dev/null */
extern void axa_clean_stdio(void);

/**
 *  Generate an erorr message string in a buffer, if we have a buffer.
 *  Log or print the message with axa_vlog_msg() if there is no buffer.
 *
 *  \param[out] emsg if something goes wrong, this will contain the reason
 *  \param[in] msg message
 *  \param[in] args arguments to p
 */
extern void axa_vpemsg(axa_emsg_t *emsg, const char *msg, va_list args);

/**
 *  axa_vpemsg() wrapper using the variadic stdarg macros (va_start(),
 *  va_end()).
 *
 *  \param[out] emsg if something goes wrong, this will contain the reason
 *  \param[in] msg message
 *  \param[in] ... variable length argument list
 */
extern void axa_pemsg(axa_emsg_t *emsg, const char *msg, ...) AXA_PF(2,3);

/** AXA syslog types */
typedef enum {
	AXA_SYSLOG_TRACE=0,		/**< trace */
	AXA_SYSLOG_ERROR=1,		/**< error */
	AXA_SYSLOG_ACCT=2		/**< accounting */
} axa_syslog_type_t;

/**
 *  Log an AXA message. Depending on type and calls to axa_parse_log_opt(),
 *  this function could write to stdout stderr, and/or syslog.
 *
 *  \param[in] type one of axa_syslog_type_t
 *  \param[in] fatal if true and fatal verbiage will be prepended to message
 *  \param[in] p message
 *  \param[in] args variadic argument list
 */
extern void axa_vlog_msg(axa_syslog_type_t type, bool fatal,
			 const char *p, va_list args);

/**
 *  Log or print an error message.  This is a wrapper for axa_vlog_msg with
 *	type of AXA_SYSLOG_ERROR with fatal == false.
 *
 *  \param[in] p message
 *  \param[in] args variadic argument list
 */
extern void axa_verror_msg(const char *p, va_list args);

/**
 *  Log or print an error message.  This is a variadic wrapper for
 *	axa_vlog_msg with type of AXA_SYSLOG_ERROR with fatal == false.
 *
 *  \param[in] p message
 *  \param[in] ... variable length argument list
 */
extern void axa_error_msg(const char *p, ...) AXA_PF(1,2);

/**
 *  Log an error message for an I/O function that has returned either
 *	a negative read or write length or the wrong length..  Complain
 *	about a non-negative length or decode errno for a negative length.
 *
 *  \param[in] op canonical string referring to the I/O event that caused the
 *  error
 *  \param[in] src error message
 *  \param[in] len length of src
 */
extern void axa_io_error(const char *op, const char *src, ssize_t len);

/**
 *  Log a trace message in the tracing syslog stream as opposed to the
 *	error syslog stream.
 *
 *  \param[in] p message
 *  \param[in] args variadic argument list
 */
extern void axa_vtrace_msg(const char *p, va_list args);

/**
 *  Log a trace message in the tracing stream or AXA_SYSLOG_TRACE
 *  as opposed to the error or AXA_SYSLOG_ERROR stream.
 *
 *  \param[in] p message
 *  \param[in] ... variable length argument list
 */
extern void axa_trace_msg(const char *p, ...) AXA_PF(1,2);

/**
 *  Log a serious error message and either exit with a specified exit code
 *	or crash if the exit code is EX_SOFTWARE.
 *
 *  \param[in] ex_code exit code
 *  \param[in] p message
 *  \param[in] args variadic argument list
 */
extern void axa_vfatal_msg(int ex_code, const char *p, va_list args)
    AXA_NORETURN;

/**
 *  Log a serious error message and either exit with a specified exit code
 *	or crash if the exit code is EX_SOFTWARE.
 *
 *  \param[in] ex_code exit code
 *  \param[in] p our last words
 *  \param[in] ... va_args business
 */
extern void axa_fatal_msg(int ex_code, const char *p, ...)
    AXA_PF(2,3) AXA_NORETURN;

/**
 *  Get a logical line from a stdio stream, with leading and trailing
 *  whitespace trimmed and "\\\n" deleted as a continuation.
 *  If the buffer is NULL or it is not big enough, it is freed and a new
 *  buffer is allocated. This must be freed by the caller after the last use
 *  of this function.
 *
 *  Except at error or EOF, the start of the next line is returned,
 *  which might not be at the start of the buffer.
 *
 *  \param[in] f the file to read from (for error reporting)
 *  \param[in] file_name name of the file (for error reporting)
 *  \param[in,out] line_num line number of file, will be progressively updated
 *  \param[in,out] bufp buffer to store line, can be NULL
 *  \param[in,out] buf_sizep size of buffer pointed to by bufp
 *
 *  \return The return value is NULL and emsg->c[0]=='\0' at EOF or NULL and
 *  emsg->c[0]!='\0' after an error.
 */
extern char *axa_fgetln(FILE *f, const char *file_name, uint *line_num,
			char **bufp, size_t *buf_sizep);

/**
 *  Strip leading and trailing white space.
 *
 *  \param[in,out] str in: string to cleanse, out: cleansed string
 *  \param[in,out] lenp in: length of the string, out: new length
 *
 *  \return the string cleansed of whitespace
 */
extern const char *axa_strip_white(const char *str, size_t *lenp);

/**
 *  Copy the next token from a string to a buffer and return the
 *  size of the string put into the buffer.
 *  Honor quotes and backslash.
 *  The caller must skip leading token separators (e.g. blanks) if necessary.
 *  When the separators include whitespace and whitespace ends the token,
 *  then all trailing whitespace is skipped.
 *
 *  \param[in,out] token token goes here
 *  \param[in,out] token_len length of the token
 *  \param[in,out] stringp input string
 *  \param[in,out] seps string of token separators
 *
 *  \return the size of the string, -1 on error
 */
extern ssize_t axa_get_token(char *token, size_t token_len,
			     const char **stringp, const char *seps);

/**
 *  Assert and bail if false.
 *  Macro checks if c is true, if so it does nothing, if c is false it
 *  calls axa_fatal_msg() with an exit code of 0 and the supplied variadic
 *  arguments.
 *
 *  \param[in] c condition to assert
 *  \param[in] ... variadic arguments (should be a string and any trailing
 *  parameters)
 *
 *  \retval 0 if c is true (otherwise call axa_fatal_msg() and exit)
 */
#define AXA_ASSERT_MSG(c,...) ((c) ? 0 : axa_fatal_msg(0, __VA_ARGS__))

/**
 *  Wrapper for AXA_ASSERT_MSG() with the string "<condition> is false".
 *
 *  \param[in] c condition to assert
 *
 *  \retval 0 if c is true (otherwise call axa_fatal_msg() and exit)
 */
#define AXA_ASSERT(c) AXA_ASSERT_MSG((c), "\""#c"\" is false")

/**
 *  Wrapper for axa_fatal_msg() with an exit code of 0 and the supplied
 *  variadic arguments.
 *
 *  \param[in] ... variadic arguments
 */
#define AXA_FAIL(...) axa_fatal_msg(0, __VA_ARGS__)


/* hash_divisor.c */
/**
 *  Get a modulus for a hash function that is tolerably likely to be
 *  relatively prime to most inputs.  We get a prime for initial values
 *  not larger than 1 million.  We often get a prime after that.
 *  This works well in practice for hash tables up to at least 100 million
 *  and better than a multiplicative hash.
 *
 *  The algorithm starts by finding either the smallest prime number that
 *  is larger than the initial parameter value and not larger than 1009 or
 *  the largest prime smaller than the initial value and not larger than 1009.
 *  The algorithm is finished if the initial value is at most 1009.
 *  Otherwise, it finds the smallest (or largest) number that is relatively
 *  prime to all prime numbers <=1009.
 *
 *  \param[in] initial is the starting point for searching for number with no
 *	small divisors. That is usually the previous size of an expanding
 *	hash table or the initial guess for a new hash table.
 *  \param[in] smaller false if you want a value smaller than the initial
 *	value.
 *
 *  \return the modulus
 */
extern uint32_t axa_hash_divisor(uint32_t initial, bool smaller);

/* parse_ch.c */
/**
 *  Parse a channel string into a binary channel in host byte order
 *
 *  \param[out] emsg if something goes wrong, this will contain the reason
 *  \param[out] chp pointer to a axa_p_ch_t channel in host byte order
 *  \param[in,out] str string containing channel number or "all" keyword
 *  \param[in,out] str_len length of str
 *  \param[in] all_ok boolean indicating "all" is allowed
 *  \param[in] number_ok boolean indicating "202 is the same as ch202"
 *
 *  \retval true if no errors were encountered, chp will contain the channel
 *  \retval false error was was encountered, emsg will contain the reason
 */
extern bool axa_parse_ch(axa_emsg_t *emsg, uint16_t *chp,
			 const char *str, size_t str_len,
			 bool all_ok, bool number_ok);

/* time.c */
/**
 *  Compute (tv1 - tv2) in milliseconds, but limited or clamped to 1 day
 *  or between  +/- 24*60*60*1000.
 *
 *  \param[in] tv1 const struct timeval * to first time value
 *  \param[in] tv2 const struct timeval * to second time value
 *
 *  \return the difference between the two tv_sec values, in ms
 */
extern time_t axa_tv_diff2ms(const struct timeval *tv1,
			     const struct timeval *tv2);

/**
 *  Compute the positive elapsed time between two timevals in milliseconds,
 *  but limited or clamped to at least 0 ms and at most 1 day
 *  or between 0 and 24*60*60*1000.  Negative elapsed time is impossible
 *  except when the system clock is changed.
 *
 *  \param[in] now const struct timeval * to first time value
 *  \param[in] then const struct timeval * to second time value
 *
 *  \return the difference between the two tv_sec values, in ms
 */
extern time_t axa_elapsed_ms(const struct timeval *now, struct timeval *then);

#endif /* AXA_AXA_H */