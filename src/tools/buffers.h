//
// Created by clemens on 12.02.19.
//

#ifndef SELF_O_MAT_BUFFERS_H
#define SELF_O_MAT_BUFFERS_H

#include <iostream>

namespace selfomat {
    namespace tools {
        bool requireBufferWithSize(void **buffer, size_t *bufferSize, size_t requestedSize);
    }
}

#endif //SELF_O_MAT_BUFFERS_H
