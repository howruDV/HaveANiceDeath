xcopy /s /y  /exclude:exclude_list.txt ".\Projects\Engine\*.h" ".\External\Include\Engine\"
xcopy /s /y  /exclude:exclude_list.txt ".\Projects\Engine\*.inl" ".\External\Include\Engine\"
xcopy /s /y  /exclude:exclude_list.txt ".\Projects\Engine\*.fx" ".\OutputFile\Contents\shader\"