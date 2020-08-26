#include <string>
#include <iostream>

#include <QApplication>
#include <QDebug>

#include "main_window.hpp"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  std::string filename;

  if(argc == 2){
    filename = argv[1];
    MainWindow w(filename);
    w.show();
    return app.exec();
  }
  if(argc > 2){
    std::cerr << "Error: invalid number of arguments to sldraw" << std::endl;
    return EXIT_FAILURE;
  }

  MainWindow w;
  w.setMinimumSize(800,600);
  w.show();
  return app.exec();
}
