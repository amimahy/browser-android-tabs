// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/browser/origin_manifest/origin_manifest_parser.h"  // nogncheck

#include <stddef.h>
#include <stdint.h>

namespace content {

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  std::string json(reinterpret_cast<const char*>(data), size);

  origin_manifest_parser::Parse(json);

  return 0;
}

}  // namespace content
