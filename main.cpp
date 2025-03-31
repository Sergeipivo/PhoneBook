#include "Lab8_3.h"
#include <QtWidgets/QApplication>
#include "PhonebookApp.h"


int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    PhonebookApp window;
    window.setWindowTitle("Phone Book");
    window.resize(400, 600);
    window.show();
    return app.exec();


}
