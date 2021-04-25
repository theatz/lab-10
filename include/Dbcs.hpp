//
// Created by mrbgn on 4/21/21.
//

#include <rocksdb/db.h>
#include <rocksdb/options.h>
#include <rocksdb/slice.h>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/expressions/keyword.hpp>


#include <ctime>
#include <string>
#include <iostream>

#include "PicoSHA.hpp"
#include "ThreadPool.hpp"

#ifndef TEMPLATE_DBCS_HPP
#define TEMPLATE_DBCS_HPP

/**
 * @brief Struct to forward key-value pairs to consumer
 * @param key Key from key-value pair
 * @param value value from key-value pair
 */

struct KeyValuePair {
  rocksdb::Slice key;
  rocksdb::Slice value;
};

/**
 * @brief Class to operate with existed ( created by time ) rocksdb DataBase
 * @class Dbcs
 *
 * @param log_level Log level to Boost_log
 * @param thread_count Quantity of threads to operate with db
 * @param output Link to output db
 * @param input Link to input db
 *
 */

class Dbcs {
 public:
  Dbcs(std::string& log_level, uint32_t& thread_count, std::string& output,
       std::string& input);
  /**
   * @brief void to create test db with random key-value pairs
   */
  void CreateTestDataBase();
  /**
   *  @brief void to read from db in one threads
   */
  void ReadFromDataBaseSync();

  /**
   * @brief void to create new key-hash entry in multiple threads
   */
  void AddNewKeyHashEntry(KeyValuePair& pair);

  /**
   * @brief void to enable logging via Boost::Log
   */

  void EnableLogging();

 private:
  ThreadPool _threadPool;
  rocksdb::DB* _dbRead;
  rocksdb::DB* _dbWrite;
  //  std::string _kDBPath;
};

#endif  // TEMPLATE_DBCS_HPP
