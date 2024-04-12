// Copyright 2024, NVIDIA CORPORATION & AFFILIATES. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of NVIDIA CORPORATION nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include "tensorrt_llm/batch_manager/namedTensor.h"

#include <list>

using namespace tensorrt_llm::batch_manager;

namespace triton::backend::inflight_batcher_llm
{

// Represent an answer from TRT-LLM that can be sent back to Triton
// This class provides helper methods to serialize/deserialize
class InferenceAnswer
{
public:
    explicit InferenceAnswer(uint64_t request_id)
        : request_id_(request_id)
    {
    }

    InferenceAnswer(uint64_t request_id, std::list<NamedTensor> const& response_tensors, bool final_response,
        std::string const& err_msg)
        : request_id_(request_id)
        , response_tensors_(response_tensors)
        , final_response_(final_response)
        , err_msg_(err_msg)
    {
    }

    uint64_t GetRequestId() const
    {
        return request_id_;
    }

    bool IsFinalResponse() const
    {
        return final_response_;
    }

    std::list<NamedTensor> const& GetTensors() const
    {
        return response_tensors_;
    }

    std::string const& GetErrorMessage() const
    {
        return err_msg_;
    }

    [[nodiscard]] std::vector<int64_t> serialize() const;

    static std::shared_ptr<InferenceAnswer> deserialize(std::vector<int64_t> const& packed);

    static std::shared_ptr<InferenceAnswer> deserialize(int64_t const* packed_ptr);

private:
    uint64_t request_id_;
    std::list<NamedTensor> response_tensors_;
    bool final_response_;
    std::string err_msg_;
};

} // namespace triton::backend::inflight_batcher_llm
