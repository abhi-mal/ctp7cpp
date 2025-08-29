#include <iostream>
#include <fstream>
#include "algo_unpacked.h"
#include "defines.h"
#include <unistd.h> 
#include <cmath>

int main() {
    char current_path[1024];
    if (getcwd(current_path, sizeof(current_path)) != NULL) {
        std::cout << "INFO: C++ program is running from: " << current_path << std::endl;
    } else {
        perror("getcwd() error");
    }    
    // Open your input and golden output files
    std::ifstream infile("/nfs_scratch/mallampalli/open_data/cicada/synthesizing_with_wrapper/hgq/ctp7cpp/vivado_hls/tb_data/inputs.dat");
    std::ifstream goldfile("/nfs_scratch/mallampalli/open_data/cicada/synthesizing_with_wrapper/hgq/ctp7cpp/vivado_hls/tb_data/golden_outputs.dat");
    //ap_uint<128> test_value;
    if (!infile.is_open()) {
        std::cerr << "ERROR: Cannot open inputs.dat" << std::endl;
        return 1;
    }
    //infile >> test_value;
    //infile.close();
    //std::cout << "--- Minimal test successful! Read value: " << test_value.to_string(16) << " ---" << std::endl;
    //// We return 0 immediately to bypass the rest of the logic for this test
    //return 0;

    int total_errors = 0;
    const int NUM_TESTS = 62;//6379;//1000;
    ap_uint<128> link_in_data[N_CH_IN];
    ap_uint<192> link_out_data[N_CH_OUT];
    result_t golden_output[N_LAYER_13];
    result_t unpacked_result[N_LAYER_13];
    const int frac_bits = result_t::width - result_t::iwidth;
    std::cout << "frac_bits: " << frac_bits << std::endl;
    double scaling_factor = pow(2, frac_bits);
    double read_value;
    double float_value;

    // Loop through all 1000 test cases
    for (int t = 0; t < NUM_TESTS; ++t) {

        // --- Read one line of data for the current test case ---
        for (int i = 0; i < N_CH_IN; ++i) {
            infile >> link_in_data[i];
            //std::cout << "input value " << link_in_data[i] << "!" << std::endl;
        }
        for (int i = 0; i < N_LAYER_13; ++i) {
            goldfile >> read_value;
            std::cout << "read value " << read_value << "!" << std::endl;
            float_value = read_value/scaling_factor;
            std::cout << "float_value " << float_value << "!" << std::endl;
            golden_output[i] = float_value;
        }

        // --- Execute the wrapper ---
        algo_unpacked(link_in_data, link_out_data);

        // --- Verify the result for the current test case ---
        unpacked_result[0].range(15, 12) = link_out_data[0].range(31, 28);
        unpacked_result[0].range(11, 8)  = link_out_data[0].range(63, 60);
        unpacked_result[0].range(7, 4)   = link_out_data[0].range(95, 92);
        unpacked_result[0].range(3, 0)   = link_out_data[0].range(127, 124);

        int current_errors = 0;
        for (int i = 0; i < N_LAYER_13; i++) {
            if (unpacked_result[i] != golden_output[i]) {
                std::cout << "ERROR: Mismatch at output index " << i << "!" << std::endl;
                std::cout << "  Got: " << unpacked_result[i]
                      << ", Expected: " << golden_output[i] << std::endl;
                current_errors++;
            }
        }

        if (current_errors > 0) {
            std::cout << "ERROR: Mismatch found in test case " << t << std::endl;
            total_errors++;
        }
    } // End of loop for all test cases

    infile.close();
    goldfile.close();

    // Final PASS/FAIL based on all tests
    if (total_errors == 0) {
        std::cout << "--- All " << NUM_TESTS << " tests PASSED! ---" << std::endl;
        return 0; // PASS
    } else {
        std::cout << "--- FAILED: " << total_errors << " out of " << NUM_TESTS 
                  << " tests had errors. ---" << std::endl;
        return 1; // FAIL
    }
}