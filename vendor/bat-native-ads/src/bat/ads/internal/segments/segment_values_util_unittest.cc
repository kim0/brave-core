/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/segments/segment_values_util.h"

#include "base/test/values_test_util.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- brave_unit_tests --filter=BatAds*

namespace ads {

namespace {

constexpr char kSegmentsAsJson[] =
    R"(["technology & computing","personal finance-banking","food & drink-restaurants"])";
constexpr char kNoSegmentsAsJson[] = "[]";

}  // namespace

TEST(BatAdsSegmentValuesUtilTest, SegmentsToValue) {
  // Arrange

  // Act
  const base::Value::List list =
      SegmentsToValue({"technology & computing", "personal finance-banking",
                       "food & drink-restaurants"});

  // Assert
  const base::Value value = base::test::ParseJson(kSegmentsAsJson);
  const base::Value::List* expected_list = value.GetIfList();
  ASSERT_TRUE(expected_list);

  EXPECT_EQ(*expected_list, list);
}

TEST(BatAdsSegmentValuesUtilTest, NoSegmentsToValue) {
  // Arrange

  // Act
  const base::Value::List list = SegmentsToValue({});

  // Assert
  const base::Value value = base::test::ParseJson(kNoSegmentsAsJson);
  const base::Value::List* expected_list = value.GetIfList();
  ASSERT_TRUE(expected_list);

  EXPECT_EQ(*expected_list, list);
}

TEST(BatAdsSegmentValuesUtilTest, SegmentsFromValue) {
  // Arrange
  const base::Value value = base::test::ParseJson(kSegmentsAsJson);
  const base::Value::List* list = value.GetIfList();
  ASSERT_TRUE(list);

  // Act
  const SegmentList segments = SegmentsFromValue(*list);

  // Assert
  const SegmentList expected_segments = {"technology & computing",
                                         "personal finance-banking",
                                         "food & drink-restaurants"};
  EXPECT_EQ(expected_segments, segments);
}

TEST(BatAdsSegmentValuesUtilTest, NoSegmentsFromValue) {
  // Arrange
  const base::Value value = base::test::ParseJson(kNoSegmentsAsJson);
  const base::Value::List* list = value.GetIfList();
  ASSERT_TRUE(list);

  // Act
  const SegmentList segments = SegmentsFromValue(*list);

  // Assert
  EXPECT_TRUE(segments.empty());
}

}  // namespace ads
