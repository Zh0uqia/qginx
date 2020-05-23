#pragma once

enum class TransportDirection : uint8_t{
    DOWNSTREAM, // toward the client
    UPSTREAM // toward the upstream server (e.g PHP)
};
