

#include <SDL/SDL.h>
#define main main

class SDL_YUV_Display {
public:

  enum SDL_Chroma {
    SDL_CHROMA_MONO = 400,
    SDL_CHROMA_420 = 420,
    SDL_CHROMA_422 = 422,
    SDL_CHROMA_444 = 444
  };

  bool init(int frame_width, int frame_height, enum SDL_Chroma chroma = SDL_CHROMA_420);

  void display(const unsigned char *Y, const unsigned char *U, const unsigned char *V,
               int stride, int chroma_stride);

  void close();

  bool doQuit() const;

  bool isOpen() const { return mWindowOpen; }

private:
  SDL_Surface *mScreen;
  SDL_Overlay *mYUVOverlay;
  SDL_Rect rect;
  bool mWindowOpen;

  SDL_Chroma mChroma;

  void display400(const unsigned char *Y,
                  int stride);

  void display420(const unsigned char *Y,
                  const unsigned char *U,
                  const unsigned char *V,
                  int stride, int chroma_stride);

  void display422(const unsigned char *Y,
                  const unsigned char *U,
                  const unsigned char *V,
                  int stride, int chroma_stride);

  void display444as422(const unsigned char *Y,
                       const unsigned char *U,
                       const unsigned char *V,
                       int stride, int chroma_stride);

  void display444as420(const unsigned char *Y,
                       const unsigned char *U,
                       const unsigned char *V,
                       int stride, int chroma_stride);
};
