#pragma once

#include <stddef.h>
#include <stdint.h>

struct source_location {
  char const *file_name;
  uint32_t line;
  uint32_t column;
};

struct type_descriptor {
  uint16_t type_kind;
  uint16_t type_info;
  char type_name[1];
};

struct type_mismatch_data {
  struct source_location loc;
  struct type_descriptor *type;
  unsigned long alignment;
  unsigned char type_check_kind;
};

struct overflow_data {
  struct source_location loc;
  struct type_descriptor *type;
};

struct shift_out_of_bounds_data {
  struct source_location loc;
  struct type_descriptor *lhs_type;
  struct type_descriptor *rhs_type;
};

struct out_of_bounds_data {
  struct source_location loc;
  struct type_descriptor *array_type;
  struct type_descriptor *index_type;
};

struct unreachable_data {
  struct source_location loc;
};

struct vla_bound_data {
  struct source_location loc;
  struct type_descriptor *type;
};

struct invalid_value_data {
  struct source_location loc;
  struct type_descriptor *type;
};

struct nonnull_arg_data {
  struct source_location loc;
};
