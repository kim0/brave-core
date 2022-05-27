#ifndef BRAVE_COMPONENTS_OMNIBOX_BROWSER_BRAVE_SEARCH_PROVIDER_H_
#define BRAVE_COMPONENTS_OMNIBOX_BROWSER_BRAVE_SEARCH_PROVIDER_H_

#include "components/omnibox/browser/search_provider.h"

class BraveSearchProvider : public SearchProvider {
 public:
  void DoHistoryQuery(bool minimal_changes) override;
};

#endif  // BRAVE_COMPONENTS_OMNIBOX_BROWSER_BRAVE_SEARCH_PROVIDER_H_
