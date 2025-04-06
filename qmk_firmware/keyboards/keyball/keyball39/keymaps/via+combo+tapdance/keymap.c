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

// タップダンスの定義
enum {
    TD_F_ENG,
    TD_J_JPN,
    TD_Q_TAB,
};

// タップダンスの状態を管理する構造体
typedef struct {
    bool is_press_action;
    uint8_t state;
} tap;

// タップダンスの状態を定義
enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD = 2,
    DOUBLE_TAP = 3,
    DOUBLE_HOLD = 4,
    DOUBLE_SINGLE_TAP = 5,
    TRIPLE_TAP = 6,
    TRIPLE_HOLD = 7,
    MORE_TAPS = 8
};

// タップダンスの状態を格納する変数
static tap dance_state[3];

// タップダンスの状態を返す関数
uint8_t dance_step(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return SINGLE_TAP;
        else return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted) return DOUBLE_SINGLE_TAP;
        else if (state->pressed) return DOUBLE_HOLD;
        else return DOUBLE_TAP;
    }
    return MORE_TAPS;
}

// Fキーのタップダンス処理
void on_dance_f(tap_dance_state_t *state, void *user_data) {
    dance_state[TD_F_ENG].state = dance_step(state);
    switch (dance_state[TD_F_ENG].state) {
        case SINGLE_TAP: register_code(KC_F); break;
        case SINGLE_HOLD: layer_on(1); break;
        case DOUBLE_TAP: tap_code(KC_F); tap_code16(KC_LNG2); break;
    }
}

void on_dance_f_reset(tap_dance_state_t *state, void *user_data) {
    switch (dance_state[TD_F_ENG].state) {
        case SINGLE_TAP: unregister_code(KC_F); break;
        case SINGLE_HOLD: layer_off(1); break;
        case DOUBLE_TAP: break;
    }
    dance_state[TD_F_ENG].state = 0;
}

// Jキーのタップダンス処理
void on_dance_j(tap_dance_state_t *state, void *user_data) {
    dance_state[TD_J_JPN].state = dance_step(state);
    switch (dance_state[TD_J_JPN].state) {
        case SINGLE_TAP: register_code(KC_J); break;
        case SINGLE_HOLD: layer_on(2); break;
        case DOUBLE_TAP: tap_code(KC_J); tap_code16(KC_LNG1); break;
    }
}

void on_dance_j_reset(tap_dance_state_t *state, void *user_data) {
    switch (dance_state[TD_J_JPN].state) {
        case SINGLE_TAP: unregister_code(KC_J); break;
        case SINGLE_HOLD: layer_off(2); break;
        case DOUBLE_TAP: break;
    }
    dance_state[TD_J_JPN].state = 0;
}

// Qキーのタップダンス処理
void on_dance_q(tap_dance_state_t *state, void *user_data) {
    dance_state[TD_Q_TAB].state = dance_step(state);
    switch (dance_state[TD_Q_TAB].state) {
        case SINGLE_TAP: register_code(KC_Q); break;
        case SINGLE_HOLD: register_code(KC_Q); break;
        case DOUBLE_TAP: tap_code(KC_TAB); break;
    }
}

void on_dance_q_reset(tap_dance_state_t *state, void *user_data) {
    switch (dance_state[TD_Q_TAB].state) {
        case SINGLE_TAP: unregister_code(KC_Q); break;
        case SINGLE_HOLD: unregister_code(KC_Q); break;
        case DOUBLE_TAP: break;
    }
    dance_state[TD_Q_TAB].state = 0;
}

// タップダンスアクションの定義
tap_dance_action_t tap_dance_actions[] = {
    [TD_F_ENG] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, on_dance_f, on_dance_f_reset),
    [TD_J_JPN] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, on_dance_j, on_dance_j_reset),
    [TD_Q_TAB] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, on_dance_q, on_dance_q_reset),
};

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

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // keymap for default (VIA)
  [0] = LAYOUT_universal(
    TD(TD_Q_TAB), KC_W, KC_E, KC_R, KC_T,                                    KC_Y, KC_U, KC_I, KC_O, KC_P,
    LCTL_T(KC_A), KC_S, KC_D, TD(TD_F_ENG), KC_G,                            LT(4, KC_H), TD(TD_J_JPN), KC_K, LT(3, KC_L), KC_MINS,
    LSFT_T(KC_Z), KC_X, KC_C, LT(2, KC_V), KC_B,                             KC_N, LT(2, KC_M), KC_COMM, KC_DOT, LSFT_T(KC_SLSH),
    KC_LSFT, KC_LGUI, KC_LALT, LALT_T(KC_TAB), KC_LGUI, LT(1, KC_SPC),       LGUI_T(KC_ENT), LT(1, KC_BSPC), _______, _______, _______, LSFT_T(KC_BSLS)
  ),

  [1] = LAYOUT_universal(
    KC_F1, KC_F2, KC_F3, KC_F4, KC_RBRC,                             KC_F6, KC_F7, KC_F8, KC_F9, KC_F10,
    KC_F5, KC_EXLM, S(KC_6), S(KC_INT3), S(KC_8),                    S(KC_INT1), KC_BTN1, KC_PGUP, KC_BTN2, KC_SCLN,
    S(KC_EQL), S(KC_LBRC), S(KC_7), S(KC_2), S(KC_RBRC),            KC_LBRC, KC_DLR, KC_PGDN, KC_BTN3, KC_F11,
    KC_INT1, KC_EQL, S(KC_3), _______, _______, _______, TO(2), TO(0), _______, KC_RALT, KC_RGUI, KC_F12
  ),

  [2] = LAYOUT_universal(
    KC_TAB, KC_7, KC_8, KC_9, KC_MINS,                               KC_NUHS, _______, KC_BTN3, _______, KC_BSPC,
    S(KC_QUOT), KC_4, KC_5, KC_6, S(KC_SCLN),                        S(KC_9), KC_BTN1, KC_UP, KC_BTN2, KC_QUOT,
    KC_SLSH, KC_1, KC_2, KC_3, S(KC_MINS),                           S(KC_NUHS), KC_LEFT, KC_DOWN, KC_RGHT, _______,
    KC_ESC, KC_0, KC_DOT, KC_DEL, KC_ENT, KC_BSPC, _______, _______, _______, _______, _______, _______
  ),

  [3] = LAYOUT_universal(
    RGB_TOG, AML_TO, AML_I50, AML_D50, _______,                      _______, _______, SSNP_HOR, SSNP_VRT, SSNP_FRE,
    RGB_MOD, RGB_HUI, RGB_SAI, RGB_VAI, SCRL_DVI,                    _______, _______, _______, _______, _______,
    RGB_RMOD, RGB_HUD, RGB_SAD, RGB_VAD, SCRL_DVD,                   CPI_D1K, CPI_D100, CPI_I100, CPI_I1K, KBC_SAVE,
    QK_BOOT, KBC_RST, _______, _______, _______, _______, _______, _______, _______, _______, KBC_RST, QK_BOOT
  ),
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