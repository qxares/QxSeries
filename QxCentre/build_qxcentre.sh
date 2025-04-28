#!/bin/bash
# Script to build and test QxCentre with QxWrite and QxSheet

# Clean old build files
cd /home/ares/QxCentre/QxCentre
rm -f *.o moc_*.cpp moc_predefs.h Makefile qrc_*.cpp *.pro.user

# Generate Makefile and build
qmake qxcentre.pro -spec linux-g++ CONFIG+=debug
make clean
make

# Verify file structure
cd /home/ares/QxCentre
ls -l
ls -l QxDocument

# Run QxCentre with debug logs
cd /home/ares/QxCentre/QxCentre
QT_LOGGING_RULES="qt5ct.debug=true" ./QxCentre

# Grep for QxText to ensure no leftovers
echo "Checking for QxText references in QxCentre:"
grep -r "QxText" /home/ares/QxCentre/QxCentre
echo "Checking for QxText references in QxDocument:"
grep -r "QxText" /home/ares/QxCentre/QxDocument
