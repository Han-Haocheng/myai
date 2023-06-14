//
// Created by HanHaocheng on 2023/06/10.
//

#ifndef THINK_FUNCATION_H
#define THINK_FUNCATION_H

#include <string>

namespace thinksystem::utils
{

template<typename Ty>
std::string to_path(Ty)
{
  return {".\\"};
}

}// namespace thinksystem::utils

#endif//THINK_FUNCATION_H
