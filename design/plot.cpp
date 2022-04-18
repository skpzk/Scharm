#include "plot.h"


#include <pybind11/embed.h>  // py::scoped_interpreter
#include <pybind11/stl.h>    // bindings from C++ STL containers to Python types
#include <pybind11/numpy.h>

#include <vector>

namespace py = pybind11;

using namespace py::literals;

namespace plt{
  void show(){
    // Save the necessary local variables in a Python dict
    py::dict locals = py::dict{
    };

    // Execute Python code, using the variables saved in `locals`
    py::exec(R"(
    
    import matplotlib.pyplot as plt
    plt.show()
    
    )",
      py::globals(), locals);
  }
  
  void figure(){
    // Save the necessary local variables in a Python dict
    // py::dict locals = py::dict{
    // };

    // Execute Python code, using the variables saved in `locals`
    py::exec(R"(
    
    import matplotlib.pyplot as plt
    plt.figure()
    
    )",
      py::globals());
      // , locals);
  }
  void start(){
    //start the python interpreter
    py::scoped_interpreter guard{};
  }

  void plot(void* buffer, int size){
    sample_t* buf = (sample_t*) buffer;

    std::vector<double> s;

    for(int i=0;i<size;i++){
      s.push_back(buf[i]);
    }

    plot(s);

  }

  void plot(std::vector<double> s){

    // Save the necessary local variables in a Python dict
    py::dict locals = py::dict{
      "signal"_a = s,
    };

    // Execute Python code, using the variables saved in `locals`
    py::exec(R"(
    
    import matplotlib.pyplot as plt
    plt.plot(signal)
    
    )",
    py::globals(), locals);

  }

  // implicitly plots audio buffer
  void plot(void* buffer){

    sample_t* inbuf = (sample_t*) buffer;

    //construct mono vector
    std::vector<double> signal;

    for(int i=0;i<FRAMES_PER_BUFFER;i++){
      signal.push_back(inbuf[2*i]);
    }

    // plot buffer

    plot(signal);

  }
}