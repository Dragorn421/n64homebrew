#include <libdragon.h>

__attribute__((visibility("default"))) void wrap_fmv_play(void) {
  video_register_codec(&h264_codec);
  fmv_play("rom:/video.h264", &(fmv_parms_t){.loop = true});
}
