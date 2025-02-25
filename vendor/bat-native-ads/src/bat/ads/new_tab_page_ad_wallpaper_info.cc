/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/new_tab_page_ad_wallpaper_info.h"

#include <tuple>

namespace ads {

NewTabPageAdWallpaperInfo::NewTabPageAdWallpaperInfo() = default;

NewTabPageAdWallpaperInfo::NewTabPageAdWallpaperInfo(
    const NewTabPageAdWallpaperInfo& info) = default;

NewTabPageAdWallpaperInfo& NewTabPageAdWallpaperInfo::operator=(
    const NewTabPageAdWallpaperInfo& info) = default;

NewTabPageAdWallpaperInfo::~NewTabPageAdWallpaperInfo() = default;

bool NewTabPageAdWallpaperInfo::operator==(
    const NewTabPageAdWallpaperInfo& rhs) const {
  auto tie = [](const NewTabPageAdWallpaperInfo& info) {
    return std::tie(info.image_url, info.focal_point);
  };

  return tie(*this) == tie(rhs);
}

}  // namespace ads
