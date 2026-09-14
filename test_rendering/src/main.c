#include <libdragon.h>

sprite_t *sprite_big_energy_ball; // rgba32, translucent
sprite_t *sprite_ring;            // rgba16, opaque
sprite_t *sprite_sunflower;       // ci4, cutout

// TRF = Test Rendering Fragment

typedef void (*trf)(void);

void trf_rdpq_mode_alphacompare_100(void) { rdpq_mode_alphacompare(100); }
void trf_rdpq_mode_antialias_standard(void) {
  rdpq_mode_antialias(AA_STANDARD);
}
void trf_rdpq_mode_blender_multiply(void) {
  rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
}
void trf_rdpq_mode_combiner_tex(void) { rdpq_mode_combiner(RDPQ_COMBINER_TEX); }
void trf_rdpq_mode_dithering_square_square(void) {
  rdpq_mode_dithering(DITHER_SQUARE_SQUARE);
}
void trf_rdpq_mode_filter_bilinear(void) { rdpq_mode_filter(FILTER_BILINEAR); }
void trf_rdpq_mode_fog_standard(void) { rdpq_mode_fog(RDPQ_FOG_STANDARD); }
void trf_rdpq_mode_persp_true(void) { rdpq_mode_persp(true); }
void trf_rdpq_mode_tlut_rgba16(void) { rdpq_mode_tlut(TLUT_RGBA16); }
void trf_rdpq_mode_zbuf_compare_update(void) { rdpq_mode_zbuf(true, true); }
void trf_rdpq_set_fog_color_white(void) {
  rdpq_set_fog_color(color_from_packed32(0xFFFFFFFF));
}

void trf_rdpq_sprite_upload_big_energy_ball(void) {
  rdpq_sprite_upload(TILE0, sprite_big_energy_ball, &(rdpq_texparms_t){});
}
void trf_rdpq_tex_upload_big_energy_ball(void) {
  surface_t surf = sprite_get_pixels(sprite_big_energy_ball);
  rdpq_tex_upload(TILE0, &surf, &(rdpq_texparms_t){});
}

void trf_rdpq_sprite_upload_ring(void) {
  rdpq_sprite_upload(TILE0, sprite_ring, &(rdpq_texparms_t){});
}
void trf_rdpq_tex_upload_ring(void) {
  surface_t surf = sprite_get_pixels(sprite_ring);
  rdpq_tex_upload(TILE0, &surf, &(rdpq_texparms_t){});
}

void trf_rdpq_sprite_upload_sunflower(void) {
  rdpq_sprite_upload(TILE0, sprite_sunflower,
                     &(rdpq_texparms_t){
                         .s.repeats = REPEAT_INFINITE,
                         .t.repeats = REPEAT_INFINITE,
                     });
}
void trf_rdpq_tex_upload_sunflower(void) {
  surface_t surf = sprite_get_pixels(sprite_sunflower);
  rdpq_tex_upload(TILE0, &surf,
                  &(rdpq_texparms_t){
                      .palette = 0,
                      .s.repeats = REPEAT_INFINITE,
                      .t.repeats = REPEAT_INFINITE,
                  });
}
void trf_rdpq_tex_upload_tlut_sunflower(void) {
  rdpq_tex_upload_tlut(sprite_get_palette(sprite_sunflower), 0, 16);
}

// TESTS

trf trf_test1[] = {
    trf_rdpq_mode_filter_bilinear,
    trf_rdpq_sprite_upload_sunflower,
    trf_rdpq_mode_fog_standard,
    trf_rdpq_set_fog_color_white,
};
trf trf_test2[] = {
    trf_rdpq_sprite_upload_sunflower,
    trf_rdpq_mode_alphacompare_100,
    trf_rdpq_mode_antialias_standard,
};
trf trf_test3[] = {
    trf_rdpq_sprite_upload_big_energy_ball,
    trf_rdpq_mode_blender_multiply,
    trf_rdpq_mode_antialias_standard,
};

#define TEST(trfs) {#trfs, trfs, sizeof(trfs) / sizeof(trfs[0])}
struct {
  const char *name;
  trf *trfs;
  int count;
} trf_list[] = {
    TEST(trf_test1),
    TEST(trf_test2),
    TEST(trf_test3),
};
#undef TEST

// Implementation

struct vertex {
  int16_t pos[3];
  alignas(4) int16_t st[2];
};
mg_vertex_attribute_t vertex_attributes[] = {
    {
        .input = MGFX_ATTRIBUTE_POSITION,
        .offset = offsetof(struct vertex, pos),
    },
    {
        .input = MGFX_ATTRIBUTE_TEXCOORD,
        .offset = offsetof(struct vertex, st),
    },
};
/*
 0----3  y ^        +--> s
 |    |    |        |
 |    |    +--> x   v t
 1----2
*/
struct vertex my_vertices[] = {
    {{-1, +1, 0}, {0 << 6, 0 << 6}},
    {{-1, -1, 0}, {0 << 6, 1 << 6}},
    {{+1, -1, 0}, {1 << 6, 1 << 6}},
    {{+1, +1, 0}, {1 << 6, 0 << 6}},
};

void log_draw_surf(surface_t *surf) {
  assert(surface_get_format(surf) == FMT_RGBA16);
  for (int y = 0; y < surf->height; y++) {
    for (int x = 0; x < surf->width; x++) {
      uint16_t rgba16 =
          *(uint16_t *)((char *)surf->buffer + surf->stride * y + 2 * x);
      int r = (rgba16 & 0xF800) >> 11;
      int g = (rgba16 & 0x07C0) >> 6;
      int b = (rgba16 & 0x003E) >> 1;
      r = (r << 3) | (r >> 2);
      g = (g << 3) | (g >> 2);
      b = (b << 3) | (b >> 2);
      fprintf(stderr, "\x1B[48;2;%d;%d;%dm  \x1B[0m", r, g, b);
    }
    fprintf(stderr, "\n");
  }
}

int main() {
  debug_init_emulog();
  int dfs_init_res = dfs_init(DFS_DEFAULT_LOCATION);
  assertf(dfs_init_res == DFS_ESUCCESS, "%s", dfs_strerror(dfs_init_res));
  rdpq_init();
  rspq_init();
  mg_init();

  sprite_big_energy_ball =
      sprite_load("rom:/assets/big_energy_ball.rgba32.sprite");
  sprite_ring = sprite_load("rom:/assets/ring.rgba16.sprite");
  sprite_sunflower = sprite_load("rom:/assets/sunflower.ci4.sprite");

  int fb_width = 48, fb_height = 32;

  mg_pipeline_t *pipeline = mg_pipeline_create(&(mg_pipeline_parms_t){
      .vertex_shader_ucode = mgfx_get_shader_ucode(0),
      .vertex_layout.attribute_count =
          sizeof(vertex_attributes) / sizeof(vertex_attributes[0]),
      .vertex_layout.attributes = vertex_attributes,
      .vertex_layout.stride = sizeof(struct vertex),
  });

  struct uniforms {
    mgfx_fog_t fog;
    mgfx_lighting_t lighting;
    mgfx_texturing_t texturing;
    mgfx_matrices_t matrices;
  };
  struct uniforms *uniform_data = malloc_uncached(sizeof(struct uniforms));

  float z_near = 0.1f, z_far = 10.0f;

  mgfx_get_fog(&uniform_data->fog,
               &(mgfx_fog_parms_t){.start = 4.5, .end = 5.5});

  mgfx_get_lighting(&uniform_data->lighting,
                    &(mgfx_lighting_parms_t){
                        .light_count = 0,
                        .ambient_color = color_from_packed32(0xFFFFFFFF),
                    });

  mgfx_get_texturing(&uniform_data->texturing,
                     &(mgfx_texturing_parms_t){.scale = {32, 32}});

  fm_mat4_t mat_projection;
  mg_mat4_perspective(&mat_projection, FM_DEG2RAD(60),
                      (float)fb_width / fb_height, z_near, z_far);

  fm_mat4_t mat_view;
  fm_mat4_lookat(&mat_view, &(fm_vec3_t){{2, 0, 5}}, &(fm_vec3_t){{0, 0, 0}},
                 &(fm_vec3_t){{0, 1, 0}});
  fm_mat4_t mat_model;
  fm_mat4_identity(&mat_model);
  fm_mat4_scale(&mat_model, &(fm_vec3_t){{3, 3, 1}});
  fm_mat4_t mat_view_model;
  fm_mat4_mul(&mat_view_model, &mat_view, &mat_model);
  fm_mat4_t mat_projection_view_model;
  fm_mat4_mul(&mat_projection_view_model, &mat_projection, &mat_view_model);

  fm_mat4_t identity;
  fm_mat4_identity(&identity);
  mgfx_get_matrices(&uniform_data->matrices,
                    &(mgfx_matrices_parms_t){
                        .model_view_projection = mat_projection_view_model.m[0],
                        .model_view = mat_view_model.m[0],
                        .normal = identity.m[0],
                    });

  const mg_uniform_t *fog_uniform =
      mg_pipeline_get_uniform(pipeline, MGFX_BINDING_FOG);
  const mg_uniform_t *lighting_uniform =
      mg_pipeline_get_uniform(pipeline, MGFX_BINDING_LIGHTING);
  const mg_uniform_t *texturing_uniform =
      mg_pipeline_get_uniform(pipeline, MGFX_BINDING_TEXTURING);
  const mg_uniform_t *matrices_uniform =
      mg_pipeline_get_uniform(pipeline, MGFX_BINDING_MATRICES);

  surface_t surf = surface_alloc(FMT_RGBA16, fb_width, fb_height);
  surface_t surf_z = surface_alloc(FMT_RGBA16, fb_width, fb_height);

  rdpq_attach(&surf, &surf_z);

  mg_pipeline_bind(pipeline);

  mg_set_viewport(&(mg_viewport_t){
      .x = 0,
      .y = 0,
      .width = surf.width,
      .height = surf.height,
      .minDepth = 0.0f,
      .maxDepth = 1.0f,
      .z_near = z_near,
      .z_far = z_far,
  });

  mg_set_culling(&(mg_culling_parms_t){
      .cull_mode = MG_CULL_MODE_NONE,
  });

  mg_set_geometry_flags(MG_GEOMETRY_FLAGS_SHADE_ENABLED |
                        MG_GEOMETRY_FLAGS_TEX_ENABLED |
                        MG_GEOMETRY_FLAGS_Z_ENABLED);

  int n_failures = 0;

  surface_t ref_surf = surface_alloc(FMT_RGBA16, fb_width, fb_height);
  surface_t ref_surf_z = surface_alloc(FMT_RGBA16, fb_width, fb_height);
  for (int i = 0; i < sizeof(trf_list) / sizeof(trf_list[0]); i++) {
    fprintf(stderr, "trf name = %s\n", trf_list[i].name);
    bool has_ref = false;
    for (int test_with_mode_batch = 0; test_with_mode_batch <= 1;
         test_with_mode_batch++) {
      fprintf(stderr, "  %s\n",
              test_with_mode_batch ? "with mode batch" : "without mode batch");
      enum {
        TEST_TYPE_NAIVE,
        TEST_TYPE_BLOCK,
        // TEST_TYPE_FROZENBLOCK,
        TEST_TYPE_MAX
      };
      const char *test_names[TEST_TYPE_MAX] = {
          "naive",
          "block",
          // "frozenblock",
      };
      for (int test_type = 0; test_type < TEST_TYPE_MAX; test_type++) {
        fprintf(stderr, "    test_type = %s\n", test_names[test_type]);
        for (int k = 0; k < 1; k++) { // TODO permute trf_list[i].trfs
          fprintf(stderr, "      k=%d\n", k);
          rdpq_clear(RGBA32(150, 200, 100, 255));
          rdpq_clear_z(ZBUF_MAX);

          rspq_wait();

          if (test_type == TEST_TYPE_BLOCK) {
            rspq_block_begin();
          }

          if (test_with_mode_batch) {
            rdpq_mode_begin();
          }

          rdpq_set_mode_standard();
          for (int j = 0; j < trf_list[i].count; j++) {
            trf_list[i].trfs[j]();
          }

          if (test_with_mode_batch) {
            rdpq_mode_end();
          }

          if (test_type == TEST_TYPE_BLOCK) {
            rspq_block_t *block = rspq_block_end();

            rspq_block_run(block);

            rspq_wait();
            rspq_block_free(block);
          }

          mg_uniform_load(fog_uniform, &uniform_data->fog);
          mg_uniform_load(lighting_uniform, &uniform_data->lighting);
          mg_uniform_load(texturing_uniform, &uniform_data->texturing);
          mg_uniform_load(matrices_uniform, &uniform_data->matrices);

          mg_bind_vertex_buffer(my_vertices);

          mg_draw_begin();
          mg_load_vertices(0, 0, 4);
          mg_draw_triangle(0, 1, 2);
          mg_draw_triangle(2, 3, 0);
          mg_draw_end();

          rspq_wait();

          if (!has_ref) {
            memcpy(ref_surf.buffer, surf.buffer, surf.stride * surf.height);
            memcpy(ref_surf_z.buffer, surf_z.buffer,
                   surf_z.stride * surf_z.height);
            has_ref = true;
            fprintf(stderr, "Reference:\n");
            log_draw_surf(&surf);
          } else {
            bool surf_match = memcmp(ref_surf.buffer, surf.buffer,
                                     surf.stride * surf.height) == 0;
            bool surf_z_match = memcmp(ref_surf_z.buffer, surf_z.buffer,
                                       surf_z.stride * surf_z.height) == 0;
            if (!surf_match || !surf_z_match) {
              fprintf(stderr, "Test %s, reference:\n", trf_list[i].name);
              log_draw_surf(&ref_surf);
              fprintf(stderr, "differs from (%s, %s, k=%d):\n",
                      test_with_mode_batch ? "with mode batch"
                                           : "without mode batch",
                      test_names[test_type], k);
              if (surf_match) {
                fprintf(stderr, "(NOTE: only depth differs)\n");
              }
              log_draw_surf(&surf);
              n_failures++;
            }
          }
          // Make sure we don't later writeback to the framebuffer
          data_cache_writeback_invalidate_all();
        }
      }
    }
  }

  fprintf(stderr, "n_failures = %d\n", n_failures);
}
