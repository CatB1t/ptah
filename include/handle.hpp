#pragma once

namespace ptah {
class Handle {
 private:
  unsigned int m_id;
  bool m_valid;

 public:
  Handle() : m_id(0), m_valid(false) {};
  Handle(unsigned int id) : m_id(id), m_valid(true) {};
  void Reset() {
    m_id = 0;
    m_valid = false;
  }
  void Set(unsigned int id) {
    m_id = id;
    m_valid = true;
  }
};
}  // namespace ptah