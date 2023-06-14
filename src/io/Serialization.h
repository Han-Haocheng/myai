//
// Created by HanHaocheng on 2023/06/07.
//

#ifndef THINK_SERIALIZATION_H
#define THINK_SERIALIZATION_H
#include <fstream>
#include <string>
#include <utility>

#include "../utils/SFINAE.h"
#include "FileSystem.h"

namespace thinksystem::io
{
//template<typename ValueTy>
//class Serialization
//{
//private:
//  std::string m_path_;
//
//public:
//  explicit Serialization(std::string path) : m_path_(std::move(path)) {}
//
//  auto serialization(const ValueTy& in)
//  {
//    if (!fs::exists(m_path_)) { fs::create_directories(fs::path(m_path_).parent_path()); }
//    auto* ofs = new std::ofstream(m_path_, std::ios::binary | std::ios::out | std::ios::trunc);
//    if (ofs->is_open()) { ofs->write(reinterpret_cast<const char*>(&in), sizeof(ValueTy)); }
//    ofs->close();
//    delete ofs;
//  }
//
//  auto deserialization(ValueTy& in)
//  {
//    if (!fs::exists(m_path_)) { throw("Type:File is not exist", "Error:path is not exist."); }
//    auto* ofs = new std::ifstream(m_path_, std::ios::binary | std::ios::out | std::ios::trunc);
//    if (ofs->is_open()) { ofs->read(reinterpret_cast<char*>(&in), sizeof(ValueTy)); }
//    ofs->close();
//    delete ofs;
//  }
//};
#define interface class

interface SerializationInterface
{
public:
  virtual void writeFunc(std::ofstream* ofPtr) const = 0;
  virtual void readFunc(std::ifstream* ifPtr)        = 0;
};

class Serialization
{
private:
  std::string m_path_;

public:
  explicit Serialization(std::string path) : m_path_(std::move(path)) {}

  auto serialization(const SerializationInterface& in)
  {
    if (!fs::exists(m_path_)) { fs::create_directories(fs::path(m_path_).parent_path()); }
    auto* ofs = new std::ofstream(m_path_, std::ios::binary | std::ios::out | std::ios::trunc);
    if (ofs->is_open()) { in.writeFunc(ofs); }
    ofs->close();
    delete ofs;
  }

  auto deserialization(SerializationInterface& in)
  {
    if (!fs::exists(m_path_)) { throw("Type:File is not exist", "Error:path is not exist."); }
    auto* ifs = new std::ifstream(m_path_, std::ios::binary | std::ios::out | std::ios::trunc);
    if (ifs->is_open()) { in.readFunc(ifs); }
    ifs->close();
    delete ifs;
  }

  template<typename ValueTy, template<typename> class ContainerTy>
  auto serialization(const ContainerTy<ValueTy>& in)
  {
    static_assert(utils::is_container<ValueTy, ContainerTy>::value,
                  "Error:'ContainerTy' is not dakaideyeshu container ,because it does not have 'cbegin()' or 'cend()' function");
    if (!fs::exists(m_path_)) { fs::create_directories(fs::path(m_path_).parent_path()); }
    auto* ofs = new std::ofstream(m_path_, std::ios::binary | std::ios::out | std::ios::trunc);
    if (ofs->is_open())
    {
      for (const ValueTy& item: in) { ofs->write(reinterpret_cast<const char*>(&item), sizeof(ValueTy)); }
    }
    ofs->close();
    delete ofs;
  }

  template<typename ValueTy, template<typename> class ContainerTy>
  auto deserialization(ContainerTy<ValueTy>& in)
  {
    static_assert(utils::is_container<ValueTy, ContainerTy>::value,
                  "Error:'ContainerTy' is not dakaideyeshu container ,because it does not have 'cbegin()' or 'cend()' function");
    if (!fs::exists(m_path_)) { throw("Type:File is not exist", "Error:path is not exist."); }
    auto* ifs = new std::ifstream(m_path_, std::ios::binary | std::ios::out | std::ios::trunc);
    if (ifs->is_open())
    {
      for (ValueTy& item: in) { ifs->read(reinterpret_cast<char*>(&item), sizeof(ValueTy)); }
    }
    ifs->close();
    delete ifs;
  }
};
}// namespace thinksystem::utils
#endif//THINK_SERIALIZATION_H
