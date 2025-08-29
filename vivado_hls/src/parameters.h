#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include "ap_fixed.h"
#include "ap_int.h"

#include "nnet_utils/nnet_code_gen.h"
#include "nnet_utils/nnet_helpers.h"
// hls-fpga-machine-learning insert includes
#include "nnet_utils/nnet_activation.h"
#include "nnet_utils/nnet_activation_stream.h"
#include "nnet_utils/nnet_conv2d.h"
#include "nnet_utils/nnet_conv2d_stream.h"
#include "nnet_utils/nnet_dense.h"
#include "nnet_utils/nnet_dense_compressed.h"
#include "nnet_utils/nnet_dense_stream.h"

// hls-fpga-machine-learning insert weights
#include "weights/w4.h"
#include "weights/b4.h"
#include "weights/w9.h"
#include "weights/b9.h"
#include "weights/w13.h"
#include "weights/b13.h"


// hls-fpga-machine-learning insert layer-config
// conv
struct config4_mult : nnet::dense_config {
    static const unsigned n_in = 4;
    static const unsigned n_out = 4;
    static const unsigned reuse_factor = 1;
    static const unsigned strategy = nnet::latency;
    static const unsigned n_zeros = 12;
    static const unsigned multiplier_limit = DIV_ROUNDUP(n_in * n_out, reuse_factor) - n_zeros / reuse_factor;
    typedef conv_accum_t accum_t;
    typedef conv_bias_t bias_t;
    typedef conv_weight_t weight_t;
    template<class data_T, class res_T, class CONFIG_T>
    using kernel = nnet::DenseLatency<data_T, res_T, CONFIG_T>;
    template<class x_T, class y_T>
    using product = nnet::product::mult<x_T, y_T>;
};

struct config4 : nnet::conv2d_config {
    static const unsigned pad_top = 0;
    static const unsigned pad_bottom = 0;
    static const unsigned pad_left = 0;
    static const unsigned pad_right = 0;
    static const unsigned in_height = 18;
    static const unsigned in_width = 14;
    static const unsigned n_chan = 1;
    static const unsigned filt_height = 2;
    static const unsigned filt_width = 2;
    static const unsigned kernel_size = filt_height * filt_width;
    static const unsigned n_filt = 4;
    static const unsigned stride_height = 2;
    static const unsigned stride_width = 2;
    static const unsigned out_height = 9;
    static const unsigned out_width = 7;
    static const unsigned reuse_factor = 1;
    static const unsigned n_zeros = 12;
    static const unsigned multiplier_limit =
        DIV_ROUNDUP(kernel_size * n_chan * n_filt, reuse_factor) - n_zeros / reuse_factor;
    static const bool store_weights_in_bram = false;
    static const unsigned strategy = nnet::latency;
    static const nnet::conv_implementation implementation = nnet::conv_implementation::linebuffer;
    static const unsigned min_height = 18;
    static const unsigned min_width = 14;
    static const ap_uint<filt_height * filt_width> pixels[min_height * min_width];
    static const unsigned n_partitions = 3;
    static const unsigned n_pixels = out_height * out_width / n_partitions;
    template<class data_T, class CONFIG_T>
    using fill_buffer = nnet::fill_buffer_4<data_T, CONFIG_T>;
    typedef conv_accum_t accum_t;
    typedef conv_bias_t bias_t;
    typedef conv_weight_t weight_t;
    typedef config4_mult mult_config;
    template<unsigned K, unsigned S, unsigned W>
    using scale_index_height = nnet::scale_index_regular<K, S, W>;
    template<unsigned K, unsigned S, unsigned W>
    using scale_index_width = nnet::scale_index_regular<K, S, W>;
};
const ap_uint<config4::filt_height * config4::filt_width> config4::pixels[] = {0};

// conv_relu
struct relu_config5 : nnet::activ_config {
    static const unsigned n_in = 252;
    static const unsigned table_size = 4096;
    static const unsigned io_type = nnet::io_parallel;
    static const unsigned reuse_factor = 1;
    typedef conv_relu_table_t table_t;
};

// dense1
struct config9 : nnet::dense_config {
    static const unsigned n_in = 252;
    static const unsigned n_out = 16;
    static const unsigned io_type = nnet::io_parallel;
    static const unsigned strategy = nnet::resource;
    static const unsigned reuse_factor = 1;
    static const unsigned n_zeros = 3699;
    static const unsigned n_nonzeros = 333;
    static const unsigned multiplier_limit = DIV_ROUNDUP(n_in * n_out, reuse_factor) - n_zeros / reuse_factor;
    static const bool store_weights_in_bram = false;
    typedef dense1_accum_t accum_t;
    typedef dense1_bias_t bias_t;
    typedef dense1_weight_t weight_t;
    typedef layer9_index index_t;
    template<class data_T, class res_T, class CONFIG_T>
    using kernel = nnet::DenseResource_rf_leq_nin<data_T, res_T, CONFIG_T>;
    template<class x_T, class y_T>
    using product = nnet::product::mult<x_T, y_T>;
};

// dense1_relu
struct relu_config10 : nnet::activ_config {
    static const unsigned n_in = 16;
    static const unsigned table_size = 32768;
    static const unsigned io_type = nnet::io_parallel;
    static const unsigned reuse_factor = 1;
    typedef dense1_relu_table_t table_t;
};

// dense2
struct config13 : nnet::dense_config {
    static const unsigned n_in = 16;
    static const unsigned n_out = 1;
    static const unsigned io_type = nnet::io_parallel;
    static const unsigned strategy = nnet::resource;
    static const unsigned reuse_factor = 1;
    static const unsigned n_zeros = 6;
    static const unsigned n_nonzeros = 10;
    static const unsigned multiplier_limit = DIV_ROUNDUP(n_in * n_out, reuse_factor) - n_zeros / reuse_factor;
    static const bool store_weights_in_bram = false;
    typedef dense2_accum_t accum_t;
    typedef dense2_bias_t bias_t;
    typedef dense2_weight_t weight_t;
    typedef layer13_index index_t;
    template<class data_T, class res_T, class CONFIG_T>
    using kernel = nnet::DenseResource_rf_leq_nin<data_T, res_T, CONFIG_T>;
    template<class x_T, class y_T>
    using product = nnet::product::mult<x_T, y_T>;
};

// dense2_relu
struct relu_config14 : nnet::activ_config {
    static const unsigned n_in = 1;
    static const unsigned table_size = 4096;
    static const unsigned io_type = nnet::io_parallel;
    static const unsigned reuse_factor = 1;
    typedef dense2_relu_table_t table_t;
};



#endif
