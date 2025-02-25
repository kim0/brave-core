/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/binance/binance_protocol_handler.h"

#include <map>
#include <string>
#include <utility>

#include "base/strings/strcat.h"
#include "base/strings/string_util.h"
#include "brave/browser/binance/binance_service_factory.h"
#include "brave/components/binance/browser/binance_service.h"
#include "brave/components/constants/url_constants.h"
#include "chrome/browser/profiles/profile.h"
#include "components/user_prefs/user_prefs.h"
#include "content/public/browser/browser_task_traits.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/render_frame_host.h"
#include "net/base/url_util.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "url/origin.h"

namespace {

void LoadNewTabURL(const GURL& url,
                   content::WebContents::OnceGetter web_contents_getter,
                   ui::PageTransition page_transition,
                   bool has_user_gesture,
                   const absl::optional<url::Origin>& initiating_origin) {
  content::WebContents* web_contents = std::move(web_contents_getter).Run();
  if (!web_contents) {
    return;
  }

  const auto ref_url = web_contents->GetURL();
  if (!ref_url.is_valid()) {
    return;
  }

  // We should only allow binance scheme to be used from
  // https://accounts.binance.com
  url::Origin allowed_origin =
      url::Origin::Create(GURL("https://accounts.binance.com"));
  url::Origin last_committed_origin =
      url::Origin::Create(web_contents->GetLastCommittedURL());
  if (last_committed_origin != allowed_origin ||
      !initiating_origin.has_value() || initiating_origin != allowed_origin) {
    return;
  }

  std::map<std::string, std::string> parts;
  for (net::QueryIterator it(url); !it.IsAtEnd(); it.Advance()) {
    parts[std::string(it.GetKey())] = it.GetUnescapedValue();
  }
  if (parts.find("code") != parts.end()) {
    std::string auth_token = parts["code"];
    Profile* profile =
        Profile::FromBrowserContext(web_contents->GetBrowserContext());
    BinanceServiceFactory::GetInstance()->GetForProfile(profile)->SetAuthToken(
        auth_token);
  }

  web_contents->GetController().LoadURL(GURL("chrome://newtab?binanceAuth=1"),
                                        content::Referrer(), page_transition,
                                        std::string());
}

}  // namespace

namespace binance {

void HandleBinanceProtocol(const GURL& url,
                           content::WebContents::OnceGetter web_contents_getter,
                           ui::PageTransition page_transition,
                           bool has_user_gesture,
                           const absl::optional<url::Origin>& initiator) {
  DCHECK(IsBinanceProtocol(url));
  content::GetUIThreadTaskRunner({})->PostTask(
      FROM_HERE,
      base::BindOnce(&LoadNewTabURL, url, std::move(web_contents_getter),
                     page_transition, has_user_gesture, initiator));
}

bool IsBinanceProtocol(const GURL& url) {
  return url.SchemeIs(kBinanceScheme);
}

}  // namespace binance
