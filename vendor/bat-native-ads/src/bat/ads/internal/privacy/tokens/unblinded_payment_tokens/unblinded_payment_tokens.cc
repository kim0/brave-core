/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed w
 * h this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/privacy/tokens/unblinded_payment_tokens/unblinded_payment_tokens.h"

#include <algorithm>
#include <string>
#include <utility>

#include "base/check_op.h"
#include "base/guid.h"
#include "base/notreached.h"
#include "bat/ads/internal/base/logging_util.h"
#include "bat/ads/internal/privacy/challenge_bypass_ristretto/unblinded_token.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace ads {
namespace privacy {

UnblindedPaymentTokens::UnblindedPaymentTokens() = default;

UnblindedPaymentTokens::~UnblindedPaymentTokens() = default;

const UnblindedPaymentTokenInfo& UnblindedPaymentTokens::GetToken() const {
  DCHECK_NE(Count(), 0);

  return unblinded_payment_tokens_.front();
}

const UnblindedPaymentTokenList& UnblindedPaymentTokens::GetAllTokens() const {
  return unblinded_payment_tokens_;
}

base::Value::List UnblindedPaymentTokens::GetTokensAsList() {
  base::Value::List list;

  for (const auto& unblinded_payment_token : unblinded_payment_tokens_) {
    const absl::optional<std::string> unblinded_token_base64 =
        unblinded_payment_token.value.EncodeBase64();
    if (!unblinded_token_base64) {
      NOTREACHED();
      continue;
    }

    const absl::optional<std::string> public_key_base64 =
        unblinded_payment_token.public_key.EncodeBase64();
    if (!public_key_base64) {
      NOTREACHED();
      continue;
    }

    base::Value::Dict dict;
    dict.Set("transaction_id", unblinded_payment_token.transaction_id);
    dict.Set("unblinded_token", *unblinded_token_base64);
    dict.Set("public_key", *public_key_base64);
    dict.Set("confirmation_type",
             unblinded_payment_token.confirmation_type.ToString());
    dict.Set("ad_type", unblinded_payment_token.ad_type.ToString());
    list.Append(std::move(dict));
  }

  return list;
}

void UnblindedPaymentTokens::SetTokens(
    const UnblindedPaymentTokenList& unblinded_payment_tokens) {
  unblinded_payment_tokens_ = unblinded_payment_tokens;
}

void UnblindedPaymentTokens::SetTokensFromList(const base::Value::List& list) {
  UnblindedPaymentTokenList unblinded_payment_tokens;

  for (const auto& item : list) {
    if (!item.is_dict()) {
      BLOG(0, "Unblinded payment token should be a dictionary");
      continue;
    }
    const base::Value::Dict& dict = item.GetDict();

    UnblindedPaymentTokenInfo unblinded_payment_token;

    // Transaction id
    const std::string* transaction_id_value = dict.FindString("transaction_id");
    if (!transaction_id_value) {
      // Migrate legacy confirmations
      unblinded_payment_token.transaction_id =
          base::GUID::GenerateRandomV4().AsLowercaseString();
    } else {
      unblinded_payment_token.transaction_id = *transaction_id_value;
    }

    // Unblinded payment token
    const std::string* unblinded_payment_token_value =
        dict.FindString("unblinded_token");
    if (!unblinded_payment_token_value) {
      BLOG(
          0,
          "Unblinded payment token dictionary missing unblinded payment token");
      continue;
    }
    unblinded_payment_token.value =
        cbr::UnblindedToken(*unblinded_payment_token_value);
    if (!unblinded_payment_token.value.has_value()) {
      BLOG(0, "Unblinded payment token is invalid");
      continue;
    }

    // Public key
    const std::string* public_key_value = dict.FindString("public_key");
    if (!public_key_value) {
      BLOG(0, "Unblinded payment token dictionary missing public_key");
      continue;
    }
    unblinded_payment_token.public_key = cbr::PublicKey(*public_key_value);
    if (!unblinded_payment_token.public_key.has_value()) {
      BLOG(0, "Unblinded payment token public key is invalid");
      continue;
    }

    // Confirmation type
    const std::string* confirmation_type_value =
        dict.FindString("confirmation_type");
    if (confirmation_type_value) {
      unblinded_payment_token.confirmation_type =
          ConfirmationType(*confirmation_type_value);
    }

    // Ad type
    const std::string* ad_type_value = dict.FindString("ad_type");
    if (ad_type_value) {
      unblinded_payment_token.ad_type = AdType(*ad_type_value);
    }

    unblinded_payment_tokens.push_back(unblinded_payment_token);
  }

  SetTokens(unblinded_payment_tokens);
}

void UnblindedPaymentTokens::AddTokens(
    const UnblindedPaymentTokenList& unblinded_payment_tokens) {
  for (const auto& unblinded_payment_token : unblinded_payment_tokens) {
    if (TokenExists(unblinded_payment_token)) {
      continue;
    }

    unblinded_payment_tokens_.push_back(unblinded_payment_token);
  }
}

bool UnblindedPaymentTokens::RemoveToken(
    const UnblindedPaymentTokenInfo& unblinded_payment_token) {
  auto iter = std::find_if(
      unblinded_payment_tokens_.cbegin(), unblinded_payment_tokens_.cend(),
      [&unblinded_payment_token](const UnblindedPaymentTokenInfo& value) {
        return unblinded_payment_token == value;
      });

  if (iter == unblinded_payment_tokens_.end()) {
    return false;
  }

  unblinded_payment_tokens_.erase(iter);

  return true;
}

void UnblindedPaymentTokens::RemoveTokens(
    const UnblindedPaymentTokenList& unblinded_payment_tokens) {
  const auto iter = std::remove_if(
      unblinded_payment_tokens_.begin(), unblinded_payment_tokens_.end(),
      [&unblinded_payment_tokens](
          const UnblindedPaymentTokenInfo& unblinded_payment_token) {
        return std::find(unblinded_payment_tokens.cbegin(),
                         unblinded_payment_tokens.cend(),
                         unblinded_payment_token) !=
               unblinded_payment_tokens.end();
      });

  unblinded_payment_tokens_.erase(iter, unblinded_payment_tokens_.end());
}

void UnblindedPaymentTokens::RemoveAllTokens() {
  unblinded_payment_tokens_.clear();
}

bool UnblindedPaymentTokens::TokenExists(
    const UnblindedPaymentTokenInfo& unblinded_payment_token) {
  auto iter = std::find_if(
      unblinded_payment_tokens_.cbegin(), unblinded_payment_tokens_.cend(),
      [&unblinded_payment_token](const UnblindedPaymentTokenInfo& value) {
        return unblinded_payment_token == value;
      });

  if (iter == unblinded_payment_tokens_.end()) {
    return false;
  }

  return true;
}

int UnblindedPaymentTokens::Count() const {
  return unblinded_payment_tokens_.size();
}

bool UnblindedPaymentTokens::IsEmpty() const {
  return unblinded_payment_tokens_.empty();
}

}  // namespace privacy
}  // namespace ads
