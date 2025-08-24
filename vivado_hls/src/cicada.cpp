#include <iostream>

#include "cicada.h"
#include "parameters.h"


void cicada(
    inputs__t inputs_[N_INPUT_0_1*N_INPUT_1_1*N_INPUT_2_1],
    result_t layer14_out[N_LAYER_13]
) {

    // hls-fpga-machine-learning insert IO
    #pragma HLS INLINE

    // hls-fpga-machine-learning insert load weights
#ifndef __SYNTHESIS__
    static bool loaded_weights = false;
    if (!loaded_weights) {
        nnet::load_weights_from_txt<conv_weight_t, 16>(w4, "w4.txt");
        nnet::load_weights_from_txt<conv_bias_t, 4>(b4, "b4.txt");
        nnet::load_weights_from_txt<dense1_weight_t, 4032>(w9, "w9.txt");
        nnet::load_weights_from_txt<dense1_bias_t, 16>(b9, "b9.txt");
        nnet::load_weights_from_txt<dense2_weight_t, 16>(w13, "w13.txt");
        nnet::load_weights_from_txt<dense2_bias_t, 1>(b13, "b13.txt");
        loaded_weights = true;    }
#endif
    // ****************************************
    // NETWORK INSTANTIATION
    // ****************************************

    // hls-fpga-machine-learning insert layers

    conv_iq_t layer3_out[N_SIZE_0_2*N_SIZE_1_2*N_SIZE_2_2];
    #pragma HLS ARRAY_PARTITION variable=layer3_out complete dim=0

    conv_t layer4_out[OUT_HEIGHT_4*OUT_WIDTH_4*N_FILT_4];
    #pragma HLS ARRAY_PARTITION variable=layer4_out complete dim=0

    conv_relu_t layer5_out[OUT_HEIGHT_4*OUT_WIDTH_4*N_FILT_4];
    #pragma HLS ARRAY_PARTITION variable=layer5_out complete dim=0

    auto& layer6_out = layer5_out;
    dense1_iq_t layer8_out[N_SIZE_0_6];
    #pragma HLS ARRAY_PARTITION variable=layer8_out complete dim=0

    dense1_t layer9_out[N_LAYER_9];
    #pragma HLS ARRAY_PARTITION variable=layer9_out complete dim=0

    dense1_relu_t layer10_out[N_LAYER_9];
    #pragma HLS ARRAY_PARTITION variable=layer10_out complete dim=0

    dense2_iq_t layer12_out[N_LAYER_9];
    #pragma HLS ARRAY_PARTITION variable=layer12_out complete dim=0

    dense2_t layer13_out[N_LAYER_13];
    #pragma HLS ARRAY_PARTITION variable=layer13_out complete dim=0

    nnet::conv_iq<inputs__t, conv_iq_t>(inputs_, layer3_out); // conv_iq

    nnet::conv_2d_cl<conv_iq_t, conv_t, config4>(layer3_out, layer4_out, w4, b4); // conv

    nnet::relu<conv_t, conv_relu_t, relu_config5>(layer4_out, layer5_out); // conv_relu

    nnet::dense1_iq<conv_relu_t, dense1_iq_t>(layer6_out, layer8_out); // dense1_iq

    nnet::dense<dense1_iq_t, dense1_t, config9>(layer8_out, layer9_out, w9, b9); // dense1

    nnet::relu<dense1_t, dense1_relu_t, relu_config10>(layer9_out, layer10_out); // dense1_relu

    // Create a new array to act as a pipeline register between the ReLU and the next Dense layer.
    // Use 'volatile' to prevent HLS from optimizing this register away.
    //volatile dense1_relu_t layer10_out_registered[N_LAYER_9];
    //#pragma HLS ARRAY_PARTITION variable=layer10_out_registered complete dim=0    

    // This pipelined loop copies the data in one clock cycle, effectively inserting a register stage.
    //Register_Stage_1: for (int i = 0; i < N_LAYER_9; i++) {
    //    #pragma HLS PIPELINE
    //    layer10_out_registered[i] = layer10_out[i];
    //}
    #pragma HLS INLINE off
    nnet::dense2_iq<dense1_relu_t, dense2_iq_t>(layer10_out, layer12_out); // dense2_iq

    nnet::dense<dense2_iq_t, dense2_t, config13>(layer12_out, layer13_out, w13, b13); // dense2

    nnet::relu<dense2_t, result_t, relu_config14>(layer13_out, layer14_out); // dense2_relu

}

