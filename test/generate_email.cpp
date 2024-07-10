#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <random>
#include <cassert>

std::random_device rd;


char random_char() {
  static std::default_random_engine random(rd());
  char tmp = random() % 256 - 128;
  return tmp;
}

int main(int argc, char **argv) {
  unsigned key_num = 0;
  unsigned key_length = 0;
  char* filename = nullptr;
  char* key_type = nullptr;
  if (argc % 2 == 0) {
    printf("ERROR: invalid arg num %d\n", argc);
    return 0;
  }

  int opt_index = 1;
  while (opt_index < argc) {
    char* opt_p = argv[opt_index];
    
    if (strcmp(opt_p, "--key-num") == 0) {
      key_num = atoi(argv[opt_index + 1]);
    } else if (strcmp(opt_p, "--key-length") == 0) {
      key_length = atoi(argv[opt_index + 1]);
    } else if (strcmp(opt_p, "--filename") == 0) {
      filename = argv[opt_index + 1];
    } else if (strcmp(opt_p, "--key-type") == 0) {
      key_type = argv[opt_index + 1];
    } else {
      printf("ERROR: unknown option \"%s\"\n", opt_p);
      return 0;
    }

    opt_index += 2;
  }

  std::ofstream f(filename, std::ios::binary);
  char* buffer = new char[key_length + 1];
  buffer[key_length] = '\n';
  

  if (key_type == nullptr or strcmp(key_type, "random") == 0) {
    for (unsigned i = 0; i < key_num; i++) {
      for (unsigned j = 0; j < key_length; j++) {
        buffer[j] = random_char();
      }
      f.write((const char *) &key_length, 4);
      f.write(buffer, key_length);
    }
  } else if (strcmp(key_type, "small") == 0) {
    assert (key_length > 17);
    for (unsigned i = 0; i < key_num; i++) {
      unsigned random_len = key_length - 17;
      buffer[0] = '\0';
      for (unsigned j = 0; j < random_len; j++) {
        buffer[1+j] = random_char();
      }
      for (unsigned j = 0; j < 8; j++) {
        buffer[1+random_len+j] = '\0';
      }
      unsigned long last = i;
      last += 1ull<<56;
      memcpy(buffer+1+random_len+8, (void *)&last, sizeof(last));
      f.write((const char *) &key_length, 4);
      f.write(buffer, key_length);
    }
    
  } else if (strcmp(key_type, "big") == 0) {
    assert(key_length > 8);
    for (unsigned i = 0; i < key_num; i++) {
      for (unsigned j = 0; j < key_length-8; j++) {
        buffer[j] = random_char();
      }
      memcpy(buffer+key_length-8, (void *)&i, sizeof(i));
      f.write((const char *) &key_length, 4);
      f.write(buffer, key_length);
    }
  } else {
    printf("ERROR: unknown key_type \"%s\"\n", key_type);
  }
  f.close();
  delete[] buffer;

  return 0;
}