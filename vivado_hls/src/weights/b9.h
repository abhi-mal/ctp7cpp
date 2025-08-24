//Numpy array shape [16]
//Min -10.000000000000
//Max 7.000000000000
//Number of zeros 6

#ifndef B9_H_
#define B9_H_

#ifndef __SYNTHESIS__
dense1_bias_t b9[16];
#else
dense1_bias_t b9[16] = {0, -8, 0, -8, 0, 0, -7, -7, -7, 7, 0, 0, -7, -8, -8, -10};

#endif

#endif
