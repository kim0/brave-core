#include "brave/components/omnibox/browser/brave_bookmark_provider.h"
#include <memory>
#include "base/strings/string_piece_forward.h"
#include "base/strings/utf_string_conversions.h"
#include "brave/common/pref_names.h"
#include "brave/components/omnibox/browser/fake_autocomplete_provider_client.h"
#include "components/bookmarks/browser/bookmark_model.h"
#include "components/bookmarks/test/test_bookmark_client.h"
#include "components/omnibox/browser/autocomplete_input.h"
#include "components/omnibox/browser/test_scheme_classifier.h"
#include "components/prefs/pref_service.h"
#include "gmock/gmock.h"
#include "third_party/googletest/src/googletest/include/gtest/gtest.h"

class BraveBookmarkProviderTest : public testing::Test {
 public:
  BraveBookmarkProviderTest()
      : model_(bookmarks::TestBookmarkClient::CreateModel()),
        provider_(new BraveBookmarkProvider(&client_)) {
    provider_->set_bookmark_model_for_testing(model_.get());
  }

  AutocompleteInput CreateAutocompleteInput(base::StringPiece text) {
    AutocompleteInput input(base::UTF8ToUTF16(text),
                            metrics::OmniboxEventProto::OTHER, classifier_);
    return input;
  }

  void SetUp() override {
    EXPECT_CALL(client_, GetBookmarkModel())
        .WillRepeatedly(testing::Return(model_.get()));
    EXPECT_CALL(client_, GetSchemeClassifier())
        .WillRepeatedly(testing::ReturnRef(classifier_));
    auto* node = client_.GetBookmarkModel()->other_node();
    client_.GetBookmarkModel()->AddURL(node, 0, u"Hello",
                                       GURL("https://example.com"));
  }

  PrefService* prefs() { return client_.GetPrefs(); }

 protected:
  TestSchemeClassifier classifier_;
  FakeAutocompleteProviderClient client_;
  std::unique_ptr<bookmarks::BookmarkModel> model_;
  scoped_refptr<BraveBookmarkProvider> provider_;
};

TEST_F(BraveBookmarkProviderTest, SuggestionsDisabledNoResults) {
  prefs()->SetBoolean(kBookmarkSuggestionsEnabled, false);
  provider_->Start(CreateAutocompleteInput("Hello"), true);
  EXPECT_TRUE(provider_->matches().empty());
}

TEST_F(BraveBookmarkProviderTest, SuggestionsEnabledHasResults) {
  prefs()->SetBoolean(kBookmarkSuggestionsEnabled, true);
  provider_->Start(CreateAutocompleteInput("Hello"), true);
  EXPECT_FALSE(provider_->matches().empty());
}
