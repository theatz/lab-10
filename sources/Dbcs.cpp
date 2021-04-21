//
// Created by mrbgn on 4/21/21.
//

#include "Dbcs.hpp"

//Dbcs::Dbcs() {
//  _kDBPath = "../check.db";
//  rocksdb::Options options;
//  options.create_if_missing = true;
//  rocksdb::DB *db;
//  rocksdb::Status s = rocksdb::DB::Open(options, _kDBPath, &db);
//  assert(s.ok());
//}

/**
 * @todo operation with ThreadPool
 * @param log_level
 * @param thread_count
 * @param output
 */

Dbcs::Dbcs(std::string &log_level, uint32_t &thread_count,
           std::string &output)
    : _threadPool(thread_count)
{
}
