/*
 * Copyright 2016 Facebook, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <thrift/test/gen-cpp2/reflection_fatal_enum.h>

#include <thrift/lib/cpp2/fatal/reflection.h>
#include <thrift/lib/cpp2/fatal/internal/test_helpers.h>

#include <gtest/gtest.h>

namespace test_cpp2 {
namespace cpp_reflection {

FATAL_STR(enum1s, "enum1");
FATAL_STR(field0s, "field0");
FATAL_STR(field1s, "field1");
FATAL_STR(field2s, "field2");

using field0v = std::integral_constant<enum1, enum1::field0>;
using field1v = std::integral_constant<enum1, enum1::field1>;
using field2v = std::integral_constant<enum1, enum1::field2>;

TEST(fatal_enum, sanity_check) {
  using traits = fatal::enum_traits<enum1>;

  EXPECT_SAME<enum1, traits::type>();
  EXPECT_SAME<enum1s, traits::name>();
  EXPECT_SAME<std::underlying_type<enum1>::type, traits::int_type>();

  EXPECT_SAME<field0s, traits::str::field0>();
  EXPECT_SAME<field1s, traits::str::field1>();
  EXPECT_SAME<field2s, traits::str::field2>();

  EXPECT_SAME<
    fatal::build_type_map<
      field0s, field0v,
      field1s, field1v,
      field2s, field2v
    >,
    traits::name_to_value
  >();

  EXPECT_SAME<
    fatal::build_type_map<
      field0v, field0s,
      field1v, field1s,
      field2v, field2s
    >,
    traits::value_to_name
  >();

  using names = fatal::type_list<field0s, field1s, field2s>;
  EXPECT_SAME<names, traits::names>();
  EXPECT_SAME<fatal::type_list<field0v, field1v, field2v>, traits::values>();

  EXPECT_EQ("field0", fatal::enum_to_string(enum1::field0));
  EXPECT_EQ("field1", fatal::enum_to_string(enum1::field1));
  EXPECT_EQ("field2", fatal::enum_to_string(enum1::field2));
  EXPECT_EQ(nullptr, fatal::enum_to_string(static_cast<enum1>(99999)));

  EXPECT_EQ("field0", traits::to_string(enum1::field0));
  EXPECT_EQ("field1", traits::to_string(enum1::field1));
  EXPECT_EQ("field2", traits::to_string(enum1::field2));
  EXPECT_EQ(nullptr, traits::to_string(static_cast<enum1>(99999)));

  EXPECT_EQ(enum1::field0, traits::parse(std::string("field0")));
  EXPECT_EQ(enum1::field1, traits::parse(std::string("field1")));
  EXPECT_EQ(enum1::field2, traits::parse(std::string("field2")));
  EXPECT_THROW(traits::parse("not a field"), std::invalid_argument);

  enum1 e;
  EXPECT_TRUE(traits::try_parse(e, std::string("field0")));
  EXPECT_EQ(enum1::field0, e);
  EXPECT_TRUE(traits::try_parse(e, std::string("field1")));
  EXPECT_EQ(enum1::field1, e);
  EXPECT_TRUE(traits::try_parse(e, std::string("field2")));
  EXPECT_EQ(enum1::field2, e);
  EXPECT_FALSE(traits::try_parse(e, std::string("not a field")));
  EXPECT_EQ(enum1::field2, e);
}

FATAL_STR(enum3_annotation1k, "_now.with.an.underscore");
FATAL_STR(enum3_annotation1v, "_now.with.an.underscore");
FATAL_STR(enum3_annotation2k, "another.there");
FATAL_STR(enum3_annotation2v, ".");
FATAL_STR(enum3_annotation3k, "duplicate_id_annotation_1");
FATAL_STR(enum3_annotation3v, "duplicate id annotation");
FATAL_STR(enum3_annotation4k, "duplicate_id_annotation_2");
FATAL_STR(enum3_annotation4v, "duplicate.id.annotation");
FATAL_STR(enum3_annotation5k, "one.here");
FATAL_STR(enum3_annotation5v, "with some value associated");
FATAL_STR(enum3_annotation6k, "yet.another");
FATAL_STR(enum3_annotation6v, "and yet more text - it's that easy");

TEST(fatal_enum, annotations) {
  EXPECT_SAME<
    fatal::type_map<>,
    apache::thrift::reflect_enum<enum1>::annotations::map
  >();

  EXPECT_SAME<
    fatal::type_map<>,
    apache::thrift::reflect_enum<enum2>::annotations::map
  >();

  using enum3_annotations = apache::thrift::reflect_enum<enum3>::annotations;

  EXPECT_SAME<
    enum3_annotation1k,
    enum3_annotations::keys::_now_with_an_underscore
  >();
  EXPECT_SAME<
    enum3_annotation1v,
    enum3_annotations::values::_now_with_an_underscore
  >();
  EXPECT_SAME<
    enum3_annotation2k,
    enum3_annotations::keys::another_there
  >();
  EXPECT_SAME<
    enum3_annotation2v,
    enum3_annotations::values::another_there
  >();
  EXPECT_SAME<
    enum3_annotation3k,
    enum3_annotations::keys::duplicate_id_annotation_1
  >();
  EXPECT_SAME<
    enum3_annotation3v,
    enum3_annotations::values::duplicate_id_annotation_1
  >();
  EXPECT_SAME<
    enum3_annotation4k,
    enum3_annotations::keys::duplicate_id_annotation_2
  >();
  EXPECT_SAME<
    enum3_annotation4v,
    enum3_annotations::values::duplicate_id_annotation_2
  >();
  EXPECT_SAME<
    enum3_annotation5k,
    enum3_annotations::keys::one_here
  >();
  EXPECT_SAME<
    enum3_annotation5v,
    enum3_annotations::values::one_here
  >();
  EXPECT_SAME<
    enum3_annotation6k,
    enum3_annotations::keys::yet_another
  >();
  EXPECT_SAME<
    enum3_annotation6v,
    enum3_annotations::values::yet_another
  >();

  EXPECT_SAME<
    fatal::build_type_map<
      enum3_annotation1k, enum3_annotation1v,
      enum3_annotation2k, enum3_annotation2v,
      enum3_annotation3k, enum3_annotation3v,
      enum3_annotation4k, enum3_annotation4v,
      enum3_annotation5k, enum3_annotation5v,
      enum3_annotation6k, enum3_annotation6v
    >,
    enum3_annotations::map
  >();
}

} // namespace cpp_reflection {
} // namespace test_cpp2 {
