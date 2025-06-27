#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "CTEST_AnnotatorController.h"
#include "ctests_decls.h"
#include "src/annotator-business/AnnotatorController.h"
#include "src/annotator-events/ImagesAnnotatorEventController.h"
#include "src/annotator-events/events/ImagesDirChanged.h"
#include "src/app/ApplicationContext.h"
#include "src/log/log.h"

using namespace testing;
using namespace iannotator;
using namespace events::events;

class CTEST_AnnotationsDirDB : public CTEST_AnnotatorController
{
 public:
  inline static const std::string tmp_data_dir =
      ctests_decls::tmp_data_path_root + "/CTEST_AnnotationsDirDB";
  inline static const std::string ctest_data_dir = CTEST_DATA_ROOT;

  inline static const std::string ctest_data_json_plan_name =
      "basic-structure-plan.json";
  inline static const std::string nonexist_ctest_data_json_plan =
      ctest_data_dir + "/nonexistent-annotations-db-file.not-a-json";

  ~CTEST_AnnotationsDirDB() = default;

  CTEST_AnnotationsDirDB()
  {
    EXPECT_NE(controller, nullptr);
    EXPECT_NE(actx, nullptr);
  }

  inline void set_actx_annotations_db(const std::string& dbname)
  {
    EXPECT_NE(actx, nullptr);
    actx->annotation_db_path = ctest_data_dir + "/" + dbname;
  }
};

TEST_F(CTEST_AnnotationsDirDB, no_context_init_failure)
{
  EXPECT_FALSE(controller->init({}));
}

TEST_F(CTEST_AnnotationsDirDB, empty_context_init_success)
{
  EXPECT_TRUE(controller->init(actx));
}

TEST_F(CTEST_AnnotationsDirDB, non_empty_invalid_annotation_db_init_failure)
{
  set_actx_annotations_db(nonexist_ctest_data_json_plan);

  EXPECT_FALSE(controller->init(actx));
}

TEST_F(CTEST_AnnotationsDirDB, non_empty_annotation_db_init_success)
{
  static const std::string dbpath = "non_empty_annotation_db_init_success.json";

  set_actx_annotations_db(dbpath);

  EXPECT_TRUE(controller->init(actx));
}

TEST_F(CTEST_AnnotationsDirDB, basic_json_plan_db_init_success)
{
  set_actx_annotations_db(ctest_data_json_plan_name);

  EXPECT_TRUE(controller->init(actx));
}
