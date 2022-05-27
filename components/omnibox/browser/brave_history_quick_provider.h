#ifndef BRAVE_COMPONENTS_OMNIBOX_BROWSER_BRAVE_HISTORY_QUICK_PROVIDER_H_
#define BRAVE_COMPONENTS_OMNIBOX_BROWSER_BRAVE_HISTORY_QUICK_PROVIDER_H_

#include "components/omnibox/browser/history_quick_provider.h"

class BraveHistoryQuickProvider : public HistoryQuickProvider {
 public:
  explicit BraveHistoryQuickProvider(AutocompleteProviderClient* client);
  BraveHistoryQuickProvider(const BraveHistoryQuickProvider&) = delete;
  BraveHistoryQuickProvider& operator=(const BraveHistoryQuickProvider&) =
      delete;

  // AutocompleteProvider. |minimal_changes| is ignored since there is no asynch
  // completion performed.
  void Start(const AutocompleteInput& input, bool minimal_changes) override;

 private:
  ~BraveHistoryQuickProvider() override;
};

#endif  // BRAVE_COMPONENTS_OMNIBOX_BROWSER_BRAVE_HISTORY_QUICK_PROVIDER_H_
