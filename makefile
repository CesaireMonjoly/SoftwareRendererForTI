# ----------------------------
# Makefile Options
# ----------------------------

NAME = main
ICON = icon.png
DESCRIPTION = "main"
COMPRESSED = NO
ARCHIVED = NO

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)
