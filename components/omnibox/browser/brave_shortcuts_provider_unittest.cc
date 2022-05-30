#include "brave/components/omnibox/browser/brave_shortcuts_provider.h"
#include <memory>
#include <vector>
#include "base/files/file_path.h"
#include "base/memory/scoped_refptr.h"
#include "base/run_loop.h"
#include "base/strings/string_piece_forward.h"
#include "base/strings/utf_string_conversions.h"
#include "base/test/task_environment.h"
#include "base/time/time.h"
#include "brave/common/pref_names.h"
#include "brave/components/omnibox/browser/fake_autocomplete_provider_client.h"
#include "components/bookmarks/browser/bookmark_model.h"
#include "components/bookmarks/test/test_bookmark_client.h"
#include "components/history/core/browser/history_service.h"
#include "components/omnibox/browser/autocomplete_input.h"
#include "components/omnibox/browser/autocomplete_match.h"
#include "components/omnibox/browser/autocomplete_match_type.h"
#include "components/omnibox/browser/shortcuts_backend.h"
#include "components/omnibox/browser/shortcuts_database.h"
#include "components/omnibox/browser/shortcuts_provider.h"
#include "components/omnibox/browser/shortcuts_provider_test_util.h"
#include "components/omnibox/browser/test_scheme_classifier.h"
#include "components/prefs/pref_service.h"
#include "components/search_engines/omnibox_focus_type.h"
#include "components/search_engines/search_terms_data.h"
#include "gmock/gmock.h"
#include "omnibox_input_type.pb.h"
#include "third_party/googletest/src/googletest/include/gtest/gtest.h"
#include "ui/base/page_transition_types.h"

class MockHistoryService : public history::HistoryService {
 public:
  MockHistoryService() = default;
  MOCK_METHOD1(DeleteURLs, void(const std::vector<GURL>&));
};

class BraveShortcutsProviderTest : public testing::Test {
 public:
  BraveShortcutsProviderTest()
       {}

  AutocompleteInput CreateAutocompleteInput(base::StringPiece text) {
    AutocompleteInput input(base::UTF8ToUTF16(text),
                            metrics::OmniboxEventProto::OTHER,
                            classifier_);
    input.set_focus_type(OmniboxFocusType::DEFAULT);
    return input;
  }

  void SetUp() override {
    EXPECT_CALL(client_, GetSchemeClassifier())
        .WillRepeatedly(testing::ReturnRef(classifier_));
    EXPECT_CALL(client_, GetHistoryService())
        .WillRepeatedly(testing::Return(&history_service_));

    backend_ = base::MakeRefCounted<ShortcutsBackend>(
        client_.GetTemplateURLService(), std::make_unique<SearchTermsData>(),
        client_.GetHistoryService(), base::FilePath(), true);
    client_.set_shortcuts_backend(backend_);

    backend_->Init();

    ShortcutsDatabase::Shortcut shortcut(
        "BD85DBA2-8C29-49F9-84AE-48E1E90880E9", u"hello",
        ShortcutsDatabase::Shortcut::MatchCore(
            u"hello", GURL("https://example.com"),
            AutocompleteMatch::DocumentType::NONE, u"hello World", "0,0",
            u"hello world", "0,4", ui::PageTransition::PAGE_TRANSITION_TYPED,
            AutocompleteMatch::Type::HISTORY_TITLE, u""),
        base::Time::Now(), 5);
    backend_->AddShortcut(shortcut);

    EXPECT_EQ(1ul, backend_->shortcuts_map().size());

    provider_ = base::MakeRefCounted<BraveShortcutsProvider>(&client_);
  }

  PrefService* prefs() { return client_.GetPrefs(); }

 protected:
  base::test::TaskEnvironment task_environment_;

  MockHistoryService history_service_;
  TestSchemeClassifier classifier_;
  FakeAutocompleteProviderClient client_;
  scoped_refptr<BraveShortcutsProvider> provider_;
  scoped_refptr<ShortcutsBackend> backend_;
};

TEST_F(BraveShortcutsProviderTest, SuggestionsDisabledNoResults) {
  prefs()->SetBoolean(kHistorySuggestionsEnabled, false);
  provider_->Start(CreateAutocompleteInput("hel"), true);
  EXPECT_TRUE(provider_->matches().empty());
}

TEST_F(BraveShortcutsProviderTest, SuggestionsEnabledHasResults) {
  prefs()->SetBoolean(kHistorySuggestionsEnabled, true);
  provider_->Start(CreateAutocompleteInput("hello"), true);
  EXPECT_FALSE(provider_->matches().empty());
}
