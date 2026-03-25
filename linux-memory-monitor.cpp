#include<iostream>
#include<fstream>
#include<string>
#include<unistd.h>

size_t get_memory()
{
  std::ifstream file("/proc/self/stat");

  if (!file.is_open()) return 1;

  std::string ignore;
  long rss;

  for (int i = 0; i < 24; i++)
  {
    file >> ignore;
  }

  file >> rss;

  file.close();

  long page_size = sysconf(_SC_PAGESIZE);

  return rss * page_size;
}

int main()
{
  std::cout << "<memory monitor> " << std::endl;
  std::cout << "PID: " << getpid() << std::endl;

  std::cout << "initial usage> " << get_memory() << " /bytes" << std::endl;

    std::cout << "add 10mb... " << std::endl;
    
    int* array = new int[1024 * 1024 * 10]; 
    
      for(int i=0; i < 1024*1024*10; i++) {
        array[i] = i; 
    }

    std::cout << "usage before add> " << get_memory() << " /bytes" << std::endl;

    delete[] array;
    
    std::cout << "usage before delete add> " << get_memory() << " bytes" << std::endl;

    return 0;
}
