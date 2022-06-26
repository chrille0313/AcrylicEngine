@echo off
cls
cloc ../Acrylic ../Acrylic-Glass ../Sandbox --exclude-dir=vendor --exclude-lang=XML --by-file
cloc ../Acrylic ../Acrylic-Glass ../Sandbox --exclude-dir=vendor --exclude-lang=XML
PAUSE