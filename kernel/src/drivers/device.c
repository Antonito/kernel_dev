#include <assert.h>
#include <drivers/device.h>
#include <kernel/logger.h>
#include <stddef.h>
#include <string.h>

#define MAX_DEVICE (64)

static struct device_t devices[MAX_DEVICE] = {
    {NULL, 0, DEVICE_UKNOWN, NULL, NULL, NULL, NULL}};
static int32_t device_last_id = 0;

int32_t device_add(struct device_t const *dev) {
  assert(dev);
  LOG(LOG_INFO, "Adding device[%s] %d\r\n", dev->name, device_last_id);
  memcpy(&devices[device_last_id], dev, sizeof(*dev));
  ++device_last_id;
  return device_last_id - 1;
}

int32_t device_get_nb(void) { return device_last_id; }

struct device_t *device_get(int32_t const id) {
  return &devices[id];
}
