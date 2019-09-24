//
// Created by clemens on 12.02.19.
//

#include "buffers.h"

bool selfomat::tools::requireBufferWithSize(void **buffer, size_t *bufferSize, size_t requestedSize) {
    if(requestedSize <= *bufferSize && *buffer != NULL) {
        // Buffer exists and is large enough -> we're done
        return  true;
    }
    // Free current buffer
    free(*buffer);

    // Null the buffer for safety
    *buffer = nullptr;
    *bufferSize = 0;

    // Create the new buffer
    void* newBuffer = malloc(requestedSize);

    if(newBuffer == nullptr) {
        LOG_E("BUFFERS", "Error allocating new buffer");
        return false;
    }

#ifdef DEBUG_BUFFER_FILL
    uint8_t current_value = 0;
    uint8_t *ptr = (uint8_t*)newBuffer;
    for(int i = 0; i < requestedSize; i++) {
        if(i%4 == 0)
            current_value++;
        ptr[i] = current_value;
    }
#endif

    // It worked, set the buffer and size, then return
    *buffer = newBuffer;
    *bufferSize = requestedSize;

    LOG_I("BUFFERS", "Allocated new buffer with size:", std::to_string(requestedSize));

    return true;
}