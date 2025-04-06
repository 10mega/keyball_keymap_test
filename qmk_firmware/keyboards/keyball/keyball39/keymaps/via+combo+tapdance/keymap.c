/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

#include "quantum.h"

#ifdef COMBO_ENABLE
const uint16_t PROGMEM my_mcomma[] = {KC_M, KC_COMM, COMBO_END};
const uint16_t PROGMEM my_jk[] = {KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM my_comma_dot[] = {KC_COMM, KC_DOT, COMBO_END};
const uint16_t PROGMEM my_kl[] = {KC_K, KC_L, COMBO_END};
const uint16_t PROGMEM my_qwdot[] = {KC_Q, KC_W, KC_DOT, COMBO_END};

combo_t key_combos[] = {
    COMBO(my_mcomma, KC_BTN1),
    COMBO(my_jk, KC_BTN1),
    COMBO(my_comma_dot, KC_BTN2),
    COMBO(my_kl, KC_BTN2),
    COMBO(my_qwdot, KC_ESC),
};
#endif

// Existing tap dance action definitions (kept as is)
enum {
    TD_F_ENG,
    TD_J_JPN,
    TD_Q_TAB,
};

tap_dance_action_t tap_dance_actions[] = {
    [TD_F_ENG] = ACTION_TAP_DANCE_LAYER_MOVE(KC_F, 1),
    [TD_J_JPN] = ACTION_TAP_DANCE_LAYER_MOVE(KC_J, 1),
    [TD_Q_TAB] = ACTION_TAP_DANCE_DOUBLE(KC_Q, KC_TAB),
};

#undef KC_F
#define KC_F TD(TD_F_ENG)
#undef KC_J
#define KC_J TD(TD_J_JPN)
#undef KC_Q
#define KC_Q TD(TD_Q_TAB)


// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // Layer 0: Base Layer (from PDF)
  [0] = LAYOUT_universal(
    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,            KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
    KC_A,    KC_S,    KC_D, LT(2,KC_F), LT(4,KC_G),       LT(4,KC_H), LT(2,KC_J), LT(3,KC_K), LT(3,KC_L), LT(3,KC_MINS),
    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,            KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
    KC_NO,   KC_NO, KC_LCTL, KC_LALT, KC_LGUI,           KC_ENT, LT(1,KC_BSPC), KC_BSLS, KC_NO,   KC_NO,
    KC_NO,   KC_NO, KC_LSFT,                           KC_RSFT, KC_NO,   KC_NO
    // Mapping based on default keymap and PDF (Thumbs: Left: K32=LCTL, K33=LALT, K34=LGUI, K42=LSFT; Right: K35=ENT, K36=LT(1,BSPC), K37=BSLS, K43=RSFT)
    // PDF shows Left: Ctrl, Shift, Tab(*Alt), Win, Space(L1) -> Mapped: K32=LCTL, K42=LSFT, K33=LALT(KC_TAB)?, K34=LGUI, K42=LT(1,SPC)? This mapping is ambiguous. Using a plausible interpretation matching default layout structure.
    // Corrected mapping attempt based on PDF visually:
    // Left Thumbs (Inner cluster K32, K33, K34): LCTL, LALT(KC_TAB)?, LGUI -> Using KC_LCTL, KC_LALT, KC_LGUI
    // Left Outer Thumb (K42): LSFT
    // Right Thumbs (Inner cluster K35, K36, K37): ENT(Win/Cmd?), LT(1,BSPC), BSLS(Shift?) -> Using KC_ENT, LT(1,BSPC), KC_BSLS
    // Right Outer Thumb (K43): RSFT
    // Space (Left thumb area in PDF): LT(1, KC_SPC) -> Let's put this on K34 (replacing LGUI temporarily as PDF is unclear)
    // Tab (*Alt) (Left thumb area): KC_TAB -> Put on K33 (replacing LALT)
    // Win (Left thumb area): KC_LGUI -> Put on K32 (replacing LCTL)

    /* Revised Layer 0 based on PDF Visual Thumb keys */
    // KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,            KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
    // KC_A,    KC_S,    KC_D, LT(2,KC_F), LT(4,KC_G),       LT(4,KC_H), LT(2,KC_J), LT(3,KC_K), LT(3,KC_L), LT(3,KC_MINS),
    // KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,            KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
    // KC_NO,   KC_NO, KC_LGUI, KC_TAB, LT(1,KC_SPC),     KC_ENT, LT(1,KC_BSPC), KC_BSLS, KC_NO,   KC_NO,
    // KC_NO,   KC_NO, KC_LSFT,                           KC_RSFT, KC_NO,   KC_NO
    // This revised mapping places keys based *visually* on PDF thumb cluster, overriding default structural assumptions. LCTL & LALT are lost from thumbs.
    // Sticking to the first interpretation which preserves mods structurally similar to default. User might need Remap/VIA.
  ),

  // Layer 1: Symbol/Mouse Layer (from PDF)
  [1] = LAYOUT_universal(
    KC_ESC, S(KC_GRV), S(KC_SLSH), S(KC_1), KC_TRNS,      AG(KC_UP), C(KC_LEFT), KC_BTN3, C(KC_RGHT), KC_TRNS,
    KC_TAB, KC_TRNS, S(KC_3), S(KC_2), KC_NO,             AG(KC_DOWN), KC_BTN1,S(KC_BTN1), KC_BTN2, LT(3,S(KC_SCLN)),
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          AG(KC_LEFT),G(KC_LEFT),G(KC_RGHT),AG(KC_RGHT), KC_TRNS,
    KC_NO,   KC_NO, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_NO,   KC_NO,
    KC_NO,   KC_NO, KC_TRNS,                           KC_TRNS, KC_NO,   KC_NO
    // Note: *S+W 4 is mapped to KC_NO. AG() used for *A+W. GUI() or G() used for *Win/Cmd.
  ),

  // Layer 2: Number/Navigation Layer (from PDF)
  [2] = LAYOUT_universal(
    S(KC_7), S(KC_8), S(KC_9), KC_PAST, KC_TRNS,          KC_TRNS, KC_LEFT, KC_UP,   KC_RGHT, KC_TRNS,
    S(KC_4), S(KC_5), S(KC_6),S(KC_EQL), KC_EQL,          KC_TRNS, KC_LEFT, KC_UP,   KC_RGHT, S(KC_SCLN),
    S(KC_1), S(KC_2), S(KC_3),S(KC_SLSH), KC_SLSH,        KC_TRNS, KC_TRNS, KC_DOWN, KC_RGHT, KC_TRNS,
    KC_NO,   KC_NO, KC_1,    KC_0,    KC_TRNS,          KC_TRNS, KC_TRNS, KC_BSPC, KC_NO,   KC_NO, // Assuming BS is right thumb K37
    KC_NO,   KC_NO, KC_TRNS,                           KC_TRNS, KC_NO,   KC_NO
    // Note: PDF layout for numbers/nav symbols mapped. (*S+8)* mapped to KC_PAST. (*S+;) : mapped to S(KC_SCLN).
    // BS placement on PDF thumb is ambiguous, mapped to right inner thumb K37 here.
  ),

  // Layer 3: Function/RGB Layer (from PDF)
  [3] = LAYOUT_universal(
    KC_NO,   KC_NO,   KC_NO,   KC_TRNS, KC_TRNS,          KC_TRNS, KC_NO,   KC_NO,   KC_NO,   KC_TRNS,
    RGB_HUI, RGB_SAI, RGB_VAI, KC_NO,   KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_LCTL,
    RGB_HUD, RGB_SAD, RGB_VAD, KC_NO,   KC_TRNS,          KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
    KC_NO,   KC_NO, KC_NO,   KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS, QK_BOOT, KC_NO,   KC_NO,
    KC_NO,   KC_NO, KC_TRNS,                           KC_TRNS, KC_NO,   KC_NO
    // Note: Kb X keys mapped to KC_NO. Ctrl* mapped to KC_LCTL. Bootloader mapped to QK_BOOT.
  ),

  // Layer 4: F-Key Layer (from PDF)
  [4] = LAYOUT_universal(
    KC_F7,   KC_F8,   KC_F9,   KC_F12,  KC_TRNS,          KC_LBRC,S(KC_LBRC),KC_RBRC,S(KC_RBRC), S(KC_QUOT),
    KC_F4,   KC_F5,   KC_F6,   KC_F11,  KC_TRNS,          S(KC_9),S(KC_9), S(KC_0), S(KC_0),  KC_QUOT, // PDF shows (*S+9)( and (*S+0)) -> Mapped S(KC_9) and S(KC_0)
    KC_F1,   KC_F2,   KC_F3,   KC_F10,  KC_TRNS,          A(KC_9), A(KC_0), KC_TRNS, KC_TRNS, KC_TRNS,
    KC_NO,   KC_NO, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_NO,   KC_NO,
    KC_NO,   KC_NO, KC_TRNS,                           KC_TRNS, KC_NO,   KC_NO
    // Note: *Alt 9/0 mapped to A(KC_9)/A(KC_0).
  ),

  // Layer 5: Empty Layer (from PDF)
  [5] = LAYOUT_universal(
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_NO,   KC_NO, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_NO,   KC_NO,
    KC_NO,   KC_NO, KC_TRNS,                           KC_TRNS, KC_NO,   KC_NO
  )
};
// clang-format on

// Existing layer state change function (kept as is)
layer_state_t layer_state_set_user(layer_state_t state) {
    // Auto enable scroll mode when the highest layer is 3? PDF doesn't show scroll mode keys. Adjust if needed.
    // keyball_set_scroll_mode(get_highest_layer(state) == 3);
    return state;
}

#ifdef OLED_ENABLE

#    include "lib/oledkit/oledkit.h"

// Existing OLED render function (kept as is)
void oledkit_render_info_user(void) {
    keyball_oled_render_keyinfo();
    keyball_oled_render_ballinfo();
    keyball_oled_render_layerinfo();
}
#endif