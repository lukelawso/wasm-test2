#pragma once

extern const int c_screen_width;
extern const int c_screen_height;

class Shape {
public:
  virtual void clamp_to_map(float &x, float &y) = 0;
  virtual ~Shape() = default;
};

class Rect : public Shape {
  float width;
  float height;

public:
  Rect(float width, float height) : width(width), height(height) {}
  ~Rect() = default;

  void clamp_to_map(float &x, float &y) override {
    // simply clamp to edges of screen now
    if (x < 0)
      x = 0;
    if (y < 0)
      y = 0;
    if (x > c_screen_width - width)
      x = c_screen_width - width;
    if (y > c_screen_height - height)
      y = c_screen_height - height;
  }
};
