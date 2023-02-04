#include "storage_engine.h"

#include <glog/logging.h>
#include <vector>

StorageEngine* StorageEngine::storage_engine_ = nullptr;

StorageEngine::StorageEngine()
    : is_open_(false),
      leveldb_(nullptr) {
}

StorageEngine::~StorageEngine() {
  assert(!is_open_);
}

/* This is not a thread-safe function */
void StorageEngine::Init() {
  if (!storage_engine_) {
    storage_engine_ = new StorageEngine();
  }
}

StorageEngine* StorageEngine::GetCurrent() {
  assert(storage_engine_ != nullptr);
  return storage_engine_;
}

Status StorageEngine::Open(const std::string& path) {
  assert(!is_open_);
  is_open_ = true;

  Options options;
  options.create_if_missing = true;
  Status s = leveldb::DB::Open(options, path, &leveldb_);
  return Status::OK();
}

void StorageEngine::Close() {
  assert(is_open_);
  delete leveldb_;

  leveldb_ = nullptr;
  is_open_ = false;
}

bool StorageEngine::FlushAll() {
  assert(is_open_);
  delete leveldb_;
  int flag = system("rm -rf /root/Git/KV-Server/db");
  if (flag != -1) {
    is_open_ = false;
    Open("./db");
    return true;
  } else {
    return false;
  }
}

Status StorageEngine::Set(const std::string& key, const std::string& value) {
  Status status = leveldb_->Put(leveldb::WriteOptions(), key, value);
  return status;
}

Status StorageEngine::Get(const std::string& key, std::string* const value) {
  Status status = leveldb_->Get(leveldb::ReadOptions(), key, value);
  return status;
}

Status StorageEngine::Delete(const std::string& key) {
  Status status = leveldb_->Delete(leveldb::WriteOptions(), key);
  return status;
}

void StorageEngine::Keys(std::vector<std::string>& argv) {
  leveldb::Iterator* it = leveldb_->NewIterator(leveldb::ReadOptions());
  for (it->SeekToFirst(); it->Valid(); it->Next()) {
    argv.push_back(it->key().ToString());
  }
  delete it;
}
