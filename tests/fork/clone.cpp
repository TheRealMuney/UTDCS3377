#include <iostream>
#include <unistd.h>

using namespace std;

int main() {

pid_t cpid = fork();

if(cpid == 0) {
cout << "Child: " << getpid() << endl;
}
else {
cout << "Parent: " << getpid() << endl;
}

}
