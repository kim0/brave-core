#ifndef BRAVE_COMPONENTS_OMNIBOX_BROWSER_BRAVE_SHORTCUTS_PROVIDER_H_
#define BRAVE_COMPONENTS_OMNIBOX_BROWSER_BRAVE_SHORTCUTS_PROVIDER_H_

#include "components/omnibox/browser/shortcuts_provider.h"

class BraveShortcutsProvider : public ShortcutsProvider {
 public:
  explicit BraveShortcutsProvider(AutocompleteProviderClient* client);

  BraveShortcutsProvider(const BraveShortcutsProvider&) = delete;
  BraveShortcutsProvider& operator=(const BraveShortcutsProvider&) = delete;

  void Start(const AutocompleteInput& input, bool minimal_changes) override;

 private:
  ~BraveShortcutsProvider() override;
};

#endif  // BRAVE_COMPONENTS_OMNIBOX_BROWSER_BRAVE_SHORTCUTS_PROVIDER_H_
