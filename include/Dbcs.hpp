//
// Created by mrbgn on 4/21/21.
//

#include <rocksdb/db.h>
#include <rocksdb/options.h>
#include <rocksdb/slice.h>
#include <string>

#include "PicoSHA.hpp"
#include "ThreadPool.hpp"

#ifndef TEMPLATE_DBCS_HPP
#define TEMPLATE_DBCS_HPP

/**
 * @brief Class to operate with existed ( created by time ) rocksdb DataBase
 * @class Dbcs
 *
 * @param log_level Log level to Boost_log
 * @param thread_count Quantity of threads to operate with db
 * @param output Link to output file
 *
 */

class Dbcs{
 public:
  Dbcs(std::string& log_level, uint32_t& thread_count, std::string& output);
  /**
   * @brief void to create test db with random key-value pairs
   */
  void CreateTestDataBase();
  /**
   *  @brief void to read from db in multiple threads
   */
   void ReadFromDataBase();

   /*
    * @brief void to create new key-hash entry in multiple threads
    */
   void AddNewKeyHashEntry();

 private:
  ThreadPool _threadPool;
//  rocksdb::DB** _db;
//  std::string _kDBPath;

};

#endif  // TEMPLATE_DBCS_HPP
