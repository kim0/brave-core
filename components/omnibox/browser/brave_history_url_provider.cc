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
  if (!client()->GetPrefs()->GetBoolean(kHistorySuggestionsEnabled)) {
    // TODO: For some reason if we return here then we get a crash on entering a
    // URL.
    // return;
  }

  HistoryURLProvider::Start(input, minimal_changes);
}
