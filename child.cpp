#include <iostream>
#include <sys/wait.h>
#include <string>

size_t recv(int& err, int rd, char* b, size_t k) {
  size_t r = 0;
  while (r < k) {
    err = read(rd, b + r, k - r);
    if (err <= 0) {
      break;
    }
    r += err;
  }
  return r;
}

int main(int argc, char** argv) {
  int rd = std::atoi(argv[1]);
  std::string msg;
  char buffer[255] = {};
  int err = 0;
  int get_chars = recv(err, rd, buffer, sizeof(buffer));
  while (get_chars == 255) {
    msg.append(buffer, sizeof(buffer));
    get_chars = recv(err, rd, buffer, sizeof(buffer));
  }
  msg.append(buffer, get_chars);
  err = close(rd);
  std::cout << msg;
}
