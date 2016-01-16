/* ReaPack: Package manager for REAPER
 * Copyright (C) 2015-2016  Christian Fillion
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

#ifndef REAPACK_SQLITE_HPP
#define REAPACK_SQLITE_HPP

#include <functional>
#include <string>
#include <vector>

class reapack_error;

struct sqlite3;
struct sqlite3_stmt;

namespace SQLite {
  class Statement;

  class Database {
  public:
    Database(const std::string &filename = ":memory:");
    ~Database();

    Statement *prepare(const char *sql);
    void query(const char *sql);

  private:
    friend Statement;

    reapack_error lastError() const;

    sqlite3 *m_db;
    std::vector<Statement *> m_statements;
  };

  class Statement {
  public:
    typedef std::function<bool (void)> ExecCallback;

    void bind(const int index, const char *text);
    void bind(const int index, const uint64_t integer);
    void exec();
    void exec(const ExecCallback &);

    uint64_t uint64Column(const int index) const;

  private:
    friend Database;

    Statement(const char *sql, Database *db);
    ~Statement();

    Database *m_db;
    sqlite3_stmt *m_stmt;
  };
};

#endif
