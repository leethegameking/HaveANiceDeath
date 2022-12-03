#pragma once

inline Vec2::operator ImVec2() const { return ImVec2(x, y); }
inline ImVec2::operator Vec2() const { return Vec2(x, y); }
inline Vec2::operator f2() const { return (f2)x; }
// inline Vec2::operator i2() const { int _x = (int)x; return (i2)_x; }

