#include <stddef.h>
#include "lz77.h"

LZ77Match find_longest_match(unsigned char *input_bytes, size_t input_length, size_t position) {
    
    LZ77Match result = {0, 0};
    int max_window_size = 32768;

    // search all possible window 
    size_t search_start = position > max_window_size ? position - max_window_size : 0;    
    size_t search_end = position < (search_start + max_window_size) ? position : search_start + max_window_size;

    while (search_start < position) {
        // curr window size
        size_t k = search_end - search_start;
        int curr_distance = position - search_start;
        int curr_best_length = 0;

        // DEFLATE requires us to limit the match length to 258 bytes & no more
        for (size_t i=0; i < k && i < 258 && (position + i) < input_length; i++) {
            if((input_bytes[position + i] ^ input_bytes[search_start+i]) == 0) {
                curr_best_length = i + 1;
            } else {
                break;
            }
        }
        
        if (curr_best_length > result.length) {
            result.length = curr_best_length;
            result.distance = curr_distance; 
        }

        search_start += 1;
    }

    return result;
}