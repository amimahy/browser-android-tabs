// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMECAST_NET_FAKE_STREAM_SOCKET_H_
#define CHROMECAST_NET_FAKE_STREAM_SOCKET_H_

#include <stdint.h>

#include <memory>

#include "base/macros.h"
#include "net/base/ip_endpoint.h"
#include "net/log/net_log_with_source.h"
#include "net/socket/stream_socket.h"
#include "net/traffic_annotation/network_traffic_annotation.h"

namespace chromecast {
class SocketBuffer;

// Fake StreamSocket that communicates with another instance in memory.
class FakeStreamSocket : public net::StreamSocket {
 public:
  explicit FakeStreamSocket(const net::IPEndPoint& local_address);
  ~FakeStreamSocket() override;

  // Sets the peer for this socket.
  void SetPeer(FakeStreamSocket* peer);

  // net::StreamSocket implementation:
  int Read(net::IOBuffer* buf,
           int buf_len,
           const net::CompletionCallback& callback) override;
  int Write(
      net::IOBuffer* buf,
      int buf_len,
      const net::CompletionCallback& callback,
      const net::NetworkTrafficAnnotationTag& traffic_annotation) override;
  int SetReceiveBufferSize(int32_t size) override;
  int SetSendBufferSize(int32_t size) override;
  int Connect(const net::CompletionCallback& callback) override;
  void Disconnect() override;
  bool IsConnected() const override;
  bool IsConnectedAndIdle() const override;
  int GetPeerAddress(net::IPEndPoint* address) const override;
  int GetLocalAddress(net::IPEndPoint* address) const override;
  const net::NetLogWithSource& NetLog() const override;
  void SetSubresourceSpeculation() override;
  void SetOmniboxSpeculation() override;
  bool WasEverUsed() const override;
  bool WasAlpnNegotiated() const override;
  net::NextProto GetNegotiatedProtocol() const override;
  bool GetSSLInfo(net::SSLInfo* ssl_info) override;
  void GetConnectionAttempts(net::ConnectionAttempts* out) const override;
  void ClearConnectionAttempts() override;
  void AddConnectionAttempts(const net::ConnectionAttempts& attempts) override;
  int64_t GetTotalReceivedBytes() const override;
  void ApplySocketTag(const net::SocketTag& tag) override;

 private:
  const net::IPEndPoint local_address_;
  const std::unique_ptr<SocketBuffer> buffer_;
  FakeStreamSocket* peer_;
  net::NetLogWithSource net_log_;

  DISALLOW_COPY_AND_ASSIGN(FakeStreamSocket);
};

}  // namespace chromecast

#endif  // CHROMECAST_NET_FAKE_STREAM_SOCKET_H_
