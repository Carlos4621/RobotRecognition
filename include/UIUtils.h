#ifndef UI_UTILS_HEADER
#define UI_UTILS_HEADER

#include <QMessageBox>
#include <QString>

class UiUtils {
public:
    static void showError(const QString& mensaje) {
        QMessageBox::critical(nullptr, "Error", mensaje);
    }
};

#endif // !UI_UTILS_HEADER
