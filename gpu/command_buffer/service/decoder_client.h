// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_COMMAND_BUFFER_SERVICE_DECODER_CLIENT_H_
#define GPU_COMMAND_BUFFER_SERVICE_DECODER_CLIENT_H_

#include <stdint.h>

#include <string>

#include "gpu/gpu_export.h"

namespace gpu {

struct SyncToken;

class GPU_EXPORT DecoderClient {
 public:
  virtual ~DecoderClient() = default;

  // Prints a message (error/warning) to the console.
  virtual void OnConsoleMessage(int32_t id, const std::string& message) = 0;

  // Cache a newly linked shader.
  virtual void CacheShader(const std::string& key,
                           const std::string& shader) = 0;

  // Called when the decoder releases a fence sync. Allows the client to
  // reschedule waiting decoders.
  virtual void OnFenceSyncRelease(uint64_t release) = 0;

  // Called when the decoder needs to wait on a sync token. If the wait is valid
  // (fence sync is not released yet), the client must unschedule the command
  // buffer and return true. The client is responsible for rescheduling the
  // command buffer when the fence is released.  If the wait is a noop (fence is
  // already released) or invalid, the client must leave the command buffer
  // scheduled, and return false.
  virtual bool OnWaitSyncToken(const gpu::SyncToken&) = 0;

  // Called when the decoder needs to be descheduled while waiting for a fence
  // completion. The client is responsible for descheduling the command buffer
  // before returning, and then calling PerformPollingWork periodically to test
  // for the fence completion and possibly reschedule.
  virtual void OnDescheduleUntilFinished() = 0;

  // Called from PerformPollingWork when the decoder needs to be rescheduled
  // because the fence completed.
  virtual void OnRescheduleAfterFinished() = 0;
};

}  // namespace gpu

#endif  // GPU_COMMAND_BUFFER_SERVICE_DECODER_CLIENT_H_
