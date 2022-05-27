#include "components/omnibox/browser/search_provider.h"
#include "brave/components/omnibox/browser/brave_search_provider.h"

void BraveSearchProvider::DoHistoryQuery(bool minimal_changes) {
    SearchProvider::DoHistoryQuery(minimal_changes);
}
