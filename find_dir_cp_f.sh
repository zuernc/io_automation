#"shopt" command not available, alternative:
$ find /"directory" -type d -name "certain_directory" -print0 | 
while IFS= read -r -d '' directory; do cp -r "$directory"/* /"different_directory"/;
done  
