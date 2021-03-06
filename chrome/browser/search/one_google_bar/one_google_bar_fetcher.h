// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SEARCH_ONE_GOOGLE_BAR_ONE_GOOGLE_BAR_FETCHER_H_
#define CHROME_BROWSER_SEARCH_ONE_GOOGLE_BAR_ONE_GOOGLE_BAR_FETCHER_H_

#include "base/callback_forward.h"
#include "base/optional.h"

struct OneGoogleBarData;

// Interface for fetching OneGoogleBarData over the network.
class OneGoogleBarFetcher {
 public:
  enum class Status {
    // Received a valid response.
    OK,
    // Some transient error occurred, e.g. the network request failed because
    // there is no network connectivity. A previously cached response may still
    // be used.
    TRANSIENT_ERROR,
    // A fatal error occurred, such as the server responding with an error code
    // or with invalid data. Any previously cached response should be cleared.
    FATAL_ERROR
  };
  using OneGoogleCallback =
      base::OnceCallback<void(Status, const base::Optional<OneGoogleBarData>&)>;

  virtual ~OneGoogleBarFetcher() = default;

  // Initiates a fetch from the network. On completion (successful or not), the
  // callback will be called with the result, which will be nullopt on failure.
  virtual void Fetch(OneGoogleCallback callback) = 0;
};

#endif  // CHROME_BROWSER_SEARCH_ONE_GOOGLE_BAR_ONE_GOOGLE_BAR_FETCHER_H_
