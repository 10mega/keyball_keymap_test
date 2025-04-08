/*
This is the c configuration file for the keymap

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

#pragma once

#define TAP_CODE_DELAY 5

#define POINTING_DEVICE_AUTO_MOUSE_ENABLE
#define AUTO_MOUSE_DEFAULT_LAYER 1

#define DYNAMIC_KEYMAP_LAYER_COUNT 6

#undef LOCKING_SUPPORT_ENABLE
#undef LOCKING_RESYNC_ENABLE

// 容量削減のための設定
#undef RGBLIGHT_ENABLE
#undef RGB_MATRIX_ENABLE
#undef RGB_MATRIX_ANIMATIONS
#undef RGB_MATRIX_FRAMEBUFFER_EFFECTS
#undef RGB_MATRIX_KEYPRESSES
#undef RGB_MATRIX_KEYRELEASES

// OLED設定
#define OLED_UPDATE_INTERVAL 50
#define OLED_BRIGHTNESS 128

// タップダンスの設定
#define TAPPING_TERM 170  // タップ判定時間を150msに設定
#define PERMISSIVE_HOLD  // ホールドの判定を早くする