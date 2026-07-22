#ifndef IMAGES_ANNOTATOR_PROJECT_GTKMM_TESTS_GTK_INIT_FIXTURE_H
#define IMAGES_ANNOTATOR_PROJECT_GTKMM_TESTS_GTK_INIT_FIXTURE_H

#include <gtest/gtest.h>
#include <gtkmm.h>
#include <gtkmm/init.h>

/**
 * @brief Base fixture for the GTK widget component tests.
 *
 * Constructing GTK widgets requires an initialized GDK display. When none is
 * available (a headless build agent without an X / broadway / wayland display)
 * the tests self-skip instead of crashing, so the suite stays green.
 */
class GtkInitFixture : public ::testing::Test
{
 protected:
  void SetUp() override
  {
    // Probe for a usable display first so the suite self-skips (instead of
    // aborting) on a headless agent. GTK4's gtk_init_check() takes no
    // arguments and does not abort.
    static const bool gtk_ok = gtk_init_check();

    if (!gtk_ok) {
      GTEST_SKIP() << "GTK could not initialize (no display available)";
    }

    // GTK4 has no Gtk::Main. Register the gtkmm C++ wrapper layer explicitly so
    // the widget wrappers associate with their C objects; without this the
    // wrappers abort with a "quark > 0" GObject assertion. Done once per
    // process.
    static bool gtkmm_inited = false;
    if (!gtkmm_inited) {
      Gtk::init_gtkmm_internals();
      gtkmm_inited = true;
    }
  }
};

#endif  // IMAGES_ANNOTATOR_PROJECT_GTKMM_TESTS_GTK_INIT_FIXTURE_H
