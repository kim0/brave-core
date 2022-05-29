#include "brave/components/omnibox/browser/brave_bookmark_provider.h"
#include "brave/common/pref_names.h"
#include "brave/components/omnibox/browser/brave_history_quick_provider.h"
#include "components/omnibox/browser/bookmark_provider.h"
#include "components/prefs/pref_service.h"

BraveBookmarkProvider::BraveBookmarkProvider(AutocompleteProviderClient* client)
    : BookmarkProvider(client) {}

BraveBookmarkProvider::~BraveBookmarkProvider() = default;

void BraveBookmarkProvider::Start(const AutocompleteInput& input,
                                  bool minimal_changes) {
  if (!client_->GetPrefs()->GetBoolean(kBookmarkSuggestionsEnabled))
    return;
  BookmarkProvider::Start(input, minimal_changes);
}
