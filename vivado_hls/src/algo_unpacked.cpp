#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <hls_math.h>

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

#include "cicada.h"
#include "algo_unpacked.h"

const uint16_t NRegionsPerLink = 7; // Bits 8-21, 22-39, 40-55,..., 104-119, keeping ranges (7, 0) and (127, 120) unused
const uint16_t MaxRegions = N_CH_IN * NRegionsPerLink;

 /*
  * algo_unpacked interface exposes fully unpacked input and output link data.
  * This version assumes use of 10G 8b10b links, and thus providing 192bits/BX/link.
  *
  * !!! N.B.: Do NOT use the first and the last bytes of link_in/link_out (i.e. link_in/link_out[].range(7,0)
  * and link_in/link_out[].range(127, 120) as these fields are reserved for transmission of 8b10b input/output 
  * link alignment markers and CRC checksum word
  *
  * The remaining 112 bits are available for algorithm use.
  *
  * !!! N.B. 2: make sure to assign every bit of link_out[] data. First byte should be assigned zero.
  */

void algo_unpacked(ap_uint<128> link_in[N_CH_IN], ap_uint<192> link_out[N_CH_OUT])
{

// !!! Retain these 4 #pragma directives below in your algo_unpacked implementation !!!
#pragma HLS ARRAY_PARTITION variable=link_in complete dim=0
#pragma HLS ARRAY_PARTITION variable=link_out complete dim=0
#pragma HLS PIPELINE II=4//#pragma HLS DATAFLOW//
#pragma HLS INTERFACE ap_ctrl_hs port=return//#pragma HLS INTERFACE ap_ctrl_chain port=return //
   

    // null algo specific pragma: avoid fully combinatorial algo by specifying min latency
    // otherwise algorithm clock input (ap_clk) gets optimized away
#pragma HLS latency min=4

//  for (int lnk = 0; lnk < N_CH_OUT ; lnk++) {
//#pragma HLS UNROLL
////  pass-through "algo"
//       link_out[lnk].range(7,0) = 0;            // reserved for 8b10b control word
//       link_out[lnk].range(127,120) = 0;        // reserved for CRC word
//       link_out[lnk].range(119, 8) = link_in[lnk].range(119, 8) ;   // input to output pass-through
//    }

        ap_uint<192> tmp_link_out[N_CH_OUT];
#pragma HLS ARRAY_PARTITION variable=tmp_link_out    complete dim=0
        for (int idx = 0; idx < N_CH_OUT; idx++){
#pragma HLS UNROLL
                tmp_link_out[idx] = 0;
        }

        inputs__t et_calo_ad[N_INPUT_0_1*N_INPUT_1_1*N_INPUT_2_1];
#pragma HLS ARRAY_RESHAPE variable=et_calo_ad complete dim=0
        result_t cicada_out[N_LAYER_13];
#pragma HLS ARRAY_RESHAPE variable=cicada_out complete dim=0

        regionLoop: for(int iRegion = 0; iRegion < NR_CNTR_REG; iRegion++) {
#pragma HLS UNROLL
                if(iRegion > MaxRegions) {
                        fprintf(stderr, "Too many regions - aborting");
                        exit(1);
                }
                int link_idx = iRegion / NRegionsPerLink;
                int bitLo = ((iRegion - link_idx * NRegionsPerLink) % NRegionsPerLink) * 16 + 8;
                int bitHi = bitLo + 15;
                uint16_t region_raw = link_in[link_idx].range(bitHi, bitLo);
                et_calo_ad[iRegion] = (region_raw & 0x3FF >> 0);   // 10 bits
        }

        // Anomlay detection algorithm
        cicada(et_calo_ad, cicada_out);

        // Assign the algorithm outputs
        tmp_link_out[0].range(31, 28) = cicada_out[0].range(15, 12);
        tmp_link_out[0].range(63, 60) = cicada_out[0].range(11, 8);
        tmp_link_out[0].range(95, 92) = cicada_out[0].range(7, 4);
        tmp_link_out[0].range(127, 124) = cicada_out[0].range(3, 0);

        for(int i = 0; i < N_CH_OUT; i++){
#pragma HLS unroll
                link_out[i] = tmp_link_out[i];
        }
}
