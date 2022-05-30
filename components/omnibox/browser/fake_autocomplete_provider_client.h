/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_OMNIBOX_BROWSER_FAKE_AUTOCOMPLETE_PROVIDER_CLIENT_H_
#define BRAVE_COMPONENTS_OMNIBOX_BROWSER_FAKE_AUTOCOMPLETE_PROVIDER_CLIENT_H_

#include <memory>

#include "base/memory/scoped_refptr.h"
#include "components/omnibox/browser/mock_autocomplete_provider_client.h"
#include "components/omnibox/browser/shortcuts_backend.h"
#include "components/prefs/testing_pref_service.h"

class FakeAutocompleteProviderClient : public MockAutocompleteProviderClient {
 public:
  FakeAutocompleteProviderClient();
  FakeAutocompleteProviderClient(const FakeAutocompleteProviderClient&) =
      delete;
  FakeAutocompleteProviderClient& operator=(
      const FakeAutocompleteProviderClient&) = delete;
  ~FakeAutocompleteProviderClient() override;
  PrefService* GetPrefs() const override;

  void set_shortcuts_backend(scoped_refptr<ShortcutsBackend> shortcuts_backend) {
      shortcuts_backend_ = shortcuts_backend;
  }

  scoped_refptr<ShortcutsBackend> GetShortcutsBackend() override;

 private:
  std::unique_ptr<TestingPrefServiceSimple> pref_service_;
  scoped_refptr<ShortcutsBackend> shortcuts_backend_;
};

#endif  // BRAVE_COMPONENTS_OMNIBOX_BROWSER_FAKE_AUTOCOMPLETE_PROVIDER_CLIENT_H_
