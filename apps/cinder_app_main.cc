#include <visualizer/connect_four_app.h>

using connect_four::visualizer::ConnectFourApp;

void prepareSettings(ConnectFourApp::Settings* settings) {
  settings->setResizable(false);
}

// This line is a macro that expands into an "int main()" function.
CINDER_APP(ConnectFourApp, ci::app::RendererGl, prepareSettings);
