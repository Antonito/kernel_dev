#include <kernel/vfs.h>
#include <stddef.h>

fs_node_t *fs_root = NULL; // The root of the filesystem.

uint32_t read_fs(fs_node_t *node, uint32_t offset, uint32_t size,
                 uint8_t *buffer) {
  // Has the node got a read callback?
  if (node->read) {
    return node->read(node, offset, size, buffer);
  }
  return 0;
}

uint32_t write_fs(fs_node_t *node, uint32_t offset, uint32_t size,
                  uint8_t *buffer) {
  // Has the node got a write callback?
  if (node->write) {
    return node->write(node, offset, size, buffer);
  }
  return 0;
}

void open_fs(fs_node_t *node, __attribute__((unused)) uint8_t read,
             __attribute__((unused)) uint8_t write) {
  // Has the node got a open callback?
  if (node->open) {
    node->open(node);
  }
}

void close_fs(fs_node_t *node) {
  // Has the node got a close callback?
  if (node->close) {
    node->close(node);
  }
}

struct dirent *readdir_fs(fs_node_t *node, uint32_t index) {
  // Has the node got a readdir callback?
  if ((node->flags & 0x7) == FS_DIRECTORY && node->readdir) {
    return node->readdir(node, index);
  }
  return NULL;
}

fs_node_t *finddir_fs(fs_node_t *node, char *name) {
  // Has the node got a finddir callback?
  if ((node->flags & 0x7) == FS_DIRECTORY && node->finddir) {
    return node->finddir(node, name);
  }
  return NULL;
}
