/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "src/content/public/common/content_features.cc"

#include "base/feature_override.h"
#include "build/build_config.h"

namespace features {

OVERRIDE_FEATURE_DEFAULT_STATES({{
    {kDigitalGoodsApi, base::FEATURE_DISABLED_BY_DEFAULT},
    {kEarlyHintsPreloadForNavigation, base::FEATURE_DISABLED_BY_DEFAULT},
    {kFedCm, base::FEATURE_DISABLED_BY_DEFAULT},
    {kFirstPartySets, base::FEATURE_DISABLED_BY_DEFAULT},
    {kIdleDetection, base::FEATURE_DISABLED_BY_DEFAULT},
    {kNavigationRequestPreconnect, base::FEATURE_DISABLED_BY_DEFAULT},
    {kNotificationTriggers, base::FEATURE_DISABLED_BY_DEFAULT},
    {kPrivacySandboxAdsAPIsOverride, base::FEATURE_DISABLED_BY_DEFAULT},
    {kSignedExchangeSubresourcePrefetch, base::FEATURE_DISABLED_BY_DEFAULT},
    {kSignedHTTPExchange, base::FEATURE_DISABLED_BY_DEFAULT},
    {kSubresourceWebBundles, base::FEATURE_DISABLED_BY_DEFAULT},
#if BUILDFLAG(IS_ANDROID)
    {kWebNfc, base::FEATURE_DISABLED_BY_DEFAULT},
#endif
    {kWebOTP, base::FEATURE_DISABLED_BY_DEFAULT},
}});

}  // namespace features
