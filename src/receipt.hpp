/* ReaPack: Package manager for REAPER
 * Copyright (C) 2015-2017  Christian Fillion
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef REAPACK_RECEIPT_HPP
#define REAPACK_RECEIPT_HPP

#include "registry.hpp"
#include "errors.hpp"

#include <array>
#include <vector>
#include <set>

class Index;
class InstallTicket;
class Path;
class ReceiptPage;
class Version;

typedef std::shared_ptr<const Index> IndexPtr;
typedef std::array<ReceiptPage, 4> ReceiptPages;

class Receipt {
public:
  enum Flag {
    NoFlags     = 0,
    RestartNeeded = 1<<0,
  };

  Receipt();

  bool test(Flag f) const { return (m_flags & f) != 0; }

  bool empty() const;
  ReceiptPages pages() const;

  void addInstall(const Version *, const Registry::Entry &);
  void addRemoval(const Path &p);
  void addRemovals(const std::set<Path> &);
  void addExport(const Path &p);
  void addError(const ErrorInfo &);

private:
  int m_flags;
  std::vector<InstallTicket> m_installs;
  std::set<Path> m_removals;
  std::set<Path> m_exports;
  std::vector<ErrorInfo> m_errors;
};

class ReceiptPage {
public:
  template<typename T>
  ReceiptPage(const T &list, const char *singular, const char *plural = nullptr)
    : m_size(list.size())
  {
    setTitle(m_size == 1 || !plural ? singular : plural);

    std::ostringstream stream;

    for(const auto &item : list)
      stream << item << "\r\n";

    m_contents = stream.str();
  }

  const std::string &title() const { return m_title; }
  const std::string &contents() const { return m_contents; }
  bool empty() const { return m_size == 0; }

private:
  void setTitle(const char *title);
  size_t m_size;
  std::string m_title;
  std::string m_contents;
};

class InstallTicket {
public:
  InstallTicket(const Version *ver, const Registry::Entry &previousEntry);

private:
  friend std::ostream &operator<<(std::ostream &, const InstallTicket &);

  const Version *m_version;
  VersionName m_previous;
  bool m_isUpdate;

  IndexPtr m_index; // to keep it alive long enough
};

std::ostream &operator<<(std::ostream &, const InstallTicket &);

#endif
