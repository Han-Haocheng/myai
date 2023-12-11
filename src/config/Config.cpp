//
// Created by HanHaocheng on 2023/11/7.
//

#include "Config.h"

#include <utility>

namespace myai
{
ConfigBase::ConfigBase(std::string name, std::string comment)
    : m_name(std::move(name)), m_comment(std::move(comment)) {}
const std::string &ConfigBase::getName() const { return m_name; }
const std::string &ConfigBase::getComment() const { return m_comment; }

}// namespace myai