#!/bin/sh

touch README INSTALL NEWS AUTHORS ChangeLog COPYING

autoreconf --force --install --verbose
