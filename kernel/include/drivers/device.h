#pragma once

#include <kernel/vfs.h>
#include <stdint.h>

enum kernel_device_e {
  DEVICE_UKNOWN = -1,
  DEVICE_CHAR,
  DEVICE_BLOCK,
  NB_DEVICE
};

struct device_t {
  char *name;                    // Device name
  uint32_t unique_id;            // ID of the device
  enum kernel_device_e dev_type; // Device type
  struct fs_node_t *fs;          // Associated filesystem
  uint8_t (*read)(uint8_t *buffer, uint32_t offset, uint32_t len, void *dev);
  uint8_t (*write)(uint8_t *buffer, uint32_t offset, uint32_t len, void *dev);
  void *priv;
};

int32_t device_add(struct device_t const *dev);
int32_t device_get_nb(void);
struct device_t *device_get(int32_t const id);
