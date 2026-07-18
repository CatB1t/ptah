#pragma once

namespace ptah {
class Context {
 private:
  const unsigned int kGlMajorVersion = 4;
  const unsigned int kGlMinorVersion = 6;
  static bool m_initialized;

 public:
  Context();
  bool IsInitialized();
  ~Context();
};
}  // namespace ptah