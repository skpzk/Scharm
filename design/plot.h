#ifndef DESIGN_PLOT_H
#define DESIGN_PLOT_H

#include <vector>

#include "../audioApi/AudioConstants.h"
namespace plt{
  void show();
  void figure();
  void start();
  void plot(void*, int size);
  void plot(void*);
  void plot(void*, void*, int size);
  void plot(void*, void*);

  void plot(std::vector<double>);
}

#endif /* DESIGN_PLOT_H */
