/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_rewards/common/rewards_args.h"

#include <string>
#include <vector>

#include "base/command_line.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/string_split.h"
#include "base/strings/string_util.h"

namespace brave_rewards {

namespace {

static const char kSwitchName[] = "rewards";

bool ReadBoolFlag(const std::string& value) {
  std::string lower = base::ToLowerASCII(value);
  return lower == "true" || lower == "1";
}

template <typename F>
absl::optional<int> ReadInt(const std::string& value, F&& fn) {
  int int_value;
  if (base::StringToInt(value, &int_value) && fn(int_value)) {
    return int_value;
  }
  return {};
}

absl::optional<int> ReadInt(const std::string& value) {
  return ReadInt(value, [](int v) { return true; });
}

absl::optional<int> ReadPositiveInt(const std::string& value) {
  return ReadInt(value, [](int v) { return v > 0; });
}

}  // namespace

RewardsArgs RewardsArgs::Parse(const std::string& input) {
  RewardsArgs args;

  std::vector<std::string> flags = base::SplitString(
      input, ",", base::KEEP_WHITESPACE, base::SPLIT_WANT_NONEMPTY);

  for (auto& flag : flags) {
    std::vector<std::string> values = base::SplitString(
        flag, "=", base::KEEP_WHITESPACE, base::SPLIT_WANT_NONEMPTY);

    std::string name = values.empty() ? "" : base::ToLowerASCII(values[0]);
    std::string value = values.size() > 1 ? values[1] : "";

    if (name == "staging") {
      args.environment = ReadBoolFlag(value) ? Environment::kStaging
                                             : Environment::kProduction;
    } else if (name == "development") {
      if (ReadBoolFlag(value) && !args.environment) {
        args.environment = Environment::kDevelopment;
      }
    } else if (name == "debug") {
      args.debug = ReadBoolFlag(value);
    } else if (name == "reconcile-interval") {
      args.reconcile_interval = ReadPositiveInt(value);
    } else if (name == "retry-interval") {
      args.retry_interval = ReadPositiveInt(value);
    } else if (name == "gemini-retries") {
      args.gemini_retries = ReadInt(value, [](int v) { return v >= 0; });
    } else if (name == "persist-logs") {
      args.persist_logs = ReadBoolFlag(value);
    } else if (name == "countryid") {
      args.country_id = ReadInt(value);
    }
  }

  return args;
}

const RewardsArgs& RewardsArgs::ForCurrentProcess() {
  static absl::optional<RewardsArgs> parsed_args;
  if (!parsed_args) {
    std::string input;
    const auto* command_line = base::CommandLine::ForCurrentProcess();
    if (command_line->HasSwitch(kSwitchName)) {
      input = command_line->GetSwitchValueASCII(kSwitchName);
    }
    parsed_args = Parse(input);
  }
  return *parsed_args;
}

}  // namespace brave_rewards
