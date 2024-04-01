#ifndef MYAI_Node_Controller_H_
#define MYAI_Node_Controller_H_
#include "../config/define.h"
#include "Thinker.h"
#include <string>

MYAI_BEGIN

class ThinkerController {

public:
  ThinkerController(std::string path);
  ~ThinkerController();
  void loadConfig();
  void saveToYaml();

  void run();
  void stop();
  void pause();

private:
  std::string m_config_path;
  Thinker::ptr m_thinker;

};

MYAI_END

#endif //!MYAI_Node_Controller_H_