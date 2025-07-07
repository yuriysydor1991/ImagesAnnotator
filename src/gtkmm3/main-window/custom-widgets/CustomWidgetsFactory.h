#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM_WINDOWLOADER_CUSTOMWIDGETS_FACTORY_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM_WINDOWLOADER_CUSTOMWIDGETS_FACTORY_CLASS_H

#include <memory>
#include <vector>

#include "src/annotator-events/events/ImageRecord.h"
#include "src/annotator-events/events/ImageRecordRect.h"
#include "src/annotator-events/events/ImagesPathsDBProvider.h"
#include "src/gtkmm3/main-window/custom-widgets/AllAnnotationsLabel.h"
#include "src/gtkmm3/main-window/custom-widgets/CentralWorkingCanvas.h"
#include "src/gtkmm3/main-window/custom-widgets/ImagePathLabel.h"
#include "src/gtkmm3/main-window/custom-widgets/ImageRectsLabel.h"

namespace templateGtkmm3::window::custom_widgets
{

/**
 * @brief Class to hold the window loader routines from the UI xml files.
 */
class CustomWidgetsFactory
{
 public:
  using ImageRecord = events::events::ImageRecord;
  using ImageRecordsSet = events::events::ImageRecordsSet;
  using ImagesVisualDB = std::vector<std::shared_ptr<ImagePathLabel>>;
  using ImageRecordRectPtr = events::events::ImageRecordRectPtr;
  using ImageRecordRectSet = events::events::ImageRecordRectSet;
  using AnnotationsList =
      events::events::ImagesPathsDBProvider::AnnotationsList;
  using AnnotationsVisualList =
      std::vector<std::shared_ptr<AllAnnotationsLabel>>;

  virtual ~CustomWidgetsFactory() = default;
  CustomWidgetsFactory() = default;

  virtual std::shared_ptr<ImagePathLabel> create_image_db_label(
      std::shared_ptr<ImageRecord> ir);

  virtual std::shared_ptr<CentralWorkingCanvas> create_working_canvas(
      std::shared_ptr<ImagePathLabel>& nci);

  virtual ImagesVisualDB create_images_visual_db(ImageRecordsSet& queue);

  virtual std::shared_ptr<Gtk::FileChooserDialog> create_json_db_dialog(
      Gtk::Window* parentWindow);
  virtual std::shared_ptr<Gtk::FileChooserDialog> create_folder_choose_dialog(
      Gtk::Window* parentWindow);
  virtual std::shared_ptr<Gtk::FileChooserDialog> create_save_json_db_dialog(
      Gtk::Window* parentWindow);

  virtual ImageRectsLabelPtr create_rect_label(ImageRecordRectPtr rect);
  virtual ImageRectsLabelSet create_rects_labels(ImageRecordRectSet& rectRecs);

  virtual AnnotationsVisualList create_annotations_labels(
      const AnnotationsList& strList);

  virtual void prepare_about(Gtk::AboutDialog* about,
                             Gtk::Window* parentWindow);

  virtual std::shared_ptr<Gtk::FileChooserDialog>
  create_txt_export_folder_choose_dialog(Gtk::Window* parentWindow);

  virtual std::shared_ptr<Gtk::MessageDialog> create_error_dialog(
      const std::string& errDesc, Gtk::Window* parentWindow);
};

}  // namespace templateGtkmm3::window::custom_widgets

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM_WINDOWLOADER_CUSTOMWIDGETS_FACTORY_CLASS_H
