#pragma once

namespace ptah {
template <typename T>
struct Handle {
 private:
  unsigned int m_id;
  bool m_valid;

 public:
  Handle() : m_id(0), m_valid(false) {};
  explicit Handle(unsigned int id) : m_id(id), m_valid(true) {};
  void Reset() {
    m_id = 0;
    m_valid = false;
  }
  void Set(unsigned int id) {
    m_id = id;
    m_valid = true;
  }
  unsigned int Id() const { return m_id; }
};

using MeshHandle = Handle<struct _Mesh>;
using MaterialHandle = Handle<struct _Material>;
using BufferHandle = Handle<struct _BufferArray>;

}  // namespace ptah
