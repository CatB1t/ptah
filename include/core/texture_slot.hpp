#pragma once

namespace ptah {

enum class TextureSlot : unsigned int {
  Albedo = 0,
  Normal = 1,
  // TODO: Maybe use the packed AORM format
  Roughness = 2,
  Metalness = 3,
  AO = 4,
  Slot5 = 5,
  Slot6 = 6,
  Environment = 7,
  Slot8 = 8,
  Slot9 = 9,
  Slot10 = 10,
  Slot11 = 11,
  Slot12 = 12,
  Slot13 = 13,
  Slot14 = 14,
  Slot15 = 15,
  Count
};

}