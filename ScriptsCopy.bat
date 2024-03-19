del /q /f ".\External\Include\Scripts\*.*"
xcopy /s /y  /exclude:exclude_list.txt ".\Projects\Scripts\*.h" ".\External\Include\Scripts"