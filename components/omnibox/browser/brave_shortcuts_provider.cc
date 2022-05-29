/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/omnibox/browser/brave_shortcuts_provider.h"
#include "brave/common/pref_names.h"
#include "components/omnibox/browser/autocomplete_provider_client.h"
#include "components/omnibox/browser/shortcuts_provider.h"
#include "components/prefs/pref_service.h"

BraveShortcutsProvider::BraveShortcutsProvider(
    AutocompleteProviderClient* client)
    : ShortcutsProvider(client) {}

BraveShortcutsProvider::~BraveShortcutsProvider() = default;

void BraveShortcutsProvider::Start(const AutocompleteInput& input,
                                   bool minimal_changes) {
  if (!client_->GetPrefs()->GetBoolean(kHistorySuggestionsEnabled))
    return;
  ShortcutsProvider::Start(input, minimal_changes);
}
