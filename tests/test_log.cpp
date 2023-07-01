#include "../src/log/LoggerManager.h"

int main(int argc, char *argv[])
{
  MYAI_LOG_DEBUG(MYAI_LOGGER_ROOT) << "test";
  MYAI_LOG_DEBUG(MYAI_LOGGER_ROOT) << "test";
  MYAI_LOG_DEBUG(MYAI_LOGGER_ROOT) << "test";
  MYAI_LOG_DEBUG(MYAI_LOGGER_ROOT) << "test";
  MYAI_LOG_DEBUG(MYAI_LOGGER_NAME("system")) << "system test";
  return 0;
}