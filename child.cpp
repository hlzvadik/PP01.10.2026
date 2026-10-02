#include <iostream>
#include <sys/wait.h>
#include <string>

size_t recv(int& err, int rd, char* b, size_t k) {
  size_t r = 0;
  while (r < k) {
    err = read(rd, b + r, k - r);
    if (err < 0) {
      break;
    }
    r += err;
  }
  return r;
}

size_t recvCString(int& err, int rd, char* b, size_t k) {
  size_t r = 0;
  while (r < k) {
    err = read(rd, b + r, k - r);
    if (err < 0) {
      break;
    }
    if (err == 0 && r != 0 && b[r - 1] == '\0') {
      break;
    }
    r += err;
  }
  return r;
}

int main(int argc, char** argv) {
  int rd = std::atoi(argv[1]);
  char buffer[255] = {};
  int err = 0;
  int get_chars = recvCString(err, rd, buffer, sizeof(buffer));
  printf("Recieved string:\n");
  while (get_chars == 255) {
    printf(buffer, sizeof(buffer));
    if (buffer[sizeof(buffer) - 1] == '\0') {
      break;
    }
    get_chars = recv(err, rd, buffer, sizeof(buffer));
  }
  if (get_chars != 255) {
    printf(buffer, get_chars);
  }
  err = close(rd);
}
