#include <libdragon.h>

extern void *things[];

int main() {
  things[0] = 0;
  debug_init_emulog();
  dfs_init(DFS_DEFAULT_LOCATION);
  rdpq_init();
  void *handle = dlopen("rom:/fmv.dso", 0);
  void (*wrap_fmv_play)(void) = dlsym(handle, "wrap_fmv_play");
  wrap_fmv_play();
}
