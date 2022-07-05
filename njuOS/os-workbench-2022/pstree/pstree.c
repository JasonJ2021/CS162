#include <stdio.h>
#include <assert.h>
#include <string.h>

/**
 * @brief 
 * -p, --show-pids: 打印每个进程的进程号。
 * -n --numeric-sort: 按照pid的数值从小到大顺序输出一个进程的直接孩子。
 * -V --version: 打印版本信息。
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */

int main(int argc, char *argv[]) {
  char p = 0; // 打印每个进程的进程号
  char n = 0; // 按照pid的大小输出
  char v = 0; // 打印版本信息

  for (int i = 0; i < argc; i++) {
    assert(argv[i]);
    if(strcmp(argv[i] , "-V") == 0 || strcmp(argv[i] , "--version") == 0){
      v = 1;
    }
    if(strcmp(argv[i] , "-n") == 0 || strcmp(argv[i] , "--numeric-sort") == 0){
      n= 1;
    }
    if(strcmp(argv[i] , "-p") == 0 || strcmp(argv[i] , "--show-pids") == 0){
      p = 1;
    }


    printf("argv[%d] = %s\n", i, argv[i]);
  }
  printf("p = %d , n = %d , v = %d\n" , p , n , v );
  assert(!argv[argc]);
  return 0;
}
