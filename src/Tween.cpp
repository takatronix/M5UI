#include "Tween.hpp"
std::vector<Tween*> Tween::_tweens;
std::vector<Tween*> Tween::_toRemove;
int Tween::_tweenNo = 0;