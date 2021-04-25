//
// Created by mrbgn on 4/21/21.
//

#include "Dbcs.hpp"

// Dbcs::Dbcs() {
//   _kDBPath = "../check.db";
//   rocksdb::Options options;
//   options.create_if_missing = true;
//   rocksdb::DB *db;
//   rocksdb::Status s = rocksdb::DB::Open(options, _kDBPath, &db);
//   assert(s.ok());
// }

/**
 * @todo operation with ThreadPool
 * @param log_level
 * @param thread_count
 * @param output
 */

Dbcs::Dbcs(std::string &log_level, uint32_t &thread_count, std::string &output,
           std::string &input)
    : _threadPool(thread_count) {
  rocksdb::Options option;
  option.create_if_missing = false;
  rocksdb::Status s = rocksdb::DB::Open(option, input, &_dbRead);
  if (!s.ok()) {
    option.create_if_missing = true;
    s = rocksdb::DB::Open(option, input, &_dbRead);
    CreateTestDataBase();
  }
  option.create_if_missing = true;
  s = rocksdb::DB::Open(option, output, &_dbWrite);
  if (!s.ok()) {
    std::cout << "error occured with creating db" << std::endl;
    return;
  }
  ReadFromDataBaseSync();
}

/**
 * @brief iterative adding random key-value pair in 1 thread
 */
void Dbcs::CreateTestDataBase() {
  rocksdb::WriteOptions write_options;
  write_options.sync = false;
  srand((unsigned)time(NULL));
  for (uint32_t i = 0; i < 100; ++i)
    _dbRead->Put(write_options, std::to_string(std::rand()),
                 std::to_string(std::rand()));
}
/**
 * iterative reading db in 1 thread and forwarding to multiple thread's consumer
 */
void Dbcs::ReadFromDataBaseSync() {
  rocksdb::Iterator *it = _dbRead->NewIterator(rocksdb::ReadOptions());
  it->SeekToLast();
  for (it->SeekToFirst(); it->Valid(); it->Next()) {
    KeyValuePair pair{it->key(), it->value()};
    _threadPool.enqueue(&Dbcs::AddNewKeyHashEntry, std::ref(*this), pair);
  }
  delete it;
}

void Dbcs::AddNewKeyHashEntry(KeyValuePair &pair) {
  rocksdb::Status s;
  rocksdb::WriteOptions write_options;
  write_options.sync = false;
//  std::cout << "Added task pair: " << pair.key.ToString()
//            << " : Value: " << pair.value.ToString() << std::endl;
  s = _dbWrite->Put(write_options, pair.key,
                    picosha2::hash256_hex_string(pair.value.ToString()));
  if (!s.ok())
    std::cout << "error" << std::endl;
  else
    std::cout << "it's ok" << std::endl;
}
void Dbcs::EnableLogging()
{
  const std::string format = "%TimeStamp% <%Severity%> (%ThreadID%): %Message%";
  boost::log::add_console_log(std::cout,
                              boost::log::keywords::format = "[%TimeStamp%] [%Severity%] %Message%",
                              boost::log::keywords::auto_flush = true,
                              boost::log::keywords::filter = boost::log::trivial::severity
                                              == boost::log::trivial::n  );

  typedef boost::log::sinks::synchronous_sink<boost::log::sinks::text_file_backend> file_sink;
  boost::shared_ptr<file_sink> sink(new file_sink(
      boost::log::keywords::file_name = "./logs/file_%5N.log",
      boost::log::keywords::rotation_size = 5 * 1024 * 1024,
      boost::log::keywords::auto_flush = true));

  sink->set_formatter(boost::log::expressions::stream
                          << "["
                          << boost::log::expressions::attr<boost::posix_time::ptime>("TimeStamp")
                          << "] [" << boost::log::trivial::severity << "] "
                          << boost::log::expressions::smessage);

  boost::log::core::get()->add_sink(sink);
  boost::log::add_common_attributes();
}
