
/*
 * performance_test.cpp
 *
 * This includes performance test for index structures such as std::map, 
 * std::unordered_map, stx::btree and stx::btree_multimap
 */

#include "test_suite.h"

/*
 * TestStdMapInsertReadPerformance() - As name suggests
 */
void TestStdMapInsertReadPerformance(int key_num) {
  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();

  // Insert 1 million keys into std::map
  std::map<long, long> test_map{};
  for(int i = 0;i < key_num;i++) {
    test_map[i] = i;
  }

  end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds = end - start;

  std::cout << "std::map: " << 1.0 * key_num / (1024 * 1024) / elapsed_seconds.count()
            << " million insertion/sec" << "\n";

  ////////////////////////////////////////////
  // Test read
  std::vector<long> v{};
  v.reserve(100);

  start = std::chrono::system_clock::now();

  int iter = 10;
  for(int j = 0;j < iter;j++) {
    // Read 1 million keys from std::map
    for(int i = 0;i < key_num;i++) {
      long t = test_map[i];

      v.push_back(t);
      v.clear();
    }
  }

  end = std::chrono::system_clock::now();

  elapsed_seconds = end - start;
  std::cout << "std::map: " << (1.0 * iter * key_num) / (1024 * 1024) / elapsed_seconds.count()
            << " million read/sec" << "\n";

  return;
}

/*
 * TestStdUnorderedMapInsertReadPerformance() - As name suggests
 */
void TestStdUnorderedMapInsertReadPerformance(int key_num) {
  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();

  // Insert 1 million keys into std::map
  std::unordered_map<long, long> test_map{};
  for(int i = 0;i < key_num;i++) {
    test_map[i] = i;
  }

  end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds = end - start;

  std::cout << "std::unordered_map: " << 1.0 * key_num / (1024 * 1024) / elapsed_seconds.count()
            << " million insertion/sec" << "\n";

  ////////////////////////////////////////////
  // Test read
  std::vector<long> v{};
  v.reserve(100);

  start = std::chrono::system_clock::now();

  int iter = 10;
  for(int j = 0;j < iter;j++) {
    // Read 1 million keys from std::map
    for(int i = 0;i < key_num;i++) {
      long t = test_map[i];

      v.push_back(t);
      v.clear();
    }
  }

  end = std::chrono::system_clock::now();

  elapsed_seconds = end - start;
  std::cout << "std::unordered_map: " << (1.0 * iter * key_num) / (1024 * 1024) / elapsed_seconds.count()
            << " million read/sec" << "\n";

  return;
}

/*
 * TestBTreeInsertReadPerformance() - As name suggests
 */
void TestBTreeInsertReadPerformance(int key_num) {
  std::chrono::time_point<std::chrono::system_clock> start, end;

  // Insert 1 million keys into stx::btree
  btree<long,
        long,
        std::pair<long, long>,
        KeyComparator> test_map{KeyComparator{1}};

  start = std::chrono::system_clock::now();

  for(long i = 0;i < key_num;i++) {
    test_map.insert(i, i);
  }

  end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds = end - start;

  std::cout << "stx::btree: " << 1.0 * key_num / (1024 * 1024) / elapsed_seconds.count()
            << " million insertion/sec" << "\n";

  ////////////////////////////////////////////
  // Test read
  std::vector<long> v{};
  v.reserve(100);

  start = std::chrono::system_clock::now();

  int iter = 10;
  for(int j = 0;j < iter;j++) {
    // Read keys from stx::btree
    for(int i = 0;i < key_num;i++) {
      auto it = test_map.find(i);

      v.push_back(it->second);
      v.clear();
    }
  }

  end = std::chrono::system_clock::now();

  elapsed_seconds = end - start;
  std::cout << "stx::btree " << (1.0 * iter * key_num) / (1024 * 1024) / elapsed_seconds.count()
            << " million read/sec" << "\n";

  return;
}

/*
 * TestBTreeMultimapInsertReadPerformance() - As name suggests
 *
 * This function tests btree_multimap in a sense that retrieving values
 * are not considered complete until values have all been pushed
 * into the vector. This requires getting the iterator pair first, and
 * then iterate on the interval, pushing values into the vector
 */
void TestBTreeMultimapInsertReadPerformance(int key_num) {
  std::chrono::time_point<std::chrono::system_clock> start, end;

  // Initialize multimap with a key comparator that is not trivial
  btree_multimap<long, long, KeyComparator> test_map{KeyComparator{1}};

  start = std::chrono::system_clock::now();

  // Insert 1 million keys into stx::btree_multimap
  for(long i = 0;i < key_num;i++) {
    test_map.insert(i, i);
  }

  end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds = end - start;

  std::cout << "stx::btree_multimap: " << (1.0 * key_num / (1024 * 1024)) / elapsed_seconds.count()
            << " million insertion/sec" << "\n";

  ////////////////////////////////////////////
  // Test read
  std::vector<long> v{};
  v.reserve(100);

  start = std::chrono::system_clock::now();

  int iter = 10;
  for(int j = 0;j < iter;j++) {
    // Read 1 million keys from stx::btree
    for(int i = 0;i < key_num;i++) {
      auto it_pair = test_map.equal_range(i);

      // For multimap we need to write an external loop to
      // extract all keys inside the multimap
      // This is the place where btree_multimap is slower than
      // btree
      for(auto it = it_pair.first;it != it_pair.second;it++) {
        v.push_back(it->second);
      }

      v.clear();
    }
  }

  end = std::chrono::system_clock::now();

  elapsed_seconds = end - start;
  std::cout << "stx::btree_multimap " << (1.0 * iter * key_num / (1024 * 1024)) / elapsed_seconds.count()
            << " million read/sec" << "\n";

  return;
}


/*
 * TestCuckooHashTableInsertReadPerformance() - Tests cuckoo hash table
 */
void TestCuckooHashTableInsertReadPerformance(int key_num) {
  std::chrono::time_point<std::chrono::system_clock> start, end;

  // Initialize multimap with a key comparator that is not trivial
  cuckoohash_map<long, long> test_map{};

  start = std::chrono::system_clock::now();

  // Insert 1 million keys into stx::btree_multimap
  for(long i = 0;i < key_num;i++) {
    test_map.insert(i, i);
  }

  end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds = end - start;

  std::cout << "cuckoohash_map: " << (1.0 * key_num / (1024 * 1024)) / elapsed_seconds.count()
            << " million insertion/sec" << "\n";

  ////////////////////////////////////////////
  // Test read
  std::vector<long> v{};
  v.reserve(100);

  start = std::chrono::system_clock::now();

  int iter = 10;
  for(int j = 0;j < iter;j++) {
    // Read 1 million keys from stx::btree
    for(int i = 0;i < key_num;i++) {
      long int ret;
      
      test_map.find(i, ret);

      v.push_back(ret);

      v.clear();
    }
  }

  end = std::chrono::system_clock::now();

  elapsed_seconds = end - start;
  std::cout << "cuckoohash_map " << (1.0 * iter * key_num / (1024 * 1024)) / elapsed_seconds.count()
            << " million read/sec" << "\n";

  return;
}

/*
 * TestBwTreeInsertReadDeletePerformance() - As name suggests
 *
 * This function runs the following tests:
 *
 * 1. Sequential insert (key, key)
 * 2. Sequential read
 * 3. Forward iterate
 * 4. Backward iterate
 * 5. Reverse order insert (key, key + 1)
 * 6. Reverse order read
 * 7. Remove all values
 */
void TestBwTreeInsertReadDeletePerformance(TreeType *t, int key_num) {
  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();

  for(int i = 0;i < key_num;i++) {
    t->Insert(i, i);
  }

  end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds = end - start;

  std::cout << "BwTree: " << (key_num / (1024.0 * 1024.0)) / elapsed_seconds.count()
            << " million insertion/sec" << "\n";

  // Then test read performance
  int iter = 10;
  std::vector<long> v{};

  v.reserve(100);

  start = std::chrono::system_clock::now();

  for(int j = 0;j < iter;j++) {
    for(int i = 0;i < key_num;i++) {
      t->GetValue(i, v);

      v.clear();
    }
  }

  end = std::chrono::system_clock::now();

  elapsed_seconds = end - start;
  std::cout << "BwTree: " << (iter * key_num / (1024.0 * 1024.0)) / elapsed_seconds.count()
            << " million read/sec" << "\n";

  ///////////////////////////////////////////////////////////////////
  // Test Iterator (forward, single value)
  ///////////////////////////////////////////////////////////////////
  
  start = std::chrono::system_clock::now();
  {
    for(int j = 0;j < iter;j++) {
      auto it = t->Begin();
      while(it.IsEnd() == false) {
        v.push_back(it->second);

        v.clear();
        it++;
      }
      
      it--;
      if(it->first != key_num - 1) {
        throw "Error: Forward iterating does not reach the end";
      } 
    }

    end = std::chrono::system_clock::now();

    elapsed_seconds = end - start;
    std::cout << "BwTree: " << (iter * key_num / (1024.0 * 1024.0)) / elapsed_seconds.count()
              << " million forward iteration/sec" << "\n";
  }
  
  ///////////////////////////////////////////////////////////////////
  // Test Iterator (backward, single value)
  ///////////////////////////////////////////////////////////////////
  
  start = std::chrono::system_clock::now();
  {
    for(int j = 0;j < iter;j++) {
      auto it = t->Begin(key_num - 1);
      while(it.IsREnd() == false) {
        v.push_back(it->second);

        v.clear();
        it--;
      }
      
      it++;
      if(it->first != 0) {
        throw "Error: Forward iterating does not reach the beginning";
      } 
    }
    
    end = std::chrono::system_clock::now();

    elapsed_seconds = end - start;
    std::cout << "BwTree: " << (iter * key_num / (1024.0 * 1024.0)) / elapsed_seconds.count()
              << " million backward iteration/sec" << "\n";
  }
  
  ///////////////////////////////////////////////////////////////////
  // Insert 2nd value
  ///////////////////////////////////////////////////////////////////
  
  start = std::chrono::system_clock::now();

  for(int i = key_num - 1;i >= 0;i--) {
    t->Insert(i, i + 1);
  }

  end = std::chrono::system_clock::now();

  elapsed_seconds = end - start;

  std::cout << "BwTree: " << (key_num / (1024.0 * 1024.0)) / elapsed_seconds.count()
            << " million insertion (reverse order)/sec" << "\n";

  // Read again

  start = std::chrono::system_clock::now();

  for(int j = 0;j < iter;j++) {
    for(int i = 0;i < key_num;i++) {
      t->GetValue(i, v);

      v.clear();
    }
  }

  end = std::chrono::system_clock::now();

  elapsed_seconds = end - start;
  std::cout << "BwTree: " << (iter * key_num / (1024.0 * 1024.0)) / elapsed_seconds.count()
            << " million read (2 values)/sec" << "\n";

  // Verify reads

  for(int i = 0;i < key_num;i++) {
    t->GetValue(i, v);

    assert(v.size() == 2);
    if(v[0] == (i)) {
      assert(v[1] == (i + 1));
    } else if(v[0] == (i + 1)) {
      assert(v[1] == (i));
    } else {
      assert(false);
    }

    v.clear();
  }

  std::cout << "    All values are correct!\n";

  // Finally remove values

  start = std::chrono::system_clock::now();

  for(int i = 0;i < key_num;i++) {
    t->Delete(i, i);
  }

  for(int i = key_num - 1;i >= 0;i--) {
    t->Delete(i, i + 1);
  }

  end = std::chrono::system_clock::now();

  elapsed_seconds = end - start;
  std::cout << "BwTree: " << (key_num * 2 / (1024.0 * 1024.0)) / elapsed_seconds.count()
            << " million remove/sec" << "\n";

  for(int i = 0;i < key_num;i++) {
    t->GetValue(i, v);

    assert(v.size() == 0);
  }

  std::cout << "    All values have been removed!\n";

  return;
}

/*
 * TestBwTreeInsertReadPerformance() - As name suggests
 */
void TestBwTreeInsertReadPerformance(TreeType *t, int key_num) {
  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();

  for(int i = 0; i < key_num; i++) {
    t->Insert(i, i);
  }

  end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds = end - start;

  std::cout << "BwTree: " << (key_num / (1024.0 * 1024.0)) / elapsed_seconds.count()
            << " million insertion/sec" << "\n";

  // Then test read performance

  int iter = 10;
  std::vector<long> v{};

  v.reserve(100);

  start = std::chrono::system_clock::now();

  for(int j = 0; j < iter; j++) {
    for(int i = 0; i < key_num; i++) {
      t->GetValue(i, v);
      v.clear();
    }
  }

  end = std::chrono::system_clock::now();

  elapsed_seconds = end - start;
  std::cout << "BwTree: " << (iter * key_num / (1024.0 * 1024.0)) / elapsed_seconds.count()
            << " million read/sec" << "\n";

  return;
}

#define MY_MIN(a, b) ((a) < (b) ? (a) : (b))
struct MyKey {
  std::vector<char> v;
  bool operator < (const MyKey & other) const {
    int x = memcmp(v.data(), other.v.data(), MY_MIN(v.size(), other.v.size()));
    if (x) {
      return x < 0;
    }
    return v.size() < other.v.size();
  }
  bool operator > (const MyKey & other) const {
    return other < (*this);
  }
  bool operator == (const MyKey & other) const {
    return v.size() == other.v.size() and memcmp(v.data(), other.v.data(), v.size()) == 0;
  }

  bool operator != (const MyKey & other) const {
    return v.size() != other.v.size() or memcmp(v.data(), other.v.data(), v.size());
  }
};

/*
 * TestBwTreeEmailInsertPerformance() - Tests insert performance on string
 *                                      workload (email)
 *
 * This function requires a special email file that is not distributed
 * publicly
 */
void TestBwTreeEmailInsertPerformance(BwTree<std::string, long int> *t,
                                      std::string filename) {
  std::ifstream email_file{filename, std::ios::binary};
  if (email_file.good() == false) {
    std::cout << "Unable to open file: " << filename << std::endl;
    return;
  }

  std::vector<MyKey> key_list{};
  int counter = 0;
  MyKey k;
  int keylen = 0;
  while(!email_file.eof()) {
    email_file.read((char *)&keylen, 4);
    if(!email_file.good()) break;
    k.v.resize(keylen);
    email_file.read(k.v.data(), keylen);
    key_list.push_back(k);
    counter++;
  }

  printf("Successfully loaded %d entries\n", counter);
  email_file.close();

  int key_num = counter;
  const int iter = 10;
  
  std::chrono::time_point<std::chrono::system_clock> start, end;
  std::chrono::duration<double> elapsed_seconds;
  std::vector<long int> value_buffer;

  ///////////////////////////////////////////////////////////////////
  // Test BwTree cannot be used now, because key type is string, not byte array
  ///////////////////////////////////////////////////////////////////

  // start = std::chrono::system_clock::now();

  // for(int i = 0; i < key_num; i++) {
  //   t->Insert(string_list[i], i);
  // }

  // end = std::chrono::system_clock::now();

  print_flag = true;
  delete t;
  print_flag = false;
  
  ///////////////////////////////////////////////////////////////////
  // Then test stx::btree_multimap
  ///////////////////////////////////////////////////////////////////

  stx::btree_multimap<MyKey, long int> bt{};
  start = std::chrono::system_clock::now();

  for(int i = 0; i < key_num; i++) {
    bt.insert(key_list[i], i);
  }

  end = std::chrono::system_clock::now();

  elapsed_seconds = end - start;

  std::cout << "stx::btree_multimap: " << (key_num / (1024.0 * 1024.0)) / elapsed_seconds.count()
            << " million email insertion/sec" << "\n";

  start = std::chrono::system_clock::now();

  for (int it = 0; it < iter; it++) {
    for (int i = 0; i < key_num; i++) {
      auto bt_v = bt.equal_range(key_list[i]);
      for (auto ii = bt_v.first; ii != bt_v.second; ii++)
        value_buffer.push_back(ii->second);
      value_buffer.clear();
    }
  }

  end = std::chrono::system_clock::now();

  elapsed_seconds = end - start;

  std::cout << "stx::btree_multimap: " << (iter * key_num / (1024.0 * 1024.0)) / elapsed_seconds.count()
            << " million email read/sec" << "\n";

  ///////////////////////////////////////////////////////////////////
  // Then test skiplist
  ///////////////////////////////////////////////////////////////////

  sl_map_gc<MyKey, long int> slist;

  //   << Insertion >>
  
  start = std::chrono::system_clock::now();
  
  for(int i = 0; i < key_num; i++) {
    slist.insert(std::make_pair(key_list[i], i));
  }

  end = std::chrono::system_clock::now();

  elapsed_seconds = end - start;

  std::cout << "skiplist map: " << (key_num / (1024.0 * 1024.0)) / elapsed_seconds.count()
            << " million email insertion/sec" << "\n";
  
  start = std::chrono::system_clock::now();

  for (int it = 0; it < iter; it++) {
    for (int i = 0; i < key_num; i++) {
      auto sl_v = slist.find(key_list[i]);
      value_buffer.push_back(sl_v->second);
      value_buffer.clear();
    }
  }

  end = std::chrono::system_clock::now();

  elapsed_seconds = end - start;

  std::cout << "skiplist map: " << (iter * key_num / (1024.0 * 1024.0)) / elapsed_seconds.count()
            << " million email read/sec" << "\n";

  ///////////////////////////////////////////////////////////////////
  // Then test std::map
  ///////////////////////////////////////////////////////////////////

  std::map<MyKey, long int> m{};

  start = std::chrono::system_clock::now();

  for(int i = 0; i < key_num; i++) {
    m.insert({key_list[i], i});
  }

  end = std::chrono::system_clock::now();

  elapsed_seconds = end - start;

  std::cout << "std::map: " << (key_num / (1024.0 * 1024.0)) / elapsed_seconds.count()
            << " million email insertion/sec" << "\n";

  start = std::chrono::system_clock::now();

  for (int it = 0; it < iter; it++) {
    for (int i = 0; i < key_num; i++) {
      auto m_v = m.find(key_list[i]);
      value_buffer.push_back(m_v->second);
      value_buffer.clear();
    }
  }

  end = std::chrono::system_clock::now();

  elapsed_seconds = end - start;

  std::cout << "std::map: " << (iter * key_num / (1024.0 * 1024.0)) / elapsed_seconds.count()
            << " million email read/sec" << "\n";

  ///////////////////////////////////////////////////////////////////
  // Then test ART
  ///////////////////////////////////////////////////////////////////

  ARTType at;
  art_tree_init(&at);

  long int* array = new long int[key_num];
  for (int i = 0; i < key_num; i++) array[i] = i;
  
  // std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();

  for (int i = 0; i < key_num; i++) {
    art_insert(&at, (unsigned char*)key_list[i].v.data(), key_list[i].v.size(), array + i);
  }

  end = std::chrono::system_clock::now();

  elapsed_seconds = end - start;

  std::cout << "art_tree: " << (key_num / (1024.0 * 1024.0)) / elapsed_seconds.count()
            << " million email insertion/sec" << "\n";
            
  start = std::chrono::system_clock::now();

  for (int it = 0; it < iter; it++) {
    for (int i = 0; i < key_num; i++) {
      long int at_v = *reinterpret_cast<long int*>(
        art_search(&at, (unsigned char*)key_list[i].v.data(), key_list[i].v.size())
      );
      value_buffer.push_back(at_v);
      value_buffer.clear();
    }
  }

  end = std::chrono::system_clock::now();

  elapsed_seconds = end - start;

  std::cout << "art_tree: " << (iter * key_num / (1024.0 * 1024.0)) / elapsed_seconds.count()
            << " million email read/sec" << "\n";

  print_flag = true;
  art_tree_destroy(&at);
  print_flag = false;
  delete[] array;

  std::cout << "test success" << std::endl;

}