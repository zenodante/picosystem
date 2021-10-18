#pragma once

#include <memory>
#include <cstdint>
#include <climits>
#include <initializer_list>

#include <string>
#include <vector>

#include "pico/stdlib.h"

void init();
void update(uint32_t time_ms);
void draw();

namespace picosystem {

  extern uint32_t _debug;

  typedef uint16_t color_t;

  struct buffer_t {
    int32_t w, h;
    color_t *data;

    color_t *p(int32_t x, int32_t y) {
      return data + (x + y * w);
    }
  };

  struct voice_t {
    int32_t  bend; // pitch bend (hz)
    uint32_t bend_ms; // bend speed (ms)
    uint32_t attack, decay, sustain, release; // envelope (ms, ms, %, ms)
    uint32_t reverb; // effects (ms)
    uint32_t noise, distort; // effects (strength 0..100)
  };

  using blend_func_t =
    void(*)(color_t* source, uint32_t so, int32_t ss,
            color_t*   dest, uint32_t count);

  // drawing state
  extern color_t        _pen;                 // pen
  extern int32_t        _cx, _cy, _cw, _ch;   // clip rect
  extern int32_t        _tx, _ty;             // text cursor position
  extern uint32_t       _io, _lio;            // io state and last io state
  extern int32_t        _camx, _camy;         // camera
  extern blend_func_t   _bf;                  // blend function
  extern buffer_t      * SCREEN;              // framebuffer
  extern buffer_t      *_dt;                  // drawing target
  extern buffer_t      *_ss;                  // sprite sheet
  extern uint8_t       *_font;                // font data

  // audio state
  extern voice_t        _v;                   // current voice
  extern uint32_t       _bpm;                 // beats per minute

  // helpers
  extern float          _fsin_lut[256];       // fast sin lut
  constexpr float       _PI = 3.1415927f;

  // state
  void        pen(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 15);
  void        pen(color_t p);
  void        clip(int32_t x, int32_t y, uint32_t w, uint32_t h);
  void        blend(blend_func_t bf);
  void        target(buffer_t *dt);
  void        camera(int32_t x, int32_t y);
  void        spritesheet(buffer_t &ss);
  void        cursor(int32_t x, int32_t y);

  // primitives
  void        clear();
  void        pixel(int32_t x, int32_t y);
  void        hline(int32_t x, int32_t y, int32_t c);
  void        vline(int32_t x, int32_t y, int32_t c);
  void        rect(int32_t x, int32_t y, int32_t w, int32_t h);
  void        circle(int32_t x, int32_t y, int32_t r);
  void        poly(const int32_t *p, uint32_t l);
  void        poly(const std::initializer_list<int32_t> &pts);
  void        frect(int32_t x, int32_t y, int32_t w, int32_t h);
  void        fcircle(int32_t x, int32_t y, int32_t r);
  void        fpoly(const int32_t *p, uint32_t l);
  void        fpoly(const std::initializer_list<int32_t> &pts);
  void        line(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
  void        blit(
                buffer_t &source,
                int32_t x, int32_t y, int32_t w, int32_t h,
                int32_t dx, int32_t dy);
  void        blit(
                buffer_t &source,
                int32_t sx, int32_t sy, int32_t sw, int32_t sh,
                int32_t dx, int32_t dy, int32_t dw, int32_t dh);
  void        sprite(uint32_t i, int32_t x, int32_t y);
  void        sprite(uint32_t i, int32_t x, int32_t y, int32_t cx, int32_t cy);
  void        sprite(
                uint32_t i,
                int32_t x, int32_t y, int32_t cx, int32_t cy,
                int32_t dw, int32_t dh);
  void        text(const char &c, int32_t x, int32_t y);
  void        text(const char &c);
  void        text(const std::string &t, int32_t x, int32_t y);
  void        text(const std::string &t);

  // blend functions
  void        COPY(
                color_t* ps, uint32_t so, int32_t ss,
                color_t* pd, uint32_t c);
  void        BLEND(
                color_t* ps, uint32_t so, int32_t ss,
                color_t* pd, uint32_t c);
  void        MASK(
                color_t* ps, uint32_t so, int32_t ss,
                color_t* pd, uint32_t c);

  // audio
  void        play(
                voice_t voice, uint32_t frequency,
                uint32_t duration = 500, uint32_t volume = 100);
  uint8_t     audio_sample(uint32_t ms);
  uint32_t    audio_position();

  // utility
  std::string str(float v, uint8_t precision = 2);
  std::string str(int32_t v);
  std::string str(uint32_t v);
  color_t     rgb(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 15);
  color_t     hsv(float h, float s, float v, float a = 1.0f);
  buffer_t*   buffer(uint32_t w, uint32_t h, void *data = nullptr);
  voice_t     voice(
                uint32_t attack = 100, uint32_t decay = 50,
                uint32_t sustain = 80, uint32_t release = 100,
                int32_t bend = 0, uint32_t bend_ms = 0, uint32_t reverb = 0,
                uint32_t noise = 0, uint32_t distort = 0);
  uint32_t    time();
  uint32_t    time_us();
  void        sleep(uint32_t d);
  void        sleep_us(uint32_t d);
  bool        intersects(
                int32_t  x, int32_t  y, int32_t  w, int32_t  h,
                int32_t cx, int32_t cy, int32_t cw, int32_t ch);
  void        intersection(
                int32_t &x, int32_t &y, int32_t &w, int32_t &h,
                int32_t cx, int32_t cy, int32_t cw, int32_t ch);
  bool        contains(
                int32_t  x, int32_t  y,
                int32_t cx, int32_t cy, int32_t cw, int32_t ch);
  bool        contains(
                int32_t  x, int32_t  y, int32_t  w, int32_t  h,
                int32_t cx, int32_t cy, int32_t cw, int32_t ch);
  void        wrap(std::string &t, std::size_t chars);
  uint32_t    text_width(std::string &t);
  std::vector<std::string> split(const std::string& t, char d = '\n');
  float       fsin(float v);
  float       fcos(float v);

  // hardware
  bool        pressed(uint32_t b);
  bool        button(uint32_t b);
  uint32_t    battery();
  void        led(uint8_t r, uint8_t g, uint8_t b);
  void        backlight(uint8_t b);


  // internal methods - do not call directly, will change!
  void       _logo();
  void       _init_hardware();
  uint32_t   _gpio_get();
  float      _battery_voltage();
  void       _reset_to_dfu();
  void       _wait_vsync();
  void       _flip();
  bool       _is_flipping();
  void       _camera_offset(int32_t &x, int32_t &y);
  void       _update_audio();
  void       _play_note(uint32_t f, uint32_t v);

  // input pins
  enum button {
    UP    = 23,
    DOWN  = 20,
    LEFT  = 22,
    RIGHT = 21,
    A     = 18,
    B     = 19,
    X     = 17,
    Y     = 16
  };

  // assets
  const extern uint8_t _default_font[96][9];
  const extern uint8_t _picosystem_logo[550];
  const extern color_t _default_sprite_sheet[16384];

  #ifndef NO_SPRITESHEET
  enum _sprites_ids {
    CHERRY, APPLE, BANANA, ORANGE, AUBERGINE, CARROT, POTATO, LEAF,
    PARSNIP, MEAT, BEAN, RADISH, STRAWBERRY, PUMPKIN, CUCUMBER, SALT,
    SWORD1, SWORD2, SWORD3, AXE, DAGGER, MACE, BOW, ARROW1,
    ARROW2, ARROW3, TRIDENT1, SPEAR, TRIDENT2, MACHINEGUN, PISTOL, SHOTGUN,
    GEM1, GEM2, GEM3, GEM4, GEM5, GEM6, AMULET, FOLDER,
    COIN, SKULL, TICK, CROSS, IDOL1, IDOL2, IDOL3, IDOL4,
    SWEETCORN, BROCCOLI, BISCUIT, BERRIES, SCEPTRE, FIRE, LASERGUN1, LASERGUN2,
    HELMET1, HELMET2, HELMET3, HELMET4,
    // etc... can we name all 256 built in sprites?!
  };
  #endif
}