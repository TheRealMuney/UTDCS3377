#include <unistd.h> // for pipe(), read(), write()
#include <cstdlib> // for exit()
#include <iostream>
#include <string>
using namespace std;

int main() {
  int pipe_fd[2];
  if (pipe(pipe_fd) == -1) {
    cerr << "Failed to create pipe\n";
    exit(EXIT_FAILURE);
  }

  pid_t pid = fork();
  if (pid == -1) {
    cerr << "Failed to fork process\n";
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    // Child process (the reader)
    close(pipe_fd[1]); // Close the write end of the pipe

    string message;
    char buffer[1024];
    int bytes_read = read(pipe_fd[0], buffer, sizeof(buffer));
    if (bytes_read == -1) {
      cerr << "Failed to read from pipe\n";
      exit(EXIT_FAILURE);
    }
    message = string(buffer, bytes_read);
    cout << "Message received: " << message << endl;

    close(pipe_fd[0]); // Close the read end of the pipe
    exit(EXIT_SUCCESS);
  } else {
    // Parent process (the writer)
    close(pipe_fd[0]); // Close the read end of the pipe

    string message = "Hello, world!";
    int bytes_written = write(pipe_fd[1], message.c_str(), message.size());
    if (bytes_written == -1) {
      cerr << "Failed to write to pipe\n";
      exit(EXIT_FAILURE);
    }

    close(pipe_fd[1]); // Close the write end of the pipe
    exit(EXIT_SUCCESS);
  }

  return 0;
}

