#include "brave/components/omnibox/browser/brave_history_quick_provider.h"

#include <stddef.h>

#include <algorithm>
#include <functional>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "base/format_macros.h"
#include "base/memory/raw_ptr.h"
#include "base/run_loop.h"
#include "base/strings/utf_string_conversions.h"
#include "base/test/task_environment.h"
#include "brave/common/pref_names.h"
#include "components/bookmarks/browser/bookmark_model.h"
#include "components/bookmarks/test/bookmark_test_helpers.h"
#include "components/bookmarks/test/test_bookmark_client.h"
#include "components/history/core/browser/history_backend.h"
#include "components/history/core/browser/history_database.h"
#include "components/history/core/browser/history_service.h"
#include "components/history/core/browser/history_service_observer.h"
#include "components/history/core/browser/url_database.h"
#include "components/history/core/test/history_service_test_util.h"
#include "components/omnibox/browser/autocomplete_match.h"
#include "components/omnibox/browser/autocomplete_result.h"
#include "components/omnibox/browser/fake_autocomplete_provider_client.h"
#include "components/omnibox/browser/history_test_util.h"
#include "components/omnibox/browser/history_url_provider.h"
#include "components/omnibox/browser/in_memory_url_index_test_util.h"
#include "components/prefs/pref_service.h"
#include "components/search_engines/omnibox_focus_type.h"
#include "components/search_engines/search_terms_data.h"
#include "testing/gtest/include/gtest/gtest.h"

class BraveHistoryQuickProviderTest : public testing::Test {
 public:
  BraveHistoryQuickProviderTest() {}

  AutocompleteInput CreateAutocompleteInput(base::StringPiece text) {
    AutocompleteInput input(base::UTF8ToUTF16(text),
                            metrics::OmniboxEventProto::OTHER, classifier_);
    return input;
  }

  void FillData() {
    history::URLRow row{GURL("https://example.com")};
    row.set_title(u"Hello");
    row.set_visit_count(5);
    row.set_typed_count(2);
    row.set_last_visit(base::Time::Now());
    history::AddFakeURLToHistoryDB(history_service_->history_backend_->db(),
                                   row);
  }

  void SetUp() override {
    CHECK(history_dir_.CreateUniqueTempDir());
    EXPECT_CALL(client_, GetSchemeClassifier())
        .WillRepeatedly(testing::ReturnRef(classifier_));

    bookmark_model_ = bookmarks::TestBookmarkClient::CreateModel();
    EXPECT_CALL(client_, GetBookmarkModel())
        .WillRepeatedly(testing::Return(bookmark_model_.get()));

    history_service_ =
        history::CreateHistoryService(history_dir_.GetPath(), true);
    EXPECT_CALL(client_, GetHistoryService())
        .WillRepeatedly(testing::Return(history_service_.get()));

    in_memory_url_index_ = std::make_unique<InMemoryURLIndex>(
        bookmark_model_.get(), history_service_.get(), nullptr,
        history_dir_.GetPath(), SchemeSet());
    in_memory_url_index_->Init();
    EXPECT_CALL(client_, GetInMemoryURLIndex())
        .WillRepeatedly(testing::Return(in_memory_url_index_.get()));
    BlockUntilInMemoryURLIndexIsRefreshed(in_memory_url_index_.get());

    ASSERT_NO_FATAL_FAILURE(FillData());
    in_memory_url_index_->RebuildFromHistory(
        history_service_->history_backend_->db());
    // history::BlockUntilHistoryProcessesPendingRequests(history_service_.get());

    // provider_ = new BraveHistoryQuickProvider(client_.get());
  }

  PrefService* prefs() { return client_.GetPrefs(); }

 protected:
  base::test::TaskEnvironment task_environment_;
  std::unique_ptr<history::HistoryService> history_service_;
  std::unique_ptr<InMemoryURLIndex> in_memory_url_index_;
  TestSchemeClassifier classifier_;
  FakeAutocompleteProviderClient client_;
  std::unique_ptr<bookmarks::BookmarkModel> bookmark_model_;
  scoped_refptr<BraveHistoryQuickProvider> provider_;

 private:
  base::ScopedTempDir history_dir_;
};

TEST_F(BraveHistoryQuickProviderTest, SuggestionsDisabledNoResults) {
  prefs()->SetBoolean(kHistorySuggestionsEnabled, false);
  provider_->Start(CreateAutocompleteInput("Hello"), true);
  EXPECT_TRUE(provider_->matches().empty());
}

TEST_F(BraveHistoryQuickProviderTest, SuggestionsEnabledHasResults) {
  prefs()->SetBoolean(kHistorySuggestionsEnabled, true);
  provider_->Start(CreateAutocompleteInput("Hello"), true);
  EXPECT_FALSE(provider_->matches().empty());
}
