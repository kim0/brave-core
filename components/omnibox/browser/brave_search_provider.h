#ifndef BRAVE_COMPONENTS_OMNIBOX_BROWSER_BRAVE_SEARCH_PROVIDER_H_
#define BRAVE_COMPONENTS_OMNIBOX_BROWSER_BRAVE_SEARCH_PROVIDER_H_

#include "components/omnibox/browser/search_provider.h"
#include "components/prefs/pref_member.h"

class BraveSearchProvider : public SearchProvider {
 public:
  BraveSearchProvider(AutocompleteProviderClient* client,
                      AutocompleteProviderListener* listener);
  BraveSearchProvider(const BraveSearchProvider&) = delete;
  BraveSearchProvider& operator=(const BraveSearchProvider&) = delete;

  void DoHistoryQuery(bool minimal_changes) override;

 protected:
  ~BraveSearchProvider() override;
};

#endif  // BRAVE_COMPONENTS_OMNIBOX_BROWSER_BRAVE_SEARCH_PROVIDER_H_
