#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsDirDB.h"

#include <exception>
#include <fstream>
#include <nlohmann/json.hpp>

#include "src/log/log.h"

namespace iannotator::dbs::annotations
{

bool AnnotationsDirDB::load_db(const std::string& fpath)
{
  assert(fpath != nullptr);

  if (fpath.empty()) {
    LOGE("No filepath is given");
    return false;
  }

  try {
    std::ifstream f(fpath);

    if (!f.is_open()) {
      int cerrno = errno;
      LOGE("Can't open file: " << fpath << " because " << strerror(cerrno));
      return false;
    }

    json = nlohmann::json::parse(f);
  }
  catch (const std::exception& e) {
    LOGE("Exception occured: " << e.what());
    return false;
  }

  return true;
}

}  // namespace iannotator::dbs::annotations
