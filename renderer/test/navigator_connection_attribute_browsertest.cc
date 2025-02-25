/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/path_service.h"
#include "base/test/scoped_feature_list.h"
#include "brave/components/constants/brave_paths.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/test/base/ui_test_utils.h"
#include "content/public/browser/render_frame_host.h"
#include "content/public/browser/web_contents.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/browser_test_utils.h"
#include "net/dns/mock_host_resolver.h"
#include "net/test/embedded_test_server/embedded_test_server.h"
#include "third_party/blink/public/common/features.h"
#include "url/gurl.h"

class NavigatorConnectionAttributeBrowserTest
    : public InProcessBrowserTest,
      public ::testing::WithParamInterface<bool> {
 public:
  NavigatorConnectionAttributeBrowserTest()
      : https_server_(net::EmbeddedTestServer::TYPE_HTTPS) {
    brave::RegisterPathProvider();
    base::FilePath test_data_dir;
    base::PathService::Get(brave::DIR_TEST_DATA, &test_data_dir);
    https_server_.ServeFilesFromDirectory(test_data_dir);
  }

  ~NavigatorConnectionAttributeBrowserTest() override = default;

  bool IsNavigatorConnectionAttributeEnabled() { return GetParam(); }

  void SetUp() override {
    if (IsNavigatorConnectionAttributeEnabled()) {
      scoped_feature_list_.InitAndEnableFeature(
          blink::features::kNavigatorConnectionAttribute);
    }
    InProcessBrowserTest::SetUp();
  }

  void SetUpOnMainThread() override {
    InProcessBrowserTest::SetUpOnMainThread();

    EXPECT_TRUE(https_server_.Start());
    // Map all hosts to localhost.
    host_resolver()->AddRule("*", "127.0.0.1");
  }

  content::WebContents* web_contents() {
    return browser()->tab_strip_model()->GetActiveWebContents();
  }

  content::RenderFrameHost* main_frame() {
    return web_contents()->GetMainFrame();
  }

 protected:
  net::EmbeddedTestServer https_server_;
  base::test::ScopedFeatureList scoped_feature_list_;
};

IN_PROC_BROWSER_TEST_P(NavigatorConnectionAttributeBrowserTest,
                       IsAttributeAvailable) {
  EXPECT_EQ(IsNavigatorConnectionAttributeEnabled(),
            base::FeatureList::IsEnabled(
                blink::features::kNavigatorConnectionAttribute));

  const GURL url = https_server_.GetURL("/simple.html");
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));

  auto result = content::EvalJs(main_frame(), "'connection' in navigator");
  EXPECT_EQ(IsNavigatorConnectionAttributeEnabled(), result);
}

INSTANTIATE_TEST_SUITE_P(NavigatorConnectionAttributeBrowserTest,
                         NavigatorConnectionAttributeBrowserTest,
                         ::testing::Bool());
