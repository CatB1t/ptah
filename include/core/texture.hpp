#pragma once

namespace ptah {
class Texture {
 public:
  virtual void Bind(unsigned int slot) = 0;
  virtual ~Texture() {};
};
}  // namespace ptah