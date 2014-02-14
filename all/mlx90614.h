#ifndef _MLX90614_H_
#define _MLX90614_H_

class MLX90614 {
 public:
  int initialise(void);
  int read(float& ambientTemp, float& remoteTemp);
};

#ifdef _MAIN_
MLX90614 mlx90614;
#else
extern MLX90614 mlx90614;
#endif

#endif
