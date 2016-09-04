#pragma once

#include "cocos2d.h"
using namespace cocos2d;

// KEYBOARD
const EventKeyboard::KeyCode ENTER = EventKeyboard::KeyCode::KEY_KP_ENTER;
const EventKeyboard::KeyCode UP_ARROW = EventKeyboard::KeyCode::KEY_UP_ARROW;
const EventKeyboard::KeyCode DOWN_ARROW = EventKeyboard::KeyCode::KEY_DOWN_ARROW;
const EventKeyboard::KeyCode LEFT_ARROW = EventKeyboard::KeyCode::KEY_LEFT_ARROW;
const EventKeyboard::KeyCode RIGHT_ARROW = EventKeyboard::KeyCode::KEY_RIGHT_ARROW;
const EventKeyboard::KeyCode SPACEBAR = EventKeyboard::KeyCode::KEY_SPACE;

const float SCALE_FACTOR = 1.2f;
const float ENEMY_SCALE_FACTOR = 1.4f;
const float PLAYER_MAX_VELOCITY = 5.0f;
const float PLAYER_JUMP_VELOCITY = 12.0f;
const float DAMPING = 0.87f;
const float GRAVITY = 0.37f;
const int TRANSITION_TIME = 2;
const int startingX = 35;
const int startingY = 3;
