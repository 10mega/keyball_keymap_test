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
// #include "quantum.h" // QMK_KEYBOARD_H に含まれるため通常不要

// --- タップダンス定義 ---

// タップダンスの種類を識別するための enum
enum {
    TD_F_ENG, // Fキー: Tap=F, Hold=Layer2, DoubleTap=英数(LNG2)
    TD_J_JPN, // Jキー: Tap=J, Hold=Layer2, DoubleTap=かな(LNG1)
    TD_Q_TAB, // Qキー: Tap=Q, Hold=Q(維持), DoubleTap=Tab
};

// タップダンスの状態を示す enum (実際に使用するものに限定)
enum {
    TD_STATE_NONE = 0, // 初期状態
    TD_STATE_SINGLE_TAP = 1,
    TD_STATE_SINGLE_HOLD = 2,
    TD_STATE_DOUBLE_TAP = 3,
    // 必要であれば DOUBLE_HOLD, DOUBLE_SINGLE_TAP なども追加定義する
};

// タップダンスの状態を格納する変数 (シンプルな uint8_t 配列に変更)
static uint8_t dance_state[3]; // TD_F_ENG, TD_J_JPN, TD_Q_TAB に対応

// タップダンスの状態を判定するヘルパー関数 (使用する状態のみ判定)
uint8_t determine_dance_state(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) {
            // タップ中に他のキー入力があった、または TAPPING_TERM 内に離された -> シングルタップ
            return TD_STATE_SINGLE_TAP;
        } else {
            // TAPPING_TERM を超えて押されている -> シングルホールド
            return TD_STATE_SINGLE_HOLD;
        }
    } else if (state->count == 2) {
        // ダブルタップ判定 (ここでは interrupted や pressed を細かく見ず、ダブルタップとして扱う)
        // 必要に応じて DOUBLE_SINGLE_TAP や DOUBLE_HOLD の判定を追加
        if (!state->pressed) {
          return TD_STATE_DOUBLE_TAP;
        }
        // DOUBLE_HOLD など他の状態が必要な場合はここに追加
        // else if (state->pressed) return TD_STATE_DOUBLE_HOLD;
    }
    // 3回以上のタップなどはここで定義・処理しない (必要なら追加)
    return TD_STATE_NONE; // 不明な状態または未処理の状態
}

// --- Fキー タップダンスハンドラ ---
void on_dance_f(tap_dance_state_t *state, void *user_data) {
    uint8_t current_state = determine_dance_state(state);
    dance_state[TD_F_ENG] = current_state;

    switch (current_state) {
        case TD_STATE_SINGLE_TAP: register_code(KC_F); break; // タップ: F を押す
        case TD_STATE_SINGLE_HOLD: layer_on(2); break;        // ホールド: レイヤー2 を有効化
        case TD_STATE_DOUBLE_TAP: tap_code(KC_LNG2); break; // ダブルタップ: 英数キー (macOS/Win) を送信
    }
}

void on_dance_f_reset(tap_dance_state_t *state, void *user_data) {
    switch (dance_state[TD_F_ENG]) {
        case TD_STATE_SINGLE_TAP: unregister_code(KC_F); break; // F を離す
        case TD_STATE_SINGLE_HOLD: layer_off(2); break;         // レイヤー2 を無効化
        case TD_STATE_DOUBLE_TAP: break;                        // tap_code は自動解除されるので何もしない
    }
    dance_state[TD_F_ENG] = TD_STATE_NONE; // 状態をリセット
}

// --- Jキー タップダンスハンドラ ---
void on_dance_j(tap_dance_state_t *state, void *user_data) {
    uint8_t current_state = determine_dance_state(state);
    dance_state[TD_J_JPN] = current_state;

    switch (current_state) {
        case TD_STATE_SINGLE_TAP: register_code(KC_J); break; // タップ: J を押す
        case TD_STATE_SINGLE_HOLD: layer_on(2); break;        // ホールド: レイヤー2 を有効化
        case TD_STATE_DOUBLE_TAP: tap_code(KC_LNG1); break; // ダブルタップ: かなキー (macOS/Win) を送信
    }
}

void on_dance_j_reset(tap_dance_state_t *state, void *user_data) {
    switch (dance_state[TD_J_JPN]) {
        case TD_STATE_SINGLE_TAP: unregister_code(KC_J); break; // J を離す
        case TD_STATE_SINGLE_HOLD: layer_off(2); break;         // レイヤー2 を無効化
        case TD_STATE_DOUBLE_TAP: break;                        // tap_code は自動解除
    }
    dance_state[TD_J_JPN] = TD_STATE_NONE; // 状態をリセット
}

// --- Qキー タップダンスハンドラ ---
void on_dance_q(tap_dance_state_t *state, void *user_data) {
    uint8_t current_state = determine_dance_state(state);
    dance_state[TD_Q_TAB] = current_state;

    switch (current_state) {
        case TD_STATE_SINGLE_TAP: register_code(KC_Q); break; // タップ: Q を押す
        case TD_STATE_SINGLE_HOLD: register_code(KC_Q); break; // ホールド: Q を押し続ける (意図的な動作か確認推奨)
        case TD_STATE_DOUBLE_TAP: tap_code(KC_TAB); break;    // ダブルタップ: Tab を送信
    }
}

void on_dance_q_reset(tap_dance_state_t *state, void *user_data) {
    switch (dance_state[TD_Q_TAB]) {
        case TD_STATE_SINGLE_TAP: unregister_code(KC_Q); break; // Q を離す
        case TD_STATE_SINGLE_HOLD: unregister_code(KC_Q); break; // ホールドしていた Q を離す
        case TD_STATE_DOUBLE_TAP: break;                        // tap_code は自動解除
    }
    dance_state[TD_Q_TAB] = TD_STATE_NONE; // 状態をリセット
}

// --- タップダンスアクションの登録 ---
tap_dance_action_t tap_dance_actions[] = {
    [TD_F_ENG] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, on_dance_f, on_dance_f_reset, 150), // TAPPING_TERM を個別指定する場合
    // [TD_F_ENG] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, on_dance_f, on_dance_f_reset), // グローバル TAPPING_TERM を使う場合
    [TD_J_JPN] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, on_dance_j, on_dance_j_reset),
    [TD_Q_TAB] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, on_dance_q, on_dance_q_reset),
};
// Note: ACTION_TAP_DANCE_FN_ADVANCED_TIME を使うと、config.h の TAPPING_TERM_PER_KEY がなくてもキーごとにタップ判定時間を指定できます。
// グローバル設定を使う場合は ACTION_TAP_DANCE_FN_ADVANCED のままでOKです。

// --- コンボ定義 ---
#ifdef COMBO_ENABLE
// コンボのキーシーケンス定義
const uint16_t PROGMEM combo_mcomma[] = {KC_M, KC_COMM, COMBO_END};       // M + ,
// const uint16_t PROGMEM combo_jk[] = {KC_J, KC_K, COMBO_END};          // J + K (コメントアウト: 不要なら削除)
const uint16_t PROGMEM combo_commadot[] = {KC_COMM, KC_DOT, COMBO_END}; // , + .
const uint16_t PROGMEM combo_kl[] = {KC_K, KC_L, COMBO_END};             // K + L
const uint16_t PROGMEM combo_qw[] = {TD(TD_Q_TAB), KC_W, COMBO_END};     // Q(TapDance) + W
const uint16_t PROGMEM combo_tdjk[] = {TD(TD_J_JPN), KC_K, COMBO_END};   // J(TapDance) + K

// コンボと対応するキーコードのマッピング
combo_t key_combos[] = {
    // 注意: combo_mcomma と combo_tdjk が同じ KC_BTN1 に割り当てられています。意図通りか確認してください。
    COMBO(combo_mcomma, KC_BTN1), // M + , => Mouse Button 1
    COMBO(combo_tdjk, KC_BTN1),   // J(TapDance) + K => Mouse Button 1

    // 注意: combo_commadot と combo_kl が同じ KC_BTN2 に割り当てられています。意図通りか確認してください。
    COMBO(combo_commadot, KC_BTN2), // , + . => Mouse Button 2
    COMBO(combo_kl, KC_BTN2),       // K + L => Mouse Button 2

    COMBO(combo_qw, KC_ESC),      // Q(TapDance) + W => Escape
};
#endif

// --- キーマップ定義 ---
// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // Layer 0: Default Layer
  [0] = LAYOUT_universal(
    TD(TD_Q_TAB), KC_W,         KC_E,         KC_R,         KC_T,                                    KC_Y,         KC_U,         KC_I,         KC_O,         KC_P,
    LCTL_T(KC_A), KC_S,         KC_D,         TD(TD_F_ENG), LT(4, KC_G),                            LT(4, KC_H), TD(TD_J_JPN), KC_K,      LT(3, KC_L), KC_MINS,
    LSFT_T(KC_Z), KC_X,         KC_C,         LT(2, KC_V), KC_B,                                    KC_N,         LT(2, KC_M), KC_COMM,      KC_DOT,       LSFT_T(KC_SLSH),
    KC_LSFT,      KC_LGUI,      KC_LALT,      LALT_T(KC_TAB), KC_LGUI, LT(1, KC_SPC),                  LGUI_T(KC_ENT), LT(1, KC_BSPC), _______,      _______,      _______,      LSFT_T(KC_BSLS)
  ),

  // Layer 1: Symbols and Function Keys
  [1] = LAYOUT_universal(
    KC_F1,        KC_F2,        KC_F3,        KC_F4,        KC_RBRC,                                 KC_F6,        KC_F7,        KC_F8,        KC_F9,        KC_F10,
    KC_F5,        KC_EXLM,      S(KC_6),      S(KC_INT3),   S(KC_8),                                 S(KC_INT1),   KC_BTN1,      KC_PGUP,      KC_BTN2,      KC_SCLN,
    S(KC_EQL),    S(KC_LBRC),   S(KC_7),      S(KC_2),      S(KC_RBRC),                              KC_LBRC,      KC_DLR,       KC_PGDN,      KC_BTN3,      KC_F11,
    KC_INT1,      KC_EQL,       S(KC_3),      _______,      _______,      _______,                    TO(2),        TO(0),        _______,      KC_RALT,      KC_RGUI,      KC_F12
  ),

  // Layer 2: Numpad and Navigation
  [2] = LAYOUT_universal(
    KC_TAB,       KC_7,         KC_8,         KC_9,         KC_MINS,                                 KC_NUHS,      _______,      KC_BTN3,      _______,      KC_BSPC,
    S(KC_QUOT),   KC_4,         KC_5,         KC_6,         S(KC_SCLN),                              S(KC_9),      KC_BTN1,      KC_UP,        KC_BTN2,      KC_QUOT,
    KC_SLSH,      KC_1,         KC_2,         KC_3,         S(KC_MINS),                              S(KC_NUHS),   KC_LEFT,      KC_DOWN,      KC_RGHT,      _______,
    KC_ESC,       KC_0,         KC_DOT,       KC_DEL,       KC_ENT,       KC_BSPC,                    _______,      _______,      _______,      _______,      _______,      _______
  ),

  // Layer 3: System, RGB, Mouse Control (Keyball specific?)
  [3] = LAYOUT_universal(
    RGB_TOG,      AML_TO,       AML_I50,      AML_D50,      _______,                                 _______,      _______,      SSNP_HOR,     SSNP_VRT,     SSNP_FRE,
    RGB_MOD,      RGB_HUI,      RGB_SAI,      RGB_VAI,      SCRL_DVI,                                _______,      _______,      _______,      _______,      _______,
    RGB_RMOD,     RGB_HUD,      RGB_SAD,      RGB_VAD,      SCRL_DVD,                                CPI_D1K,      CPI_D100,     CPI_I100,     CPI_I1K,      KBC_SAVE,
    QK_BOOT,      KBC_RST,      _______,      _______,      _______,      _______,                    _______,      _______,      _______,      _______,      KBC_RST,      QK_BOOT
  ),

  // Layer 4: (定義はありますが、レイアウトがありません。LT(4, KC_G) などで参照されています)
  // 必要であれば定義を追加してください
  // [4] = LAYOUT_universal( ... )
};
// clang-format on

// --- ユーザー関数 ---


// レイヤー変更時の処理 (Keyball スクロールモード制御)
layer_state_t layer_state_set_user(layer_state_t state) {
    // レイヤー3がアクティブならスクロールモード有効化 (Keyball機能)
    keyball_set_scroll_mode(get_highest_layer(state) == 3);
    return state;
}

// OLED ディスプレイ表示処理
#ifdef OLED_ENABLE
#    include "lib/oledkit/oledkit.h" // OLEDが有効な場合のみインクルード

// OLED 表示内容の描画 (Keyball機能)
void oledkit_render_info_user(void) {
    keyball_oled_render_keyinfo();
    keyball_oled_render_ballinfo();
    keyball_oled_render_layerinfo();
}
#endif