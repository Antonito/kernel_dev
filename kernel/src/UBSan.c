/*
 * Copyright (c) 2016, Linaro Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

//
// Add support for UBSanitizer
// Source: https://github.com/OP-TEE/optee_os/blob/master/core/kernel/ubsan.c
// Source: http://wiki.osdev.org/Undefined_Behavior_Sanitization
//

#include <UBSan.h>
#include <kernel/logger.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void __ubsan_handle_type_mismatch(struct type_mismatch_data *data,
                                  unsigned long ptr);
void __ubsan_handle_add_overflow(struct overflow_data *data, unsigned long lhs,
                                 unsigned long rhs);
void __ubsan_handle_sub_overflow(struct overflow_data *data, unsigned long lhs,
                                 unsigned long rhs);
void __ubsan_handle_mul_overflow(struct overflow_data *data, unsigned long lhs,
                                 unsigned long rhs);
void __ubsan_handle_negate_overflow(struct overflow_data *data,
                                    unsigned long old_val);
void __ubsan_handle_divrem_overflow(struct overflow_data *data,
                                    unsigned long lhs, unsigned long rhs);
void __ubsan_handle_shift_out_of_bounds(struct shift_out_of_bounds_data *data,
                                        unsigned long lhs, unsigned long rhs);
void __ubsan_handle_out_of_bounds(struct out_of_bounds_data *data,
                                  unsigned long idx);
void __ubsan_handle_unreachable(struct unreachable_data *data);
void __ubsan_handle_missing_return(struct unreachable_data *data);
void __ubsan_handle_vla_bound_not_positive(struct vla_bound_data *data,
                                           unsigned long bound);
void __ubsan_handle_load_invalid_value(struct invalid_value_data *data,
                                       unsigned long val);
void __ubsan_handle_nonnull_arg(struct nonnull_arg_data *data
#if defined __GCC_VERSION && __GCC_VERSION < 60000
                                ,
                                size_t arg_no
#endif
);

static void print_loc(const char *func, struct source_location *loc) {
  const char *f = func;
  const char func_prefix[] = "__ubsan_handle";

  if (!memcmp(f, func_prefix, sizeof(func_prefix) - 1))
    f += sizeof(func_prefix);

  LOG(LOG_CRITICAL, "Undefined behavior %s at %s:%d col %d", f, loc->file_name,
      loc->line, loc->column);
}

static volatile bool ubsan_panic = true;

void __ubsan_handle_type_mismatch(struct type_mismatch_data *data,
                                  unsigned long ptr __attribute__((unused))) {
  print_loc(__func__, &data->loc);
  if (ubsan_panic) {
    panic("UBSan type mismatch");
  }
}

void __ubsan_handle_add_overflow(struct overflow_data *data,
                                 unsigned long lhs __attribute__((unused)),
                                 unsigned long rhs __attribute__((unused))) {
  print_loc(__func__, &data->loc);
  if (ubsan_panic) {
    panic("UBSan add overflow");
  }
}

void __ubsan_handle_sub_overflow(struct overflow_data *data,
                                 unsigned long lhs __attribute__((unused)),
                                 unsigned long rhs __attribute__((unused))) {
  print_loc(__func__, &data->loc);
  if (ubsan_panic) {
    panic("UBSan sub overflow");
  }
}

void __ubsan_handle_mul_overflow(struct overflow_data *data,
                                 unsigned long lhs __attribute__((unused)),
                                 unsigned long rhs __attribute__((unused))) {
  print_loc(__func__, &data->loc);
  if (ubsan_panic) {
    panic("UBSan mul overflow");
  }
}

void __ubsan_handle_negate_overflow(struct overflow_data *data,
                                    unsigned long old_val
                                    __attribute__((unused))) {
  print_loc(__func__, &data->loc);
  if (ubsan_panic) {
    panic("UBSan negate overflow");
  }
}

void __ubsan_handle_divrem_overflow(struct overflow_data *data,
                                    unsigned long lhs __attribute__((unused)),
                                    unsigned long rhs __attribute__((unused))) {
  print_loc(__func__, &data->loc);
  if (ubsan_panic) {
    panic("UBSan divrem overflow");
  }
}

void __ubsan_handle_shift_out_of_bounds(struct shift_out_of_bounds_data *data,
                                        unsigned long lhs
                                        __attribute__((unused)),
                                        unsigned long rhs
                                        __attribute__((unused))) {
  print_loc(__func__, &data->loc);
  if (ubsan_panic) {
    panic("UBSan shift out of bounds");
  }
}

void __ubsan_handle_out_of_bounds(struct out_of_bounds_data *data,
                                  unsigned long idx __attribute__((unused))) {
  print_loc(__func__, &data->loc);
  if (ubsan_panic) {
    panic("UBSan out of bounds");
  }
}

void __ubsan_handle_unreachable(struct unreachable_data *data) {
  print_loc(__func__, &data->loc);
  if (ubsan_panic) {
    panic("UBSan unreachable");
  }
}

void __attribute__((noreturn))
__ubsan_handle_missing_return(struct unreachable_data *data) {
  print_loc(__func__, &data->loc);
  panic("UBSan missing return");
}

void __ubsan_handle_vla_bound_not_positive(struct vla_bound_data *data,
                                           unsigned long bound
                                           __attribute__((unused))) {
  print_loc(__func__, &data->loc);
  if (ubsan_panic) {
    panic("UBSan vla bound not positive");
  }
}

void __ubsan_handle_load_invalid_value(struct invalid_value_data *data,
                                       unsigned long val
                                       __attribute__((unused))) {
  print_loc(__func__, &data->loc);
  if (ubsan_panic) {
    panic("UBSan load invalid value");
  }
}

void __ubsan_handle_nonnull_arg(struct nonnull_arg_data *data
#if defined __GCC_VERSION && __GCC_VERSION < 60000
                                ,
                                size_t arg_no __attribute__((unused))
#endif
) {
  print_loc(__func__, &data->loc);
  if (ubsan_panic) {
    panic("UBSan non null argument");
  }
}
