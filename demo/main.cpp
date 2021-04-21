//#include "Dbcs.hpp"

#include <rocksdb/db.h>
#include <rocksdb/options.h>
#include <rocksdb/slice.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <boost/program_options.hpp>

/*
using namespace rocksdb;

void create_test_bd(DB* db)
{
  rocksdb::WriteOptions write_options;
  write_options.sync = false;
  srand( (unsigned)time(NULL) );
  for (uint32_t i = 0; i < 10; ++i)
      db->Put(write_options, std::to_string(std::rand()), std::to_string(std::rand()));
  db->Put(write_options, "new Iteration" + std::to_string(std::rand()), "new Iteration" + std::to_string(std::rand()));
}

int main() {
  std::string kDBPath = "../check.db";
  Options options;
  options.create_if_missing = true;
  DB* db;
  Status s = DB::Open(options, kDBPath, &db);
  // Put key-value
//  s = db->Put(WriteOptions(), "key2", "value2");
//  assert(s.ok());
  std::string value;
//
// Get value

//  s = db->Get(ReadOptions(), "key3", &value);
//  if (!s.ok()) std::cout << "error" << std::endl;
//    else std::cout << "value : " << value << std::endl;
  //
// Delete value
//    db ->GetProperty("")
  std::cout << "Adding key - value pairs" << std::endl;

//  create_test_bd(db);

  std::vector<std::thread> _threads;
  for (uint32_t i = 0; i < 10; ++i)
    _threads.emplace_back(&create_test_bd, std::ref(db));
  for (auto& worker: _threads)
    worker.join();

  std::cout << "Iterator : " << std::endl;
  Iterator* it = db->NewIterator(rocksdb::ReadOptions());
  for (it->SeekToFirst(); it->Valid(); it->Next()) {
    std::cout << it->key().ToString() << ": " << it->value().ToString() << std::endl;
  }
  delete it;
}
 */

using namespace boost::program_options;

int main(int argc, char** argv) {
  std::string usage =\
 R"(Usage:

  dbcs [options] <path/to/input/storage.db>

  Options:

  --log-level <string>          = "info"|"warning"|"error"
                                = default: "error"
  --thread-count <number>       =
                                = default: count of logical core
  --output <path>               = <path/to/output/storage.db>
                                = default: <path/to/input/dbcs-storage.db>)";

  options_description desc("General options");
  std::string task_type;
  desc.add_options()
      ("help", usage.c_str())
      ("log-level", value<std::string>()->default_value("error"))
      ("thread-count", value<uint32_t>()->default_value(std::thread::hardware_concurrency()))
      ("output", value<std::string>()->default_value("../dbcs-storage.db"));
  variables_map vm;
  try {
    parsed_options parsed = command_line_parser(argc, argv).options(desc).allow_unregistered().run();
    store(parsed, vm);

    if (vm.count("help")) {
      std::cout << usage << std::endl;
      return 0;
    }

    std::string log_level = vm["log-level"].as<std::string>();
    uint32_t thread_count = vm["thread-count"].as<uint32_t>();
    std::string output = vm["output"].as<std::string>();

  }
  catch(std::exception& ex) {
    std::cout << usage << std::endl;
    return 1;
  }

}