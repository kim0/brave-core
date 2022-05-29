#include "brave/components/omnibox/browser/brave_history_url_provider.h"
#include "brave/common/pref_names.h"
#include "components/history/core/browser/history_service.h"
#include "components/prefs/pref_service.h"

BraveHistoryURLProvider::BraveHistoryURLProvider(
    AutocompleteProviderClient* client,
    AutocompleteProviderListener* listener)
    : HistoryURLProvider(client, listener) {}

BraveHistoryURLProvider::~BraveHistoryURLProvider() = default;

void BraveHistoryURLProvider::Start(const AutocompleteInput& input,
                                    bool minimal_changes) {
  // Unlike other providers, we can't simply stop the search here. The
  // HistoryURLProvider doesn't only search history, it is also responsible for
  // navigating to exact urls (i.e. https://example.com/), so we need to disable
  // **ONLY** history searches. Fortunately, Chromium has a flag for this.
  search_url_database_ =
      client()->GetPrefs()->GetBoolean(kHistorySuggestionsEnabled);

  HistoryURLProvider::Start(input, minimal_changes);
}
