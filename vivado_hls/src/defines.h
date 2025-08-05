#ifndef DEFINES_H_
#define DEFINES_H_

#include "ap_fixed.h"
#include "ap_int.h"
#include "nnet_utils/nnet_types.h"
#include <cstddef>
#include <cstdio>


// hls-fpga-machine-learning insert numbers
constexpr size_t N_SIZE_0_6 = 252;
constexpr size_t N_LAYER_13 = 1;
constexpr size_t OUT_HEIGHT_4 = 9;
constexpr size_t OUT_WIDTH_4 = 7;
constexpr size_t N_FILT_4 = 4;
constexpr size_t N_LAYER_9 = 16;
constexpr size_t N_SIZE_1_2 = 14;
constexpr size_t N_SIZE_0_2 = 18;
constexpr size_t N_INPUT_1_1 = 14;
constexpr size_t N_SIZE_2_2 = 1;
constexpr size_t N_INPUT_2_1 = 1;
constexpr size_t N_INPUT_0_1 = 18;

// hls-fpga-machine-learning insert layer-precision
typedef ap_fixed<7,6,AP_TRN,AP_SAT,0> inputs__t;
typedef ap_fixed<6,6> conv_iq_t;
typedef ap_fixed<10,5> conv_accum_t;
typedef ap_fixed<7,5> conv_t;
typedef ap_fixed<6,1> conv_weight_t;
typedef ap_ufixed<1,32> conv_bias_t;
typedef ap_ufixed<6,4> conv_relu_t;
typedef ap_fixed<18,8> conv_relu_table_t;
typedef ap_fixed<5,4> dense1_iq_t;
typedef ap_fixed<15,9> dense1_accum_t;
typedef ap_fixed<14,9> dense1_t;
typedef ap_fixed<10,4> dense1_weight_t;
typedef ap_fixed<7,5> dense1_bias_t;
typedef ap_uint<1> layer9_index;
typedef ap_ufixed<13,8> dense1_relu_t;
typedef ap_fixed<18,8> dense1_relu_table_t;
typedef ap_fixed<12,8> dense2_iq_t;
typedef ap_fixed<19,10> dense2_accum_t;
typedef ap_fixed<19,10> dense2_t;
typedef ap_fixed<8,2> dense2_weight_t;
typedef ap_ufixed<1,32> dense2_bias_t;
typedef ap_uint<1> layer13_index;
typedef ap_ufixed<18,9> result_t;
typedef ap_fixed<18,8> dense2_relu_table_t;


#endif
