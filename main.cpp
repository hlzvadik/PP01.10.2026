#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <string>

size_t send(int& err, int wr, const char* b, size_t k) {
  size_t r = 0;
  while (r < k) {
    err = write(wr, b + r, k - r);
    if (err < 0) {
      break;
    }
    r += err;
  }
  return r;
}

int main() {
  int pps[2] = {};
  int err = pipe(pps);
  int rd = pps[0], wr = pps[1];
  pid_t pid = fork();
  if (!pid) {
    err = close(wr);
    char p[100] = {};
    err = sprintf(p, "%d", rd);
    execl("./child", "child", p, NULL);
  }
  err = close(rd);
  printf("Type string, complete with EOF:\n");
  char msg[255];
  int scans = fread(msg, 1, sizeof(msg), stdin);
  while (scans == 255) {
    send(err, wr, msg, scans);
    scans = fread(msg, 1, sizeof(msg), stdin);
  }
  msg[scans] = '\0';
  send(err, wr, msg, scans + 1);
  err = close(wr);
  err = waitpid(pid, 0, 0);
}
