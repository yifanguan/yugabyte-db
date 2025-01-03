//
// Copyright (c) YugabyteDB, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
// in compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied.  See the License for the specific language governing permissions and limitations
// under the License.
//
//

#include <yb/tserver/ysql_advisory_lock_table.h>
#include "yb/client/yb_table_name.h"
#include "yb/client/client.h"
#include "yb/master/master_defaults.h"

DECLARE_bool(yb_enable_advisory_lock);

namespace yb {

YsqlAdvisoryLocksTable::YsqlAdvisoryLocksTable(client::YBClient& client)
    : client_(client) {}

YsqlAdvisoryLocksTable::~YsqlAdvisoryLocksTable() {
}

Result<client::YBTablePtr> YsqlAdvisoryLocksTable::GetTable() {
  SCHECK(FLAGS_yb_enable_advisory_lock, NotSupported, "Advisory locks are not enabled");
  std::lock_guard<std::mutex> l(mutex_);
  if (!table_) {
    static const client::YBTableName table_name(
        YQL_DATABASE_CQL, master::kSystemNamespaceName, kPgAdvisoryLocksTableName);
    table_ = VERIFY_RESULT(client_.OpenTable(table_name));
  }
  return table_;
}

} // namespace yb
