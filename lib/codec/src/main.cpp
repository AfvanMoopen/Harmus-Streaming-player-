

#include "codec/codec.h"

Codec *codec;

extern "C" {
void codecInit() {
  if (codec == nullptr) {
    codec = new Codec();
  }
}

void codecSetBridgeName(char *bridgeName) {
  if (codec != nullptr) {
    codec->setBridgeName(string(bridgeName));
  }
}

void codecSetAudioBuffer(char *buffer) {
  if (codec != nullptr) {
    codec->setAudioBuffer((uint8_t *) buffer);
  }
}

void codecSetVideoBuffer(char *buffer) {
  if (codec != nullptr) {
    codec->setVideoBuffer((uint8_t *) buffer);
  }
}

void codecDecode(uint8_t *bytes, uint32_t length) {
  if (codec != nullptr) {
    codec->decode(bytes, length);
  }
}

int codecTry2Seek(char *buffer, uint32_t length) {
  if (codec != nullptr) {
    return codec->try2seek((uint8_t *) buffer, length);
  }
  return 1;
}

void codecFree() {
  if (codec != nullptr) {
    delete codec;
    codec = nullptr;
  }
}
}

// #include <iostream>
// #include <fstream>
//
// int main() {
//   std::ifstream infile("/Users/zhaoyang/Downloads/music1.flv", std::ifstream::binary);
//   if (!infile.is_open()) {
//     return -1;
//   }
//
//   infile.seekg(0, std::ios::end);
//   size_t length = infile.tellg();
//   infile.seekg(0, std::ios::beg);
//   std::cout << "file size: " << length << std::endl;
//
//   char *bytes = new char[length];
//   infile.read(bytes, length);
//
//   Codec codec;
//   codec.decode((uint8_t *)bytes, length);
//   delete[] bytes;
//   return 0;
// }
