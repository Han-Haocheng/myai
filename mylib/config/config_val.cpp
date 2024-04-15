//
// Created by HanHaocheng on 2024/4/9.
//

#include "config_val.h"

MYLIB_SPACE_BEGIN
ConfigValueBase::ConfigValueBase(String name, String comment, String type_name)
    : m_type_name(std::move(type_name)), m_name(std::move(name)), m_comment(std::move(comment)) {
}

MYLIB_SPACE_END
