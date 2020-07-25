FOLDER=$1
> "log"
find $FOLDER -type d  >> "log"
find $FOLDER -type f  >> "log"