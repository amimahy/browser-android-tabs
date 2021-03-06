// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/fetch/FetchRequestData.h"

#include "core/dom/ExecutionContext.h"
#include "core/fetch/BlobBytesConsumer.h"
#include "core/fetch/BodyStreamBuffer.h"
#include "core/fetch/BytesConsumer.h"
#include "core/fetch/FetchHeaderList.h"
#include "core/fetch/FormDataBytesConsumer.h"
#include "core/loader/ThreadableLoader.h"
#include "platform/bindings/ScriptState.h"
#include "platform/loader/fetch/ResourceLoaderOptions.h"
#include "platform/loader/fetch/ResourceRequest.h"
#include "platform/network/http_names.h"
#include "public/platform/WebURLRequest.h"
#include "public/platform/modules/serviceworker/WebServiceWorkerRequest.h"

namespace blink {

FetchRequestData* FetchRequestData::Create() {
  return new FetchRequestData();
}

FetchRequestData* FetchRequestData::Create(
    ScriptState* script_state,
    const WebServiceWorkerRequest& web_request) {
  FetchRequestData* request = FetchRequestData::Create();
  request->url_ = web_request.Url();
  request->method_ = web_request.Method();
  for (HTTPHeaderMap::const_iterator it = web_request.Headers().begin();
       it != web_request.Headers().end(); ++it)
    request->header_list_->Append(it->key, it->value);
  if (scoped_refptr<EncodedFormData> body = web_request.Body()) {
    request->SetBuffer(new BodyStreamBuffer(
        script_state,
        new FormDataBytesConsumer(ExecutionContext::From(script_state),
                                  std::move(body))));
  } else if (web_request.GetBlobDataHandle()) {
    request->SetBuffer(new BodyStreamBuffer(
        script_state,
        new BlobBytesConsumer(ExecutionContext::From(script_state),
                              web_request.GetBlobDataHandle())));
  }
  request->SetContext(web_request.GetRequestContext());
  request->SetReferrer(
      Referrer(web_request.ReferrerUrl().GetString(),
               static_cast<ReferrerPolicy>(web_request.GetReferrerPolicy())));
  request->SetMode(web_request.Mode());
  request->SetCredentials(web_request.CredentialsMode());
  request->SetCacheMode(web_request.CacheMode());
  request->SetRedirect(web_request.RedirectMode());
  request->SetMIMEType(request->header_list_->ExtractMIMEType());
  request->SetIntegrity(web_request.Integrity());
  request->SetKeepalive(web_request.Keepalive());
  return request;
}

FetchRequestData* FetchRequestData::CloneExceptBody() {
  FetchRequestData* request = FetchRequestData::Create();
  request->url_ = url_;
  request->method_ = method_;
  request->header_list_ = header_list_->Clone();
  request->origin_ = origin_;
  request->same_origin_data_url_flag_ = same_origin_data_url_flag_;
  request->context_ = context_;
  request->referrer_ = referrer_;
  request->mode_ = mode_;
  request->credentials_ = credentials_;
  request->cache_mode_ = cache_mode_;
  request->redirect_ = redirect_;
  request->response_tainting_ = response_tainting_;
  request->mime_type_ = mime_type_;
  request->integrity_ = integrity_;
  request->keepalive_ = keepalive_;
  return request;
}

FetchRequestData* FetchRequestData::Clone(ScriptState* script_state) {
  FetchRequestData* request = FetchRequestData::CloneExceptBody();
  if (buffer_) {
    BodyStreamBuffer* new1 = nullptr;
    BodyStreamBuffer* new2 = nullptr;
    buffer_->Tee(&new1, &new2);
    buffer_ = new1;
    request->buffer_ = new2;
  }
  return request;
}

FetchRequestData* FetchRequestData::Pass(ScriptState* script_state) {
  FetchRequestData* request = FetchRequestData::CloneExceptBody();
  if (buffer_) {
    request->buffer_ = buffer_;
    buffer_ = new BodyStreamBuffer(script_state, BytesConsumer::CreateClosed());
    buffer_->CloseAndLockAndDisturb();
  }
  return request;
}

FetchRequestData::~FetchRequestData() {}

FetchRequestData::FetchRequestData()
    : method_(HTTPNames::GET),
      header_list_(FetchHeaderList::Create()),
      context_(WebURLRequest::kRequestContextUnspecified),
      same_origin_data_url_flag_(false),
      referrer_(Referrer(ClientReferrerString(), kReferrerPolicyDefault)),
      mode_(network::mojom::FetchRequestMode::kNoCORS),
      credentials_(network::mojom::FetchCredentialsMode::kOmit),
      cache_mode_(mojom::FetchCacheMode::kDefault),
      redirect_(network::mojom::FetchRedirectMode::kFollow),
      response_tainting_(kBasicTainting),
      keepalive_(false) {}

void FetchRequestData::Trace(blink::Visitor* visitor) {
  visitor->Trace(buffer_);
  visitor->Trace(header_list_);
}

}  // namespace blink
