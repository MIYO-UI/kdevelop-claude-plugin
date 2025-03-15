#ifdef STANDALONE_TEST

#include <QApplication>
#include "claudeview.h"
#include "googleauth.h"
#include "claudeapi.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    GoogleAuth* auth = new GoogleAuth();
    ClaudeAPI* api = new ClaudeAPI(auth);
    
    ClaudeView* view = new ClaudeView(nullptr, auth, api);
    view->resize(800, 600);
    view->show();
    
    return app.exec();
}

#endif // STANDALONE_TEST