del /q /f ".\External\Include\States\*.*"
xcopy /s /y  /exclude:exclude_list.txt ".\Projects\States\*.h" ".\External\Include\States"