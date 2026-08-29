#include <libdragon.h>

__attribute__((constructor)) void wrap_fmv_play(void) {
  static int is_constructor_call = 1;
  if (is_constructor_call) {
    debugf("called by constructor: ignoring\n");
    is_constructor_call = 0;
    return;
  }

  video_register_codec(&h264_codec);
  fmv_play("rom:/video.h264", &(fmv_parms_t){.loop = true});
}
