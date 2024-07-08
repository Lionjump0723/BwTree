#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <random>

int main(int argc, char **argv) {
  unsigned key_num = 0;
  unsigned key_length = 0;
  char* filename = nullptr;
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
    } else {
      printf("ERROR: unknown option \"%s\"\n", opt_p);
      return 0;
    }

    opt_index += 2;
  }

  std::ofstream f(filename);
  char* buffer = new char[key_length + 1];
  buffer[key_length] = '\n';
  
  std::random_device rd;
  std::default_random_engine random(rd());

  for (unsigned i = 0; i < key_num; i++) {
    for (unsigned j = 0; j < key_length; j++) {
      char tmp = random() % 62;
      if (tmp < 10) {
        tmp += '0';
      } else if (tmp < 36) {
        tmp -= 10;
        tmp += 'A';
      } else {
        tmp -= 36;
        tmp += 'a';
      }
      buffer[j] = tmp;
    }
    f.write(buffer, key_length + 1);
  }

  f.close();
  delete[] buffer;

  return 0;
}