#ifndef UTILITIES_H
#define UTILITIES_H

#include <algorithm>
#include <array>
#include <ostream>
#include <string_view>
#include <type_traits>
#include <utility>

namespace tftp {

namespace enumUtil {

namespace parser {

// Avoid triggering static_assert(false, ...) inside EnumValueName if GNU G++ not availble
#ifdef __GNUG__
inline constexpr bool GNUCompiler = true;
#else
inline constexpr bool GNUCompiler = false;
  #error GNU G++ is required
#endif

template <auto V>
constexpr auto EnumValueName() noexcept
{
  static_assert(std::is_enum_v<decltype(V)>, "enumValueName requires only enum type");
  static_assert(GNUCompiler, "enumValueName requires GNU G++ compiler");

  constexpr std::string_view header = "constexpr auto tftp::enumUtil::parser::enumValueName() [with auto V = ";
  constexpr std::string_view footer = "]";
  constexpr std::string_view name   = __PRETTY_FUNCTION__;

  constexpr std::string_view full_element_name = name.substr(header.size(), name.length() - header.length() - footer.length());

  // return only name of element exluding name of enum and namspaces (for MyEnum::MyEnumElement returns MyEnumElement)
  return full_element_name.substr(full_element_name.find_last_of(':') + 1);
}

template <class Enum, Enum left, Enum right>
struct EnumMapperRange {
  static_assert(static_cast<std::underlying_type_t<Enum>>(left) <= static_cast<std::underlying_type_t<Enum>>(right), "right must be >= left");

  using underlying_enum_t = std::underlying_type_t<Enum>;
  using array_enum_t      = std::array<std::string_view, static_cast<underlying_enum_t>(right) - static_cast<underlying_enum_t>(left) + 1>;

  static inline constexpr underlying_enum_t start = static_cast<underlying_enum_t>(left);
  static inline constexpr underlying_enum_t end   = static_cast<underlying_enum_t>(right);

  template <std::size_t... Is>
  static constexpr array_enum_t MakeMapping(std::index_sequence<Is...>)
  {
    array_enum_t result;
    ((result.at(Is) = EnumValueName<static_cast<Enum>(start + Is)>()), ...);

    return result;
  }

  static inline constexpr array_enum_t mapping = MakeMapping(std::make_index_sequence<array_enum_t {}.size()> {});
};

} // namespace parser

#define DECLARE_ENUM_STRINGIFYER(Enum) std::ostream& operator<<(std::ostream& os, const Enum v)

// clang-format off

#ifdef __GNUG__

// Main macros for definition enum for converting to list of strings
#define DEFINE_ENUM_STRINGIFYER_RANGE(Enum, min_value, max_value)                                                                                                         \
std::ostream& operator<<(std::ostream& os, const Enum v)                                                                                                                  \
{                                                                                                                                                                         \
  static_assert(std::is_same_v<Enum, decltype(min_value)>, "min_value must be " #Enum);                                                                                   \
  static_assert(std::is_same_v<Enum, decltype(max_value)>, "max_value must be " #Enum);                                                                                   \
  if (static_cast<std::underlying_type_t<Enum>>(v) < tftp::enumUtil::parser::EnumMapperRange<Enum, min_value, max_value>::start ||                                        \
      static_cast<std::underlying_type_t<Enum>>(v) > tftp::enumUtil::parser::EnumMapperRange<Enum, min_value, max_value>::end) {                                          \
    return os << '(' << #Enum << ')' << static_cast<int>(v);                                                                                                              \
  }                                                                                                                                                                       \
  return os << tftp::enumUtil::parser::EnumMapperRange<Enum, min_value, max_value>::mapping[static_cast<std::underlying_type_t<Enum>>(v) -                                \
                                                                                            tftp::enumUtil::parser::EnumMapperRange<Enum, min_value, max_value>::start];  \
}                                                                                                                                                                         \
static_assert(true, "") /* To require a ; after macro call */
#else
  #error GNU G++ is required
#endif

// clang-format -on

} // namespace enumUtil

} // namespace tftp

#endif // UTILITIES_H
