#ifndef API_ENGINE_H
#define API_ENGINE_H

namespace engine {

  void RenderLoop(float scale      = 1.0f,
                  int   win_width  = 2560,
                  int   win_height = 1440,
                  float col_bg[4]  = new float[] { 0.0f, 0.0f, 0.0f, 1.0f },
                  bool  resizable  = true);

} // namespace engine

#endif // API_ENGINE_H
