cd /home/ares/QxCentre/QxWrite
rm -rf *.o moc_*.cpp moc_predefs.h Makefile QxCentre qrc_*.cpp *.pro.user
export QT_QPA_PLATFORM=xcb
export QT_LOGGING_RULES="qt5.widgets=true"
qmake qxcentre.pro -spec linux-g++ CONFIG+=debug
make clean
make
./QxCentre
