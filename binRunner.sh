# © Razzy 2022
# usage: binRunner.sh FILE ARGUMENTS

FILE="$1"
ARGUMENTS="$2"

#----------------------------

BLUE='\033[0;34m'
GREEN='\033[0;92m'
RED='\033[0;31m'
ORANGE='\033[38;2;235;128;52m'
PURPLE='\033[0;95m'
NC='\033[0m'

echo -e "${BLUE}${FILE} ${ARGUMENTS}${NC}"

exec 3>&1 4>&2
TIMEFORMAT="%3R"
TIME=$( { time "${FILE}" "${ARGUMENTS}" 1>&3 2>&4; } 2>&1 )
temp=$?
exec 3>&- 4>&-

SHELL_ERROR=$(awk -v RS= '{ sub("(\n.*$)|(^[0-9]+\\.[0-9]+$)", ""); print}' <<< "${TIME}")
SHELL_ERROR=$(awk -v RS= -v A="$FILE $ARGUMENTS" '{ sub("\\$1 \\$launch_arguments 2> >\\(while IFS= read .*; fi; done\\)", A); print}' <<< "${SHELL_ERROR}")

if [ "$SHELL_ERROR" != "" ]
	then echo -e "${RED} $SHELL_ERROR"
fi

TIME=$(awk -v RS= '{ sub("^.*\n", ""); print}' <<< "${TIME}")

sleep 0.1
echo -n -e "\\n${GREEN}exited with "
if [ $temp -eq 0 ]
	then echo -n -e "${PURPLE}code=0 "
	else echo -n -e "${RED}code=$temp "
fi
echo -e "${GREEN}in ${ORANGE}${TIME} ${GREEN}seconds${NC}"
