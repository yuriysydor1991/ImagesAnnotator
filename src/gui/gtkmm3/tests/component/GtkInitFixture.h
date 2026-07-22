#ifndef IMAGES_ANNOTATOR_PROJECT_GTKMM_TESTS_GTK_INIT_FIXTURE_H
#define IMAGES_ANNOTATOR_PROJECT_GTKMM_TESTS_GTK_INIT_FIXTURE_H

#include <gtest/gtest.h>
#include <gtkmm.h>

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
    static int argc = 0;
    static char** argv = nullptr;

    // Probe for a usable display first so the suite self-skips (instead of
    // aborting) on a headless agent. gtk_init_check does not abort.
    static const bool gtk_ok = gtk_init_check(&argc, &argv);

    if (!gtk_ok) {
      GTEST_SKIP() << "GTK could not initialize (no display available)";
    }

    // A display is present: initialize the gtkmm C++ wrapper layer (registers
    // the per-object data quark) so the widget wrappers construct and destruct
    // cleanly. Kept alive for the whole process.
    static Gtk::Main gtkmm_kit(argc, argv);
  }
};

#endif  // IMAGES_ANNOTATOR_PROJECT_GTKMM_TESTS_GTK_INIT_FIXTURE_H
