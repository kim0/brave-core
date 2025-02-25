/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ledger/internal/endpoint/uphold/post_cards/post_cards.h"

#include <utility>

#include "base/json/json_reader.h"
#include "base/json/json_writer.h"
#include "bat/ledger/internal/endpoint/uphold/uphold_utils.h"
#include "bat/ledger/internal/ledger_impl.h"
#include "bat/ledger/internal/uphold/uphold_card.h"
#include "net/http/http_status_code.h"

namespace ledger {
namespace endpoint {
namespace uphold {

PostCards::PostCards(LedgerImpl* ledger):
    ledger_(ledger) {
  DCHECK(ledger_);
}

PostCards::~PostCards() = default;

std::string PostCards::GetUrl() {
  return GetServerUrl("/v0/me/cards");
}

std::string PostCards::GeneratePayload() {
  base::Value::Dict payload;
  payload.Set("label", ::ledger::uphold::kCardName);
  payload.Set("currency", "BAT");

  std::string json;
  base::JSONWriter::Write(payload, &json);
  return json;
}

type::Result PostCards::CheckStatusCode(const int status_code) {
  if (status_code == net::HTTP_UNAUTHORIZED) {
    BLOG(0, "Unauthorized access");
    return type::Result::EXPIRED_TOKEN;
  }

  if (status_code != net::HTTP_OK) {
    BLOG(0, "Unexpected HTTP status: " << status_code);
    return type::Result::LEDGER_ERROR;
  }

  return type::Result::LEDGER_OK;
}

type::Result PostCards::ParseBody(
    const std::string& body,
    std::string* id) {
  DCHECK(id);

  absl::optional<base::Value> value = base::JSONReader::Read(body);
  if (!value || !value->is_dict()) {
    BLOG(0, "Invalid JSON");
    return type::Result::LEDGER_ERROR;
  }

  const base::Value::Dict& dict = value->GetDict();
  const auto* id_str = dict.FindString("id");
  if (!id_str) {
    BLOG(0, "Missing id");
    return type::Result::LEDGER_ERROR;
  }

  *id = *id_str;

  return type::Result::LEDGER_OK;
}

void PostCards::Request(
    const std::string& token,
    PostCardsCallback callback) {
  auto url_callback = base::BindOnce(
      &PostCards::OnRequest, base::Unretained(this), std::move(callback));

  auto request = type::UrlRequest::New();
  request->url = GetUrl();
  request->content = GeneratePayload();
  request->headers = RequestAuthorization(token);
  request->content_type = "application/json; charset=utf-8";
  request->method = type::UrlMethod::POST;
  ledger_->LoadURL(std::move(request), std::move(url_callback));
}

void PostCards::OnRequest(PostCardsCallback callback,
                          const type::UrlResponse& response) {
  ledger::LogUrlResponse(__func__, response, true);

  type::Result result = CheckStatusCode(response.status_code);

  if (result != type::Result::LEDGER_OK) {
    std::move(callback).Run(result, "");
    return;
  }

  std::string id;
  result = ParseBody(response.body, &id);
  std::move(callback).Run(result, id);
}

}  // namespace uphold
}  // namespace endpoint
}  // namespace ledger
