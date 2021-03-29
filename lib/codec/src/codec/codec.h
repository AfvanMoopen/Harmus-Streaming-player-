

#ifndef CODEC_CODEC_H
#define CODEC_CODEC_H

#include <string>
#include "config.h"
#include "demuxer/decoder.h"

#ifdef USE_OPEN_H265
#include "factor/h265_codec_factor.h"
#else
#include "factor/codec_factor.h"
#endif

#ifdef USE_OPEN_H265
#include "de265/libde265/de265.h"
#elif defined(USE_OPEN_H264)
#include "openh264/codec/api/svc/codec_api.h"
#else
#include "tinyh264/tinyh264.h"
#endif

using namespace std;

class Codec {
public:
  Codec() : _decoder(make_shared<Decoder>()),adtsHeader(make_shared<Buffer>()),
            sps(make_shared<Buffer>()), pps(make_shared<Buffer>()),
            vps(make_shared<Buffer>()), audioBuffer(nullptr), videoBuffer(nullptr) {
#ifdef USE_OPEN_H265
    storage = de265_new_decoder();
    de265_set_parameter_bool(storage, DE265_DECODER_PARAM_SUPPRESS_FAULTY_PICTURES, false);
    de265_set_parameter_bool(storage, DE265_DECODER_PARAM_DISABLE_DEBLOCKING, true);
    de265_set_parameter_bool(storage, DE265_DECODER_PARAM_DISABLE_SAO, true);
#elif defined(USE_OPEN_H264)
    SDecodingParam sDecParam = {0};
    sDecParam.uiTargetDqLayer = (uint8_t) -1;
    sDecParam.sVideoProperty.eVideoBsType = VIDEO_BITSTREAM_DEFAULT;

    WelsCreateDecoder(&storage);
    storage->Initialize(&sDecParam);
#else
    storage = h264bsdAlloc();
    h264bsdInit(storage, 0);
#endif

#ifdef USE_OPEN_H265
    _factor = make_shared<H265CodecFactor>(this);
#else
    _factor = make_shared<CodecFactor>(this);
#endif
    _decoder->setFactor(_factor);
  };

  void decode(uint8_t *bytes, uint32_t byteLen);

  void setBridgeName(string bridge) {
    bridgeName = bridge;
  }

  void setAudioBuffer(uint8_t *ptr) {
    audioBuffer = ptr;
  }

  void setVideoBuffer(uint8_t *ptr) {
    videoBuffer = ptr;
  }

  uint32_t try2seek(uint8_t *bytes, uint32_t byteLen);

  ~Codec() {
#ifdef USE_OPEN_H265
    de265_flush_data(storage);
    de265_free_decoder(storage);
    storage = nullptr;
#elif defined(USE_OPEN_H264)
    storage->Uninitialize();
    WelsDestroyDecoder(storage);
    storage = nullptr;
#else
    h264bsdShutdown(storage);
    h264bsdFree(storage);
    storage = nullptr;
#endif
  }

  string bridgeName;
  uint8_t *audioBuffer;
  uint8_t *videoBuffer;
  shared_ptr<Buffer> adtsHeader;
  shared_ptr<Buffer> sps;
  shared_ptr<Buffer> pps;
  shared_ptr<Buffer> vps;
  int lengthSizeMinusOne;
private:
  shared_ptr<DecoderFactor> _factor;
  shared_ptr<Decoder> _decoder;

#ifdef USE_OPEN_H265
  public:
    de265_decoder_context *storage;
#elif defined(USE_OPEN_H264)
  public:
    ISVCDecoder *storage;
#else
  public:
    storage_t *storage;
#endif
};

#endif //CODEC_CODEC_H
