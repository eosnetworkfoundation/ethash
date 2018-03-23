// Copyright 2018 Pawel Bylica.
// Licensed under the Apache License, Version 2.0. See the LICENSE file.

///@file
///
/// API design decisions:
///
/// 1. Signed integer type is used whenever the size of the type is not
///    restricted by the Ethash specification.
///    See http://www.aristeia.com/Papers/C++ReportColumns/sep95.pdf.
///    See https://stackoverflow.com/questions/10168079/why-is-size-t-unsigned/.
///    See https://github.com/Microsoft/GSL/issues/171.

#pragma once

#include <cstddef>
#include <cstdint>

namespace ethash
{
static constexpr int epoch_length = 30000;


union hash256
{
    uint64_t words[4] = {
        0,
    };
    uint32_t hwords[8];
    char bytes[32];
};

struct result
{
    hash256 final_hash;
    hash256 mix_hash;
};

struct epoch_context;


/// Calculates the epoch number out of the block number.
inline int get_epoch_number(int block_number)
{
    return block_number / epoch_length;
}

epoch_context* create_epoch_context(int epoch_number) noexcept;

void destroy_epoch_context(epoch_context* context) noexcept;


/// Init full dataset in the epoch context.
///
/// This allocates the memory for the full dataset and inits dataset items
/// marked as "not-generated".
///
/// @param context  The epoch context.
/// @return  Returns true if memory allocations succeeded, false otherwise.
bool init_full_dataset(epoch_context& context) noexcept;


result hash_light(const epoch_context& context, const hash256& header_hash, uint64_t nonce);

result hash(const epoch_context& context, const hash256& header_hash, uint64_t nonce);

uint64_t search_light(const epoch_context& context, const hash256& header_hash, uint64_t target,
    uint64_t start_nonce, size_t iterations);

uint64_t search(const epoch_context& context, const hash256& header_hash, uint64_t target,
    uint64_t start_nonce, size_t iterations);
}
