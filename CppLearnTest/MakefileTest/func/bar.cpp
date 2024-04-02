/* bar.c */
#include <stdio.h>

#define PROGRESS_BAR "*************************"

// 输入参数
// comp: 完成比例（0% ~ 100%）
void Print_Progress_Bar(float comp) {
  char bar[] = PROGRESS_BAR;
  int len_bar = sizeof(PROGRESS_BAR) - 1;
  comp = (comp > 1.0f) ? 1.0f : comp;
  comp = (comp < 0.0f) ? 0.0f : comp;

  // 绘制进度条
  int end = comp * len_bar;
  for (int i = end; i < len_bar; i++) {
    bar[i] = ' ';
  }
  printf("|%s|\n", bar);
}
