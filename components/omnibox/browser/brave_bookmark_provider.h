#ifndef BRAVE_COMPONENTS_OMNIBOX_BROWSER_BRAVE_BOOKMARK_PROVIDER_H_
#define BRAVE_COMPONENTS_OMNIBOX_BROWSER_BRAVE_BOOKMARK_PROVIDER_H_

#include "components/omnibox/browser/bookmark_provider.h"

class BraveBookmarkProvider : public BookmarkProvider {
 public:
  explicit BraveBookmarkProvider(AutocompleteProviderClient* client);
  BraveBookmarkProvider(const BraveBookmarkProvider&) = delete;
  BraveBookmarkProvider& operator=(const BraveBookmarkProvider&) = delete;

  // AutocompleteProvider. |minimal_changes| is ignored since there is no asynch
  // completion performed.
  void Start(const AutocompleteInput& input, bool minimal_changes) override;

 private:
  ~BraveBookmarkProvider() override;
};

#endif  // BRAVE_COMPONENTS_OMNIBOX_BROWSER_BRAVE_BOOKMARK_PROVIDER_H_
