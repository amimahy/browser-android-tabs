// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/layout/ng/geometry/ng_bfc_offset.h"

#include "platform/wtf/text/WTFString.h"

namespace blink {

bool NGBfcOffset::operator==(const NGBfcOffset& other) const {
  return std::tie(other.line_offset, other.block_offset) ==
         std::tie(line_offset, block_offset);
}

bool NGBfcOffset::operator!=(const NGBfcOffset& other) const {
  return !operator==(other);
}

String NGBfcOffset::ToString() const {
  return String::Format("%dx%d", line_offset.ToInt(), block_offset.ToInt());
}

std::ostream& operator<<(std::ostream& os, const NGBfcOffset& value) {
  return os << value.ToString();
}

}  // namespace blink
