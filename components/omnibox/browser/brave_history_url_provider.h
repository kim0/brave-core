#ifndef BRAVE_COMPONENTS_OMNIBOX_BROWSER_BRAVE_HISTORY_URL_PROVIDER_H_
#define BRAVE_COMPONENTS_OMNIBOX_BROWSER_BRAVE_HISTORY_URL_PROVIDER_H_

#include "components/omnibox/browser/history_url_provider.h"

class BraveHistoryURLProvider : public HistoryURLProvider {
 public:
  BraveHistoryURLProvider(AutocompleteProviderClient* client,
                          AutocompleteProviderListener* listener);
  BraveHistoryURLProvider(const BraveHistoryURLProvider&) = delete;
  BraveHistoryURLProvider& operator=(const BraveHistoryURLProvider&) = delete;

  void Start(const AutocompleteInput& input, bool minimal_changes) override;

 private:
  ~BraveHistoryURLProvider() override;
};

#endif  // BRAVE_COMPONENTS_OMNIBOX_BROWSER_BRAVE_HISTORY_URL_PROVIDER_H_
